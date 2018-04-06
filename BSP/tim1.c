
/******************** (C) COPYRIGHT  �����Ƽ� *************************************
 * �ļ���  ��tim1.c
 * ����    ��tim1����led��˸����   
 * ʵ��ƽ̨�� STM8������
 * ��汾  ��V1.0.0
 * ����֧�֣�lijinnan@qiqirobot.com
 * �޸�ʱ�䣺2016-08-25
 * �Ա���ַ��https://lsqrd.taobao.com/index.htm?spm=2013.1.w5002-7895914584.2.kYLHaf
**********************************************************************************/

#include "stm8s_tim1.h"
#include "tim1.h"

static  u32 TimingDelay;   //��������


#define TIM1_TIME  1000 //ms

void Tim1_Init(void)    //��ʱ��1��ʼ��
{
    TIM1_TimeBaseInit(16,TIM1_COUNTERMODE_UP,TIM1_TIME,0); //16��Ƶ�����ϼ������������˴����жϣ������
    //�� 1MS �����жϣ��������Ӧ�ü��� 1000 �Σ�1MHZ/1000=1KHZ�����þ��� 1ms��
    TIM1_ARRPreloadConfig(ENABLE); //ʹ���Զ���װ
    TIM1_ITConfig(TIM1_IT_UPDATE , ENABLE); //���ݸ����ж�
    TIM1_Cmd(ENABLE); //����ʱ��
}

void TimingDelay_Decrement(void) //��ʱ��1��ʱ����
{
    if (TimingDelay != 0x00)   //�����Ϊ0��ѭ��ִ��
    { 
        TimingDelay--;      //�����Ϊ0��������
    }else{
      TimingDelay = TIM1_TIME;
    }
}

void Tim1_ms( u32 nTime)    //��ʱ1��ѭ��
{
    TimingDelay = nTime;
    while(TimingDelay != 0);
}

/******************* (C) COPYRIGHT �����Ƽ� *****END OF FILE****/
