#include <stdio.h>
#include <stdlib.h>

#include "item.h"

static void
item_free (ITEM *i)
{
	if (!i) {
		return;
	}

	OBJECT *o = &i->object;
	o->refcount--;
	if (o->refcount < 1) {
		free (i->name);
		free (i);
	}
}

ITEM *
item_create (void)
{
	ITEM *i = NULL;

	i = calloc (1, sizeof (ITEM));
	if (!i) {
		return NULL;
	}

	OBJECT *o = &i->object;
	o->refcount = 1;
	o->type     = MAGIC_ITEM;
	o->release  = (object_release_fn) item_free;

	return i;
}

void
item_display (ITEM *i, int indent)
{
	if (!i) {
		return;
	}

	printf ("%*s\033[1;36m%s\033[m\n", indent * 8, "", i->name);
}

