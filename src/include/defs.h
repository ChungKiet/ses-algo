/*
 * Project 2 - Advanced Operating System
 * CAUSAL MESSAGE ORDERING IN DISTRIBUTED SYSTEM
 * Using Schiper-Eggli-Sandoz Algorithm
 *
 * 1512284 - Ha Tan Linh
 * Email: hatanlinh13@gmail.com
 *
 * Module: Global Definitions
 */
#ifndef __DEFS_H__
#define __DEFS_H__

#define NULL 0

typedef struct host {
	char ipv4[16];
	int  port;
} host_t;

typedef struct timevec {
	int  sz; /* size (i.e. number of processes in system) */
	int *vt;
} timevec_t, vectorclock_t; /* vector clock is the same as timestamp vector */

/* Considered results of comparing two vectors */
/*
 * Sent message vector is implemented as a linked list
 * where each node contains a process identifier
 * and a timestamp vector of lastest message sent by this process
 */
typedef struct sentnode {
	int proc; /* process identifier */
	timevec_t timestamp;
	struct sentnode *next;
} sentnode_t;

typedef struct sentvec {
	int size;
	sentnode_t *root; /* linked list */
} sentvec_t;

/* defined in main */
extern int           n;
extern int           id;
extern host_t       *proc_list;
extern sentvec_t     vect_curr;
extern vectorclock_t time_curr;

#endif
