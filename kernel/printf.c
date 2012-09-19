#include <types.h>
#include <stdio.h>

#define hex2ascii(x)("0123456789ABCDEF"[x])
static void
putch(int c,int *count){
	putchr(c);
	*count++;
}
void ksprintkn(void (*func)(int,int *),int *count,uintmax_t num,int base,int width,int padc){
	int c;
	if( num >= base)
		ksprintkn(func,count,num / base, base, width-1, padc);
	else
		while(--width > 0)
			func(padc,count);
	c = hex2ascii(num%base);
	func(c,count);
}
int
getint(va_list *ap,int lflag){
	if(lflag >= 2)
		return va_arg(*ap,long long);
	else if(lflag)
		return va_arg(*ap,long);
	else
		return va_arg(*ap,int);

}
int
getuint(va_list *ap,int lflag){

	if(lflag >= 2)
		return va_arg(*ap,unsigned long long);
	else if(lflag)
		return va_arg(*ap,unsigned long);
	else
		return va_arg(*ap,unsigned int);
}


int
kvprintk(const char* format,void (*func)(int,int *),int *count,va_list ap){
	char *d;
	register const char* p;
	char padc;
	int ch,width,lflag=0,gflag=0,sign=0,neg=0,altflag=0;
	int base,precision;
	intmax_t num;
	for(;;){
		padc = ' ';
		width = -1;
		while((ch = *(unsigned char*) format++) != '%'){
			if(ch == '\0')
				return *count;
			func(ch,count);
		}
		reswitch:
			switch(ch =(unsigned char)*format++){
			case '-':
				padc='-';
				goto reswitch;
			case '0':
				padc='0';
				goto reswitch;
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				for(precision=0; ; ++format){
					precision = precision*10 + ch - '0';
					ch = *format;
					if( ch < '0' || ch > '9')
						break;
				}
				goto precision_process;
			case '*':
				precision = va_arg(ap,int);
				goto precision_process;
			case '.':
				if (width < 0)
					width =0;
				goto reswitch;
			case '#':
				altflag=1;
				goto reswitch;
precision_process:
			if(width < 0)
				width = precision, precision = -1;
			goto reswitch;
			case '%':
				func('%',count);
				goto reswitch;
			case 'c':
				func(va_arg(ap,int),count);
			case 's':
                        if ((p = va_arg(ap, char *)) == NULL)
                                p = "(null)";
                        if (width > 0 && padc != '-')
                                for (width -= strnlen(p, precision); width > 0; width--)
                                        putch(padc, count);
                        for (; (ch = *p++) != '\0' && (precision < 0 || --precision >= 0); width--)
                                if (altflag && (ch < ' ' || ch > '~'))
                                        putch('?', count);
                                else
                                        putch(ch, count);
                        for (; width > 0; width--)
                                putch(' ', count);
                        break;
			case 'l':
				lflag ++;
				goto reswitch;
			case 'd':
			case 'i':
				num = getint(&ap,lflag);
				if( (long long) num < 0){
					func('-',count);
					num= -(long long) num;
				}
				sign =1;
				base =10;
				goto number;
			case 'u':
				num = getuint(&ap,lflag);
				base = 10;
				goto number;	
			case 'o':
				num = getuint(&ap,lflag);
				base= 8;
				goto number;
			case 'x':
				num = getuint(&ap,lflag);
				base = 16;
				goto number;
			case 'p':
				func((int)'0',count);
				func((int)'x',count);
				num = (unsigned long long) (uint32_t) va_arg(ap, void*);
				base = 16;
				width = 8;
				padc = '0';
				goto number;
number:
				ksprintkn(func,count,num,base,width,padc);
				break;
			default:
				func((int)'%',count);
				for(format--;format[-1] != '%'; format--);
				break;
			}
			
	}

}
vprintk(const char* format,va_list ap){
	int count=0;
	count = kvprintk(format,putch,&count,ap);
	return count;

}
int
printk(const char *format, ...){
	va_list ap;
	int count;
	va_start(ap,format);
	count = vprintk(format,ap);
	va_end(ap);

	return count;
}
