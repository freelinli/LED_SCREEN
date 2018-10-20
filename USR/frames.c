
/******************** (C) COPYRIGHT  freelin *************************************
 * �ļ���  ��frames.c
 * ����    ��֡��������
 * ʵ��ƽ̨�� STM8������
 * ��汾  ��V1.0.0
 * ����֧�֣�wechat(17612172119)
 * �޸�ʱ�䣺2016-08-25
**********************************************************************************/

#include "head.h"


extern Ws2812b_Config_t Ws2812b_Config_data;
extern Ws2812b_Config_ture_t Ws2812b_Config_data_ture;


u8  switch_get_eeprom = 0;


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/




void Ws2812_Config_set( Ws2812b_Config_ture_t Ws2812b_Config_data)
{
    Ws2812b_Config_data_ture.data_offset =  Ws2812b_Config_data.data_offset;
    Ws2812b_Config_data_ture.frames = Ws2812b_Config_data.frames;
    Ws2812b_Config_data_ture.last_time = Ws2812b_Config_data.last_time;
    Ws2812b_Config_data_ture.led_pixel = Ws2812b_Config_data.led_pixel;
}

void Ws2812_Config_get( Ws2812b_Config_ture_t* Ws2812b_Config_data_ture )
{
    Ws2812b_Config_data_ture->data_offset = ( Ws2812b_Config_data.data_offset_h << 16 ) + ( Ws2812b_Config_data.data_offset_m << 8 ) + Ws2812b_Config_data.data_offset_l;
    Ws2812b_Config_data_ture->frames = ( Ws2812b_Config_data.frames_h << 8 ) + Ws2812b_Config_data.frames_l;
    Ws2812b_Config_data_ture->last_time = ( Ws2812b_Config_data.last_time_h << 8 ) + Ws2812b_Config_data.last_time_l;
    Ws2812b_Config_data_ture->led_pixel = Ws2812b_Config_data.led_pixel;
}

static u16 checkSumData( u8* showdata, int temp_hex_len ) //У���
{
    int i;
    unsigned char checkSum = 0;
    for( i = 0; i < temp_hex_len; i++ )
    {
        checkSum += showdata[i];
    }
    return checkSum;
}

int frames_code( u8* output, u8 cmd, u8* input, u8 inputlen )
{
    *output = 0x68;
    *( output + 1 ) = cmd;
    *( output + 2 ) = inputlen;
    if( input != NULL )
    {
        memcpy( output + 3, input, inputlen );
    }
    *( output + 3 + inputlen ) = checkSumData( output, inputlen + 3 );
    *( output + 4 + inputlen ) = 0x16;
    return 1;
}



int frames_decode( u8* data )
{
    u8 checkdata = 0, i = 0, checkdata_len;
    u8 backword[3] = {0x68, 0x0, 0x16};
    if( *data != 0x68 ) // ֡ͷ
    {
        return -1;
    }
    if( *( data + 1 ) > 5 ) // ������
    {
        return -1;
    }
    checkdata_len = 3 + *( data + 2 );
    while( i < checkdata_len )
    {
        checkdata += *( data + i++ );
    }
    if( *( data + checkdata_len ) != checkdata ) //У���Ƿ���ȷ
    {
        UART1_SendByte( checkdata );
        UART1_SendByte( checkdata_len );
        return -1;
    }
    backword[1] = *( data + 1 );
    switch( *( data + 1 ) )
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
            memcpy( &Ws2812b_Config_data, data + 3, 8 );
            Ws2812_Config_get( &Ws2812b_Config_data_ture );
            SPI_FLASH_BufferWrite( ( u8* )&Ws2812b_Config_data, FLASH_SectorToErase, sizeof( Ws2812b_Config_t ) );
            UART1_SendString( backword, 3 );
            break;
        case 0x01: //configread
            /*
                ֡ͷ  ������ ���ݳ���  У���      ֡β
                0x68  0x01  0x00    У��       0x16
            */
            SPI_FLASH_BufferRead( ( u8* )&Ws2812b_Config_data, FLASH_SectorToErase, sizeof( Ws2812b_Config_t ) );
            frames_code( data, 0, ( u8* )&Ws2812b_Config_data, sizeof( Ws2812b_Config_data ) );
            UART1_SendString( data, data[2] + 5 );
            break;
        case 0x02: //writedata
            /*
                ֡ͷ  ������ ���ݳ���  ������ʼ��ַ ������     У���          ֡β
                0x68  0x02  0x08  0x000000    ******    У��           0x16
            */
            SPI_FLASH_BufferWrite( data + 6, ( data[3] << 16 ) + ( data[4] << 8 )  + data[5], data[2] - 3 );
            UART1_SendString( backword, 3 );
            break;
        case 0x03: //readdata
            /*
                ֡ͷ  ������ ���ݳ���  ������ʼ��ַ   У���          ֡β
                0x68  0x03  0x03  0x000000       У��           0x16
            */
            SPI_FLASH_WriteEnable();
            SPI_FLASH_BufferRead( data + 6, ( data[3] << 16 ) + ( data[4] << 8 )  + data[5], Ws2812b_Config_data.led_pixel * 3 );
            frames_code( data, 3, data + 3, Ws2812b_Config_data.led_pixel * 3 + 3 );
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
            UART1_SendString( backword, 3 );
            break;
        default:
            break;
    }
    return 1;
}

/******************* (C) COPYRIGHT freelin *****END OF FILE****/