#pragma once
#ifndef __TIMEVEC_H__
#define __TIMEVEC_H__

#include "defs.h"

typedef enum {
	TV_DIFF, 
	TV_GREQ, 
	TV_LESS
} tv_cmp_rslt;

int tv_init(timevec_t *dst, int size, int val);

void tv_free(timevec_t *dst);

char *tv_construct(timevec_t *dst, char *src);

int tv_tostring(char *dst, const timevec_t *src);

int tv_get(const timevec_t *src, int pos);

int tv_set(const timevec_t *dst, int pos, int val);

tv_cmp_rslt tv_compare(const timevec_t *a, const timevec_t *b);

int tv_combine(timevec_t *dst, timevec_t *src);

#endif
