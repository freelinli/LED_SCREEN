#ifndef __SPI_H
#define __SPI_H
#include "stm8s.h"


//W25Q
#define W25Q_PORT_CS   GPIOA   //����D�˿ڣ���������޸ļ�����ֲ�Ժ�   
#define W25Q_PIN_CS    GPIO_PIN_3  //����Ҫʹ�ö˿ڵĵڰ˸��ܽ�


#define W25Q_CS_L      GPIO_WriteLow(W25Q_PORT_CS, W25Q_PIN_CS)  //����W25Q_CS_L,����PD7
#define W25Q_CS_H      GPIO_WriteHigh(W25Q_PORT_CS,W25Q_PIN_CS)  //����W25Q_CS_H,����PD7


void SPI_Configation(void);  //�Զ���SPI��ʼ������
u8 SPI_SendByte(u8 byte);

#endif

