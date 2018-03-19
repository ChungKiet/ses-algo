/*
 * Project 2 - Advanced Operating System
 * CAUSAL MESSAGE ORDERING IN DISTRIBUTED SYSTEM
 * Using Schiper-Eggli-Sandoz Algorithm
 *
 * 1512284 - Ha Tan Linh
 * Email: hatanlinh13@gmail.com
 *
 * Module: Configuration
 */
#include "config.h"
#include "defs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void config_default()
{
	n     = 15;
	id    = 0;
	nmsg  = 150;
	rmin  = 40;
	rmax  = 100;
	lport = 1400;
	proc_list = (host_t *)malloc(n * sizeof(host_t));
	for (int i = 0; i < n; i++) {
		strncpy(proc_list[i].ipv4, "127.0.0.1", 16);
		proc_list[i].port = 1400 + i;
	}
	sprintf(log_fname, "process_%d.log", id);
}

void config_get_from_file(const char *filename)
{
}
