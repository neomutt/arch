/* Copyright (c) 2016 Richard Russon <rich@flatcap.org>
 * Released under the GPLv3 -- see LICENSE.md for details */

#ifndef _EVENT_H_
#define _EVENT_H_

#include "item.h"

#define MAGIC_EVENT ((4 << 8) + MAGIC_ITEM)

typedef struct event_t {
	ITEM item;
	int day;
} EVENT;

EVENT * event_create  (EVENT *e);
void    event_destroy (EVENT *e);

#endif // _EVENT_H_
