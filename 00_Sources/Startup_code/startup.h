
#ifndef STARTUP_H
#define STARTUP_H

#define RCC_BASE_ADD 		0x40021000U

void EXTI0_Handler(void);
void startup_func(void);
int main(void);
void SysTick_Handler(void);

#endif
