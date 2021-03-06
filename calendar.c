/* Copyright (c) 2016 Richard Russon <rich@flatcap.org>
 * Released under the GPLv3 -- see LICENSE.md for details */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "calendar.h"
#include "email.h"
#include "event.h"
#include "folder.h"
#include "month.h"
#include "plugin.h"
#include "source.h"
#include "view.h"

void
calendar_destroy (CALENDAR *c)
{
	if (!c) {
		return;
	}

	// Nothing CALENDAR-specific to release

	source_destroy (&c->source);	// Destroy parent
}

CALENDAR *
calendar_create (CALENDAR *c)
{
	if (!c) {
		c = calloc (1, sizeof (CALENDAR));
		if (!c) {
			return NULL;
		}
	}

	source_create (&c->source);	// Construct parent

	OBJECT *o = &c->source.container.object;

	o->type    = MAGIC_CALENDAR;
	o->name    = strdup ("calendar");
	o->destroy = (object_destroy_fn) calendar_destroy;

	return c;
}


static SOURCE *
calendar_init (void)
{
	CALENDAR *is = calendar_create (NULL);
	if (!is) {
		return NULL;
	}

	return &is->source;
}

static int
calendar_config_item (const char *name)
{
	if (!name) {
		return 0;
	}

	if ((name[0] >= 'e') && (name[0] <= 'h')) {
		// printf ("calendar config: %s\n", name);
		return 1;
	}

	return 0;
}

static void
calendar_connect (SOURCE *s)
{
	MONTH *m1 = month_create (NULL);

	m1->folder.container.object.name = strdup ("personal");
	m1->year               = 2016;
	m1->month              = 1;

	EVENT *e1 = event_create (NULL);
	EVENT *e2 = event_create (NULL);
	EVENT *e3 = event_create (NULL);

	e1->item.object.name = strdup ("Meet Jim");
	e2->item.object.name = strdup ("Bob's birthday");
	e3->item.object.name = strdup ("Dave in town");

	e1->day = 7;
	e2->day = 12;
	e3->day = 23;

	add_child (m1, e1);
	add_child (m1, e2);
	add_child (m1, e3);

	release (e1);
	release (e2);
	release (e3);

	add_child (s, &m1->folder);

	release (m1);
}

static void
calendar_disconnect (SOURCE *src)
{
	if (!src) {
		return;
	}
}


PLUGIN calendar_plugin = {
	MAGIC_CALENDAR,
	"calendar",
	calendar_init,
	calendar_config_item,
	calendar_connect,
	calendar_disconnect
};

