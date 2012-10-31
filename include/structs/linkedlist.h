/*
 * Saad Talaat
 * CATReloaded (C) Copyrights 2011
 * http://catreloaded.net
 */
/**
 * a boot time list is a macro coded list
 * to provide ability to make sequential
 * lists at boot time.
 */
#ifndef _CATERNEL_BOOT_LIST_H_
#define _CATERNEL_BOOT_LIST_H_

/* a head is the first element of list
 * and it would be global variable with
 * only an element head
 */
#define LIST_HEAD(Instance, Type)\
	struct Instance{\
		struct Type *first;\
		struct Type *tail;\
	}

#define LIST_HEAD_INIT(head){NULL}

/* the macro is used to define entry
 * instance, JUST insance
 */
#define LIST_ENTRY(type)\
	struct {\
		struct type *next;\
		struct type **prev;\
	}

/** Functions of the list, first,next, insert, empty..etc */

#define LIST_EMPTY(xhead)	((xhead)->first == NULL)

#define LIST_FIRST(xhead)	((xhead)->first)

/* Here an element is the instance of the type
 * field is the field which is defined as entry inside
 * that insance
 */

#define LIST_NEXT(element,field)	((element)->field.next)

/** For each loop on teh list */
#define LIST_FOREACH(var, head, field)\
	for ((var) = LIST_FIRST((head));\
	(var);\
	(var) = LIST_NEXT((var),field))


/* A reseting function */
#define LIST_INIT(head)\
	do {\
		LIST_FIRST((head)) = NULL;\
	}while(0)

#define LIST_INSERT_AFTER(pre, element, field)\
	do{\
	if((LIST_NEXT((element),field) = (LIST_NEXT((pre),field))) != NULL)\
		LIST_NEXT(pre,field)->field.prev = &(LIST_NEXT((element),field));\
	LIST_NEXT((pre),field)->field.next = (element);\
	(element)->field.prev = &(LIST_NEXT((pre),field));\
	} while(0)


#define LIST_INSERT_BEFORE(post, element, field)\
	do{\
	(element)->field.prev = (post)->field.prev;\
	LIST_NEXT((element),field) = (post);\
	*(post)->field.prev = (element);\
	(post)->field.prev = &(LIST_NEXT((element),field));\
	} while(0)
			
#define LIST_REMOVE(element, field)\
	do {\
	if(LIST_NEXT((element),field) != NULL)\
		LIST_NEXT((element),field)->field.prev =\
			(element)->field.prev;\
	*(element)->field.prev = LIST_NEXT( (element), field);\
	}while(0)

#define LIST_INSERT_HEAD(head, element, field)\
	do {\
	if((LIST_NEXT((element), field) = LIST_FIRST((head))) != NULL)\
		LIST_FIRST((head))->field.prev = &(LIST_NEXT((element),field));\
	LIST_FIRST((head)) = (element);\
	(element)->field.prev = &(LIST_FIRST((head)));\
	} while(0)

#endif
