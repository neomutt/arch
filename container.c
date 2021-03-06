/* Copyright (c) 2016 Richard Russon <rich@flatcap.org>
 * Released under the GPLv3 -- see LICENSE.md for details */

#include <stdio.h>
#include <stdlib.h>

#include "container.h"

void
container_destroy (CONTAINER *c)
{
	if (!c) {
		return;
	}

	int i;
	for (i = 0; c->children[i]; i++) {
		release (c->children[i]);
	}

	object_destroy (&c->object);
}

void
container_display (CONTAINER *c, int indent)
{
	if (!c) {
		return;
	}

	int i;
	for (i = 0; i < c->num_children; i++) {
		CONTAINER *child = c->children[i];

		display (child, indent);
	}
}

CONTAINER *
container_create (CONTAINER *c)
{
	if (!c) {
		c = calloc (1, sizeof (CONTAINER));
		if (!c) {
			return NULL;
		}
	}

	OBJECT *o = &c->object;

	object_create (o);	// Construct parent

	o->type     = MAGIC_CONTAINER;
	o->destroy  = (object_destroy_fn) container_destroy;
	o->display  = (object_display_fn) container_display;

	return c;
}

int
container_add_child (CONTAINER *c, void *child)
{
	if (!c || !child) {
		return -1;
	}

	addref (child);
	c->children[c->num_children] = child;
	c->num_children++;

	return 1;
}


int
add_child (void *parent, void *child)
{
	if (!parent || !child) {
		return -1;
	}

	CONTAINER *c = parent;
	if (c->add_child) {
		c->add_child (c, child);
	} else {
		container_add_child (c, child);
	}

	return 0;
}
