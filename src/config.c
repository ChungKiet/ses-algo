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
#include "logs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_MAX 2048

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
	FILE *fp = fopen(filename, "r");
	if (fp == NULL) {
		logs_errexit("Cannot open config file.");
		term_errexit("Cannot open config file.");
	}
	char line[LINE_MAX];
	while (fgets(line, LINE_MAX, fp)) {
		char *oc;
		if ((oc = strstr(line, "n:")) != NULL) {
			sscanf(line + 2, "%d", &n);
		}
		if ((oc = strstr(line, "id:")) != NULL) {
			sscanf(line + 3, "%d", &id);
		}
		if ((oc = strstr(line, "nmsg:")) != NULL) {
			sscanf(line + 5, "%d", &nmsg);
		}
		if ((oc = strstr(line, "rmin:")) != NULL) {
			sscanf(line + 5, "%d", &rmin);
		}
		if ((oc = strstr(line, "rmax:")) != NULL) {
			sscanf(line + 5, "%d", &rmax);
		}
		if ((oc = strstr(line, "lport:")) != NULL) {
			sscanf(line + 6, "%d", &lport);
		}
		if ((oc = strstr(line, "log:")) != NULL) {
			char *nl = strrchr(line, '\n');
			if (nl) {
				strncpy(log_fname, line + 4, nl - line - 4);
				log_fname[nl - line - 4] = '\0';
			}
			else {
				strncpy(log_fname, line + 4, 255);
			}
		}
		if ((oc = strstr(line, "process:")) != NULL) {
			int ii;
			sscanf(oc + 8, "%d", &ii);
			oc = strchr(oc + 8, ':');
			char *od = strchr(oc + 1, ':');
			strncpy(proc_list[ii].ipv4, oc + 1, od - oc - 1);
			proc_list[ii].ipv4[od - oc - 1] = '\0';
			sscanf(od + 1, "%d", &proc_list[ii].port);
		}
	}
	fclose(fp);
}
