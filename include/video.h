/*
 * Saad Talaat
 * CATReloaded (C) Copyrights 2011
 * http://catreloaded.net
 */

#ifndef _CATERNEL_VIDEO_H_
#define _CATERNEL_VIDEO_H_
/* try to read the CGA/MONO documentation */
#define CGA_INDEX1	0x3D4
#define CGA_DATA1	0x3D5
#define CGA_BUFF_OFF	0xB8000
#define CGA_COLS	80
#define CGA_ROWS	25
#define CGA_SIZE	(CGA_ROWS * CGA_COLS)		
#define COLOR_DARK_GRAY	0x0700
#define COLOR_BLUE	0x0100
#define COLOR_GREEN	0x0200
#define COLOR_RED	0x0400
#define COLOR_GRAY	0x0800
#define COLOR_WHITE	0x0f00
#define BACKGROUND_GRAY	0x7000
#define BACKGROUND_BLUE	0x1000
#define BACKGROUND_GREEN	0x2000
#define BACKGROUND_RED	0x4000
#define BACKGROUND_BLINK	0x8000
#define BACKGROUND_WHITE	0xF000
//static int serial_exists;
uint16_t	cursor_position;
uint16_t	*char_buff;
void		cga_init(void);
uint16_t	cga_get_pos(void);
void		cga_set_pos(uint16_t);
void		cga_putc(int c);
void		cga_set_attr(uint16_t);
#endif
