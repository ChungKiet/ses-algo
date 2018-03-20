/*
 * Project 2 - Advanced Operating System
 * CAUSAL MESSAGE ORDERING IN DISTRIBUTED SYSTEM
 * Using Schiper-Eggli-Sandoz Algorithm
 *
 * 1512284 - Ha Tan Linh
 * Email: hatanlinh13@gmail.com
 *
 * Module: Messages Sender Threads
 */
#include "threads.h"
#include "sentvec.h"
#include "timevec.h"
#include "logs.h"
#include "defs.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUF_SIZE 100000

/* In a localhost enviroment messages are sent almost immediately
 * therefore we cannot test the delayed message functionality.
 * In order to test this, for each mes. we create a new thread that wait
 * a random period of time
 * before actually send the message.
 * To active this behavior, define _TEST_DELAY_MESSAGE_ at compile time
 */
void *delayed_send(void *args);
typedef struct {
	int sockfd;
	char *buf;
	int msg_len;
} sendargs_t;
void  nodelay_send(int sockfd, char *buf, int msg_len);

void threads_init()
{
	pthread_rwlock_rdlock(&lock_n);
	volatile int nn = n;
	pthread_rwlock_unlock(&lock_n);

	pthread_rwlock_rdlock(&lock_id);
	volatile int dd = id;
	pthread_rwlock_unlock(&lock_id);

	threadargs_t *arr[nn];

	srand(time(0));
	for (int i = 0; i < nn; i++)
		if (i != dd) {
			arr[i] = (threadargs_t *)malloc(sizeof(threadargs_t));
			arr[i]->proc = i;
			strncpy(arr[i]->ipv4, proc_list[i].ipv4, 16);
			arr[i]->port = proc_list[i].port;
			arr[i]->nmsg = nmsg;
			arr[i]->rate = rmin + (rand() % (rmax - rmin + 1));

			if (pthread_create(&trr[i], NULL, &start_sender, arr[i]) != 0) {
				logs_errexit("Cannot create a thread.");
				term_errexit("Cannot create a thread.");
			}
		}
}

void *start_sender(void *args_addr)
{
	/* argument validation */
	if (args_addr == NULL)
		return NULL;
	threadargs_t *args = (threadargs_t *)args_addr;
	if (args->nmsg == 0 || args->rate == 0)
		return NULL;

	struct timespec sleep_interval; sleep_interval.tv_sec  = 60 / args->rate;
	int sec_left = 60 - args->rate * sleep_interval.tv_sec;
	if (sec_left == 0)
		sleep_interval.tv_nsec = 0;
	else
		sleep_interval.tv_nsec = 1000000000 * sec_left / args->rate;

#ifdef _TEST_DELAY_MESSAGE_
	pthread_t st[100000];
	int cnt = 0;
#endif

	/* send messages */
	for (int i = 0; i < args->nmsg; i++) {
		nanosleep(&sleep_interval, NULL);

		/* update local vector clock */
		pthread_rwlock_rdlock(&lock_id);
		int dd = id;
		pthread_rwlock_unlock(&lock_id);

		pthread_rwlock_rdlock(&lock_time);
		int oldv = tv_get(&time_curr, dd);
		pthread_rwlock_unlock(&lock_time);

		pthread_rwlock_wrlock(&lock_time);
		tv_set(&time_curr, dd, oldv + 1);
		pthread_rwlock_unlock(&lock_time);

		/* establish connection */
		int sockfd;
		struct sockaddr_in serv;
		memset(&serv, 0, sizeof(serv));
		serv.sin_family = AF_INET;
		if (inet_pton(AF_INET, args->ipv4, &serv.sin_addr) <= 0) {
			logs_errexit("Invalid recipient IP address.");
			term_errexit("Invalid recipient IP address.");
		}
		serv.sin_port = htons(args->port);
		if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
			logs_errexit("Cannot create a socket.");
			term_errexit("Cannot create a socket.");
		}
		if (connect(sockfd, (struct sockaddr *)&serv, sizeof(serv)) < 0) {
			logs_errexit("Cannot connect to a socket.");
			term_errexit("Cannot connect to a socket.");
		}

		/* construct message */
		char *buf = (char *)malloc(BUF_SIZE);

		pthread_rwlock_rdlock(&lock_id);
		int msg_len = sprintf(buf, "Message %d\r\n\r\n%d ", i, id);
		pthread_rwlock_unlock(&lock_id);

		pthread_rwlock_rdlock(&lock_time);
		msg_len += tv_tostring(buf + msg_len, &time_curr);
		pthread_rwlock_unlock(&lock_time);

		pthread_rwlock_rdlock(&lock_vect);
		msg_len += sv_tostring(buf + msg_len, &vect_curr);
		pthread_rwlock_unlock(&lock_vect);

		buf[msg_len] = '\0';

		/* update sent message vector */
		pthread_rwlock_rdlock(&lock_time);
		pthread_rwlock_wrlock(&lock_vect);
		if (sv_search(&vect_curr, args->proc) == NULL) {
			sv_add(&vect_curr, &time_curr, args->proc);
		}
		else {
			sv_update(&vect_curr, &time_curr, args->proc);
		}
		pthread_rwlock_unlock(&lock_time);
		pthread_rwlock_unlock(&lock_vect);

#ifdef _TEST_DELAY_MESSAGE_
		sendargs_t *a = (sendargs_t *)malloc(sizeof(sendargs_t));
		if (a == NULL) {
			logs_errexit("Cannot allocate memory.");
			term_errexit("Cannot allocate memory.");
		}
		a->sockfd  = sockfd;
		a->buf     = buf;
		a->msg_len = msg_len;
		pthread_create(&st[cnt++], NULL, &delayed_send, (void *)a);
#else
		nodelay_send(sockfd, buf, msg_len);
#endif
	}

#ifdef _TEST_DELAY_MESSAGE_
	for (int i = 0; i < cnt; i++)
		pthread_join(st[i], NULL);
#endif
	pthread_exit(NULL);
}

void nodelay_send(int sockfd, char *buf, int msg_len)
{
	/* send message */
	if (send(sockfd, buf, msg_len + 1, 0) <= msg_len) {
		logs_errexit("Cannot send data to a socket.");
		term_errexit("Cannot send data to a socket.");
	}

	/* shutdown connection */
	free(buf);
	shutdown(sockfd, SHUT_RDWR);
}

void *delayed_send(void *args)
{
	sendargs_t *arg = (sendargs_t *)args;
	struct timespec delay;
	delay.tv_nsec = 0;
	delay.tv_sec  = rand() % 4;
	nanosleep(&delay, NULL);
	/* send message */
	if (send(arg->sockfd, arg->buf, arg->msg_len + 1, 0) <= arg->msg_len) {
		logs_errexit("Cannot send data to a socket.");
		term_errexit("Cannot send data to a socket.");
	}
	/* shutdown connection */
	free(arg->buf);
	shutdown(arg->sockfd, SHUT_RDWR);
	pthread_exit(NULL);
}
