#pragma once
#ifndef __THREADS_H__
#define __THREADS_H__

typedef struct args {
	int  proc;
	char ipv4[16];
	int  port;
	int  nmsg; 
	int  rate; 
} threadargs_t;

void *start_sender(void *args_addr);

void  threads_init();

#endif
