#ifndef __HEAD_H
#define __HEAD_H

#include "led.h"
#include "tim1.h"
#include "key_scan.h"

#include "beep.h"
#include "uart1.h"
#include "adc.h"

#include "idog.h"


#include "w25x16.h"
#include "spi.h"

#include "stm8s_iwdg.h"


#include "bspinit.h"


#include <string.h>



struct FSM_t
{
    unsigned int flag_switch;
    unsigned int flag_1ms;
    unsigned int flag_5ms;
    unsigned int flag_10ms;
    unsigned int flag_20ms;
    unsigned int flag_1s;

};
typedef struct key_status
{
    unsigned char key1_status;
    unsigned char key2_status;
    unsigned char shake_key_status;
} key_status_t;
//key_status_all;


#endif
