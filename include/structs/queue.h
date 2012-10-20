/*
 * CATReloaded (C) Copyrights 2011
 * http://catreloaded.net
 *
 * a boot time list is a macro coded list
 * to provide ability to make sequential
 * lists at boot time.
 */
#ifndef _CATERNEL_BOOT_QUEUE_H_
#define _CATERNEL_BOOT_QUEUE_H_
#include <structs/linkedlist.h>

#define	LIFO_HEAD(name, member)\
	struct name{		\
		struct member *first;	\
	}
#define LIFO_HEAD_INIT(h)\
	LIST_INIT(h)

#define LIFO_ENTRY(entry)	\
	LIST_ENTRY(entry)

#define LIFO_FIRST(h)	\
	((h)->first)
#define LIFO_INIT(h)	\
	LIFO_FIRST((h)) = NULL

#define LIFO_PUSH(h, element,field)	\
	LIST_INSERT_HEAD(h, element, field)

#define LIFO_POP(h,field)\
	LIST_FIRST((h));\
	LIST_REMOVE(LIST_FIRST((h)), field) 
#endif
