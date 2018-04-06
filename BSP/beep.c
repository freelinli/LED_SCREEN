
/******************** (C) COPYRIGHT  奇来科技 *************************************
 * 文件名  ：beep.c
 * 描述    ：蜂鸣器工作     
 * 实验平台： STM8开发板
 * 库版本  ：V1.0.0
 * 技术支持：lijinnan@qiqirobot.com
 * 修改时间：2016-08-25
 * 淘宝网址：https://lsqrd.taobao.com/index.htm?spm=2013.1.w5002-7895914584.2.kYLHaf
**********************************************************************************/
#include "beep.h"
#include "delay_time.h"
#include "stm8s_flash.h"
#define Beep_OptionAdd  0x4803



void Beep_Init(BEEP_Frequency_TypeDef BEEP_Frequency)
{
  BEEP_Init(BEEP_Frequency);
  CLK_LSICmd(ENABLE);
  BEEP_Cmd(ENABLE);
}
void Set_Beep_OptionByte(void)
{
  uint16_t Beep_Option_status;/*记录激活备选功能Beep的状态*/
  Beep_Option_status=FLASH_ReadOptionByte(Beep_OptionAdd);
  /*Beep_Option_status的最高位为1激活了Beep,否则不激活*/
  if(!(Beep_Option_status&0x8000))
  {
    FLASH_ProgramOptionByte(Beep_OptionAdd, (uint8_t)(Beep_Option_status|0x8000));
    /*向Beep_OptionAdd  0x4803 置1，激活了Beep*/
  }
}

void Beep_Test(void)
{
  Beep_Init(BEEP_FREQUENCY_2KHZ);
  delay_ms(1000);
  Beep_Init(BEEP_FREQUENCY_1KHZ);
  delay_ms(1000);
  Beep_Init(BEEP_FREQUENCY_4KHZ);
  delay_ms(1000);
  BEEP_Cmd(DISABLE);
}




/******************* (C) COPYRIGHT 奇来科技 *****END OF FILE****/