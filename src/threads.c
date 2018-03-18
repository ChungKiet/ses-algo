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

void *start_sender(void *args_addr)
{
	/* argument validation */
	if (args_addr == NULL)
		return NULL;
	threadargs_t *args = (threadargs_t *)args_addr;
	if (args->nmsg == 0 || args->rate == 0)
		return NULL;

	/* return value */
	int *retval = (int *)malloc(sizeof(int));
	*retval = 0;

	struct timespec sleep_interval;
	sleep_interval.tv_sec  = 60 / args->rate;
	int sec_left = 60 - args->rate * sleep_interval.tv_sec;
	if (sec_left == 0)
		sleep_interval.tv_nsec = 0;
	else
		sleep_interval.tv_nsec = 1000000000 * sec_left / args->rate;

	/* send messages */
	for (int i = 0; i < args->nmsg; i++) {
		/* establish connection */
		int sockfd;
		struct sockaddr_in serv;
		memset(&serv, 0, sizeof(serv));
		serv.sin_family = AF_INET;
		if (inet_pton(AF_INET, args->ipv4, &serv.sin_addr) <= 0) {
			term_errexit("Invalid recipient IP address.");
			logs_errexit("Invalid recipient IP address.");
		}
		serv.sin_port = htons(args->port);
		if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
			term_errexit("Cannot create a socket.");
			logs_errexit("Cannot create a socket.");
		}
		if (connect(sockfd, (struct sockaddr *)&serv, sizeof(serv)) < 0) {
			term_errexit("Cannot connect to a socket.");
			logs_errexit("Cannot connect to a socket.");
		}

		/* construct message */
		char *buf = (char *)malloc(BUF_SIZE);
		int msg_len = sprintf(buf, "Message %d\r\n\r\n%d ", i, args->proc);
		msg_len += sv_tostring(buf + msg_len, &vect_curr);
		buf[msg_len] = '\0';

		/* send message */
		if (send(sockfd, buf, msg_len + 1, 0) <= msg_len) {
			term_errexit("Cannot send data to a socket.");
			logs_errexit("Cannot send data to a socket.");
		}

		/* shutdown connection */
		free(buf);
		shutdown(sockfd, 2);

		nanosleep(&sleep_interval, NULL);
	}

	return (void *)retval;
}
