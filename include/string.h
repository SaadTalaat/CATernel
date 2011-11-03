/* Saad Talaat
 * CATReloaded Team
 * http://catreloaded.net
 */
#include <types.h>

#ifndef _CATERNEL_STRING_H_
#define _CATERNEL_STRING_H_


char 	*strcpy(char *dst,char* src);
char 	*strncpy(char *dst,char *src,uint32_t count);
char 	*strlcpy(char *dst,char *src,uint32_t count);
char 	*strcat(char *dst,char *src);
char 	*strncat(char *dst,char *src,uint32_t count);
char 	*strlcat(char *dst,char *src,uint32_t count);
int 	strcmp(const char *cmp1, const char *cmp2);
int	strncmp(const char *cmp1, const char *cmp2,uint32_t count);
char	*strchr(const char *str,int c);
char	*strrchr(const char *str,int c);
char	*strnchr(const char *str,uint32_t count, int c);
uint32_t strlen(const char* str);
uint32_t strnlen(const char* str, uint32_t count);
void	*memset(void *dst, int c, uint32_t count);
void	*memcopy(void *dst, const void *src, uint32_t count);
void	*memmove(void *dst,const void *src, uint32_t count);
void 	*memcmp(const void *cmp1,const void *cmp2, uint32_t count);

#endif
