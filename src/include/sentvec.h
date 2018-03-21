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

#include "defs.h"

/*
 * Initialize a sent messages vector for _process_
 * with 0 element
 * Return 1 on success or -1 on error
 */
int sv_init(sentvec_t *dst);

void sv_free(sentvec_t *dst);

/*
 * Construct a sent messages vector from string _src_
 * Return next position on success or NULL on error
 */
char *sv_construct(sentvec_t *dst, char *src);

/*
 * Convert vector to string
 * Return number of characters on success
 * or -1 on error
 */
int sv_tostring(char *dst, const sentvec_t *src);

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

/*
 * Override timestamp of process _proc_ in sent vector with timestamp _src_
 * Return 1 on success or -1 on error
 */
int sv_override(sentvec_t *dst, timevec_t *src, int proc);

#endif
