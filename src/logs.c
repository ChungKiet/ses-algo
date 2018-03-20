/*
 * Project 2 - Advanced Operating System
 * CAUSAL MESSAGE ORDERING IN DISTRIBUTED SYSTEM
 * Using Schiper-Eggli-Sandoz Algorithm
 *
 * 1512284 - Ha Tan Linh
 * Email: hatanlinh13@gmail.com
 *
 * Module: Log Handler
 */
#include "logs.h"
#include "defs.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void logs_init(const char *filename)
{
	logfd = fopen(filename, "w");
}

void logs_exit()
{
	fclose(logfd);
}

void term_errexit(const char *reason)
{
	pthread_mutex_lock(&lock_log);
	fprintf(stderr, "An error occured: %s\n", reason);
	fprintf(stderr, "Program exited!\n");
	exit(EXIT_FAILURE);
	pthread_mutex_unlock(&lock_log);
}

void logs_errexit(const char *reason)
{
	pthread_mutex_lock(&lock_log);
	fprintf(logfd, "An error occured: %s\n", reason);
	fprintf(logfd, "Program exited!\n");
	pthread_mutex_unlock(&lock_log);
}

void term_delayed(int proc)
{
	pthread_mutex_lock(&lock_log);
	fprintf(stdout, "Received a message from process #%d.\n", proc);
	fprintf(stdout, "----DELAYED, message is now in wait buffer.\n");
	pthread_mutex_unlock(&lock_log);
}

void logs_delayed(int proc)
{
	pthread_mutex_lock(&lock_log);
	fprintf(logfd, "Received a message from process #%d.\n", proc);
	fprintf(logfd, "----DELAYED, message is now in wait buffer.\n");
	pthread_mutex_unlock(&lock_log);
}

void term_delivered(int proc, const char *msg, int type)
{
	pthread_mutex_lock(&lock_log);
	if (type == 0) { /* new message */
		fprintf(stdout, "Received a message from process #%d.\n", proc);
		fprintf(stdout, "----DELIVERED immediately! Content: %s\n", msg);
	}
	else { /* from buffer */
		fprintf(stdout, "----DELIVERED a message of process #%d", proc);
		fprintf(stdout, " from wait buffer. Content: %s\n", msg);
	}
	pthread_mutex_unlock(&lock_log);
}

void logs_delivered(int proc, const char *msg, int type)
{
	pthread_mutex_lock(&lock_log);
	if (type == 0) { /* new message */
		fprintf(logfd, "Received a message from process #%d.\n", proc);
		fprintf(logfd, "----DELIVERED immediately! Content: %s\n", msg);
	}
	else { /* from buffer */
		fprintf(logfd, "----DELIVERED a message of process #%d", proc);
		fprintf(logfd, " from wait buffer. Content: %s\n", msg);
	}
	pthread_mutex_unlock(&lock_log);
}
