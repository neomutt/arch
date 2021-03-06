/* Copyright (c) 2016 Richard Russon <rich@flatcap.org>
 * Released under the GPLv3 -- see LICENSE.md for details */

#ifndef _M_CALENDAR_H_
#define _M_CALENDAR_H_

#include "source.h"

#define MAGIC_CALENDAR ((1 << 8) + MAGIC_SOURCE)

typedef struct plugin_t PLUGIN;

extern PLUGIN calendar_plugin;

typedef struct calendar_source_t {
	SOURCE source;
} CALENDAR;

CALENDAR * calendar_create  (CALENDAR *c);
void       calendar_destroy (CALENDAR *c);

#endif // _M_CALENDAR_H_
