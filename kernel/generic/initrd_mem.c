/**
 * @addtogroup --Add Category Here--
 * @{
 * @file initrd.c
 * @author Saad Talaat
 * @date Saturday 08/06/2013
 * @brief Initrd
 * @name Initrd
 * @{
 *
 */
#include <initrd_mem.h>

void*
initrd_mem()
{
	void *initrd_start;
	initrd_start = allocate( 3*PAGESZ, PAGESZ);
	return initrd_start;
}
/**
 * @} @}
 */

