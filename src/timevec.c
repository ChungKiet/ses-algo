#include "timevec.h"

#include <stdlib.h>
#include <stdio.h>

int tv_init(timevec_t *dst, int size, int val)
{
	if (size < 0 || dst == NULL)
		return -1;
	dst->sz = size;
	dst->vt = (int *)malloc(dst->sz * sizeof(int));
	if (dst->vt == NULL)
		return -1;
	for (int i = 0; i < dst->sz; i++)
		dst->vt[i] = val;
	return dst->sz;
}

void tv_free(timevec_t *dst)
{
	dst->sz = -1;
	free(dst->vt);
}

char *tv_construct(timevec_t *dst, char *src)
{
	if (dst == NULL || dst->sz < 0)
		return NULL;
	for (int i = 0; i < dst->sz; i++) {
		char *endptr;
		dst->vt[i] = strtol(src, &endptr, 0);
		src = endptr;
	}
	return src;
}

int tv_tostring(char *dst, const timevec_t *src)
{
	if (dst == NULL || src == NULL)
		return -1;
	int count = 0;
	for (int i = 0; i < src->sz; i++) {
		int cp = sprintf(dst, "%d ", src->vt[i]);
		count += cp;
		dst   += cp;
	}
	return count;
}

int tv_get(const timevec_t *src, int pos)
{
	if (src == NULL || pos >= src->sz || pos < 0)
		return -1;
	return src->vt[pos];
}

int tv_set(const timevec_t *dst, int pos, int val)
{
	if (dst == NULL || pos >= dst->sz || pos < 0)
		return -1;
	return dst->vt[pos] = val;
}

tv_cmp_rslt tv_compare(const timevec_t *a, const timevec_t *b)
{
	if (a == NULL || b == NULL || a->sz != b->sz)
		return TV_DIFF;
	for (int i = 0; i < a->sz; i++) {
		if (a->vt[i] < b->vt[i])
			return TV_LESS;
	}
	return TV_GREQ;
}

int tv_combine(timevec_t *dst, timevec_t *src)
{
	if (dst == NULL || src == NULL || dst->sz != src->sz)
		return -1;
	for (int i = 0; i < dst->sz; i++)
		dst->vt[i] = dst->vt[i] >= src->vt[i] ? dst->vt[i] : src->vt[i];
	return 1;
}
