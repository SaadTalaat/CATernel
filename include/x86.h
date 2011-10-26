#ifndef _ASM_X86_X86_H
#define _ASM_X86_X86_H

#include <types.h>
;
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

#endif
