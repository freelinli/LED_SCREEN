/******************** (C) COPYRIGHT  freelin *************************************
 * �ļ���  ��bspinit.c
 * ����    ���弶��ʼ��
 * ʵ��ƽ̨�� STM8������
 * ��汾  ��V1.0.0
 * ����֧�֣�wechat(17612172119)
 * �޸�ʱ�䣺2016-08-25

**********************************************************************************/

#include "head.h"
#include "delay_time.h"



Ws2812b_Config_t Ws2812b_Config_data;


/* Extern defines -----------------------------------------------------------*/

extern u16      flag_tim1_time;
extern u8       RxBuffer[RxBufferSize];
extern u16      UART_RX_NUM;
extern u8       switch_get_eeprom;



#define BSP_TEST

#ifdef BSP_TEST


#define BSP_INIT_PRINT


#endif

#define BSP_INIT_STRING "BSP_INIT ......"


void Bsp_Init( void ) //Ӳ����ʼ������
{
    struct FSM_t FSM_data = {0, 0, 0, 0, 0};
    u16 temp_move;
    delay_ms( 100 );            //��ʱ100ms ���Եȴ��ȶ�
    SPI_Configation();
    Uart_Init(115200);                // 115200
    UART1_SendString(BSP_INIT_STRING, sizeof(BSP_INIT_STRING));
#ifdef BSP_INIT_PRINT
    USART1_printf( "\r\n Uart_Init over \r\n" );
#endif
    W25x16_Test();
#ifdef BSP_INIT_PRINT
    USART1_printf( "\r\n W25x16_Test over \r\n" );
#endif
    Tim1_Init();
    __enable_interrupt();//�ж�ʹ��
#ifdef BSP_INIT_PRINT
    USART1_printf( "\r\n Tim1_Init over __enable_interrupt \r\n" );
#endif
    // switch_get_eeprom = 1; // just for debug
    SPI_FLASH_BufferRead( ( u8* )&Ws2812b_Config_data, FLASH_SectorToErase, sizeof( Ws2812b_Config_t ) ); // ��ʼ����ȡ��Ϣ
    Ws2812_Config_get();
    while( 1 )
    {
        ++FSM_data.flag_1ms;
        if( UART_RX_NUM > 4 )
        {
            temp_move = 0;
            while( temp_move < UART_RX_NUM )
            {
                if( RxBuffer[temp_move] == 0x68 )
                {
                    if( frames_decode( RxBuffer + temp_move ) > 0 )
                    {
                        UART_RX_NUM = 0;
                        memset( RxBuffer, 0, RxBufferSize );
                        break;
                    }
                }
                temp_move++;
            }
        }
        if( FSM_data.flag_1ms > 601 )
        {
            FSM_data.flag_1ms =  601;
        }
        if( ( switch_get_eeprom == 0 ) && ( FSM_data.flag_1ms > 600 ) ) // δ���ܵ�����  �� ʱ�䳬ʱʱ�䵽�����˳�
        {
            break;
        }
        while( flag_tim1_time == 0 );
        flag_tim1_time = 0;
    }
    RGB_LED_Init();
    //IWDOG_Init();
    //W25x16_Test();
    //W25x16_Save_Ws2812b_Config();
    //IWDG_ReloadCounter();
    LED_Init();
    Buttom_Init();
    Shake_Key_Init();
    ADC1_Config();
  
    Set_Beep_OptionByte();
    //        Beep_Test();
}



/******************* (C) COPYRIGHT freelin *****END OF FILE****/