#include <types.h>
#include <kbc.h>
#include <stdio.h>
#include <video.h>

#define BUFLEN	1024
static char buf[BUFLEN];

char*
readline(const char* towrite){
	int c,i;
	if(towrite != NULL)
		printk("%s",towrite);
	i=0;
	while(1){
		c = getchar();
		if(c < 0){
			printk("Error reading from the keyboard!\n");
			return NULL;
		} else if( c >= ' ' && i < BUFLEN-1){
			putchr(c);
			buf[i++] = c;
		} else if(c == '\b' && i > 0){
			putchr(c);
			i--;
		} else if(c == '\n' || c == '\r'){
			putchr(c);
			buf[i]=0;
			return buf;
		}


	}

}
