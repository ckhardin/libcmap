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
 * @file map.h
 *
 * Provide a C library for that emulates the standard template map
 * functionality in C++. This implementation is fairly simple
 * reference for for a name/value container.
 */

#ifndef _LIBCMAP_MAP_H_
#define _LIBCMAP_MAP_H_

#include <sys/cdefs.h>
#include <stdbool.h>
#include <stdint.h>

/* forward declare */
typedef struct map_s map_t;
typedef struct mapnode_s mapnode_t;

/* Compare function that is specific to the insertion into the
 * red black tree function. So, it needs to follow the
 * semantics of strcmp.
 * - return a negative if s1 is logically less than s2
 * - return zero if s1 is locically equal to s2
 * - return a positive if s1 is logically greater than s2
 */
typedef int (*map_cmpfunc_t)(const char *s1, const char *s2);

struct map_s {
	/* red black tree root node */
	int m_numnodes;
	mapnode_t *m_root;
};

struct mapnode_s {
	enum {
		MN_RED = 0,
		MN_BLACK = 1
	} mn_color;
	mapnode_t *mn_child[2]; /* 0 - left and 1 - right */

	const char *mn_key;
	const char *mv_value;
};


__BEGIN_DECLS

/**
 * Initialize a map structure for to a known state for all the
 * following methods to work on the container.
 *
 * @param  map  reference to a container to be initialized
 * @retrun zero on success or an errno value
 */
int map_init(map_t *map);

/**
 * Free up any resources in the map and return the container
 * to a known state.
 *
 * @param  map  reference that has been initialized by #map_init
 * @return zero on success or an errno value
 */
int map_destroy(map_t *map);

/**
 * Extend the map container by inserting an a new element with
 * the key value pair. Since maps don't allow duplicate keys,
 * if the same key is already in the map it will not be changed
 * and an error will be returned. For duplicate values see
 * the multimap reference.
 *
 * @param  map  reference that has been initialized by #map_init
 * @param  key  null terminated string that names the value
 * @param  val  null terminated string - void pointer avoids casts
 * @return zero on success or an errno value
 */
int map_insert(map_t *map, const char *key, const void *val);

/**
 * Remove the element in the map that is named by the key
 * parameter.
 *
 * @param  map  reference that has been initialized by #map_init
 * @param  key  null terminated string that names the element
 * @return zero on success or an errno value
 */
int map_erase(map_t *map, const char *key);

/**
 * Update the map with another maps key and values. This is modeled
 * after the python update method.
 *
 * @param  map    first map that has been initialized by #map_init
 * @param  other  the source location of the values to update with
 * @return zero on success or an errno value
 */
int map_update(map_t *map, const map_t *other);

/**
 * Clear all key value pairs from the map, but change no other
 * information that has been set in the map.
 *
 * @param  map  reference that has been initialized by #map_init
 * @return zero on success or an errno value
 */
int map_clear(map_t *map);

/* Declare some functions that work on standard types. */
int map_insert_char(map_t *map, const char *key, char val);
int map_update_char(map_t *map, const char *key, char val);

int map_insert_uchar(map_t *map, const char *key, unsigned char val);
int map_update_uchar(map_t *map, const char *key, unsigned char val);

int map_insert_short(map_t *map, const char *key, short val);
int map_update_short(map_t *map, const char *key, short val);

int map_insert_ushort(map_t *map, const char *key, unsigned short val);
int map_update_ushort(map_t *map, const char *key, unsigned short val);

int map_insert_int(map_t *map, const char *key, int val);
int map_update_int(map_t *map, const char *key, int val);

int map_insert_uint(map_t *map, const char *key, unsigned int val);
int map_update_uint(map_t *map, const char *key, unsigned int val);

int map_insert_long(map_t *map, const char *key, long val);
int map_update_long(map_t *map, const char *key, long val);

int map_insert_ulong(map_t *map, const char *key, unsigned long val);
int map_update_ulong(map_t *map, const char *key, unsigned long val);

int map_insert_llong(map_t *map, const char *key, long long val);
int map_update_llong(map_t *map, const char *key, long long val);

int map_insert_ullong(map_t *map, const char *key, unsigned long long val);
int map_update_ullong(map_t *map, const char *key, unsigned long long val);

__END_DECLS

#endif /* !_LIBCMAP_MAP_H_ */
