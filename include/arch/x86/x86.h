/*
 * Saad Talaat
 * CATReloaded (C) Copyrights 2011
 * http://catreloaded.net
 */
#ifndef _ASM_X86_X86_H
#define _ASM_X86_X86_H

#include <types.h>
static __inline void outb(uint8_t data,int port)
{
        __asm __volatile("outb %0,%w1" : : "a" (data), "d" (port));
}
static __inline uint8_t inb(int port)
{
	uint8_t data;
	__asm __volatile("inb %w1,%0" : "=a" (data) : "d" (port));
	return data;
}
static inline void outw(uint16_t data,int port)
{
        __asm __volatile("outw %0,%w1" : : "a" (data), "d" (port));

}
static inline uint16_t inw(int port)
{
	uint16_t data;
	__asm __volatile ("inw %w1,%0" : "=a" (data) : "d" (port));
	return data;
}
static __inline void outl(uint32_t data,int port)
{
        __asm __volatile("outl %0,%w1" : : "a" (data), "d" (port));

}
static inline uint32_t inl(int port)
{
	uint32_t data;
	__asm __volatile ("inl %0,%wl"  : "=a" (data) : "d" (port));
	return data;
}
static inline void insb(void *addr,int cnt,int port){
	__asm __volatile("cld\n\trepne\n\tinsb"		:
			"=D" (addr),"=c" (cnt)		:
			"d" (port), "0" (addr),"1" (cnt):
			"memory","cc");			
}
static inline void insl(void *addr,int cnt,int port){
        __asm volatile("cld\n\trepne\n\tinsl"             :
                        "=D" (addr),"=c" (cnt)          :
                        "d" (port), "0" (addr),"1" (cnt):
                        "memory","cc");
}
static inline void insw(void *addr,int cnt,int port){
        __asm volatile("cld\n\trepne\n\tinsw"             :
                        "=D" (addr),"=c" (cnt)          :
                        "d" (port), "0" (addr),"1" (cnt):
                        "memory","cc");
}
static inline void cli(void){
	__asm __volatile("cli");
}
static inline void sti(void){
	__asm __volatile("sti");
}
static inline void
write_cr0(uint32_t value)
{
	asm volatile("movl %0,%%cr0"::"r" (value));
}
static inline uint32_t
read_cr0(void)
{
	uint32_t value;
	asm volatile("movl %%cr0,%0":"=r" (value));
	return value;
}
static inline void
write_cr1(uint32_t value)
{
        asm volatile("movl %0,%%cr1"::"r" (value));
}
static inline uint32_t
read_cr1(void)
{
        uint32_t value;
        asm volatile("movl %%cr1,%0":"=r" (value));
        return value;
}
static inline void
write_cr2(uint32_t value)
{
        asm volatile("movl %0,%%cr2"::"r" (value));
}
static inline uint32_t
read_cr2(void)
{
        uint32_t value;
        asm volatile("movl %%cr2,%0":"=r" (value));
        return value;
}
static inline void
write_cr3(uint32_t value)
{
        asm volatile("movl %0,%%cr3"::"r" (value));
}
static inline uint32_t
read_cr3(void)
{
        uint32_t value;
        asm volatile("movl %%cr3,%0":"=r" (value));
        return value;
}
static inline void
write_cr4(uint32_t value)
{
        asm volatile("movl %0,%%cr4"::"r" (value));
}
static inline uint32_t
read_cr4(void)
{
        uint32_t value;
        asm volatile("movl %%cr4,%0":"=r" (value));
        return value;
}
static inline void cpuid()
{
	asm("CPUID");
}
static inline void
invlpg(void *addr)
{
        __asm __volatile("invlpg (%0)" : : "r" (addr) : "memory");
}

static inline void
write_tr(uint16_t selector)
{
	asm volatile("ltr %0" :: "r" (selector));
}
static inline uint32_t
read_esp(void)
{
	uint32_t value;
	asm volatile("movl %%esp,%0":"=r" (value));
	return value;
}
#endif
