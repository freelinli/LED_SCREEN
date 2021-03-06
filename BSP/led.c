/******************** (C) COPYRIGHT  freelin *************************************
 * 文件名  ：led.c
 * 描述    ：led控制程序   
 * 实验平台： STM8开发板
 * 库版本  ：V1.0.0
 * 技术支持：wechat(17612172119)
 * 修改时间：2016-08-25

**********************************************************************************/

#include "led.h"
#include "delay_time.h"
#include <stdlib.h>

void LED_Init(void)
{
   GPIO_Init(LED1_PORT,LED1_PIN,\
             GPIO_MODE_OUT_PP_HIGH_FAST );//定义LED的管脚的模式
}

void SetLedOFF(void)
{
    //GPIO_Write(GPIOD, 0xff);
    LED1(OFF);
}


void LED_ShowOneToOne(void) // led 交替闪烁
{
    LED1(OFF);

    delay_ms(1000);

    LED1(ON);

    delay_ms(1000);

}

void RGB_LED_Init(void)
{
   //GPIO_Init(RGB_LED1_PORT,RGB_LED1_PIN,\
             GPIO_MODE_OUT_PP_HIGH_FAST);//定义LED的管脚的模式
   GPIO_Init(RGB_LED2_PORT,RGB_LED2_PIN,\
             GPIO_MODE_OUT_PP_LOW_FAST);//定义LED的管脚的模式
 //  GPIO_MODE_OUT_PP_LOW_FAST
}




/*写入数据时序*/
#if 0  // 优化情况下，会将相关函数及运算直接以结果出现
void Send_A_bit(u8 VAL)
{
  u8 i = 0;
       if (VAL==1)
       {
              RGB_LED2_H;
              i ++ ;
              i ++ ;
              i ++ ;
              i ++ ;
            // delay_800ns;
              RGB_LED2_L;     
               i ++ ;
                 i ++ ;
            //  delay_400ns;
      
       }
       else
       {
              RGB_LED2_H;
              i ++ ;
                  i ++ ;
            // delay_400ns;
              RGB_LED2_L;        
                 i ++ ;
                 i ++ ;
                  i ++ ;
                 i ++ ;
              //delay_800ns;
       }           
      
}
#else

/*写入数据时序*/
void Send_A_bit(u8 VAL)
{
  u8 i = 0;
       if (VAL==1)
       {
          RGB_LED2_H;

          asm(" NOP");
          asm(" NOP");
          asm(" NOP");
          asm(" NOP");
          asm(" NOP");
          asm(" NOP");
          asm(" NOP");
          asm(" NOP");
          // delay_800ns;
          RGB_LED2_L;     
          asm(" NOP");
          asm(" NOP");
          asm(" NOP");
          asm(" NOP");
          //  delay_400ns;

       }
       else
       {
          RGB_LED2_H;
          asm(" NOP");
          asm(" NOP");
          asm(" NOP");
          asm(" NOP");
          // delay_400ns;
          RGB_LED2_L;        
          asm(" NOP");
          asm(" NOP");
          asm(" NOP");
          asm(" NOP");
          asm(" NOP");
          asm(" NOP");
          asm(" NOP");
          asm(" NOP");
          //delay_800ns;
       }           
      
}
#endif

#define TIMING_ONE  1
#define TIMING_ZERO 0
    


// 00 : 灯光关闭
// ff :  白色灯光，最亮
//  红色 #FF0000
//  黄色 #FFFF00
//  绿色 #00FF00
//  蓝色 #0000FF



void WS2812_send_DATA(uint8_t *color, uint16_t len)
{
 
        uint16_t i= 0, j = 0;
       
        RST_RGB2_data();                // 先进行复位清零
	while ( j < len)
	{
                        for(i=0; i<8; i++) // GREEN data
			{
                          Send_A_bit(((color[3 * j + 1]<<i) & 0x80) ? TIMING_ONE:TIMING_ZERO);
			}
			for(i=0; i<8; i++) // RED
			{
                           Send_A_bit(((color[3 * j]<<i) & 0x80) ? TIMING_ONE:TIMING_ZERO);	
			}
			for(i=0; i<8; i++) // BLUE
			{
                           Send_A_bit(((color[3 * j + 2]<<i) & 0x80) ? TIMING_ONE:TIMING_ZERO);
			}
			
		  j++;
	}
             RST_RGB2_data();                // 结束发送数据
}

void WS2812_erase_LED(uint16_t pixel)
{
  delay_ms(10);
  uint8_t *color  = (uint8_t *)malloc(pixel * 3);
  memset(color, 0x00, (pixel * 3));
  WS2812_send_DATA(color, pixel);
  free(color);
}

void WS2812_send_DATA_R( uint16_t len)
{
 
        uint16_t i= 0, j = 0;
 
       // uint16_t buf_len = len * 24;
    
      //   unsigned char *LED_BYTE_Buffer;
        // LED_BYTE_Buffer =  (unsigned char *)malloc(buf_len);
        // if(LED_BYTE_Buffer ==  NULL)
        //   return;
        // memset(LED_BYTE_Buffer, 0,buf_len);
       
	while ( j < len)
	{
                        for(i=0; i<8; i++) // GREEN data
			{
                             Send_A_bit(1);
					//LED_BYTE_Buffer[memaddr] = ((color[j][1]<<i) & 0x80) ? TIMING_ONE:TIMING_ZERO;
					//memaddr++;
			}
			for(i=0; i<8; i++) // RED
			{
                           Send_A_bit(0);
					//LED_BYTE_Buffer[memaddr] = ((color[j][0]<<i) & 0x80) ? TIMING_ONE:TIMING_ZERO;
					//memaddr++;
			}
			for(i=0; i<8; i++) // BLUE
			{
                           Send_A_bit(0);
					//LED_BYTE_Buffer[memaddr] = ((color[j][2]<<i) & 0x80) ? TIMING_ONE:TIMING_ZERO;
					//memaddr++;
			}
			
		  j++;
	}
      //  j = 0;
       
      //  while ( j < buf_len )
      //  {
       //     Send_A_bit(LED_BYTE_Buffer[j++]);
     //   }
        
    //  free(LED_BYTE_Buffer);
  
}

void RST_RGB2_data(void)
{
  RGB_LED2_L;
  delay_us(200);//延时1秒
    
}


void RGB2_TEST(void)
{
  while(1)
  {
      RGB_LED2_L;
      delay_ms(1200);//延时1秒

      RGB_LED2_H;
      delay_ms(1200);//延时1秒
  }
    
}




/******************* (C) COPYRIGHT freelin *****END OF FILE****/