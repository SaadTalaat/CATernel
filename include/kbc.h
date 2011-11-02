/*
 * Saad Talaat
 * CATReloaded (C) Copyrights 2011
 * http://catreloaded.net
 */
#include <types.h>
#ifndef _CATERNEL_KBC_H_
#define _CATERNEL_KBC_H_
/**** PS/2 Keyboard I/O Ports ********/
#define KBC_STATUSPORT	0x64
#define KBC_DATAPORT	0x60


/**** Functions **********************/

uint8_t kbc_data(void);

#endif
