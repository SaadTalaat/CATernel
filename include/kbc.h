/*
 * Saad Talaat
 * CATReloaded (C) Copyrights 2011
 * http://catreloaded.net
 */
#include <types.h>
#ifndef _CATERNEL_KBC_H_
#define _CATERNEL_KBC_H_
/**** PS/2 Keyboard I/O Ports & Commands ********/
#define KBC_STATUSPORT	0x64
#define KBC_DATAPORT	0x60
/******* Keyboard status commands **********/
#define KBC_DATAIN	0x01	/** New Data in buffer **/
#define KBC_FULLBUF	0x02	/** Buffer is full **/
#define KBC_REBOOT	0x04	/** soft reboot **/
#define KBC_COMMAND	0x08	/** data in output register is a command **/
#define KBC_SECLOCK	0x10	/** Security lock engaged **/
#define KBC_TTIMEOUT	0x20	/** transmission timeout error **/
#define KBC_RTIMEOUT	0x40	/** recieve timeout error **/
#define KBC_PARITY	0x80	/** Parity error **/

/******* PS/2 Keyboard Controller commands *************/
#define KBC_READRAM	0x20	/** Read byte 0 from the internal RAM **/
#define KBC_READLOW	0x21	/** Read byte specified in low 5 bits of the command in 804x's internal RAM **/
#define KBC_WRITERAM	0x60	/** write the data to the address specified in the 5 lower bits of command **/
#define KBC_DISMOUSE	0xA7	/** disable mouse/auxillary port **/
#define KBC_ENMOUSE	0xA8	/** enable mouse/auxillary port **/
#define KBC_TSTMOUSE	0xA9	/** test mouse/auxillary port **/
#define	KBC_SELFTEST	0xAA	/** initiate the self-test **/
#define KBC_INTRTEST	0xAB	/** initiate interface test. Results :
								0 = no error
								1 = keyboard clock line low
								2 = keyboard clock line high
								3 = keyboard data line low
								4 = keyboard data line high **/
#define KBC_DIGDUMP	0xAC 	/** dump the content of 804x's RAM, output port, input port, status word **/
#define KBC_DISKBD	0xAD	/** disable keyboard **/
#define KBC_ENBKBD	0xAE	/** enable keyboard **/
#define KBC_READVER	0xAF	/** Read keyboard version **/
#define KBC_INPREAD	0xC0	/** Read input port **/
#define KBC_OUTREAD	0xD0	/** Read Output port **/
#define KBC_OUTWRTE	0xD1	/** write output port ,next byte will be written to 804x **/
#define KBC_OUTKBD	0xD2	/** Echo keyboard output buffer **/
#define KBC_OUTAUX	0xD3	/** Echo pointing device output buffer **/
#define KBC_AUXWRITE	0xD4	/** Write to pointing device output buffer **/
#define KBC_DISA20	0xDD	/** Disable the A20 gate **/
#define KBC_ENBA20	0xDF	/** Enable the A20 gate **/
#define KBC_PULSE0	0xFE	/** Pulse output bit 0 **/
#define KBC_PULSE1	0xFD	/** ~~~~~~~~~~~~~~~~ 1 **/
#define KBC_PULSE2	0xFB	/** ~~~~~~~~~~~~~~~~ 2 **/
#define KBC_PULSE3	0xF7	/** ~~~~~~~~~~~~~~~~ 3 **/


/*** Keyboard commands for 0x60 controller data port (I) ****/
#define KBC_RESET	0xFF	/** RESET the mouse and the keyboard **/
#define KBC_RESEND	0xFE	/** Resend the last byte **/
#define KBC_DEFAULT	0xF6	/** Set the keyboard to default parameters **/
#define KBC_DISABLE	0xF5	/** disables key scanning, yet set the default parameters **/
#define KBC_ENABLE	0xF4	/** Enable key scanning **/
#define KBC_TYPEMATIC	0xF3	/** Set typematic rate/delay **/
#define KBC_READID	0xF2	/** Read Keyboard ID **/
#define KBC_SETSCAN	0xF0	/** Set/Get scancodes set **/
#define KBC_ECHODIG	0xEE	/** request a diganostic echo from the keyboard **/
#define KBC_INDICATOR	0xED	/** Set mode indicators **/


/****** Special Keys **************/
#define KEY_HOME	0xE0
#define KEY_END		0xE1
#define KEY_UP		0xE2
#define KEY_DN		0xE3
#define KEY_LF		0xE4
#define KEY_RT		0xE5
#define KEY_PGUP	0xE6
#define KEY_PGDN	0xE7
#define KEY_INS		0xE8
#define KEY_DEL		0xE9
/**** Functions **********************/

int kbc_data(void);
void kbc_interrupt(void);

#endif
