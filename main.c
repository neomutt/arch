/* Copyright (c) 2016 Richard Russon <rich@flatcap.org>
 * Released under the GPLv3 -- see LICENSE.md for details */

#include <stdio.h>
#include <string.h>

#include "config.h"
#include "plugin.h"
#include "view.h"
#include "source.h"

int
main (int argc, char *argv[])
{
	if (argc < 2) {
		printf ("Usage: %s CONFIG-FILE [...]\n", argv[0]);
		return 1;
	}

	int i;

	SOURCE *sources[10];

	for (i = 0; plugins[i]; i++) {
		sources[i] = plugins[i]->init();
		if (sources[i] == NULL) {
			printf ("plugin %s::init() failed\n", plugins[i]->name);
			return 1;
		}
	}
	sources[i] = NULL;

	for (argc--; argc > 0; argc--, argv++) {
		config_read_file (argv[1], plugins);
	}

	VIEW *v1 = view_create (NULL);
	VIEW *v2 = view_create (NULL);
	VIEW *v3 = view_create (NULL);
	VIEW *v4 = view_create (NULL);
	VIEW *v5 = view_create (NULL);
	VIEW *v6 = view_create (NULL);
	if (!v1 || !v2 || !v3 || !v4 || !v5 || !v6) {
		printf ("view_create failed\n");
		return 1;
	}

	v1->container.object.name = strdup ("mail");
	v2->container.object.name = strdup ("search");
	v3->container.object.name = strdup ("contacts");
	v4->container.object.name = strdup ("tasks");
	v5->container.object.name = strdup ("news");
	v6->container.object.name = strdup ("calendar");

	SOURCE *search = sources[3];

	for (i = 0; plugins[i]; i++) {
		SOURCE *s = sources[i];

		plugins[i]->connect (s);
		switch (i) {
			case 0:
			case 1:
			case 2:
				add_child (v1, s);
				add_child (search, s);
				break;
			case 3:
				add_child (v2, s);
				break;
			case 4:
				add_child (v3, s);
				break;
			case 5:
				add_child (v4, s);
				break;
			case 6:
			case 7:
				add_child (v5, s);
				break;
			default:
				add_child (v6, s);
				break;
		}
		release (s);
	}

	display (v1, 0);
	display (v2, 0);
	display (v3, 0);
	display (v4, 0);
	display (v5, 0);
	display (v6, 0);

	release (v1);
	release (v2);
	release (v3);
	release (v4);
	release (v5);
	release (v6);
	config_free();

	return 0;
}

