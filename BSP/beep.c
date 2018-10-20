
/******************** (C) COPYRIGHT  freelin *************************************
 * �ļ���  ��beep.c
 * ����    ������������     
 * ʵ��ƽ̨�� STM8������
 * ��汾  ��V1.0.0
 * ����֧�֣�wechat(17612172119)
 * �޸�ʱ�䣺2016-08-25

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
  uint16_t Beep_Option_status;/*��¼���ѡ����Beep��״̬*/
  Beep_Option_status=FLASH_ReadOptionByte(Beep_OptionAdd);
  /*Beep_Option_status�����λΪ1������Beep,���򲻼���*/
  if(!(Beep_Option_status&0x8000))
  {
    FLASH_ProgramOptionByte(Beep_OptionAdd, (uint8_t)(Beep_Option_status|0x8000));
    /*��Beep_OptionAdd  0x4803 ��1��������Beep*/
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




/******************* (C) COPYRIGHT freelin *****END OF FILE****/