#pragma once
#ifndef __DEFS_H__
#define __DEFS_H__

#include <pthread.h>

#ifndef NULL
#define NULL 0
#endif

typedef struct host {
	char ipv4[16];
	int  port;
} host_t;

typedef struct timevec {
	int  sz;
	int *vt;
} timevec_t, vectorclock_t;

typedef struct sentnode {
	int proc; 
	timevec_t timestamp;
	struct sentnode *next;
} sentnode_t;

typedef struct sentvec {
	int size;
	sentnode_t *root; 
} sentvec_t;

extern int           n;
extern int           id;
extern host_t       *proc_list;
extern int           nmsg;
extern int           rmin;
extern int           rmax;
extern char          log_fname[255];
extern int           lport;
extern int           servfd;
extern sentvec_t     vect_curr;
extern vectorclock_t time_curr;
extern pthread_t        trr[1024];
extern pthread_rwlock_t lock_n;
extern pthread_rwlock_t lock_id;
extern pthread_rwlock_t lock_vect;
extern pthread_rwlock_t lock_time;
extern pthread_mutex_t  lock_log;

#endif
