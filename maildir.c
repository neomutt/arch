#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "email.h"
#include "folder.h"
#include "maildir.h"
#include "plugin.h"
#include "source.h"
#include "view.h"

// static char *config[10];

static void
maildir_source_free (MAILDIR_SOURCE *s)
{
	if (!s) {
		return;
	}

	OBJECT *o = &s->source.object;
	o->refcount--;
	if (o->refcount < 1) {
		int i;
		for (i = 0; i < s->source.num_folders; i++) {
			// printf ("freeing folder %p\n", (void*) s->source.folders[i]);
			object_release (s->source.folders[i]);
		}

		for (i = 0; i < s->source.num_items; i++) {
			// printf ("freeing item %p\n", (void*) s->source.items[i]);
			object_release (s->source.items[i]);
		}

		free (s->source.name);
		free (s);
	}
}

MAILDIR_SOURCE *
maildir_source_create (void)
{
	MAILDIR_SOURCE *s = NULL;

	s = calloc (1, sizeof (MAILDIR_SOURCE));
	if (!s) {
		return NULL;
	}

	OBJECT *o = &s->source.object;

	o->refcount = 1;
	o->type     = MAGIC_MAILDIR;
	o->release  = (object_release_fn) maildir_source_free;

	return s;
}

int
maildir_init (void)
{
	// printf ("maildir init\n");
	return 1;
}

SOURCE *
maildir_connect (void)
{
	MAILDIR_SOURCE *ms = NULL;

	ms = maildir_source_create();
	if (!ms) {
		return NULL;
	}

	SOURCE *s = &ms->source;

	s->object.type = MAGIC_MAILDIR;
	s->name        = strdup ("maildir");

	// Pretend to read something

	FOLDER *f1 = folder_create();
	FOLDER *f2 = folder_create();
	FOLDER *f3 = folder_create();

	if (!f1 || !f2 || !f3) {
		printf ("maildir_connect: folder_create failed\n");
		return NULL;
	}

	f1->name = strdup ("boys");
	f2->name = strdup ("trees");
	f3->name = strdup ("fish");

	const char *names[] = { "adam", "barry", "charlie", "ash", "beech", "angel", "beluga", NULL };

	int i;
	EMAIL *e;

	for (i = 0; names[i]; i++) {
		e = email_create();
		if (!e) {
			printf ("imap_connect: email_create failed\n");
			return NULL;
		}
		e->item.name = strdup (names[i]);

		if (i < 3) {
			folder_add_child (f1, e);
		} else if (i < 5) {
			folder_add_child (f2, e);
		} else {
			folder_add_child (f3, e);
		}
		object_release (e);
	}

	folder_add_child (f1, f2);
	folder_add_child (f1, f3);

	object_release (f2);
	object_release (f3);

	source_add_child (s, f1);

	object_release (f1);

	return s;
}

int
maildir_config_item (const char *name)
{
	if (!name) {
		return 0;
	}

	if ((name[0] >= 'f') && (name[0] <= 'm')) {
		// printf ("maildir config: %s\n", name);
		return 1;
	}

	return 0;
}

void
maildir_disconnect (void)
{
}

PLUGIN maildir_plugin = {
	MAGIC_MAILDIR,
	"maildir",
	maildir_init,
	maildir_connect,
	maildir_disconnect,
	maildir_config_item
};

