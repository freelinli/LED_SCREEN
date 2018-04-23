
/******************** (C) COPYRIGHT  �����Ƽ� *************************************
 * �ļ���  ��main.c
 * ����    ��STM8S����ģ��
 * ʵ��ƽ̨�� STM8������
 * ��汾  ��V1.0.0
 * ����֧�֣�lijinnan@qiqirobot.com
 * �޸�ʱ�䣺2016-08-25
 * �Ա���ַ��https://lsqrd.taobao.com/index.htm?spm=2013.1.w5002-7895914584.2.kYLHaf
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


struct FSM_t
{
    unsigned int flag_1ms;
    unsigned int flag_5ms;
    unsigned int flag_10ms;
    unsigned int flag_20ms;
    unsigned int flag_1s;

};
struct key_status
{
    unsigned char key1_status;
    unsigned char key2_status;
    unsigned char shake_key_status;
} key_status_all;

u8 len = 0;
u16 adc_value = 0;
u8  *point_68;
u8  switch_get_eeprom = 0;
Ws2812b_Config_t Ws2812b_Config_data;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void Bsp_Init(void);


unsigned char checkSumData(u8 *showdata, int temp_hex_len )  //У���
{
    int i;
    unsigned char checkSum = 0;
    for(i = 0; i < temp_hex_len; i++)
    {
        checkSum += showdata[i];
    }
    return checkSum;
}

int frames_code(u8 *output, u8 cmd, u8 *input, u8 inputlen)
{

    *output = 0x68;
    *(output + 1) = cmd;
    *(output + 2) = inputlen;
    if(input != NULL)
        memcpy(output + 3, input, inputlen);
    *(output + 3 + inputlen) = checkSumData(output, inputlen + 3);
    *(output + 4 + inputlen) = 0x16;
    return 1;
}



int frames_decode(u8 *data)
{
    u8 checkdata = 0, i = 0, checkdata_len;
    u8 backword[3] = {0x68, 0x0, 0x16};
    if(*data != 0x68) // ֡ͷ
        return -1;
    if(*(data + 1) > 5) // ������
        return -1;

    checkdata_len = 3 + *(data + 2);

    while(i < checkdata_len)
    {
        checkdata += *(data + i++);
    }
    if( *(data + checkdata_len) != checkdata) //У���Ƿ���ȷ
        return -1;


    backword[1] = *(data + 1);
    switch(*(data + 1))
    {
    case 0x00: //configwrite
        /*
            ֡ͷ  ������ ���ݳ���  ������ʼ��ַ  ֡��   ���ص� ���ʱ��ms У���        ֡β
            0x68  0x00  0x08  0x000000     0x0002 0x28    0x0064    У��       0x16
        */

        switch_get_eeprom = 1;

        SPI_FLASH_BulkErase();//ȫƬ���
        /*
        typedef struct Ws2812b_Config{
        u8 data_offset_h;             // 1    �������ox0��ƫ�Ƶ�ַ ��λ
        u8 data_offset_m;             //  2   �������ox0��ƫ�Ƶ�ַ ��λ
        u8 data_offset_l;             // 3    �������ox0��ƫ�Ƶ�ַ ��λ
        u8 frames_h;                  // 4    ֡�� ��λ
        u8 frames_l;                  // 5    ֡�� ��λ
        u8 led_pixel;                 // 6    led�����ص����
        u8 last_time_h;               // 7    ÿ֡����ʱ�� ��λ
        u8 last_time_l;               // 8    ÿ֡����ʱ�� ��λ

        }Ws2812b_Config_t;
        */

        memcpy(&Ws2812b_Config_data, data + 3, 8);
        SPI_FLASH_BufferWrite((u8 *)&Ws2812b_Config_data, FLASH_SectorToErase, sizeof(Ws2812b_Config_t));
        UART1_SendString( backword, 3);
        break;
    case 0x01: //configread
        /*
            ֡ͷ  ������ ���ݳ���  У���      ֡β
            0x68  0x01  0x00    У��       0x16
        */
        SPI_FLASH_BufferRead((u8 *)&Ws2812b_Config_data, FLASH_SectorToErase, sizeof(Ws2812b_Config_t));
        frames_code(data, 0, (u8 *)&Ws2812b_Config_data, sizeof(Ws2812b_Config_data) );
        UART1_SendString( data, data[2] + 5 );
        break;
    case 0x02: //writedata


        /*
            ֡ͷ  ������ ���ݳ���  ������ʼ��ַ ������     У���          ֡β
            0x68  0x02  0x08  0x000000    ******    У��           0x16
        */

        SPI_FLASH_BufferWrite(data + 6, (data[3] << 16) + (data[4] << 8)  + data[5] , data[2] - 3);
        UART1_SendString( backword, 3);
        break;
    case 0x03: //readdata
        /*
            ֡ͷ  ������ ���ݳ���  ������ʼ��ַ   У���          ֡β
            0x68  0x03  0x03  0x000000       У��           0x16
        */

        SPI_FLASH_BufferRead(data + 6, (data[3] << 16) + (data[4] << 8)  + data[5] , Ws2812b_Config_data.led_pixel * 3);

        frames_code(data, 2, NULL, Ws2812b_Config_data.led_pixel * 3 + 3);

        UART1_SendString( data, data[2] + 5 );

        break;
    case 0x04: //switchover

        /*
            ֡ͷ  ������  ���ݳ���  У���      ֡β
            0x68  0x04   0x00    У��       0x16
        */
        SPI_FLASH_WriteDisable();
        SPI_FLASH_WaitForWriteEnd();
        switch_get_eeprom = 0;
        UART1_SendString( backword, 3);
        break;
    default:

        break;

    }

    return 1;

}


int main(void)
{
    /* Infinite loop */

    struct FSM_t FSM_data = {0, 0, 0, 0, 0};
    u16 temp_move;

    /*�����ڲ�ʱ��16MΪ��ʱ��*/

    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);

    Bsp_Init();
    /*!<Set High speed internal clock  */


    while (1)
    {
        /*  �����Ĵ���  */
        //  LED_ShowOneToOne();

        ++FSM_data.flag_1ms;

        if(FSM_data.flag_1ms % 5  == 0) // per 5ms once
        {

            //-------------------------------------------------------------------------- 5ms start

            {
                // ���������¼� KEY1 KEY2
                key_status_all.key1_status = Key_Scan1(Button_Port, Buttom_1, 0);
                key_status_all.key2_status = Key_Scan2(Button_Port, Buttom_2, 0);

                if(key_status_all.key1_status == BUTTON_STATUS_CLICK)
                {
                    //LED1(0);
                }
                else if(key_status_all.key1_status == BUTTON_STATUS_EXTENDED_PRESS)
                {
                    //LED1(1);
                }
                if(key_status_all.key2_status == BUTTON_STATUS_CLICK)
                {

                }
                else if(key_status_all.key2_status == BUTTON_STATUS_EXTENDED_PRESS)
                {

                }
            }
            {
                // ҡ�δ����¼�

                key_status_all.shake_key_status = Shake_Key_Scan(Shake_Key_Port, Shake_Key_Pin);
                if(key_status_all.shake_key_status == 1)
                {
                    LED1(0);
                }
                else  if(key_status_all.shake_key_status == 2)
                {
                    LED1(1);
                }

            }


            if(UART_RX_NUM > 4)
            {
                temp_move = 0;
                while(temp_move < UART_RX_NUM)
                {
                    if(RxBuffer[temp_move] == 0x68)
                    {
                        if(frames_decode(RxBuffer + temp_move) > 0)
                        {
                            UART_RX_NUM = 0;
                            memset(RxBuffer, 0, RxBufferSize);
                            break;
                        }
                    }
                    temp_move++;

                }
            }


            if(switch_get_eeprom  == 0) //������״̬�²ſ��Խ���led��˸
            {


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



void Bsp_Init(void)  //Ӳ����ʼ������
{
    Uart_Init(); // 115200
    UART1_SendString( "\r\n this is a demo \r\n", sizeof("\r\n this is a demo \r\n") );
    //  USART1_printf( "\r\n this is a demo \r\n" );
    Tim1_Init();

    SPI_Configation();
    RGB_LED_Init();
    __enable_interrupt();//�ж�ʹ��
    //  IWDOG_Init();

    W25x16_Test();
    // W25x16_Save_Ws2812b_Config();

    // IWDG_ReloadCounter();
    LED_Init();
    Buttom_Init();
    Shake_Key_Init();
    ADC1_Config();

    delay_ms(1000);//��ʱ1��

    //        while(1){
    //          delay_ms(400);//��ʱ1��
    //          RST_RGB2_data();
    //          WS2812_send_DATA(color_data, 40);
    //          delay_ms(400);//��ʱ1��
    //
    //          RST_RGB2_data();
    //          WS2812_send_DATA(color_data1, 40);
    //        }

    Set_Beep_OptionByte();
    //        Beep_Test();

}


#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8 *file, u32 line)
{
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while (1)
    {
    }
}
#endif

/******************* (C) COPYRIGHT �����Ƽ� *****END OF FILE****/