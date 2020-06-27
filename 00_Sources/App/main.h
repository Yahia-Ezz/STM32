#ifndef MAIN_H
#define MAIN_H




#define AFIO_BASE_ADD 0x40010000U
typedef struct
{
	uint16_t EVCR;	// EN - PORT 001 - PIN 0
	uint16_t ESERVED_EVCR;
	uint32_t MAPR;
	uint16_t EXTICR1;
	uint16_t RESERVED_EXTICR1;
	uint16_t EXTICR2;
	uint16_t RESERVED_EXTICR2;
	uint16_t EXTICR3;
	uint16_t RESERVED_EXTICR3;
	uint16_t EXTICR4;
	uint16_t RESERVED_EXTICR4;
	uint16_t MAPR2;
	uint16_t RESERVED_MAPR2;
}AFO_type;


#define STK_BASE_ADD 	0xE000E010

typedef struct 
{
	uint32_t CTRL;
	uint32_t LOAD;
	uint32_t VAL;
	uint32_t CALIB;	
}STK_type;

#endif
