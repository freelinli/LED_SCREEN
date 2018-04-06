#ifndef __TIM1_H    //声明
#define __TIM1_H    //声明
#include "stm8s.h"  //声明

void Tim1_Init(void);  //定时器1初始化函数
void TimingDelay_Decrement(void);  //定时器1定时函数
void Tim1_ms( u32 nTime);   //实现1秒钟精确定时函数

#endif

