#include "485.h"
#include "bsp_AdvanceTim.h" 
#include "LTC2944.h"

void ADVANCE_TIM_Mode_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef timInitStruct ;
//	GPIO_InitTypeDef GPIO_InitStructure;
//	
//  RCC_APB2PeriphClockCmd(ADVANCE_TIM_CH2N_GPIO_CLK, ENABLE);
//  GPIO_InitStructure.GPIO_Pin =  ADVANCE_TIM_CH2N_PIN;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_Init(ADVANCE_TIM_CH2N_PORT, &GPIO_InitStructure);

	//中断NVIC设置：允许中断，设置优先级  
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  
	NVIC_InitStructure.NVIC_IRQChannel =  TIM1_UP_IRQn;             //更新事件  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;   //抢占优先级0  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;          //响应优先级0  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //允许中断  
	NVIC_Init(&NVIC_InitStructure);                             //写入设置  
	  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  
	//重新将Timer设置为缺省值  
	TIM_DeInit(TIM1);  
	//采用内部时钟给TIM2提供时钟源  
//	TIM_InternalClockConfig(TIM1);  
	
	timInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;            //定时器基准频率 72MHz
	timInitStruct.TIM_Prescaler = ADVANCE_TIM_PSC;                          
	timInitStruct.TIM_CounterMode = TIM_CounterMode_Up;         //向上计数
//   timInitStruct.TIM_RepetitionCounter = 0;  	
	timInitStruct.TIM_Period = ADVANCE_TIM_PERIOD;              //这个值实际上就是TIMX->ARR，延时开始时重新设定即可  
	TIM_TimeBaseInit(TIM1, &timInitStruct);  
	
	//清溢出中断标志  
	TIM_ClearFlag(TIM1, TIM_FLAG_Update);  
	//禁止ARR预装载缓冲器  0 TIMx_ARR寄存器没有缓存
	TIM_ARRPreloadConfig(TIM1, DISABLE);  
	//开启中断  
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);                
	TIM_Cmd(TIM1, ENABLE);                                      
}
//char TEST[1] ={0x55};  //测试定时器用
char tranfserOK =0;

void TIM1_UP_IRQHandler(void)
{
	//上传定时器
	if(TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)  
  {  		  
	  
//	  Usart_SendString(USART1,TEST);
		LTC2944_read_16_bits(LTC2944_I2C_ADDRESS,LTC2944_VOLTAGE_MSB_REG);
	   tranfserOK=1;
		//清中断溢出标志  
		TIM_ClearITPendingBit(TIM1,TIM_FLAG_Update);  
		
		
  }  
}
/*********************************************END OF FILE**********************/
