/* Copyright (c) 2016 Richard Russon <rich@flatcap.org>
 * Released under the GPLv3 -- see LICENSE.md for details */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "email.h"
#include "folder.h"
#include "plugin.h"
#include "search.h"
#include "source.h"
#include "view.h"

static void
check_email (ITEM *item, FOLDER *rcpt, int rule)
{
	if (!item || !rcpt) {
		return;
	}

	const char *name = item->object.name;

	int match = 0;
	int len;
	int i;

	switch (rule) {
		case 1:
			if (name[0] == 'b') {
				match = 1;
			}
			break;
		case 2:
			len = strlen (name);
			if (name[len - 1] == 'y') {
				match = 1;
			}
			break;
		case 3:
			len = strlen (name);
			for (i = 1; i < len; i++) {
				if (name[i - 1] == name[i]) {
					match = 1;
					break;
				}
			}
			break;
	}

	if (match) {
		add_child (rcpt, item);
	}
}

static void
descend_folders (FOLDER *search, FOLDER *rcpt, int rule)
{
	if (!search || !rcpt) {
		return;
	}

	int i;
	for (i = 0; search->container.children[i]; i++) {
		OBJECT *o = (OBJECT*) search->container.children[i];
		if ((o->type & 0xFF) == MAGIC_FOLDER) {
			descend_folders ((FOLDER*) search->container.children[i], rcpt, rule);
		} else {
			ITEM *item = (ITEM*) search->container.children[i];
			check_email (item, rcpt, rule);
		}

	}

	for (i = 0; search->items[i]; i++) {
		ITEM *item = search->items[i];
		check_email (item, rcpt, rule);
	}
}

static void
find_all_mail (SOURCE **sources, FOLDER *rcpt, int rule)
{
	if (!sources || !rcpt) {
		return;
	}

	int s;
	for (s = 0; sources[s]; s++) {
		int f;
		for (f = 0; sources[s]->container.children[f]; f++) {
			descend_folders ((FOLDER*) sources[s]->container.children[f], rcpt, rule);
		}

		int i;
		for (i = 0; sources[s]->items[i]; i++) {
			ITEM *item = sources[s]->items[i];
			check_email (item, rcpt, rule);
		}
	}
}


void
search_destroy (SEARCH *s)
{
	if (!s) {
		return;
	}

	int i;
	for (i = 0; s->sources[i]; i++) {
		release (s->sources[i]);
	}

	source_destroy (&s->source);	// Destroy parent
}

int
search_add_child (SEARCH *s, void *child)
{
	if (!s || !child) {
		return -1;
	}

	OBJECT *obj = child;
	if ((obj->type & 0xFF) == MAGIC_SOURCE) {
		addref (child);
		s->sources[s->num_sources] = child;
		s->num_sources++;
	} else {
		source_add_child (&s->source, child);
	}

	return 0;
}

SEARCH *
search_create (SEARCH *s)
{
	if (!s) {
		s = calloc (1, sizeof (SEARCH));
		if (!s) {
			return NULL;
		}
	}

	source_create (&s->source);	// Construct parent

	OBJECT *o = &s->source.container.object;

	o->type    = MAGIC_SEARCH;
	o->name    = strdup ("search");
	o->destroy = (object_destroy_fn) search_destroy;

	CONTAINER *c = &s->source.container;

	c->add_child = (container_add_child_fn) search_add_child;

	return s;
}


static SOURCE *
search_init (void)
{
	SEARCH *is = search_create (NULL);
	if (!is) {
		return NULL;
	}

	return &is->source;
}

static int
search_config_item (const char *name)
{
	if (!name) {
		return 0;
	}

	if ((name[0] >= 'e') && (name[0] <= 'h')) {
		// printf ("search config: %s\n", name);
		return 1;
	}

	return 0;
}

static void
search_connect (SOURCE *s)
{
	FOLDER *f1 = folder_create (NULL);
	FOLDER *f2 = folder_create (NULL);
	FOLDER *f3 = folder_create (NULL);

	if (!f1 || !f2 || !f3) {
		printf ("search_connect: folder_create failed\n");
		return;
	}

	f1->container.object.name = strdup ("^b");
	f2->container.object.name = strdup ("y$");
	f3->container.object.name = strdup ("(.){2}");

	SEARCH *sl = (SEARCH*) s;

	find_all_mail (sl->sources, f1, 1);
	find_all_mail (sl->sources, f2, 2);
	find_all_mail (sl->sources, f3, 3);

	add_child (s, f1);
	add_child (s, f2);
	add_child (s, f3);

	release (f1);
	release (f2);
	release (f3);
}

static void
search_disconnect (SOURCE *src)
{
	if (!src) {
		return;
	}
}


PLUGIN search_plugin = {
	MAGIC_SEARCH,
	"search",
	search_init,
	search_config_item,
	search_connect,
	search_disconnect
};

