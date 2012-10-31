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
#include <arch/x86/mm/page.h>
/** LIFO QUEUE **/
#define	LIFO_HEAD(name, member)\
	struct name{		\
		struct member *first;	\
		struct member *tail;	\
	}
#define LIFO_HEAD_INIT(h)\
	LIST_INIT(h)

#define LIFO_ENTRY(entry)	\
	LIST_ENTRY(entry)

#define LIFO_FIRST(h)	\
	((h)->first)
#define LIFO_EMPTY(h)	\
	LIST_EMPTY((h))

#define LIFO_INIT(h)	\
	LIFO_FIRST((h)) = NULL

#define LIFO_PUSH(h, element,field)	\
	LIST_INSERT_HEAD(h, element, field)

#define LIFO_POP(h,field)\
	LIST_FIRST((h));\
	LIST_REMOVE(LIST_FIRST((h)), field) 

/** FIFO QUEUE **/
#define FIFO_HEAD(name, member, max)\
	struct{            \
		struct member *first;   \
		size_t items;\
		size_t tail;\
		size_t head;\
	} name =	\
	{	\
		.first = NULL,	\
		.items	= (max),	\
		.tail = 0,	\
		.head = 0	\
	}

#define FIFO_ENTRY(entry)       \
        LIST_ENTRY(entry)

#define FIFO_FIRST(h)   \
        ((h)->first)


#define FIFO_INIT(h)    \
        FIFO_FIRST(h) = allocate( sizeof(*(h)->first) * (h)->items, 0x400)

#define FIFO_PUSH(h, element)     \
	FIFO_FIRST(h)[(h)->tail = ((h)->tail +1) < (h)->items ? ((h)->tail+1): 0] = *(element)

#define FIFO_POP(h)\
	&FIFO_FIRST(h)[(h)->head = ((h)->head +1) < (h)->items ? ((h)->head +1) : 0]
#endif
