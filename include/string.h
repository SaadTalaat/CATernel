/* Saad Talaat
 * CATReloaded Team
 * http://catreloaded.net
 */
/**
 * @file string.h
 * @brief a header file for string and memory operations.
 *
 */
#include <types.h>

#ifndef _CATERNEL_STRING_H_
#define _CATERNEL_STRING_H_

/**
 * @fn uint32_t strnlen(const char*, uint32_t);
 * @param const char* string.
 * @param uint32_t the maximum size desired.
 * @return the number of chars in a string.
 * @brief the function loops over the string character values till '\0' is reached or reached max value.
 *
 * @fn void* memcopy(void *, const void*, uint32_t);
 * @param void* the destination pointer.
 * @param const void* the source pointer.
 * @param uint32_t the number of bytes to copy.
 * @return the destination pointer.
 * @brief copies bytes from a memory address to another memory address
 *
 * @fn memset(void*, int, uint32_t);
 * @param void* the pointer to use.
 * @param int character to set.
 * @param uint32_t the number of bytes to set.
 * @return the pointer after being set.
 * @brief sets a number of bytes starting from a given pointer to a given character.
 */ 
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
