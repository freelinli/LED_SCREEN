#ifndef __TIM1_H    //����
#define __TIM1_H    //����
#include "stm8s.h"  //����

void Tim1_Init(void);  //��ʱ��1��ʼ������
void TimingDelay_Decrement(void);  //��ʱ��1��ʱ����
void Tim1_ms( u32 nTime);   //ʵ��1���Ӿ�ȷ��ʱ����

#endif

