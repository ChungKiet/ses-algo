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

#include <stdio.h>

FILE *logfd;
void logs_init(const char *filename);
void logs_exit();

void term_errexit(const char *reason);
void term_delivered();
void term_delayed();

void logs_errexit(const char *reason);
void logs_delivered();
void logs_delayed();

#endif
