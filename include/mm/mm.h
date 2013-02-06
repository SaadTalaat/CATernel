/**
 * @file mm.h
 * @author Saad Talaat
 * @brief Memory management
 * @details
 * Generic Memory management unit is responsible for
 * memory allocation.
 */

#ifndef _CATENREL_MM_MM_H_
#define _CATERNEL_MM_MM_H_
#include <types.h>
#include <arch/x86/mm/page.h>
#include <structs/htable.h>

extern void mm_init(void);
extern void* kmalloc(uint32_t);

#endif
