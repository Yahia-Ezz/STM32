#ifndef SERIAL_PRINT_H
#define SERIAL_PRINT_H

#define SERIAL_PRINT_BUFF_QUEUE_SIZE	15U
#define SERIAL_PRINT_BUFF_SIZE			950U



void Serial_Print_Init(void);
void SERIAL_Print(char *fmt, ...);
#endif