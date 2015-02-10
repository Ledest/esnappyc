/* snappy.c -- wrapper of the Snappy algorithm

   This file is part of the erlang esnappyc data compression library.

   Copyright (C) 2015 Oleksandr Chumachenko
   All Rights Reserved.

   The esnappyc library is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of
   the License, or (at your option) any later version.

   The esnappyc library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with the esnappyc library; see the file COPYING.
   If not, write to the Free Software Foundation, Inc.,
   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

   Oleksandr Chumachenko
   <ledest@gmail.com>
   https://github.com/Ledest/esnappyc/
 */

#include <erl_nif.h>
#include <snappy-c.h>
#include <stdint.h>

static inline ERL_NIF_TERM make_ok(ErlNifEnv *env, ERL_NIF_TERM msg)
{
	return enif_make_tuple2(env, enif_make_atom(env, "ok"), msg);
}

static inline ERL_NIF_TERM make_error(ErlNifEnv *env, const char *msg)
{
	return enif_make_tuple2(env, enif_make_atom(env, "error"), enif_make_atom(env, msg));
}

static ERL_NIF_TERM snappy_zip_1(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
	ErlNifBinary i, o;
	size_t os;
	const char *s = "insufficient_memory";

	if (!enif_inspect_binary(env, argv[0], &i) || !enif_inspect_iolist_as_binary(env, argv[0], &i))
		return enif_make_badarg(env);

	os = snappy_max_compressed_length(i.size);
	if (enif_alloc_binary(os, &o)) {
		switch (snappy_compress((const char*)i.data, i.size, (char*)o.data, &os)) {
		case SNAPPY_OK:
			if (os == o.size || enif_realloc_binary(&o, os))
				return enif_make_binary(env, &o);
		case SNAPPY_BUFFER_TOO_SMALL:
			break;
		default:
			s = "unknown";
			break;
		}
	}
	return make_error(env, s);
}

static ERL_NIF_TERM snappy_unzip_1(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
	ErlNifBinary i, o;
	size_t os;
	const char *s = "insufficient_memory";

	if (!enif_inspect_binary(env, argv[0], &i) || !enif_inspect_iolist_as_binary(env, argv[0], &i))
		return enif_make_badarg(env);

	switch (snappy_uncompressed_length((const char*)i.data, i.size, &os)) {
	case SNAPPY_OK:
		if (enif_alloc_binary(os, &o)) {
			switch (snappy_uncompress((const char*)i.data, i.size, (char*)o.data, &os)) {
			case SNAPPY_OK:
				if (os == o.size || enif_realloc_binary(&o, os))
					return enif_make_binary(env, &o);
			case SNAPPY_BUFFER_TOO_SMALL:
				break;
			default:
				s = "unknown";
				break;
			}
			break;
		}
	case SNAPPY_INVALID_INPUT:
		s = "invalid_input";
		break;
	default:
		s = "unknown";
		break;
	}
	return make_error(env, s);
}

static ERL_NIF_TERM snappy_compress_1(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
	ErlNifBinary i, o;
	size_t os;
	const char *s = "insufficient_memory";

	if (!enif_inspect_binary(env, argv[0], &i) || !enif_inspect_iolist_as_binary(env, argv[0], &i))
		return enif_make_badarg(env);

	os = snappy_max_compressed_length(i.size);
	if (enif_alloc_binary(os, &o)) {
		switch (snappy_compress((const char*)i.data, i.size, (char*)o.data, &os)) {
		case SNAPPY_OK:
			if (os == o.size || enif_realloc_binary(&o, os))
				return make_ok(env, enif_make_binary(env, &o));
		case SNAPPY_BUFFER_TOO_SMALL:
			break;
		default:
			s = "unknown";
			break;
		}
	}
	return make_error(env, s);
}

static ERL_NIF_TERM snappy_uncompress_1(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
	ErlNifBinary i, o;
	size_t os;
	const char *s = "insufficient_memory";

	if (!enif_inspect_binary(env, argv[0], &i) || !enif_inspect_iolist_as_binary(env, argv[0], &i))
		return enif_make_badarg(env);

	switch (snappy_uncompressed_length((const char*)i.data, i.size, &os)) {
	case SNAPPY_OK:
		if (enif_alloc_binary(os, &o)) {
			switch (snappy_uncompress((const char*)i.data, i.size, (char*)o.data, &os)) {
			case SNAPPY_OK:
				if (os == o.size || enif_realloc_binary(&o, os))
					return make_ok(env, enif_make_binary(env, &o));
			case SNAPPY_BUFFER_TOO_SMALL:
				break;
			default:
				s = "unknown";
				break;
			}
			break;
		}
	case SNAPPY_INVALID_INPUT:
		s = "invalid_input";
		break;
	default:
		s = "unknown";
		break;
	}
	return make_error(env, s);
}

static ERL_NIF_TERM snappy_validate_1(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
	ErlNifBinary i;
	const char *s;

	if (!enif_inspect_iolist_as_binary(env, argv[0], &i))
		return enif_make_badarg(env);

	switch (snappy_validate_compressed_buffer((const char*)i.data, i.size)) {
	case SNAPPY_OK:
		s = "true";
		break;
	case SNAPPY_INVALID_INPUT:
		s = "false";
		break;
	default:
		return make_error(env, "unknown");
	}
	return enif_make_atom(env, s);
}

static ERL_NIF_TERM snappy_uncompressed_length_1(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
	ErlNifBinary i;
	size_t os;
	const char *s;

	if (!enif_inspect_iolist_as_binary(env, argv[0], &i))
		return enif_make_badarg(env);

	switch (snappy_uncompressed_length((const char*)i.data, i.size, &os)) {
	case SNAPPY_OK:
		return enif_make_ulong(env, os);
	case SNAPPY_INVALID_INPUT:
		s = "invalid_input";
		break;
	default:
		s = "unknown";
		break;
	}
	return make_error(env, s);
}

static int on_load(ErlNifEnv *env, void **priv, ERL_NIF_TERM info)
{
	return 0;
}

static int on_reload(ErlNifEnv *env, void**priv, ERL_NIF_TERM info)
{
	return 0;
}

static int on_upgrade(ErlNifEnv *env, void **priv, void** old_priv, ERL_NIF_TERM info)
{
	return 0;
}


static ErlNifFunc nif_functions[] = {
	{"validate", 1, snappy_validate_1},
	{"uncompressed_length", 1, snappy_uncompressed_length_1},
	{"zip", 1, snappy_zip_1},
	{"unzip", 1, snappy_unzip_1},
	{"compress", 1, snappy_compress_1},
	{"uncompress", 1, snappy_uncompress_1},
	{"decompress", 1, snappy_uncompress_1}
};

ERL_NIF_INIT(snappy, nif_functions, &on_load, &on_reload, &on_upgrade, NULL);
