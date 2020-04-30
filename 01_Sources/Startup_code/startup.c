#include <stdint.h>
#include "startup.h"
 

/* Create references to symbols defined in the linker script   */ 
extern uint32_t _data_load;
extern uint32_t _data_start;
extern uint32_t _data_end;
extern uint32_t _bss_start;
extern uint32_t _bss_end;


uint32_t * InterruptVectorArr[2] __attribute__ ((section(".interruptsvector"))) =
{

	(uint32_t*) 0x20005000,
	(uint32_t*) startup_func
};

void startup_func(void)
{
	uint32_t *SRC,*DEST;

	/*Load .data from FLASH to RAM*/
	for(SRC=&_data_load,DEST=&_data_start ; DEST < &_data_end ; SRC++,DEST++)
	{
		*DEST = * SRC;
	}


	/* Load .bss from FLASH to RAM and initialize it to 0 */
	for(SRC=&_bss_start,DEST=&_bss_end;DEST< &_bss_end ; SRC++,DEST++)
	{
		*DEST = 0;
	}
	main();
}
