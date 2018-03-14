/*
 * Project 2 - Advanced Operating System
 * CAUSAL MESSAGE ORDERING IN DISTRIBUTED SYSTEM
 * Using Schiper-Eggli-Sandoz Algorithm
 *
 * 1512284 - Ha Tan Linh
 * Email: hatanlinh13@gmail.com
 *
 * Module: Sent Messages Vector
 */
#ifndef __SENTVEC_H__
#define __SENTVEC_H__

#include "timevec.h"

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
	int proc;
	sentnode_t *root; /* linked list */
} sentvec_t;

/*
 * Initialize a sent messages vector for _process_
 * with 0 element
 * Return 1 on success or -1 on error
 */
int sv_init(sentvec_t *dst, int process);

void sv_free(sentvec_t *dst);

/*
 * Construct a sent messages vector from string _src_
 * Return process id on success or -1 on error
 */
int sv_construct(sentvec_t *dst, const char *src);

void sv_tostring(char *dst, const sentvec_t *src);

/*
 * Search for a timestamp vector of process _proc_ from sent vector
 * Return pointer to that timestamp vector if found
 * or NULL on error
 */
timevec_t *sv_search(const sentvec_t *src, int proc);

/*
 * Make a copy of timestamp _src_ and
 * add this timestamp with process indentifier _proc_ to the sent vector
 * Return 1 on success
 * 0 if _proc_ is already in sent vec
 * or -1 on error
 */
int sv_add(sentvec_t *dst, const timevec_t *src, int proc);

/*
 * Max combine timestamp of process _proc_ in sent vector with timestamp _src_
 * Return 1 on success or -1 on error
 */
int sv_update(sentvec_t *dst, timevec_t *src, int proc);

#endif
