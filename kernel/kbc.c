#include <types.h>
#include <x86.h>
#include <cli.h>
#include <kbc.h>



#define ESCODE	(1<<6)
#define CTL	(1<<1)
#define SHIFT	(1<<0)
#define ALT	(1<<2)
#define CAPSLOCK (1<<3)
#define NUMLOCK	 (1<<4)
#define SCROLLLOCK (1<<5)
static uint8_t shiftcode[256]=
{
	[0x1D] CTL,
	[0x2A] SHIFT,
	[0x36] SHIFT,
	[0x38] ALT,
	[0x9D] CTL,
	[0xB8] ALT

};
static uint8_t togglecode[256] =
{
	[0x3A] CAPSLOCK,
	[0x45] NUMLOCK,
	[0x46] SCROLLLOCK
};
#define NUL	0
static uint8_t normalmap[256] =
{
	NUL,0x1B,'1','2','3','4','5','6',
	'7','8','9','0','-','=','\b','\t',
	'q','w','e','r','t','y','u','i',
	'o','p','[',']','\n',NUL,'a','s',
	'd','f','g','h','j','k','l',';',
	'\'','`',NUL,'\\','z','x','c',
	'v','b','n','m',',','.','/',NUL,'*',
	NUL,' ',NUL,NUL,NUL,NUL,NUL,NUL,NUL,
	NUL,NUL,NUL,NUL,NUL,NUL,'7','8','9',
	'-','4','5','6','+','1','2','3','0',
	'.', NUL, NUL, NUL, NUL,
	[0x97] KEY_HOME,	[0x9C] '\n',
	[0xB5] '/',		[0xC8] KEY_UP,
	[0xC9] KEY_PGUP,	[0xCB] KEY_LF,
	[0xCD] KEY_RT,		[0xCF] KEY_END,
	[0xD0] KEY_DN,		[0xD1] KEY_PGDN,
	[0xD2] KEY_INS,		[0xD3] KEY_DEL
};

static uint8_t shiftmap[256] =
{
	NUL,0x1B,'!','@','#','$','%','^',
	'&','*','(',')','_','+','\b','\t',
	'Q','W','E','R','T','Y','U','I','O',
	'P','{','}','\n',NUL,'A','S','D','F',
	'G','H','J','K','L',':','"','~',
	NUL,'|','Z','X','C','V','B','N','M',
	'<','>','?',NUL,'*',NUL,' ',NUL,NUL,
	NUL,NUL,NUL,NUL,NUL,NUL,NUL,NUL,NUL,
	NUL,NUL,'7','8','9','-','4','5','6',
	'+','1','2','3','0','.', NUL,NUL,NUL,NUL,
	[0x97] KEY_HOME,        [0x9C] '\n',
	[0xB5] '/',             [0xC8] KEY_UP,
	[0xC9] KEY_PGUP,        [0xCB] KEY_LF,
	[0xCD] KEY_RT,          [0xCF] KEY_END,
	[0xD0] KEY_DN,          [0xD1] KEY_PGDN,
	[0xD2] KEY_INS,         [0xD3] KEY_DEL
};
#define CL(x)	((x)-'@')
static uint8_t ctlmap[256] =
{
	NUL,	NUL,	NUL,	NUL,	NUL,	NUL,	NUL,	NUL,
	NUL,	NUL,	NUL,	NUL,	NUL,	NUL,	NUL,	NUL,
	CL('Q'),CL('W'),CL('E'),CL('R'),CL('T'),CL('Y'),CL('U'),CL('I'),
	CL('O'),CL('P'),NUL,	NUL,	'\r',	NUL,	CL('A'),CL('S'),
	CL('D'),CL('F'),CL('G'),CL('H'),CL('J'),CL('K'),CL('L'),NUL,
	NUL,	NUL,	NUL,	CL('\\'),CL('Z'),CL('X'),CL('C'),CL('V'),
	CL('B'),CL('N'),CL('M'),NUL,	NUL,	CL('/'),NUL,	NUL,
	[0x97] KEY_HOME,	[0x9C] '\n',
	[0xB5] CL('/'),		[0xC8] KEY_UP,
	[0xC9] KEY_PGUP,	[0xCB] KEY_LF,
	[0xCD] KEY_RT,		[0xCF] KEY_END,
	[0xD0] KEY_DN,		[0xD1] KEY_PGDN,
	[0xD2] KEY_INS,		[0xD3] KEY_DEL


};
static uint8_t *charcode[4] = {
	normalmap,
	shiftmap,
	ctlmap,
	ctlmap
};
int kbc_data(void){
	int c;
	uint8_t data;
	static uint32_t shift;
	if((inb(KBC_STATUSPORT) & KBC_DATAIN) == 0)
		return -1;
	data = inb(KBC_DATAPORT);
	if(data == 0xE0){
		shift |= ESCODE;
		return 0;
	} else if( data & 0x80){
		data = ( shift & ESCODE? data : data &0x7F);
		shift &= ~(shiftcode[data] | ESCODE);
		return 0;
	} else if (shift & ESCODE){
		data |= 0x80;
		shift &= ~ESCODE;
	}

	shift |= shiftcode[data];
	shift ^= togglecode[data];
	c = charcode[shift & (CTL | SHIFT)][data];
	if (shift & CAPSLOCK) {
		if ('a' <= c && c <= 'z')
			c += 'A' - 'a';
		else if ('A' <= c && c <= 'Z')
			c += 'a' - 'A';
	}
	if (!(~shift & (CTL|ALT)) && c == KEY_DEL){
		outb(0x3,0x92);
	}
	return c;
}
void
kbc_interrupt(void)
{
	console_interrupt(kbc_data);
}


