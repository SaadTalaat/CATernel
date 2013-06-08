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
#include <initrd.h>

void*
initrd_init()
{
	void *initrd_start;
	initrd_start = allocate( 2*PAGESZ, PAGESZ);
	return initrd_start;
}
/**
 * @} @}
 */

