/*
 * Copyright (c) 2012  Charles Hardin <ckhardin@gmail.com>
 * All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
/**
 * @file map.c
 *
 * Provide a C library for that emulates the standard template map
 * functionality in C++. This implementation is fairly simple
 * reference for for a name/value container.
 */

#include <sys/types.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>

#include "map.h"


int
map_init(map_t *map)
{
	return ENOSYS;
}


int
map_destroy(map_t *map)
{
	return ENOSYS;
}


int
map_insert(map_t *map, const char *key, const void *val)
{
	return ENOSYS;
}


int
map_erase(map_t *map, const char *key)
{
	return ENOSYS;
}


int
map_update(map_t *map, const map_t *other)
{
	return ENOSYS;
}


int
map_clear(map_t *map)
{
	return ENOSYS;
}

mapnode_t *
map_find(const map_t *map, const char *key)
{
	int cmp, dir;
	mapnode_t *p;
	mapnode_t *mn;

	if (!map || !key)
		return NULL;

	mn = map->m_root;
	while (mn != NULL) {
		cmp = strcmp(key, mn->mn_key);
		if (cmp == 0) {
			/* match and we are it */
			return mn;
		}

		dir = cmp > 0; /* left - false(0), right - true(1) */
		mn = mn->mn_child[dir];
	}
	return NULL;
}

mapnode_t *
map_first(const map_t *map)
{
	mapnode_t *mn;

	if (!map)
		return NULL;

	mn = map->m_root;
	while (mn != NULL) {
		if (mn->mn_child[0] == NULL) /* left */
			break;
		mn = mn->mn_child[0];
	}
	return mn;
}

mapnode_t *
map_last(const map_t *map)
{
	mapnode_t *mn;

	if (!map)
		return NULL;

	mn = map->m_root;
	while (mn != NULL) {
		if (mn->mn_child[1] == NULL) /* right */
			break;
		mn = mn->mn_child[1];
	}
	return mn;
}

mapnode_t *
map_next(const mapnode_t *node)
{
	mapnode_t *p;
	mapnode_t *mn;

	if (!node)
		return NULL;

	if (node->mn_child[1] != NULL) {
		mn = node->mn_child[1]; /* right child */
		while (mn->mn_child[0] != NULL)
			mn = mn->mn_child[0]; /* go left all the way */
		return mn;
	}

	/* In the red-black tree, the insert guarantess that everything to
	 * left and down is a "smaller" - so, go up to the tree to find
	 * next node by:
	 * - if the node is a right child of the parent keep going
	 * - parent is the next node if it is the left child of the parent
	 */
	p = node->mn_parent;
	while (p != NULL) {
		if (mn == p->mn_child[0])
			break;
		mn = p;
		p = mn->mn_parent;
	}
	return p;
}

mapnode_t *
map_prev(const mapnode_t *node)
{
	mapnode_t *p;
	mapnode_t *mn;

	if (!node)
		return NULL;

	if (node->mn_child[1] != NULL) {
		mn = node->mn_child[1]; /* right child */
		while (mn->mn_child[0] != NULL)
			mn = mn->mn_child[0]; /* go left all the way */
		return mn;
	}

	/* In the red-black tree, the insert guarantess that everything to
	 * left and down is a "smaller" - so, go up to the tree to find
	 * next node by:
	 * - if the node is a left child of the parent keep going
	 * - parent is the next node if it is the right child of the parent
	 *
	 * Opposite of the next function
	 */
	p = node->mn_parent;
	while (p != NULL) {
		if (mn == p->mn_child[1])
			break;
		mn = p;
		p = mn->mn_parent;
	}
	return p;
}
