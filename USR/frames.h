#ifndef __FRAMES_H
#define __FRAMES_H
#include "stm8s.h"

void Ws2812_Config_set( Ws2812b_Config_ture_t Ws2812b_Config_data);
void Ws2812_Config_get(Ws2812b_Config_ture_t *Ws2812b_Config_data_ture);

int frames_code( u8* output, u8 cmd, u8* input, u8 inputlen );
int frames_decode( u8* data );

#endif

