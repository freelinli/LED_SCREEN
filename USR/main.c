
/******************** (C) COPYRIGHT  奇来科技 *************************************
 * 文件名  ：main.c
 * 描述    ：STM8S工程模板     
 * 实验平台： STM8开发板
 * 库版本  ：V1.0.0
 * 技术支持：lijinnan@qiqirobot.com
 * 修改时间：2016-08-25
 * 淘宝网址：https://lsqrd.taobao.com/index.htm?spm=2013.1.w5002-7895914584.2.kYLHaf
**********************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "delay_time.h"
#include "head.h"

/* Extern defines -----------------------------------------------------------*/

extern    unsigned int flag_tim1_time ;
extern u8 RxBuffer[RxBufferSize];
extern u16 UART_RX_NUM;


/* Private defines -----------------------------------------------------------*/


struct FSM_t{
            unsigned int flag_1ms;
            unsigned int flag_5ms;
            unsigned int flag_10ms;
            unsigned int flag_20ms;
            unsigned int flag_1s;

};
struct key_status{
            unsigned char key1_status;
            unsigned char key2_status;
            unsigned char shake_key_status;
}key_status_all;

u8 len = 0;
u16 adc_value = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void Bsp_Init(void);


int main(void)
{
  /* Infinite loop */
  
  struct FSM_t FSM_data = {0,0,0,0,0};
  
  /*设置内部时钟16M为主时钟*/ 
 
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);

  Bsp_Init();
  /*!<Set High speed internal clock  */
  
   
  while (1)
  {
    /*  添加你的代码  */
    //  LED_ShowOneToOne();
 
      ++FSM_data.flag_1ms; 
      
      if(FSM_data.flag_1ms % 5  == 0) // per 5ms once
      {
        
        //-------------------------------------------------------------------------- 5ms start
        
        { // 按键处理事件 KEY1 KEY2
          key_status_all.key1_status = Key_Scan1(Button_Port, Buttom_1, 0);
          key_status_all.key2_status = Key_Scan2(Button_Port, Buttom_2, 0);
          
          if(key_status_all.key1_status == BUTTON_STATUS_CLICK)
          {
          //LED1(0);
          }else if(key_status_all.key1_status == BUTTON_STATUS_EXTENDED_PRESS)
          {
          //LED1(1);
          }
          if(key_status_all.key2_status == BUTTON_STATUS_CLICK)
          {
             
          }else if(key_status_all.key2_status == BUTTON_STATUS_EXTENDED_PRESS)
          {
          
          }
        }
        { // 摇晃处理事件
          
          key_status_all.shake_key_status = Shake_Key_Scan(Shake_Key_Port, Shake_Key_Pin);
          if(key_status_all.shake_key_status == 1)
          {
          LED1(0);
          }else  if(key_status_all.shake_key_status == 2)
          {
          LED1(1);
          }
        
        }

        
            if(UART_RX_NUM&0x80)
            {
              len=UART_RX_NUM&0x3f;/*得到此次接收到的数据长度*/     
              UART1_SendString(RxBuffer,len);
              UART_RX_NUM=0;
            }

        
         //-------------------------------------------------------------------------- 5ms end
        
          if(FSM_data.flag_1ms % 10  == 0) // per 10ms once
          {
            adc_value = ADC1_Get();
       //      IWDG_ReloadCounter();
            if(adc_value < LOW_POWER_ADC)
            {
           //  USART1_printf(  "\r\n low power adc = %d \r\n", adc_value);
            }
               if(FSM_data.flag_1ms % 100  == 0) // per 100ms once
              {
                     
                      if(FSM_data.flag_1ms % 1000  == 0) // per 1000ms once
                      {
                        FSM_data.flag_1ms = 0;
                       // LED1_TOGGLE;
                      }  
              }  
          }  
      }

 
          
      while(flag_tim1_time == 0);
  
      flag_tim1_time = 0;

  }
}



void Bsp_Init(void)  //硬件初始化区域
{
        Uart_Init();
      //  USART1_printf( "\r\n this is a demo \r\n" );         
        Tim1_Init();
        
        SPI_Configation();
                RGB_LED_Init();   
        __enable_interrupt();//中断使能
      //  IWDOG_Init();
       
        W25x16_Test(); 
        W25x16_Save_Ws2812b_Config();
    
       // IWDG_ReloadCounter();
	LED_Init();
        Buttom_Init();
        Shake_Key_Init();
        ADC1_Config();
        
        delay_ms(1000);//延时1秒  
  
        while(1){
          delay_ms(400);//延时1秒    
//          RST_RGB2_data();
//          WS2812_send_DATA(color_data, 40);      
//          delay_ms(400);//延时1秒    
//
//          RST_RGB2_data();
//          WS2812_send_DATA(color_data1, 40); 
        
        }

 
        
      //  Set_Beep_OptionByte();
     //   Beep_Test();

}


#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/******************* (C) COPYRIGHT 奇来科技 *****END OF FILE****/