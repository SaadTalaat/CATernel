

#ifndef _CATERNEL_TEST_H_
#define _CATERNEL_TEST_H_
#include <stdio.h>
#include <stdarg.h>

void _panic_(const char *, int, const char *, ...) __attribute__((noreturn));

#define panic(...) \
	printk(">>>>>> KERNEL PANIC <<<<<<<\n");\
	_panic_(__FILE__,__LINE__,__VA_ARGS__)

#define assert(x)\
		if(!(x)){	\
		panic("Assertion Failed %s\n",#x);\
		}
#endif
