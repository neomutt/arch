#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "article.h"
#include "folder.h"
#include "item.h"
#include "plugin.h"
#include "rss.h"
#include "source.h"
#include "task.h"
#include "view.h"

static int
rss_release (RSS_SOURCE *s)
{
	if (!s) {
		return -1;
	}

	OBJECT *o = &s->source.object;
	o->refcount--;
	int rc = o->refcount;
	if (o->refcount < 1) {
		int i;
		for (i = 0; i < s->source.num_folders; i++) {
			object_release (s->source.folders[i]);
		}

		for (i = 0; i < s->source.num_items; i++) {
			object_release (s->source.items[i]);
		}

		for (i = 0; i < s->num_feeds; i++) {
			object_release (s->feeds[i]);
		}

		free (o->name);
		free (s);
	}

	return rc;
}

static RSS_SOURCE *
rss_create (void)
{
	RSS_SOURCE *s = NULL;

	s = calloc (1, sizeof (RSS_SOURCE));
	if (!s) {
		return NULL;
	}

	OBJECT *o = &s->source.object;

	o->refcount = 1;
	o->type     = MAGIC_RSS;
	o->release  = (object_release_fn) rss_release;
	o->display  = (object_display_fn) source_display;

	return s;
}


static SOURCE *
rss_init (void)
{
	RSS_SOURCE *rs = rss_create();
	if (!rs) {
		return NULL;
	}

	return &rs->source;
}

static int
rss_config_item (const char *name)
{
	if (!name) {
		return 0;
	}

	if ((name[0] >= 'q') && (name[0] <= 't')) {
		// printf ("rss config: %s\n", name);
		return 1;
	}

	return 0;
}

static void
rss_connect (SOURCE *s)
{
	RSS_SOURCE *rs = (RSS_SOURCE*) s;

	s->object.type = MAGIC_RSS;
	s->object.name = strdup ("rss");

	const char *feeds[] = { "slashdot.org", "arstechnica.co.uk", "bbc.co.uk", NULL };

	int i;

	for (i = 0; feeds[i]; i++) {
		rs->feeds[i] = folder_create();
		if (!rs->feeds[i]) {
			printf ("rss_connect: folder_create failed\n");
			return NULL;
		}
		rs->feeds[i]->object.name = strdup (feeds[i]);

		source_add_child (s, rs->feeds[i]);	// Source owns all feeds

		object_release (rs->feeds[i]);
	}

	rs->feeds[i] = NULL;

	const char *names[] = { "new battery tech", "privacy concerns", "faster processor", "electric vehicle", "eff defends", "old woman", "bad weather", NULL };

	for (i = 0; names[i]; i++) {
		ARTICLE *a = article_create();
		if (!a) {
			printf ("rss_connect: article_create failed\n");
			return NULL;
		}

		a->item.object.name = strdup (names[i]);

		if (i < 3) {
			folder_add_child (rs->feeds[0], a);
		} else if (i < 5) {
			folder_add_child (rs->feeds[1], a);
		} else {
			folder_add_child (rs->feeds[2], a);
		}

		object_release (a);
	}

	FOLDER *f1 = folder_create();
	FOLDER *f2 = folder_create();

	if (!f1 || !f2) {
		printf ("rss_connect: folder_create failed\n");
		return NULL;
	}

	f1->object.name = strdup ("tech");
	f2->object.name = strdup ("news");

	for (i = 0; rs->feeds[i]; i++) {
		if (i < 2) {
			folder_add_child (f1, rs->feeds[i]);
		} else {
			folder_add_child (f2, rs->feeds[i]);
		}
	}

	source_add_child (s, f1);
	source_add_child (s, f2);

	object_release (f1);
	object_release (f2);
}

static void
rss_disconnect (SOURCE *src)
{
	if (!src) {
		return;
	}
}


PLUGIN rss_plugin = {
	MAGIC_TASK,
	"task",
	rss_init,
	rss_config_item,
	rss_connect,
	rss_disconnect
};

