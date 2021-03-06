
#ifndef __UART1_H
#define __UART1_H
#include "stm8s.h"
#include <stdio.h>

/* Private macro -------------------------------------------------------------*/
#define countof(a)   (sizeof(a) / sizeof(*(a)))
#define RxBufferSize 256

void Uart_Init( u32 baud );
void UART1_SendByte( u8 data );
//int fputc(int ch, FILE *f);
void UART1_SendString( u8* Data, u16 len );
u8 UART1_ReceiveByte( void );


#define USE_USART1_printf


#ifdef USE_USART1_printf
void USART1_printf( uint8_t* Data, ... );
#endif

#endif