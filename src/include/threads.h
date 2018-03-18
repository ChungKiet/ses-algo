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
#ifndef __THREADS_H__
#define __THREADS_H__

typedef struct args {
	int  proc;
	char ipv4[16];
	int  port;
	int  nmsg; /* number of messages */
	int  rate; /* messages per minutes */
} threadargs_t;

/*
 * entry point for new messages sender thread
 * return NULL on error
 * or an address to a dynamically allocated integer
 * contains number of messages sent on success
 * this address SHOULD be freed later to avoid memory leak
 */
void *start_sender(void *args_addr);

#endif
