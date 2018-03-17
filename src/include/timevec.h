/*
 * Project 2 - Advanced Operating System
 * CAUSAL MESSAGE ORDERING IN DISTRIBUTED SYSTEM
 * Using Schiper-Eggli-Sandoz Algorithm
 *
 * 1512284 - Ha Tan Linh
 * Email: hatanlinh13@gmail.com
 *
 * Module: Timestamp Vector
 */
#ifndef __TIMEVEC_H__
#define __TIMEVEC_H__

#include "defs.h"

typedef enum {
	TV_DIFF, /* Two vectors are different (in size) */
	TV_GREQ, /* Greater or equal */
	TV_LESS
} tv_cmp_rslt;

/*
 * Initialize a timestamp vector with _size_ elements of value _val_
 * Return size on success or -1 on error
 */
int tv_init(timevec_t *dst, int size, int val);

void tv_free(timevec_t *dst);

/*
 * Construct a timestamp vector whose values are extracted from _src_
 * Return size on success or -1 on error
 */
int tv_construct(timevec_t *dst, const char *src);

/*
 * Convert a timestamp vector to string
 * Return number of characters on success
 * or -1 on error
 */
int tv_tostring(char *dst, const timevec_t *src);

/*
 * Return current value at _pos_
 * or -1 if _pos_ is invalid
 */
int tv_get(const timevec_t *src, int pos);

/*
 * Set element at _pos_ to _val_
 * Return _pos_ on success or -1 on error
 */
int tv_set(const timevec_t *dst, int pos, int val);

/* Compare two timestamp vectors */
tv_cmp_rslt tv_compare(const timevec_t *a, const timevec_t *b);

/*
 * Max combine two vectors
 * Return 1 on success or -1 on error
 */
int tv_combine(timevec_t *dst, timevec_t *src);

#endif
