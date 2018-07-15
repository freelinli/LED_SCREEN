
#include "stm8s_spi.h"
#include "stm8s_gpio.h"
#include "spi.h"

void SPI_Configation(void)  //先发送MSB  fMASTER/4   空闲状态时SCK保持高电平  数据采样从第二个时钟边沿开始  双线单向数据模式   禁止软件从设备管理
{
        //配置通信波特率1Mbps,空闲状态时，SCK保持低电平,数据采样从第一个时钟边沿开始,
                //通信时，串行移位，高位在前，低位在后,NSS引脚软件管理 
               //关闭flash SPI CS 片选，
              //关闭flash SPI CS 片选，
                GPIO_Init(W25Q_PORT_CS, W25Q_PIN_CS, GPIO_MODE_OUT_PP_LOW_SLOW);//C GPIO 2 SPI CS 引脚
                W25Q_CS_H;
                
                /* SPI configuration */
                SPI_DeInit();
               
                SPI_Init(SPI_FIRSTBIT_MSB, SPI_BAUDRATEPRESCALER_2, SPI_MODE_MASTER, SPI_CLOCKPOLARITY_LOW, 
                         SPI_CLOCKPHASE_1EDGE, SPI_DATADIRECTION_2LINES_FULLDUPLEX, SPI_NSS_SOFT, 0x07);
                //端口配置为快速摆率,
             //  GPIO_Init(GPIOC, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_MODE_OUT_PP_HIGH_FAST);
       
                CLK_PeripheralClockConfig(CLK_PERIPHERAL_SPI, ENABLE);//开启时钟

               SPI_Cmd(ENABLE);//使能SPI     
}


u8 SPI_SendByte(u8 byte)
{
    while(SPI_GetFlagStatus(SPI_FLAG_TXE) == RESET);
    SPI_SendData(byte);
    
    while(SPI_GetFlagStatus(SPI_FLAG_RXNE) == RESET);
    return SPI_ReceiveData();
}
