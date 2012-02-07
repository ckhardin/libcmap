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
 * @file map_axdr.c
 *
 * Add the integration of sending maps over XDR using the libarpc
 * library.
 */
#include <sys/types.h>
#include <libarpc/arpc.h>

#include "map.h"

static axdr_ret_t
_map_encode(axdr_state_t *xdrs, map_t *map)
{
	return AXDR_ERROR;
}


static axdr_ret_t
_map_decode(axdr_state_t *xdrs, map_t *map)
{
	return AXDR_ERROR;
}


static axdr_ret_t
_map_stringify(axdr_state_t *xdrs, map_t *map)
{
	return AXDR_ERROR;
}


axdr_ret_t
axdr_map_t(axdr_state_t *xdrs, map_t *map)
{
	switch (xdrs->x_op) {
	case AXDR_ENCODE_ASYNC:
	case AXDR_ENCODE:
		return _map_encode(xdrs, map);
	case AXDR_DECODE_ASYNC:
	case AXDR_DECODE:
		return _map_decode(xdrs, map);
	case AXDR_FREE:
		map_destroy(map);
		return AXDR_DONE;
	case AXDR_STRINGIFY:
		return _map_stringify(xdrs, map);
	default:
		break;
	}
	return AXDR_ERROR;
}
