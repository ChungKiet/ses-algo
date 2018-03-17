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
#include "sentvec.h"
#include "timevec.h"

#include <stdlib.h>
#include <stdio.h>

int sv_init(sentvec_t *dst)
{
	if (dst == NULL)
		return -1;
	dst->size = 0;
	dst->root = NULL;
	return 1;
}

void sv_free(sentvec_t *dst)
{
	if (dst == NULL)
		return;
	sentnode_t *node = dst->root;
	dst->size = 0;
	dst->root = NULL;
	while (node) {
		tv_free(&node->timestamp);
		sentnode_t *next = node->next;
		free(node);
		node = next;
	}
}

int sv_construct(sentvec_t *dst, const char *src)
{
	if (dst == NULL || src == NULL)
		return -1;
	char *endptr;
	dst->size = strtol(src, &endptr, 0);
	src = endptr;
	for (int i = 0; i < dst->size; i++) {
		sentnode_t *new_node = (sentnode_t *)malloc(sizeof(sentnode_t));
		if (new_node == NULL)
			return -1;
		new_node->proc = strtol(src, &endptr, 0);
		src = endptr;
		if (tv_init(&new_node->timestamp, n, 1) == -1)
			return -1;
		for (int j = 0; j < n; j++) {
			new_node->timestamp.vt[j] = strtol(src, &endptr, 0);
			src = endptr;
		}
		new_node->next = dst->root;
		dst->root = new_node;
	}
	return dst->size;
}

int sv_tostring(char *dst, const sentvec_t *src)
{
	if (dst == NULL || src == NULL)
		return -1;
	int count = sprintf(dst, "%d ", src->size);
	dst += count;
	sentnode_t *node = src->root;
	while (node) {
		int cp = sprintf(dst, "%d ", node->proc);
		count += cp;
		dst   += cp;
		cp = tv_tostring(dst, &node->timestamp);
		count += cp;
		dst   += cp;
		node = node->next;
	}
	return count;
}

timevec_t *sv_search(const sentvec_t *src, int proc)
{
	if (proc < 0 || src == NULL)
		return NULL;
	sentnode_t *node = src->root;
	while (node) {
		if (node->proc == proc)
			return &node->timestamp;
		node = node->next;
	}
	return NULL;
}

int sv_add(sentvec_t *dst, const timevec_t *src, int proc)
{
	if (proc < 0 || dst == NULL || src == NULL || src->vt == NULL)
		return -1;
	/* _proc_ is already in _dst_ */
	if (sv_search(dst, proc) != NULL)
		return 0;
	sentnode_t *new_node = (sentnode_t *)malloc(sizeof(sentnode_t));
	if (new_node == NULL)
		return -1;
	new_node->proc = proc;
	if (tv_init(&new_node->timestamp, n, 1) == -1)
		return -1;
	for (int i = 0; i < n; i++) {
		int val = tv_get(src, i);
		if (val == -1)
			return -1;
		if (tv_set(&new_node->timestamp, i, val) == -1)
			return -1;
	}
	new_node->next = dst->root;
	dst->root = new_node;
	dst->size++;
	return 1;
}

int sv_update(sentvec_t *dst, timevec_t *src, int proc)
{
	if (proc < 0 || dst == NULL || src == NULL || src->vt == NULL)
		return -1;
	sentnode_t *node = dst->root;
	while (node) {
		if (node->proc == proc) {
			return tv_combine(&node->timestamp, src);
		}
		node = node->next;
	}
	return -1;
}
