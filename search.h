#ifndef _M_SEARCH_H_
#define _M_SEARCH_H_

#include "source.h"

#define MAGIC_SEARCH ((8 << 8) + MAGIC_SOURCE)

typedef struct plugin_t PLUGIN;

extern PLUGIN search_plugin;

typedef struct search_source_t {
	SOURCE source;
	int num_sources;
	SOURCE *sources[10];
} SEARCH;

SEARCH * search_create    (SEARCH *s);
void     search_release   (SEARCH *s);
int      search_add_child (SEARCH *s, void *child);

#endif // _M_SEARCH_H_