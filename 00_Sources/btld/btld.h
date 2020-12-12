/*
 * btld.h
 *
 *  Created on: Dec 12, 2020
 *      Author: Yahia Ezz
 */

#ifndef BTLD_H_
#define BTLD_H_


typedef struct
{
    char *UserRequestDescription;
    void (*PreFunc)();
}DBG_LST_t;



void BTLD_CLI_Handler(void);

#endif /* BTLD_H_ */
