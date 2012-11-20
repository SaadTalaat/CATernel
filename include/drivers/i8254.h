/**
 * @file i8254.h
 * @author Saad Talaat
 * @brief Intel 8254 PIT
 */

#ifndef _CATERNEL_8254_H_
#define _CATERNEL_8254_H_
#include <types.h>
#define PIT_CHANNEL0	0x40
#define PIT_CHANNEL1	0x41
#define PIT_CHANNEL2	0x42
#define PIT_MODE	0x43
#define PIT_TICKS	0x1234DF


/* PIT MODES */
#define MODE_ONESHOT	0x2
#define MODE_RATEGEN	0x4
#define MODE_WAVEGEN	0x6
#define MODE_SOFTSRB	0x8
#define MODE_HARDSRB	0xA

/* PIT LATCH */
#define LATCH_LSB	0x10
#define LATCH_MSB	0x20
#define LATCH_ALL	0x30

/* PIT COUNTER SELECT */
#define SELECT_CNT0	0x0
#define SELECT_CNT1	0x40
#define SELECT_CNT2	0x80

#endif
