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
#include "timevec.h"
#include "sentvec.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

void print_vclock(timevec_t *ts);
void print_space(int lvl);
void print_arrow();

static int fin_count = 0;
void graceful_exit()
{
	pthread_rwlock_rdlock(&lock_n);
	int nn = n;
	pthread_rwlock_unlock(&lock_n);

	pthread_rwlock_rdlock(&lock_id);
	int dd = id;
	pthread_rwlock_unlock(&lock_id);

	for (int i = 0; i < nn; i++)
		if (i != dd) {
			pthread_join(trr[i], NULL);
		}
	tv_free(&time_curr);
	sv_free(&vect_curr);
	free(proc_list);
	logs_exit();
	exit(EXIT_SUCCESS);
}


void logs_init(const char *filename)
{
	logfd = fopen(filename, "w");
	fprintf(logfd, "LOGS file of process #%d\n~~~~\n", id);
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
	fprintf(logfd, "\nX An error occured: %s\n", reason);
	fprintf(logfd, "X Program exited!\n");
	pthread_mutex_unlock(&lock_log);
}

void term_delayed(int proc)
{
	fprintf(stdout, "Received a message from process #%d.\n", proc);
	fprintf(stdout, "----DELAYED, message is now in buffer.\n");
}

void logs_delayed(int proc, timevec_t *ts)
{
	fprintf(logfd, "\n");
	fprintf(logfd, "# Clock     : ");
	print_vclock(&time_curr);
	fprintf(logfd, "\n");
	fprintf(logfd, "# Event     : Received a message. DELAYED! Message is now in buffer.\n");
	fprintf(logfd, "# Sender    : Process #%d\n", proc);
	fprintf(logfd, "# Timestamp : ");
	print_vclock(ts);
	fprintf(logfd, "\n");
}

void term_delivered(int proc, const char *msg, int type)
{
	if (strncmp(msg, "FIN:SEND", 8) == 0) {
		pthread_rwlock_rdlock(&lock_n);
		int nn = n - 1;
		pthread_rwlock_unlock(&lock_n);
		if (++fin_count == nn)
			graceful_exit();
		return;
	}

	if (type == 0) { /* new message */
		fprintf(stdout, "Received a message from process #%d.\n", proc);
		fprintf(stdout, "----DELIVERED immediately! Content: %s\n", msg);
	}
	else { /* from buffer */
		fprintf(stdout, "----DELIVERED a message of process #%d", proc);
		fprintf(stdout, " from buffer. Content: %s\n", msg);
	}
}

void logs_delivered(int proc, const char *msg, int type, int lvl, timevec_t *ts)
{
	if (strncmp(msg, "FIN:SEND", 8) == 0)
		return;

	if (type == 0) { /* new message */
		fprintf(logfd, "\n");
		fprintf(logfd, "# Clock     : ");
		print_vclock(&time_curr);
		fprintf(logfd, "\n");
		fprintf(logfd, "# Event     : Received a message. DELIVERED immediately!\n");
		fprintf(logfd, "# Sender    : Process #%d\n", proc);
		fprintf(logfd, "# Timestamp : ");
		print_vclock(ts);
		fprintf(logfd, "\n");
		fprintf(logfd, "# Content   : %s\n", msg);
	}
	else { /* from buffer */
		print_space(lvl - 1);
		print_arrow();
		fprintf(logfd, "# Clock     : ");
		print_vclock(&time_curr);
		fprintf(logfd, "\n");
		print_space(lvl);
		fprintf(logfd, "# Event     : DELIVERED a message from buffer.\n");
		print_space(lvl);
		fprintf(logfd, "# Sender    : Process #%d\n", proc);
		print_space(lvl);
		fprintf(logfd, "# Timestamp : ");
		print_vclock(ts);
		fprintf(logfd, "\n");
		print_space(lvl);
		fprintf(logfd, "# Content   : %s\n", msg);
	}
}

void print_arrow()
{
	fprintf(logfd, "--->");
}

void print_space(int lvl)
{
	for (int i = 0; i < lvl; i++)
		fprintf(logfd, "    ");
}

void print_vclock(timevec_t *ts)
{
	fprintf(logfd, "{");
	for (int i = 0; i < ts->sz - 1; i++)
		fprintf(logfd, "%d, ", ts->vt[i]);
	fprintf(logfd, "%d}", ts->vt[ts->sz - 1]);
}
