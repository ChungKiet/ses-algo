/*
 * Project 2 - Advanced Operating System
 * CAUSAL MESSAGE ORDERING IN DISTRIBUTED SYSTEM
 * Using Schiper-Eggli-Sandoz Algorithm
 *
 * 1512284 - Ha Tan Linh
 * Email: hatanlinh13@gmail.com
 *
 * Module: SES Algorithm
 */
#include "ses.h"
#include "timevec.h"
#include "sentvec.h"
#include "logs.h"
#include "defs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>

#define MAX_CLIENTS 32768
#define BUF_SIZE    100000

/* buffer is a linked list */
typedef struct waitbuf {
	int   sender;
	char *msg;
	vectorclock_t *timestamp;
    sentvec_t     *vect;
	struct waitbuf *next;
} waitbuf_t;

static char *buf;
static struct sockaddr_in addr;
static waitbuf_t *wait = NULL;

void ses_accept();
void check_buffer();

void ses_init()
{
	tv_init(&time_curr, n, 0);
	sv_init(&vect_curr);
	servfd = socket(AF_INET, SOCK_STREAM, 0);
	if (servfd <= 0) {
		logs_errexit("Cannot create a socket.");
		term_errexit("Cannot create a socket.");
	}
	addr.sin_family      = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port        = htons(lport);
	if (bind(servfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		logs_errexit("Cannot bind socket to listen port.");
		term_errexit("Cannot bind socket to listen port.");
	} if (listen(servfd, MAX_CLIENTS) < 0) {
		logs_errexit("Cannot listen for upcomming connections.");
		term_errexit("Cannot listen for upcomming connections.");
	}
	logs_init(log_fname);
}

void ses_loop()
{
	int n_client = 0;
	int client_sock[MAX_CLIENTS];
	memset(client_sock, 0, MAX_CLIENTS * sizeof(int));
	fd_set readfds;
	buf = (char *)malloc(BUF_SIZE);
	int maxfd;
	int addrlen = sizeof(addr);

	while (1) {
		/* clear fd list */
		FD_ZERO(&readfds);
		/* add server sock to list */
		FD_SET(servfd, &readfds);
		maxfd = servfd;
		/* add client sock to list */
		for (int i = 0; i < n_client; i++) {
			if (client_sock[i] > 0)
				FD_SET(client_sock[i], &readfds);
			if (client_sock[i] > maxfd)
				maxfd = client_sock[i];
		}
		/* wait for activities from fd */
		int act = select(maxfd + 1, &readfds, NULL, NULL, NULL);
		if (act < 0 && errno != EINTR) {
			logs_errexit("Problem occurred while waiting for activities.");
			term_errexit("Problem occurred while waiting for activities.");
		}
		/* server sock ready, new connection available */
		if (FD_ISSET(servfd, &readfds)) {
			/* accept new connection */
			int new_sock;
			new_sock = accept(servfd,
			                 (struct sockaddr *)&addr,
			                 (socklen_t *)&addrlen);
			if (new_sock < 0) {
				logs_errexit("Cannot accept connection.");
				term_errexit("Cannot accept connection.");
			}
			/* add to client sock list */
			for (int i = 0; i < MAX_CLIENTS; i++)
				if (client_sock[i] == 0) { /* free */
					client_sock[i] = new_sock;
					if (i + 1 > n_client)
						n_client = i + 1;
					break;
				}

			/* check for client socks */
			for (int i = 0; i < n_client; i++) {
				int fd = client_sock[i];
				if (fd == 0) continue;

				if (FD_ISSET(fd, &readfds)) {
					int num_recv = recv(fd, buf, BUF_SIZE, 0);
					if (num_recv > 0) { /* new message */
						buf[num_recv] = '\0'; /* ensure null terminated */
						ses_accept();
					}
					else if (num_recv == 0) { /* connection closed */
						close(fd);
						client_sock[i] = 0;
					}
					else {
						logs_errexit("Cannot receive data form socket.");
						term_errexit("Cannot receive data form socket.");
					}
				}
			}
		}
	}

	free(buf);
}

void ses_accept()
{
	/* get message content */
	char *tmp = strstr(buf, "\r\n\r\n");
	if (tmp == NULL) {
		logs_errexit("Message syntax error.");
		term_errexit("Message syntax error.");
	}
	char *msg = (char *)malloc(tmp - buf + 1);
	strncpy(msg, buf, tmp - buf);
	msg[tmp - buf] = '\0';
	tmp += 4;

	/* get sender id */
	char *endptr;
	int sender;
	sender = strtol(tmp, &endptr, 0);
	tmp = endptr;

	/* get message timestamp */
	vectorclock_t *timestamp = (vectorclock_t *)malloc(sizeof(vectorclock_t));
	if (timestamp == NULL) {
		logs_errexit("Cannot allocate memory.");
		term_errexit("Cannot allocate memory.");
	}
	pthread_rwlock_rdlock(&lock_n);
	tv_init(timestamp, n, 0);
	pthread_rwlock_unlock(&lock_n);
	char *rslt = tv_construct(timestamp, tmp);
	if (rslt == NULL) {
		logs_errexit("Message syntax error.");
		term_errexit("Message syntax error.");
	}
	else {
		tmp = rslt;
	}

	/* get sent messages vector of sender process */
	sentvec_t *vect = (sentvec_t *)malloc(sizeof(sentvec_t));
	if (vect == NULL) {
		logs_errexit("Cannot allocate memory.");
		term_errexit("Cannot allocate memory.");
	}
	sv_init(vect);
	rslt = sv_construct(vect, tmp);
	if (rslt == NULL) {
		logs_errexit("Message syntax error.");
		term_errexit("Message syntax error.");
	}
	else {
		tmp = rslt;
	}

	/* SES algorithm */
	pthread_rwlock_rdlock(&lock_id);
	int dd = id;
	pthread_rwlock_unlock(&lock_id);
	timevec_t *r = sv_search(vect, dd);
	if (r == NULL) { /* no earlier message sent, deliver */
		/* update sent vector */
		sentnode_t *node = vect->root;
		while (node) {
			pthread_rwlock_rdlock(&lock_vect);
			timevec_t *rslt = sv_search(&vect_curr, node->proc);
			pthread_rwlock_unlock(&lock_vect);

			pthread_rwlock_wrlock(&lock_vect);
			if (rslt == NULL) {
				sv_add(&vect_curr, &node->timestamp, node->proc);
			}
			else {
				sv_update(&vect_curr, &node->timestamp, node->proc);
			}
			pthread_rwlock_unlock(&lock_vect);

			node = node->next;
		}

		/* update local vector clock */
		pthread_rwlock_wrlock(&lock_time);
		tv_combine(&time_curr, timestamp);
		int t = tv_get(&time_curr, dd);
		tv_set(&time_curr, dd, t + 1);
		pthread_rwlock_unlock(&lock_time);

		/* log and console output */
		logs_delivered(sender, msg, 0);
		term_delivered(sender, msg, 0);

		check_buffer();
	}
	else { /* sv_search != NULL, compare to check */
		pthread_rwlock_rdlock(&lock_time);
		tv_cmp_rslt rs = tv_compare(&time_curr, r);
		pthread_rwlock_unlock(&lock_time);
		if (rs == TV_GREQ) { /* deliver */
			/* update sent vector */
			sentnode_t *node = vect->root;
			while (node) {
				pthread_rwlock_rdlock(&lock_vect);
				timevec_t *rslt = sv_search(&vect_curr, node->proc);
				pthread_rwlock_unlock(&lock_vect);

				pthread_rwlock_wrlock(&lock_vect);
				if (rslt == NULL) {
					sv_add(&vect_curr, &node->timestamp, node->proc);
				}
				else {
					sv_update(&vect_curr, &node->timestamp, node->proc);
				}
				pthread_rwlock_unlock(&lock_vect);

				node = node->next;
			}

			/* update local vector clock */
			pthread_rwlock_wrlock(&lock_time);
			tv_combine(&time_curr, timestamp);
			int t = tv_get(&time_curr, dd);
			tv_set(&time_curr, dd, t + 1);
			pthread_rwlock_unlock(&lock_time);

			/* log and console output */
			logs_delivered(sender, msg, 0);
			term_delivered(sender, msg, 0);

			check_buffer();
		}
		else { /* delay */
			waitbuf_t *new_node = (waitbuf_t *)malloc(sizeof(waitbuf_t));
			if (new_node == NULL) {
				logs_errexit("Cannot allocate memory.");
				term_errexit("Cannot allocate memory.");
			}
			new_node->sender    = sender;
			new_node->msg       = msg;
			new_node->vect      = vect;
			new_node->timestamp = timestamp;
			new_node->next = wait;
			wait           = new_node;
		}
	}
}

void check_buffer()
{
	waitbuf_t *prev = NULL;
	waitbuf_t *node = wait;
	while (node) {
		pthread_rwlock_rdlock(&lock_id);
		int dd = id;
		pthread_rwlock_unlock(&lock_id);
		timevec_t *r = sv_search(node->vect, dd);
		if (r == NULL) {
			logs_errexit("Something went wrong.");
			term_errexit("Something went wrong.");
		}
		else {
			pthread_rwlock_rdlock(&lock_time);
			tv_cmp_rslt rs = tv_compare(&time_curr, r);
			pthread_rwlock_unlock(&lock_time);
			if (rs == TV_GREQ) { /* deliver */
				/* update sent vector */
				sentnode_t *rode = node->vect->root;
				while (rode) {
					pthread_rwlock_rdlock(&lock_vect);
					timevec_t *rslt = sv_search(&vect_curr, rode->proc);
					pthread_rwlock_unlock(&lock_vect);

					pthread_rwlock_wrlock(&lock_vect);
					if (rslt == NULL) {
						sv_add(&vect_curr, &rode->timestamp, rode->proc);
					}
					else {
						sv_update(&vect_curr, &rode->timestamp, rode->proc);
					}
					pthread_rwlock_unlock(&lock_vect);

					rode = rode->next;
				}

				/* update local vector clock */
				pthread_rwlock_wrlock(&lock_time);
				tv_combine(&time_curr, node->timestamp);
				int t = tv_get(&time_curr, dd);
				tv_set(&time_curr, dd, t + 1);
				pthread_rwlock_unlock(&lock_time);

				/* log and console output */
				logs_delivered(node->sender, node->msg, 1);
				term_delivered(node->sender, node->msg, 1);

				free(node->msg);
				free(node->timestamp);
				free(node->vect);
				if (prev == NULL) {
					wait = node->next;
					free(node);
					node = wait;
				}
				else {
					prev->next = node->next;
					free(node);
					node = prev->next;
				}
			}
			else { /* rs != TV_GREQ, pass */
				prev = node;
				node = node->next;
			}
		}
	}
}
