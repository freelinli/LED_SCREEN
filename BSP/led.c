/******************** (C) COPYRIGHT  �����Ƽ� *************************************
 * �ļ���  ��led.c
 * ����    ��led���Ƴ���   
 * ʵ��ƽ̨�� STM8������
 * ��汾  ��V1.0.0
 * ����֧�֣�lijinnan@qiqirobot.com
 * �޸�ʱ�䣺2016-08-25
 * �Ա���ַ��https://lsqrd.taobao.com/index.htm?spm=2013.1.w5002-7895914584.2.kYLHaf
**********************************************************************************/

#include "led.h"
#include "delay_time.h"

void LED_Init(void)
{
   GPIO_Init(LED1_PORT,LED1_PIN,\
             GPIO_MODE_OUT_PP_HIGH_FAST );//����LED�Ĺܽŵ�ģʽ
}

void SetLedOFF(void)
{
    //GPIO_Write(GPIOD, 0xff);
    LED1(OFF);
}


void LED_ShowOneToOne(void) // led ������˸
{
    LED1(OFF);

    delay_ms(1000);

    LED1(ON);

    delay_ms(1000);

}

void RGB_LED_Init(void)
{
   //GPIO_Init(RGB_LED1_PORT,RGB_LED1_PIN,\
             GPIO_MODE_OUT_PP_HIGH_FAST);//����LED�Ĺܽŵ�ģʽ
   GPIO_Init(RGB_LED2_PORT,RGB_LED2_PIN,\
             GPIO_MODE_OUT_PP_LOW_FAST);//����LED�Ĺܽŵ�ģʽ
 //  GPIO_MODE_OUT_PP_LOW_FAST
}




/*д������ʱ��*/
#if 0  // �Ż�����£��Ὣ��غ���������ֱ���Խ������
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

/*д������ʱ��*/
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
       //   unsigned char LED_BYTE_Buffer[500]; 
void WS2812_send_DATA(uint8_t *color, uint16_t len)
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
                          Send_A_bit(((color[3 * j + 1]<<i) & 0x80) ? TIMING_ONE:TIMING_ZERO);
					//LED_BYTE_Buffer[memaddr] = ((color[j][1]<<i) & 0x80) ? TIMING_ONE:TIMING_ZERO;
					//memaddr++;
			}
			for(i=0; i<8; i++) // RED
			{
                           Send_A_bit(((color[3 * j]<<i) & 0x80) ? TIMING_ONE:TIMING_ZERO);
					//LED_BYTE_Buffer[memaddr] = ((color[j][0]<<i) & 0x80) ? TIMING_ONE:TIMING_ZERO;
					//memaddr++;
			}
			for(i=0; i<8; i++) // BLUE
			{
                           Send_A_bit(((color[3 * j + 2]<<i) & 0x80) ? TIMING_ONE:TIMING_ZERO);
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
  delay_us(120);//��ʱ1��
    
}


void RGB2_TEST(void)
{
  while(1)
  {
      RGB_LED2_L;
      delay_ms(1200);//��ʱ1��

      RGB_LED2_H;
      delay_ms(1200);//��ʱ1��
  }
    
}




/******************* (C) COPYRIGHT �����Ƽ� *****END OF FILE****/