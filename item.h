#ifndef _ITEM_H_
#define _ITEM_H_

#include "object.h"

#define MAGIC_ITEM 4

typedef struct item_t {
	OBJECT object;
} ITEM;

ITEM * item_create (void);
void item_display (ITEM *i, int indent);

#endif // _ITEM_H_
