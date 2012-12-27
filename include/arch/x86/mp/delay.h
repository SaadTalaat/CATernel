/**
 * 
 * @file delay.h
 * @author Menna Essa
 * @brief Simple delay functions headers
 * 
 */ 

#ifndef _CATERNEL_DELAY_H_
#define _CATERNEL_DELAY_H_
void delay(uint32_t microsec);
void asm_delay_loop(uint32_t t);
void asm_fake_loop(uint32_t t);
#endif

