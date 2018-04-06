
/******************** (C) COPYRIGHT  奇来科技 *************************************
 * 文件名  ：adc.c
 * 描述    ：STM8S的adc实验    
 * 实验平台： STM8开发板
 * 库版本  ：V1.0.0
 * 技术支持：lijinnan@qiqirobot.com
 * 修改时间：2016-08-25
 * 淘宝网址：https://lsqrd.taobao.com/index.htm?spm=2013.1.w5002-7895914584.2.kYLHaf
**********************************************************************************/

#include "stm8s.h"
#include "stm8s_adc1.h"
#include "adc.h"
#include "delay_time.h"



void ADC1_Config(void)     //自定义ADC转换初始化函数
{
    ADC1_DeInit();  //ADC2初始化函数
    //ADC2_CONVERSIONMODE_SINGLE   单次转换
    //通道3
    ADC1_Init(ADC1_CONVERSIONMODE_SINGLE,ADC1_CHANNEL_3, ADC1_PRESSEL_FCPU_D18,ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT, ADC1_SCHMITTTRIG_CHANNEL3,DISABLE);
    ADC1_ITConfig(ADC1_IT_EOCIE, DISABLE);
    
    ADC1_Cmd(ENABLE);  //启动ADC2
//    ADC2_StartConversion();   //启用转换
}

u16 ADC1_Get(void)
{
    u16 adc_Value=0;    //付初值0
    ADC1_StartConversion();//开始转换
    adc_Value = ADC1_GetConversionValue();//获得转换结果
    
    return adc_Value;
}