#ifndef __BEEP_H
#define __BEEP_H
#include "stm8s.h"
#include "stm8s_beep.h"
#define LSI_128kHz 128000

void Beep_Init(BEEP_Frequency_TypeDef BEEP_Frequency);
void Set_Beep_OptionByte(void);
void Beep_Test(void);
#endif