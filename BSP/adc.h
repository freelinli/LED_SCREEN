#ifndef _ADC_H
#define _ADC_H
#include "stm8s.h"



#define LOW_POWER 3.6
#define LOW_POWER_ADC  (u16)(LOW_POWER * 1024 /6.6)

void ADC1_Config(void);   //自定义ADC转换初始化函数
u16 ADC1_Get(void); //ADC转换例程

#endif