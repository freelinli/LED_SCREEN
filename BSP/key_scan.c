/******************** (C) COPYRIGHT  freelin *************************************
 * �ļ���  ��main.c
 * ����    ������ɨ�����    
 * ʵ��ƽ̨�� STM8������
 * ��汾  ��V1.0.0
 * ����֧�֣�wechat(17612172119)
 * �޸�ʱ�䣺2016-08-25

**********************************************************************************/

#include "key_scan.h"
#include "delay_time.h"


void Buttom_Init(void)
{
    GPIO_Init(Button_Port, Buttom_1, GPIO_MODE_IN_PU_NO_IT);
    GPIO_Init(Button_Port, Buttom_2, GPIO_MODE_IN_PU_NO_IT);
}

void Shake_Key_Init(void)
{
    GPIO_Init(Shake_Key_Port, Shake_Key_Pin, GPIO_MODE_IN_PU_NO_IT);
}



uint8_t Key_Scan1(GPIO_TypeDef* GPIOx,GPIO_Pin_TypeDef Buttom, u8 butt)
{
  static uint16_t flag_click_times = 0;
  static uint8_t  flag_return_status = 0;
    
      if(GPIO_ReadInputPin(GPIOx,Buttom) == butt)
      {
        flag_click_times ++  ;
        if(flag_click_times > BUTTON_EXTENDED_CLICK) // extended press
        {
          flag_click_times = BUTTON_EXTENDED_CLICK_RUNOUT;
          flag_return_status  =     2;
        } else if(flag_click_times >  BUTTON_CLICK) // click
        {
          flag_return_status   =     1;
        } else                                  // delay
          flag_return_status =  0;

      }else
      {
          if( flag_click_times -- <= 0)
         {
              flag_click_times = 0;
              flag_return_status  =   0;
         }     
      }
      return flag_return_status;

}


uint8_t Key_Scan2(GPIO_TypeDef* GPIOx,GPIO_Pin_TypeDef Buttom, u8 butt)
{
  static uint16_t flag_click_times = 0;
  static uint8_t flag_return_status = 0;
    
    
      if(GPIO_ReadInputPin(GPIOx,Buttom) == butt)
     {
        flag_click_times ++  ;
        if(flag_click_times > BUTTON_EXTENDED_CLICK) // extended press
        {
          flag_click_times = BUTTON_EXTENDED_CLICK_RUNOUT;
          flag_return_status  =     2;
        } else if(flag_click_times>  BUTTON_CLICK) // click
        {
          flag_return_status   =     1;
        } else                                  // delay
          flag_return_status =  0;

      }else
      {
          if( flag_click_times -- <= 0)
         {
              flag_click_times = 0;
              flag_return_status  =   0;
         }     
      }
      return flag_return_status;
}
   
uint8_t Shake_Key_Scan(GPIO_TypeDef* GPIOx,GPIO_Pin_TypeDef Buttom)
{
    static uint16_t flag_click_times = 0, flag_release_times = 0;
    uint8_t flag_return_status = 0;
    uint8_t GPIO_ReadInputPin_Data = 0;  
    
    GPIO_ReadInputPin_Data = GPIO_ReadInputPin(GPIOx,Buttom);
      if(GPIO_ReadInputPin_Data == 0) // ��
     {
            flag_release_times ++  ;
            if(flag_release_times >  BUTTON_CLICK) // click
            {
                flag_release_times = BUTTON_CLICK_RUNOUT;            
                if(flag_click_times > BUTTON_CLICK)
                {
                  flag_return_status   =     2; // ҡ����-->��
                }
                flag_click_times = 0;
            } 
            
      }else    //��
      {

         flag_click_times ++  ;
         if(flag_click_times >  BUTTON_CLICK) // click
        {
          
          flag_click_times = BUTTON_CLICK_RUNOUT;          
          if(flag_release_times > BUTTON_CLICK)
          {
            flag_return_status   =     1; // ҡ����-->��
          }
          flag_release_times = 0;
        }

      }
      return flag_return_status;
}






/******************* (C) COPYRIGHT freelin *****END OF FILE****/