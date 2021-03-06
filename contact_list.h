/* Copyright (c) 2016 Richard Russon <rich@flatcap.org>
 * Released under the GPLv3 -- see LICENSE.md for details */

#ifndef _M_CONTACT_LIST_H_
#define _M_CONTACT_LIST_H_

#include "source.h"

#define MAGIC_CONTACT_LIST ((2 << 8) + MAGIC_SOURCE)

typedef struct plugin_t PLUGIN;

extern PLUGIN contact_list_plugin;

typedef struct contact_list_source_t {
	SOURCE source;
} CONTACT_LIST;

CONTACT_LIST * contact_list_create  (CONTACT_LIST *c);
void           contact_list_destroy (CONTACT_LIST *c);

#endif // _M_CONTACT_LIST_H_
