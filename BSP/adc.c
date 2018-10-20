
/******************** (C) COPYRIGHT  freelin *************************************
 * �ļ���  ��adc.c
 * ����    ��STM8S��adcʵ��    
 * ʵ��ƽ̨�� STM8������
 * ��汾  ��V1.0.0
 * ����֧�֣�wechat(17612172119)
 * �޸�ʱ�䣺2016-08-25

**********************************************************************************/

#include "stm8s.h"
#include "stm8s_adc1.h"
#include "adc.h"
#include "delay_time.h"



void ADC1_Config(void)     //�Զ���ADCת����ʼ������
{
    ADC1_DeInit();  //ADC2��ʼ������
    //ADC2_CONVERSIONMODE_SINGLE   ����ת��
    //ͨ��3
    ADC1_Init(ADC1_CONVERSIONMODE_SINGLE,ADC1_CHANNEL_3, ADC1_PRESSEL_FCPU_D18,ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT, ADC1_SCHMITTTRIG_CHANNEL3,DISABLE);
    ADC1_ITConfig(ADC1_IT_EOCIE, DISABLE);
    
    ADC1_Cmd(ENABLE);  //����ADC2
//    ADC2_StartConversion();   //����ת��
}

u16 ADC1_Get(void)
{
    u16 adc_Value=0;    //����ֵ0
    ADC1_StartConversion();//��ʼת��
    adc_Value = ADC1_GetConversionValue();//���ת�����
    
    return adc_Value;
}