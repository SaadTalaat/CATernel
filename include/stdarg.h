/* Saad Talaat
 * CATReloaded (C) Copyrights 2011
 * http://catreloaded.net
 */
/** This code is a used in both linux and BSD **/

#ifndef _CATERNEL_STDARG_H_
#define _CATERNEL_STDARG_H_
typedef char*	va_list;

#define __va_size(type)\
	(((sizeof(type) + sizeof(int)-1)/sizeof(int))*sizeof(int))
#define va_start(ap,last)\
		((ap) = (va_list)&(last) + __va_size(last))
#define va_arg(ap,type)\
	(*(type *)((ap) += __va_size(type), (ap) - __va_size(type)))
#define va_end(ap)
#endif
