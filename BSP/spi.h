#ifndef __SPI_H
#define __SPI_H
#include "stm8s.h"


//W25Q
#define W25Q_PORT_CS   GPIOA   //定义D端口，方便程序修改及可移植性好   
#define W25Q_PIN_CS    GPIO_PIN_3  //定义要使用端口的第八个管脚


#define W25Q_CS_L      GPIO_WriteLow(W25Q_PORT_CS, W25Q_PIN_CS)  //定义W25Q_CS_L,拉低PD7
#define W25Q_CS_H      GPIO_WriteHigh(W25Q_PORT_CS,W25Q_PIN_CS)  //定义W25Q_CS_H,拉高PD7


void SPI_Configation(void);  //自定义SPI初始化函数
u8 SPI_SendByte(u8 byte);

#endif

