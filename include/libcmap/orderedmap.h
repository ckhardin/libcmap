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
 * @file orderedmap.h
 *
 * Provide a C library for that emulates the standard template map
 * functionality in C++. This implementation is fairly simple
 * reference for for a name/value container.
 */
#pragma once

#include <sys/cdefs.h>
#include <stdbool.h>
#include <stdint.h>

/* forward declare */
typedef struct orderedmap orderedmap_t;
typedef struct orderedmapnode orderedmapnode_t;

struct orderedmap {
	/* red black tree root node */
	int om_numnodes;
	struct orderedmapnode *om_root;
};


__BEGIN_DECLS

/**
 * Initialize a map structure for to a known state for all the
 * following methods to work on the container.
 *
 * @param  map  reference to a container to be initialized
 * @retrun zero on success or an errno value
 */
extern int orderedmap_init(orderedmap_t *map);

/**
 * Free up any resources in the map and return the container
 * to a known state.
 *
 * @param  map  reference that has been initialized by #map_init
 * @return zero on success or an errno value
 */
extern int orderedmap_destroy(orderedmap_t *map);

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
extern int orderedmap_insert(orderedmap_t *map, const char *key,
	const void *val);

/**
 * Remove the element in the map that is named by the key
 * parameter.
 *
 * @param  map  reference that has been initialized by #map_init
 * @param  key  null terminated string that names the element
 * @return zero on success or an errno value
 */
extern int orderedmap_erase(orderedmap_t *map, const char *key);

/**
 * Update the map with another maps key and values. This is modeled
 * after the python update method.
 *
 * @param  map    first map that has been initialized by #map_init
 * @param  other  the source location of the values to update with
 * @return zero on success or an errno value
 */
extern int orderedmap_update(orderedmap_t *map, const orderedmap_t *other);

/**
 * Clear all key value pairs from the map, but change no other
 * information that has been set in the map.
 *
 * @param  map  reference that has been initialized by #map_init
 * @return zero on success or an errno value
 */
extern int orderedmap_clear(orderedmap_t *map);

/**
 * Return the the node that matches the key parameter or a null
 * pointer if no match can be made
 *
 * @param  map  reference that has been initialized by #map_init
 * @param  key  null terminated string that names the element
 * @return pointer to the matching node or null
 */
extern orderedmapnode_t *orderedmap_find(const orderedmap_t *map,
	const char *key);

/**
 * Return the first key entry in the map base on the comparison function
 *
 * @param  map  first map that has been initialized by #map_init
 * @return pointer to the first node or null
 */
extern orderedmapnode_t *orderedmap_first(const orderedmap_t *map);

/**
 * Return the last key entry in the map base on the comparison function
 *
 * @param  map    first map that has been initialized by #map_init
 * @return pointer to the last node or null
 */
extern orderedmapnode_t *orderedmap_last(const orderedmap_t *map);

/**
 * Return the next node in the map based on the comparison function
 * from the passed in node pointer
 *
 * @param  node  reference to a node in store in the map
 * @return pointer to the next node or null
 */
extern orderedmapnode_t *orderedmap_next(const orderedmapnode_t *node);

/**
 * Return the previous node in the map based on the comparison function
 * from the passed in node pointer
 *
 * @param  node  reference to a node in store in the map
 * @return pointer to the previous node or null
 */
extern orderedmapnode_t *orderedmap_prev(const orderedmapnode_t *node);

/* Declare some functions that work on standard types. */
extern int orderedmap_insert_char(orderedmap_t *map, const char *key,
	char val);
extern int orderedmap_update_char(orderedmap_t *map, const char *key,
	char val);

extern int orderedmap_insert_uchar(orderedmap_t *map, const char *key,
	unsigned char val);
extern int orderedmap_update_uchar(orderedmap_t *map, const char *key,
	unsigned char val);

extern int orderedmap_insert_short(orderedmap_t *map, const char *key,
	short val);
extern int orderedmap_update_short(orderedmap_t *map, const char *key,
	short val);

extern int orderedmap_insert_ushort(orderedmap_t *map, const char *key,
	unsigned short val);
extern int orderedmap_update_ushort(orderedmap_t *map, const char *key,
	unsigned short val);

extern int orderedmap_insert_int(orderedmap_t *map, const char *key, int val);
extern int orderedmap_update_int(orderedmap_t *map, const char *key, int val);

extern int orderedmap_insert_uint(orderedmap_t *map, const char *key,
	unsigned int val);
extern int orderedmap_update_uint(orderedmap_t *map, const char *key,
	unsigned int val);

extern int orderedmap_insert_long(orderedmap_t *map, const char *key,
	long val);
extern int orderedmap_update_long(orderedmap_t *map, const char *key,
	long val);

extern int orderedmap_insert_ulong(orderedmap_t *map, const char *key,
	unsigned long val);
extern int orderedmap_update_ulong(orderedmap_t *map, const char *key,
	unsigned long val);

extern int orderedmap_insert_llong(orderedmap_t *map, const char *key,
	long long val);
extern int orderedmap_update_llong(orderedmap_t *map, const char *key,
	long long val);

extern int orderedmap_insert_ullong(orderedmap_t *map, const char *key,
	unsigned long long val);
extern int orderedmap_update_ullong(orderedmap_t *map, const char *key,
	unsigned long long val);

__END_DECLS
