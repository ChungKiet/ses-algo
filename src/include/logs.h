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
#ifndef __LOGS_H__
#define __LOGS_H__

#include "timevec.h"

#include <stdio.h>

FILE *logfd;
void logs_init(const char *filename);
void logs_exit();

void term_errexit(const char *reason);
void term_delivered(int proc, const char *msg, int type);
void term_delayed(int proc);

void logs_errexit(const char *reason);
void logs_delivered(int proc, const char *msg, int type, int lvl, timevec_t *ts);
void logs_delayed(int proc, timevec_t *ts);

#endif
