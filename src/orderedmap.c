/*
 * Copyright (c) 2012-2023  Charles Hardin <ckhardin@gmail.com>
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
 * @file orderedmap.c
 *
 * Provide a C library for that emulates the standard template map
 * functionality in C++. This implementation is fairly simple
 * reference for for a name/value container.
 *
 * The insertion and removal is using a RED-BLACK tree derived from
 * looking thru OpenBSD's sys/tree.h
 */

#include <sys/types.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <assert.h>

#include <libcmap/orderedmap.h>

#ifndef	__DECONST
#define	__DECONST(type, var)	((type)(uintptr_t)(const void *)(var))
#endif

struct orderedmapnode {
	enum {
		OMN_RED = 0,
		OMN_BLACK = 1
	} omn_color;
	struct orderedmapnode *omn_parent;
	struct orderedmapnode *omn_child[2]; /* 0 - left and 1 - right */

	const char *omn_key;
	const char *omn_val;
};


static bool
_orderedmap_isred(const struct orderedmapnode *node)
{
	if (node == NULL)
		return false; /* NULL is considered black */
	return node->omn_color == OMN_RED;
}


static void
_orderedmap_rotate(struct orderedmap *map,
		   struct orderedmapnode *node, int dir)
{
	struct orderedmapnode *p;
	struct orderedmapnode *ntmp;

	p = node->omn_parent;
	ntmp = node->omn_child[!dir];

	node->omn_child[!dir] = ntmp->omn_child[dir];
	if (ntmp->omn_child[dir] != NULL)
		ntmp->omn_child[dir]->omn_parent = node;

	ntmp->omn_parent = p;

	if (p != NULL) {
		/* update the parent pointer, this is same in both directions */
		if (node == p->omn_child[0])
			p->omn_child[0] = ntmp;
		else
			p->omn_child[1] = ntmp;
	} else
		map->om_root = ntmp;

	ntmp->omn_child[dir] = node;
	node->omn_parent = ntmp;
	return;
}


static void
_orderedmap_fixup(struct orderedmap *map,
		  struct orderedmapnode *parent,
		  struct orderedmapnode *node)
{
	int dir;
	struct orderedmapnode *p;
	struct orderedmapnode *o;
	struct orderedmapnode *ntmp;

	p = parent;
	ntmp = node;
	while (!_orderedmap_isred(ntmp) && ntmp != map->om_root) {
		dir = ntmp == p->omn_child[0];
		o = p->omn_child[dir];
		if (_orderedmap_isred(o)) {
			o->omn_color = OMN_BLACK;
			o->omn_color = OMN_RED;
			_orderedmap_rotate(map, p, !dir); /* rotate otherway */
			o = p->omn_child[dir];
		}

		if (!_orderedmap_isred(o->omn_child[0]) &&
		    !_orderedmap_isred(o->omn_child[1])) {
			o->omn_color = OMN_RED;
			ntmp = p;
			p = ntmp->omn_parent;
			continue;
		}

		if (!_orderedmap_isred(o->omn_child[dir])) {
			if (o->omn_child[!dir] != NULL)
				o->omn_child[!dir]->omn_color = OMN_BLACK;
			o->omn_color = OMN_RED;
			_orderedmap_rotate(map, o, dir); /* rotate sameway */
			o = p->omn_child[dir];
		}

		o->omn_color = p->omn_color;
		p->omn_color = OMN_BLACK;
		if (o->omn_child[dir] != NULL)
			o->omn_child[dir]->omn_color = OMN_BLACK;
		_orderedmap_rotate(map, p, !dir); /* rotate otherway */
		ntmp = map->om_root;
		break;
	}
	if (ntmp != NULL)
		ntmp->omn_color = OMN_BLACK;
	return;
}


static void
_orderedmap_remove(struct orderedmap *map, struct orderedmapnode *node)
{
	int color;
	struct orderedmapnode *p;
	struct orderedmapnode *c;

	if (node->omn_child[0] == NULL)
		c = node->omn_child[1]; /* use right */
	else if (node->omn_child[1] == NULL)
		c = node->omn_child[0]; /* use left */
	else {
		struct orderedmapnode *ntmp;

		/* both children are null */
		ntmp = orderedmap_next(node);

		c = ntmp->omn_child[1];
		p = ntmp->omn_parent;
		color = ntmp->omn_color;
		if (c != NULL)
			c->omn_parent = p;
		if (p != NULL) {
			if (ntmp == p->omn_child[0])
				p->omn_child[0] = c;
			else
				p->omn_child[1] = c;
		} else
			map->om_root = c;
		if (p == node)
			p = ntmp;

		ntmp->omn_color = node->omn_color;
		ntmp->omn_parent = node->omn_parent;
		ntmp->omn_child[0] = node->omn_child[0];
		ntmp->omn_child[1] = node->omn_child[1];
		if (node->omn_parent != NULL) {
			if (node == node->omn_parent->omn_child[0])
				node->omn_parent->omn_child[0] = ntmp;
			else
				node->omn_parent->omn_child[1] = ntmp;
		} else
			map->om_root = ntmp;
		node->omn_child[0]->omn_parent = ntmp;
		if (node->omn_child[1] != NULL)
			node->omn_child[1]->omn_parent = ntmp;
		goto color;
	}

	p = node->omn_parent;
	color = node->omn_color;
	if (c != NULL)
		c->omn_parent = p;
	if (p != NULL) {
		if (node == p->omn_child[0])
			p->omn_child[0] = c;
		else
			p->omn_child[1] = c;
	} else
		map->om_root = c;

 color:
	if (color == OMN_BLACK)
		_orderedmap_fixup(map, p, c);
	map->om_numnodes--;
	node->omn_parent = NULL;
	node->omn_child[0] = NULL;
	node->omn_child[1] = NULL;
	return;
}


int
orderedmap_init(orderedmap_t *map)
{
	if (!map)
		return EINVAL;

	map->om_numnodes = 0;
	map->om_root = NULL;
	return 0;
}


int
orderedmap_destroy(orderedmap_t *map)
{
	int err;

	err = orderedmap_clear(map);
	if (err != 0)
		return err;

	assert(map->om_root == NULL);
	assert(map->om_numnodes == 0);
	return 0;
}


int
orderedmap_insert(orderedmap_t *map, const char *key, const void *val)
{
	int cmp, dir;
	size_t keysz;
	size_t valsz;
	struct orderedmapnode *node;
	struct orderedmapnode *nnew;
	struct orderedmapnode *uncle;
	struct orderedmapnode *p, *gp;
	char *kptr, *vptr;

	if (!map || !key || !val)
		return EINVAL;

	keysz = strlen(key);
	valsz = strlen(val);
	if (map->om_root == NULL) {
		nnew = malloc(sizeof(*nnew) + keysz + valsz);
		if (nnew == NULL)
			return ENOMEM;
		memset(nnew, 0, sizeof(*nnew));

		kptr = (char *)&nnew[1];
		vptr = &kptr[keysz];
		memcpy(kptr, key, keysz);
		memcpy(vptr, val, valsz);
		nnew->omn_key = kptr;
		nnew->omn_val = vptr;

		map->om_root = nnew;
		map->om_root->omn_color = OMN_BLACK;
		map->om_numnodes++;
		return 0;
	}

	node = map->om_root;
	for (;;) {
		cmp = strcmp(key, node->omn_key);
		if (cmp == 0) {
			/* match and we are it */
			return EPERM;
		}

		dir = cmp > 0; /* left - false(0), right - true(1) */
		if (node->omn_child[dir] != NULL) {
			node = node->omn_child[dir];
			continue;
		}

		/* insert the key value pair into the tree */
		nnew = malloc(sizeof(*nnew) + keysz + valsz);
		if (nnew == NULL)
			return ENOMEM;
		memset(nnew, 0, sizeof(*nnew));

		kptr = (char *)&nnew[1];
		vptr = &kptr[keysz];
		memcpy(kptr, key, keysz);
		memcpy(vptr, val, valsz);
		nnew->omn_key = kptr;
		nnew->omn_val = vptr;

		node->omn_child[dir] = nnew;
		nnew->omn_parent = node;
		nnew->omn_color = OMN_RED;
		break;
	}

	/* rebalance the tree if required */
	node = nnew;
	while ((p = node->omn_parent) != NULL) {
		if (!_orderedmap_isred(p))
			break;

		gp = p->omn_parent;
		assert(gp != NULL);

		dir = (p == gp->omn_child[0]);
		uncle = gp->omn_child[dir];
		if (uncle != NULL && _orderedmap_isred(uncle)) {
			uncle->omn_color = OMN_BLACK;
			p->omn_color = OMN_BLACK;
			gp->omn_color = OMN_RED;
			node = gp;
			continue;
		}

		if (node == p->omn_child[dir]) {
			register struct orderedmapnode *ntmp;

			_orderedmap_rotate(map, p, !dir); /* rotate otherway */
			ntmp = p;
			p = node;
			node = ntmp;
		}

		p->omn_color = OMN_BLACK;
		gp->omn_color = OMN_RED;
		_orderedmap_rotate(map, gp, dir); /* rotate sameway */
	}

	map->om_root->omn_color = OMN_BLACK;
	map->om_numnodes++;
	return 0;
}


int
orderedmap_erase(orderedmap_t *map, const char *key)
{
	struct orderedmapnode *node;

	node = orderedmap_find(map, key);
	if (node == NULL)
		return ENOENT;

	_orderedmap_remove(map, node);
	free(node);
	return 0;
}


int
orderedmap_update(orderedmap_t *map, const orderedmap_t *other)
{
	int err;
	struct orderedmapnode *node;

	if (!map || !other)
		return EINVAL;

	for (node = orderedmap_first(other);
	     node != NULL;
	     node = orderedmap_next(node)) {
		orderedmap_erase(map, node->omn_key);
		err = orderedmap_insert(map, node->omn_key, node->omn_val);
		if (err != 0) {
			return err;
		}
	}
	return 0;
}


int
orderedmap_clear(orderedmap_t *map)
{
	struct orderedmapnode *node;

	if (!map)
		return EINVAL;

	node = map->om_root;
	while (node != NULL) {
		_orderedmap_remove(map, node);
		free(node);

		node = map->om_root;
	}
	return 0;
}


orderedmapnode_t *
orderedmap_find(const orderedmap_t *map, const char *key)
{
	int cmp;
	int dir;
	struct orderedmapnode *node;

	if (!map || !key)
		return NULL;

	node = map->om_root;
	while (node != NULL) {
		cmp = strcmp(key, node->omn_key);
		if (cmp == 0) {
			/* match and we are it */
			return node;
		}

		dir = cmp > 0; /* left - false(0), right - true(1) */
		node = node->omn_child[dir];
	}
	return NULL;
}


orderedmapnode_t *
orderedmap_first(const orderedmap_t *map)
{
	struct orderedmapnode *node;

	if (!map)
		return NULL;

	node = map->om_root;
	while (node != NULL) {
		if (node->omn_child[0] == NULL) /* left */
			break;
		node = node->omn_child[0];
	}
	return node;
}


orderedmapnode_t *
map_last(const orderedmap_t *map)
{
	struct orderedmapnode *node;

	if (!map)
		return NULL;

	node = map->om_root;
	while (node != NULL) {
		if (node->omn_child[1] == NULL) /* right */
			break;
		node = node->omn_child[1];
	}
	return node;
}


orderedmapnode_t *
orderedmap_next(const orderedmapnode_t *node)
{
	struct orderedmapnode *p;
	struct orderedmapnode *ntmp;

	if (node == NULL)
		return NULL;

	if (node->omn_child[1] != NULL) {
		ntmp = node->omn_child[1]; /* right child */
		while (ntmp->omn_child[0] != NULL)
			ntmp = ntmp->omn_child[0]; /* go left all the way */
		return ntmp;
	}

	/* The insert guarantees that everything left and down is "smaller"
	 * So, go up the tree to find the next node by:
	 * - if the node is a right child of the parent keep going
	 * - if the node is a left child of the parent, then the parent
	 *   is the next node
	 */
	ntmp = __DECONST(typeof(ntmp), node);
	p = node->omn_parent;
	while (p != NULL) {
		if (ntmp == p->omn_child[0])
			break;
		ntmp = p;
		p = ntmp->omn_parent;
	}
	return p;
}


orderedmapnode_t *
map_prev(const orderedmapnode_t *node)
{
	struct orderedmapnode *p;
	struct orderedmapnode *ntmp;

	if (!node)
		return NULL;

	if (node->omn_child[1] != NULL) {
		ntmp = node->omn_child[1]; /* right child */
		while (ntmp->omn_child[0] != NULL)
			ntmp = ntmp->omn_child[0]; /* go left all the way */
		return ntmp;
	}

	/* The insert guarantees that everything left and down is "smaller"
	 * So, go up the tree to find the previous node by:
	 * - if the node is a left child of the parent keep going
	 * - if the node is a right child of the parent, then the parent
	 *   is the previous node
	 *
	 * Opposite of the next function
	 */
	ntmp = __DECONST(typeof(ntmp), node);
	p = node->omn_parent;
	while (p != NULL) {
		if (ntmp == p->omn_child[1])
			break;
		ntmp = p;
		p = ntmp->omn_parent;
	}
	return p;
}
