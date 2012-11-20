/**
 * @file kconsole.h
 * @author Saad Talaat
 * @date 19 November, 2012
 * @brief Kernel console for debugging ease
 */

#ifndef _CATERNEL_KCONSOLE_H_
#define _CATERNEL_KCONSOLE_H_

#include <types.h>
#include <proc/proc.h>
#include <arch/x86/processor.h>
#include <arch/x86/interrupt.h>
#include <arch/x86/mm/page.h>
#define KCONSOLE_MAX_CMDS 30
typedef struct kcommand{
	const char *name;
	const char *hint;
	void (*operation)(void);
} kcommand_t;

void kconsole_init(void);
void kconsole(void);
void kcommand_register(kcommand_t *);
kcommand_t *kcommand_match(const char *);
extern kcommand_t *kconsole_commands;

/** KCOMMANDS */
void kscheduler_info(void);
void kcpu_info(void);
void kconsole_help(void);
#endif
