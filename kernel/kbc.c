#include <types.h>
#include <x86.h>
#include <kbc.h>
uint8_t kbc_data(void){
	uint8_t data;
	data = inb(KBC_STATUSPORT);
	return data;
}
