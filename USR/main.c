
/******************** (C) COPYRIGHT  freelin *************************************
 * 文件名  ：main.c
 * 描述    ：STM8S LED SCREEN
 * 实验平台： STM8
 * 库版本  ：V1.0.0
 * 技术支持：wechat(17612172119)
 * 修改时间：2016-08-25
**********************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "delay_time.h"
#include "head.h"

/* Extern defines -----------------------------------------------------------*/
extern u16 flag_tim1_time ;
extern u8 RxBuffer[RxBufferSize];
extern u16 UART_RX_NUM;

/* Private defines -----------------------------------------------------------*/

key_status_t  key_status_all;
Ws2812b_Config_ture_t Ws2812b_Config_data_ture;

int main( void )
{
    /* Infinite loop */
    struct FSM_t FSM_data = {0, 0, 0, 0, 0, 0};
    u16 temp_move, i = 0;
    /*设置内部时钟16M为主时钟*/
    CLK_HSIPrescalerConfig( CLK_PRESCALER_HSIDIV1 );
    /*全局硬件初始化*/
    Bsp_Init();
    /*等待系统稳定*/
    delay_ms( 100 );
    /*全局参数初始化*/
    Bsp_data_Init();

    while( 1 )
    {
        //  LED_ShowOneToOne();
        ++FSM_data.flag_1ms;
        if( ++FSM_data.flag_switch > ( Ws2812b_Config_data_ture.last_time / 5 ) )
        {
            FSM_data.flag_switch  = 0;
            memset( RxBuffer, 0, RxBufferSize );
            SPI_FLASH_BufferRead( RxBuffer, Ws2812b_Config_data_ture.data_offset + Ws2812b_Config_data_ture.led_pixel * 3 * i, \
                                  Ws2812b_Config_data_ture.led_pixel * 3 );
            if( ++i >=  Ws2812b_Config_data_ture.frames )
            {
                i = 0;
            }
            //print_by_hex(RxBuffer,  Ws2812b_Config_data_ture.led_pixel * 3 );
//            WS2812_send_DATA( ( char* )RxBuffer,  Ws2812b_Config_data_ture.led_pixel );
              WS2812_send_DATA( ( char* )RxBuffer,  3);
            print_by_hex( RxBuffer,  1 );
            USART1_printf( "OK", 2 );
        }
        if( FSM_data.flag_1ms % 1  == 0 ) // per 5ms once
        {
            //-------------------------------------------------------------------------- 5ms start
            {
                // 按键处理事件 KEY1 KEY2
                key_status_all.key1_status = Key_Scan1( Button_Port, Buttom_1, 0 );
                key_status_all.key2_status = Key_Scan2( Button_Port, Buttom_2, 0 );
                if( key_status_all.key1_status == BUTTON_STATUS_CLICK )
                {
                    //LED1(0);
                }
                else if( key_status_all.key1_status == BUTTON_STATUS_EXTENDED_PRESS )
                {
                    //LED1(1);
                }
                if( key_status_all.key2_status == BUTTON_STATUS_CLICK )
                {
                }
                else if( key_status_all.key2_status == BUTTON_STATUS_EXTENDED_PRESS )
                {
                }
            }
            {
                // 摇晃处理事件
                key_status_all.shake_key_status = Shake_Key_Scan( Shake_Key_Port, Shake_Key_Pin );
                if( key_status_all.shake_key_status == 1 )
                {
                    //                    LED1(0);
                }
                else  if( key_status_all.shake_key_status == 2 )
                {
                    //                    LED1(1);
                }
            }
            //-------------------------------------------------------------------------- 5ms end
            if( FSM_data.flag_1ms % 2  == 0 ) // per 10ms once
            {
                //      IWDG_ReloadCounter();
                u16 adc_value = 0;
                if( ( adc_value = ADC1_Get() ) < LOW_POWER_ADC )
                {
                    //  USART1_printf(  "\r\n low power adc = %d \r\n", adc_value);
                }
                if( FSM_data.flag_1ms % 20  == 0 ) // per 100ms once
                {
                    if( FSM_data.flag_1ms % 200  == 0 ) // per 1000ms once
                    {
                        FSM_data.flag_1ms = 0;
                        LED1_TOGGLE;
                    }
                }
            }
        }
        // 阻塞，在tim 未到的情况下，禁止进行新一轮操作。注意：需要保证以上流程执行小于规定时间
        while( flag_tim1_time == 0 );
        flag_tim1_time = 0;
    }
}


#ifdef USE_FULL_ASSERT

void assert_failed( u8* file, u32 line )
{
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* Infinite loop */
    while( 1 )
    {
    }
}
#endif

/******************* (C) COPYRIGHT freelin *****END OF FILE****/