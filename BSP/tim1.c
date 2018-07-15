
/******************** (C) COPYRIGHT  奇来科技 *************************************
 * 文件名  ：tim1.c
 * 描述    ：tim1控制led闪烁程序   
 * 实验平台： STM8开发板
 * 库版本  ：V1.0.0
 * 技术支持：lijinnan@qiqirobot.com
 * 修改时间：2016-08-25
 * 淘宝网址：https://lsqrd.taobao.com/index.htm?spm=2013.1.w5002-7895914584.2.kYLHaf
**********************************************************************************/

#include "stm8s_tim1.h"
#include "tim1.h"

static  u32 TimingDelay;   //定义类型


#define TIM1_TIME  5000 //ms

void Tim1_Init(void)    //定时器1初始化
{
    TIM1_TimeBaseInit(16,TIM1_COUNTERMODE_UP,TIM1_TIME,0); //16分频，向上计数，计数完了触发中断，如果想
    //再 1MS 触发中断，则计数器应该计数 1000 次，1MHZ/1000=1KHZ，正好就是 1ms。
    TIM1_ARRPreloadConfig(ENABLE); //使能自动重装
    TIM1_ITConfig(TIM1_IT_UPDATE , ENABLE); //数据更新中断
    TIM1_Cmd(ENABLE); //开定时器
}

void TimingDelay_Decrement(void) //定时器1定时函数
{
    if (TimingDelay != 0x00)   //如果不为0，循环执行
    { 
        TimingDelay--;      //如果不为0，作减法
    }else{
      TimingDelay = TIM1_TIME;
    }
}

void Tim1_ms( u32 nTime)    //延时1秒循环
{
    TimingDelay = nTime;
    while(TimingDelay != 0);
}

/******************* (C) COPYRIGHT 奇来科技 *****END OF FILE****/
