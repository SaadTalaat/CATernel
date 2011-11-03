#include <string.h>


uint32_t strnlen(const char* str, uint32_t count){
	int n;
	for (n=0;count > 0 && *str != '\0';str++,count--){
		n++;
	}
	return n;
}
void *
memcopy(void *dst, const void *src, uint32_t count){
	const char *src2;
	char *dst2;
	src2=src;
	dst2=dst;
	while(count-- > 0){
		*dst2++=*src2++;
	}
	return dst2;

}
