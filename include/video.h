#ifndef _CATERNEL_VIDEO_H_
#define _CATERNEL_VIDEO_H_
/* try to read the CGA/MONO documentation */
#define CGA_INDEX1	0x3D4
#define CGA_DATA1	0x3D5
#define CGA_BUFF_OFF	0xB8000
#define CGA_COLS	80
#define CGA_ROWS	25
#define CGA_SIZE	(CGA_ROWS * CGA_COLS)		

//static int serial_exists;
uint16_t	cursor_position;
uint16_t	*char_buff;
void		cga_init(void);
uint16_t	cga_get_pos(void);
void		cga_set_pos(uint16_t);
void		cga_putc(int c);
#endif
