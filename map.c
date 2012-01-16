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

#include "map.h"


static bool
_map_isred(const mapnode_t *mn)
{
	if (mn == NULL)
		return false; /* NULL is considered black */
	return mn->mn_color == MN_RED;
}


static void
_map_rotate(map_t *map, mapnode_t *mn, int dir)
{
        mapnode_t *p;
        mapnode_t *mntmp;

        p = mn->mn_parent;
        mntmp = mn->mn_child[!dir];

        mn->mn_child[!dir] = mntmp->mn_child[dir];
        if (mntmp->mn_child[dir] != NULL)
                mntmp->mn_child[dir]->mn_parent = mn;

        mntmp->mn_parent = p;

        if (p != NULL) {
                /* update the parent pointer, this is same in both directions */
                if (mn == p->mn_child[0])
                        p->mn_child[0] = mntmp;
                else
                        p->mn_child[1] = mntmp;
        } else
                map->m_root = mntmp;

        mntmp->mn_child[dir] = mn;
        mn->mn_parent = mntmp;
        return;
}


static void
_map_fixup(map_t *map, mapnode_t *parent, mapnode_t *node)
{
	int dir;
	mapnode_t *p;
	mapnode_t *o;
	mapnode_t *mn;

	p = parent;
	mn = node;
	while (!_map_isred(mn) && mn != map->m_root) {
		dir = mn == p->mn_child[0];
		o = p->mn_child[dir];
		if (_map_isred(o)) {
			o->mn_color = MN_BLACK;
			o->mn_color = MN_RED;
			_map_rotate(map, p, !dir); /* rotate otherway */
			o = p->mn_child[dir];
		}

		if (!_map_isred(o->mn_child[0]) &&
		    !_map_isred(o->mn_child[1])) {
			o->mn_color = MN_RED;
			mn = p;
			p = mn->mn_parent;
			continue;
		}

		if (!_map_isred(o->mn_child[dir])) {
			if (o->mn_child[!dir] != NULL)
				o->mn_child[!dir]->mn_color = MN_BLACK;
			o->mn_color = MN_RED;
			_map_rotate(map, o, dir); /* rotate sameway */
			o = p->mn_child[dir];
		}

		o->mn_color = p->mn_color;
		p->mn_color = MN_BLACK;
		if (o->mn_child[dir] != NULL)
			o->mn_child[dir]->mn_color = MN_BLACK;
		_map_rotate(map, p, !dir); /* rotate otherway */
		mn = map->m_root;
		break;
	}
	if (mn != NULL)
		mn->mn_color = MN_BLACK;
	return;
}


static mapnode_t *
_map_remove(map_t *map, mapnode_t *node)
{
	int color;
	mapnode_t *p;
	mapnode_t *c;
	mapnode_t *mn;

	if (node->mn_child[0] == NULL)
		c = node->mn_child[1]; /* use right */
	else if (node->mn_child[1] == NULL)
		c = node->mn_child[0]; /* use left */
	else {
		/* both children are null */
		mn = map_next(node);

		c = mn->mn_child[1];
		p = mn->mn_parent;
		color = mn->mn_color;
		if (c != NULL)
			c->mn_parent = p;
		if (p != NULL) {
			if (mn == p->mn_child[0])
				p->mn_child[0] = c;
			else
				p->mn_child[1] = c;
		} else
			map->m_root = c;
		if (p == node)
			p = mn;

		mn->mn_color = node->mn_color;
		mn->mn_parent = node->mn_parent;
		mn->mn_child[0] = node->mn_child[0];
		mn->mn_child[1] = node->mn_child[1];
		if (node->mn_parent != NULL) {
			if (node == node->mn_parent->mn_child[0])
				node->mn_parent->mn_child[0] = mn;
			else
				node->mn_parent->mn_child[1] = mn;
		} else
			map->m_root = mn;
		node->mn_child[0]->mn_parent = mn;
		if (node->mn_child[1] != NULL)
			node->mn_child[1]->mn_parent = mn;
		goto color;
	}

	p = mn->mn_parent;
	color = mn->mn_color;
	if (c != NULL)
		c->mn_parent = p;
	if (p != NULL) {
		if (node == p->mn_child[0])
			p->mn_child[0] = c;
		else
			p->mn_child[1] = c;
	} else
		map->m_root = c;

 color:
	if (color == MN_BLACK)
		_map_fixup(map, p, c);
	map->m_numnodes--;
	node->mn_parent = NULL;
	node->mn_child[0] = NULL;
	node->mn_child[1] = NULL;
}


int
map_init(map_t *map)
{
	if (!map)
		return EINVAL;

	map->m_numnodes = 0;
	map->m_root = NULL;
	return 0;
}


int
map_destroy(map_t *map)
{
	int err;
	mapnode_t *mn;

	err = map_clear(map);
	if (err != 0)
		return err;

	assert(map->m_root == NULL);
	assert(map->m_numnodes == 0);
	return 0;
}


int
map_insert(map_t *map, const char *key, const void *val)
{
	int cmp, dir;
	size_t keysz;
	size_t valsz;
	mapnode_t *mn;
	mapnode_t *mnew;
	mapnode_t *uncle;
	mapnode_t *p, *gp;
	char *kptr, *vptr;

	if (!map || !key || !val)
		return EINVAL;

	keysz = strlen(key);
	valsz = strlen(val);
	if (map->m_root == NULL) {
		mnew = malloc(sizeof(*mnew) + keysz + valsz);
		if (mnew == NULL)
			return ENOMEM;
		memset(mnew, 0, sizeof(*mnew));

		kptr = (char *)&mnew[1];
		vptr = &kptr[keysz];
		memcpy(kptr, key, keysz);
		memcpy(vptr, val, valsz);
		mnew->mn_key = kptr;
		mnew->mn_val = vptr;

		map->m_root = mnew;
		map->m_root->mn_color = MN_BLACK;
		map->m_numnodes++;
		return 0;
	}

	mn = map->m_root;
	while (mn != NULL) {
		cmp = strcmp(key, mn->mn_key);
		if (cmp == 0) {
			/* match and we are it */
			return EPERM;
		}

		dir = cmp > 0; /* left - false(0), right - true(1) */
		if (mn->mn_child[dir] != NULL) {
			mn = mn->mn_child[dir];
			continue;
		}

		/* insert the key value pair into the tree */
		mnew = malloc(sizeof(*mnew) + keysz + valsz);
		if (mnew == NULL)
			return ENOMEM;
		memset(mnew, 0, sizeof(*mnew));

		kptr = (char *)&mnew[1];
		vptr = &kptr[keysz];
		memcpy(kptr, key, keysz);
		memcpy(vptr, val, valsz);
		mnew->mn_key = kptr;
		mnew->mn_val = vptr;

		mn->mn_child[dir] = mnew;
		mnew->mn_parent = mn;
		mnew->mn_color = MN_RED;
		break;
	}

	/* rebalance the tree if required */
	mn = mnew;
	while ((p = mn->mn_parent) != NULL) {
		if (!_map_isred(p))
			break;

		gp = p->mn_parent;
		assert(gp != NULL, "grand parent of red node %p is null", p);

		dir = (p == gp->mn_child[0]);
		uncle = gp->mn_child[dir];
		if (uncle != NULL && _map_isred(uncle)) {
			uncle->mn_color = MN_BLACK;
			p->mn_color = MN_BLACK;
			gp->mn_color = MN_RED;
			mn = gp;
			continue;
		}

		if (mn == p->mn_child[dir]) {
			register mapnode_t *mntmp;

			_map_rotate(map, p, !dir); /* rotate otherway */
			mntmp = p;
			p = mn;
			mn = mntmp;
		}

		p->mn_color = MN_BLACK;
		gp->mn_color = MN_RED;
		_map_rotate(map, gp, dir); /* rotate sameway */
	}

	map->m_root->mn_color = MN_BLACK;
	map->m_numnodes++;
	return 0;
}


int
map_erase(map_t *map, const char *key)
{
	mapnode_t *mn;

	mn = map_find(map, key);
	if (mn == NULL)
		return ENOENT;

	_map_remove(map, mn);
	free(mn);
	return 0;
}


int
map_update(map_t *map, const map_t *other)
{
	return ENOSYS;
}


int
map_clear(map_t *map)
{
	mapnode_t *mn;

	if (!map)
		return EINVAL;

	mn = map->m_root;
	while (mn != NULL) {
		_map_remove(map, mn);
		free(mn);

		mn = map->m_root;
	}
	return 0;
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
