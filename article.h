/* Copyright (c) 2016 Richard Russon <rich@flatcap.org>
 * Released under the GPLv3 -- see LICENSE.md for details */

#ifndef _ARTICLE_H_
#define _ARTICLE_H_

#include "item.h"

#define MAGIC_ARTICLE ((1 << 8) + MAGIC_ITEM)

typedef struct article_t {
	ITEM item;
} ARTICLE;

ARTICLE * article_create  (ARTICLE *a);
void      article_destroy (ARTICLE *a);

#endif // _ARTICLE_H_
