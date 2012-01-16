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
 * @file map_types.c
 *
 * Provide a type specific api to manipulate the map key and values. This
 * allows the basic C types to be used as parameters for the map manipulation.
 */

#include <sys/types.h>
#include <sys/param.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

#include "map.h"

#ifndef STRINGIFY
#define STRINGIFY(_s)   STRINGIFY1(_s)
#define STRINGIFY1(_s)  #_s
#endif

#define CHARSTRFMT	"%d"
#define CHARSTRSZ	sizeof(STRINGIFY(CHAR_MIN))
#define UCHARSTRFMT	"%u"
#define UCHARSTRSZ	sizeof(STRINGIFY(UCHAR_MAX))
#define SHRTSTRFMT	"%d"
#define SHRTSTRSZ	sizeof(STRINGIFY(SHRT_MIN))
#define USHRTSTRFMT	"%u"
#define USHRTSTRSZ	sizeof(STRINGIFY(USHRT_MAX))
#define INTSTRFMT	"%d"
#define INTSTRSZ	sizeof(STRINGIFY(INT_MIN))
#define UINTSTRFMT	"%u"
#define UINTSTRSZ	sizeof(STRINGIFY(UINT_MAX))
#define LONGSTRFMT	"%ld"
#define LONGSTRSZ	sizeof(STRINGIFY(LONG_MIN))
#define ULONGSTRFMT	"%lu"
#define ULONGSTRSZ	sizeof(STRINGIFY(ULONG_MAX))
#define LLONGSTRFMT	"%lld"
#define LLONGSTRSZ	sizeof(STRINGIFY(LLONG_MIN))
#define ULLONGSTRFMT	"%llu"
#define ULLONGSTRSZ	sizeof(STRINGIFY(ULLONG_MAX))


#define _DEFINE_MAPTYPE(_label, _type, _strsz, _strfmt)	\
int map_insert_ ## _label(map_t *map, const char *key, _type val)	\
{									\
	char buf[_strsz];						\
									\
	if(!map || !key)						\
		return EINVAL;						\
	snprintf(buf, sizeof(buf), _strfmt, val);			\
	return map_insert(map, key, buf);				\
}									\
									\
int map_update_ ## _label(map_t *map, const char *key, _type val)	\
{									\
	char buf[_strsz];						\
									\
	if(!map || !key)						\
		return EINVAL;						\
	snprintf(buf, sizeof(buf), _strfmt, val);			\
	map_erase(map, key); /* ignore error code */			\
	return map_insert(map, key, buf);				\
}									\
									\
extern int __dummy_ ## _label

_DEFINE_MAPTYPE(char, char, CHARSTRSZ, CHARSTRFMT);
_DEFINE_MAPTYPE(uchar, unsigned char, UCHARSTRSZ, UCHARSTRFMT);
_DEFINE_MAPTYPE(short, short, SHRTSTRSZ, SHRTSTRFMT);
_DEFINE_MAPTYPE(ushort, unsigned short, USHRTSTRSZ, USHRTSTRFMT);
_DEFINE_MAPTYPE(int, int, INTSTRSZ, INTSTRFMT);
_DEFINE_MAPTYPE(uint, unsigned int, UINTSTRSZ, UINTSTRFMT);
_DEFINE_MAPTYPE(long, long, LONGSTRSZ, LONGSTRFMT);
_DEFINE_MAPTYPE(ulong, unsigned long, ULONGSTRSZ, ULONGSTRFMT);
_DEFINE_MAPTYPE(llong, long long, LLONGSTRSZ, LLONGSTRFMT);
_DEFINE_MAPTYPE(ullong, unsigned long long, ULLONGSTRSZ, ULLONGSTRFMT);
