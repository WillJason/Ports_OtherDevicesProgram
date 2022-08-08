#ifndef __BSP_ADVANCETIME_H
#define __BSP_ADVANCETIME_H

#include "stm32f10x.h"

#define            ADVANCE_TIM                   TIM1
#define            ADVANCE_TIM_APBxClock_FUN     RCC_APB2PeriphClockCmd //72Mhz
#define            ADVANCE_TIM_CLK               RCC_APB2Periph_TIM1

// PWM 信号的频率 F = TIM_CLK/{(ARR+1)*(PSC+1)}
#define            ADVANCE_TIM_PERIOD            (100-1)  //  01K/10Khz =100ms
#define            ADVANCE_TIM_PSC               (7200-1)    //  10Khz
#define            ADVANCE_TIM_PULSE              0 

#define            ADVANCE_TIM_IRQ               TIM1_UP_IRQn
#define            ADVANCE_TIM_IRQHandler        TIM1_UP_IRQHandler

// TIM1 输出比较通道的互补通道
#define            ADVANCE_TIM_CH2N_GPIO_CLK      RCC_APB2Periph_GPIOB
#define            ADVANCE_TIM_CH2N_PORT          GPIOB
#define            ADVANCE_TIM_CH2N_PIN           GPIO_Pin_14

/**************************函数声明********************************/

void ADVANCE_TIM_Mode_Config(void);


#endif	/* __BSP_ADVANCETIME_H */


