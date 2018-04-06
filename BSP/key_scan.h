
#ifndef  __KEY_SCAN_H
#define  __KEY_SCAN_H
#include "stm8s_gpio.h"

#define Buttom_ON   0
#define Buttom_OFF  1


#define Button_Port     GPIOB
#define Buttom_1        GPIO_PIN_4
#define Buttom_2        GPIO_PIN_5

#define Shake_Key_Port  GPIOC
#define Shake_Key_Pin   GPIO_PIN_4


#define BUTTON_CLICK 20
#define BUTTON_CLICK_RUNOUT 25
#define BUTTON_EXTENDED_CLICK 100
#define BUTTON_EXTENDED_CLICK_RUNOUT 105



#define BUTTON_STATUS_RELEASE           0
#define BUTTON_STATUS_CLICK             1
#define BUTTON_STATUS_EXTENDED_PRESS    2

void Buttom_Init(void);
uint8_t Key_Scan1(GPIO_TypeDef* GPIOx,GPIO_Pin_TypeDef Buttom, u8 butt);
uint8_t Key_Scan2(GPIO_TypeDef* GPIOx,GPIO_Pin_TypeDef Buttom, u8 butt);

void Shake_Key_Init(void);
uint8_t Shake_Key_Scan(GPIO_TypeDef* GPIOx,GPIO_Pin_TypeDef Buttom);

/*
Key_Scan1(Button_Port, Buttom_1, 0);
Key_Scan2(Button_Port, Buttom_2, 0);
Shake_Key_Scan(Shake_Key_Port£¬Shake_Key_Pin);
*/


#endif