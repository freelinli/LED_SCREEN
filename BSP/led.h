#ifndef __LED_H
#define __LED_H
#include "stm8s_gpio.h"

#define ON  0
#define OFF 1

#define LED1_PORT       GPIOA
#define LED1_PIN        GPIO_PIN_1

#define LED1(ON_OFF)  if(ON_OFF==ON)GPIO_WriteLow(LED1_PORT, LED1_PIN);\
                      else GPIO_WriteHigh(LED1_PORT, LED1_PIN)

            
                        
#define LED1_TOGGLE   GPIO_WriteReverse(LED1_PORT, LED1_PIN)
                        
                        
                        
#define RGB_LED1_PIN        GPIO_PIN_3
#define RGB_LED1_PORT       GPIOD

#define RGB_LED1_L      GPIOD->ODR &= (uint8_t)(~GPIO_PIN_3);
#define RGB_LED1_H      GPIOD->ODR |= (uint8_t)GPIO_PIN_3; 
                        
                        
#define RGB_LED2_PIN        GPIO_PIN_3
#define RGB_LED2_PORT       GPIOC

#define RGB_LED2_L      GPIOC->ODR &= (uint8_t)(~GPIO_PIN_3);
#define RGB_LED2_H      GPIOC->ODR |= (uint8_t)GPIO_PIN_3; 
                        

void LED_Init(void);
void SetLedOFF(void);
void LED_Display(void);
void LED_ShowOneToOne(void);



void RGB_LED_Init(void);
void WS2812_send_DATA(uint8_t (*color)[3], uint16_t len);
void RST_RGB2_data(void);

void WS2812_send_DATA_R( uint16_t len);


void RGB2_TEST(void);
#endif

