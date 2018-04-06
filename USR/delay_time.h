#ifndef __DELAY_TIME_H
#define __DELAY_TIME_H
#include "stm8s.h"

void delay_us(u16 nCount);
void delay_ms(u16 nCount);

#define delay_400ns asm("nop"); asm("nop")
#define delay_800ns asm("nop"); asm("nop"); asm("nop"); asm("nop")


#endif

