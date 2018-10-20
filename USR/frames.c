
/******************** (C) COPYRIGHT  freelin *************************************
 * 文件名  ：frames.c
 * 描述    ：帧处理函数
 * 实验平台： STM8开发板
 * 库版本  ：V1.0.0
 * 技术支持：wechat(17612172119)
 * 修改时间：2016-08-25
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

static u16 checkSumData( u8* showdata, int temp_hex_len ) //校验和
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
    if( *data != 0x68 ) // 帧头
    {
        return -1;
    }
    if( *( data + 1 ) > 5 ) // 控制码
    {
        return -1;
    }
    checkdata_len = 3 + *( data + 2 );
    while( i < checkdata_len )
    {
        checkdata += *( data + i++ );
    }
    if( *( data + checkdata_len ) != checkdata ) //校验是否正确
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
                帧头  控制码 数据长度  存入起始地址  帧数   像素点 间隔时间ms 校验和        帧尾
                0x68  0x00  0x08  0x000000     0x0002 0x28    0x0064    校验       0x16
            */
            switch_get_eeprom = 1;
            SPI_FLASH_BulkErase();//全片清空
            /*
            typedef struct Ws2812b_Config{
            u8 data_offset_h;             // 1    数据相对ox0的偏移地址 高位
            u8 data_offset_m;             //  2   数据相对ox0的偏移地址 中位
            u8 data_offset_l;             // 3    数据相对ox0的偏移地址 低位
            u8 frames_h;                  // 4    帧数 高位
            u8 frames_l;                  // 5    帧数 低位
            u8 led_pixel;                 // 6    led的像素点个数
            u8 last_time_h;               // 7    每帧持续时间 高位
            u8 last_time_l;               // 8    每帧持续时间 低位

            }Ws2812b_Config_t;
            */
            memcpy( &Ws2812b_Config_data, data + 3, 8 );
            Ws2812_Config_get( &Ws2812b_Config_data_ture );
            SPI_FLASH_BufferWrite( ( u8* )&Ws2812b_Config_data, FLASH_SectorToErase, sizeof( Ws2812b_Config_t ) );
            UART1_SendString( backword, 3 );
            break;
        case 0x01: //configread
            /*
                帧头  控制码 数据长度  校验和      帧尾
                0x68  0x01  0x00    校验       0x16
            */
            SPI_FLASH_BufferRead( ( u8* )&Ws2812b_Config_data, FLASH_SectorToErase, sizeof( Ws2812b_Config_t ) );
            frames_code( data, 0, ( u8* )&Ws2812b_Config_data, sizeof( Ws2812b_Config_data ) );
            UART1_SendString( data, data[2] + 5 );
            break;
        case 0x02: //writedata
            /*
                帧头  控制码 数据长度  存入起始地址 数据域     校验和          帧尾
                0x68  0x02  0x08  0x000000    ******    校验           0x16
            */
            SPI_FLASH_BufferWrite( data + 6, ( data[3] << 16 ) + ( data[4] << 8 )  + data[5], data[2] - 3 );
            UART1_SendString( backword, 3 );
            break;
        case 0x03: //readdata
            /*
                帧头  控制码 数据长度  存入起始地址   校验和          帧尾
                0x68  0x03  0x03  0x000000       校验           0x16
            */
            SPI_FLASH_WriteEnable();
            SPI_FLASH_BufferRead( data + 6, ( data[3] << 16 ) + ( data[4] << 8 )  + data[5], Ws2812b_Config_data.led_pixel * 3 );
            frames_code( data, 3, data + 3, Ws2812b_Config_data.led_pixel * 3 + 3 );
            UART1_SendString( data, data[2] + 5 );
            break;
        case 0x04: //switchover
            /*
                帧头  控制码  数据长度  校验和      帧尾
                0x68  0x04   0x00    校验       0x16
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