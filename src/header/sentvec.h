#pragma once
#ifndef __SENTVEC_H__
#define __SENTVEC_H__

#include "defs.h"

int sv_init(sentvec_t *dst);

void sv_free(sentvec_t *dst);

char *sv_construct(sentvec_t *dst, char *src);

int sv_tostring(char *dst, const sentvec_t *src);

timevec_t *sv_search(const sentvec_t *src, int proc);

int sv_add(sentvec_t *dst, const timevec_t *src, int proc);

int sv_update(sentvec_t *dst, timevec_t *src, int proc);

int sv_override(sentvec_t *dst, timevec_t *src, int proc);

#endif
