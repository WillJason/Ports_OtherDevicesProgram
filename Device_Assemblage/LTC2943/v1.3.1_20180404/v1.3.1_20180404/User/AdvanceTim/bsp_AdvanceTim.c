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

	//�ж�NVIC���ã������жϣ��������ȼ�  
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  
	NVIC_InitStructure.NVIC_IRQChannel =  TIM1_UP_IRQn;             //�����¼�  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;   //��ռ���ȼ�0  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;          //��Ӧ���ȼ�0  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //�����ж�  
	NVIC_Init(&NVIC_InitStructure);                             //д������  
	  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  
	//���½�Timer����Ϊȱʡֵ  
	TIM_DeInit(TIM1);  
	//�����ڲ�ʱ�Ӹ�TIM2�ṩʱ��Դ  
//	TIM_InternalClockConfig(TIM1);  
	
	timInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;            //��ʱ����׼Ƶ�� 72MHz
	timInitStruct.TIM_Prescaler = ADVANCE_TIM_PSC;                          
	timInitStruct.TIM_CounterMode = TIM_CounterMode_Up;         //���ϼ���
//   timInitStruct.TIM_RepetitionCounter = 0;  	
	timInitStruct.TIM_Period = ADVANCE_TIM_PERIOD;              //���ֵʵ���Ͼ���TIMX->ARR����ʱ��ʼʱ�����趨����  
	TIM_TimeBaseInit(TIM1, &timInitStruct);  
	
	//������жϱ�־  
	TIM_ClearFlag(TIM1, TIM_FLAG_Update);  
	//��ֹARRԤװ�ػ�����  0 TIMx_ARR�Ĵ���û�л���
	TIM_ARRPreloadConfig(TIM1, DISABLE);  
	//�����ж�  
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);                
	TIM_Cmd(TIM1, ENABLE);                                      
}
//char TEST[1] ={0x55};  //���Զ�ʱ����
char tranfserOK =0;

void TIM1_UP_IRQHandler(void)
{
	//�ϴ���ʱ��
	if(TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)  
  {  		  
	  
//	  Usart_SendString(USART1,TEST);
		LTC2944_read_16_bits(LTC2944_I2C_ADDRESS,LTC2944_VOLTAGE_MSB_REG);
	   tranfserOK=1;
		//���ж������־  
		TIM_ClearITPendingBit(TIM1,TIM_FLAG_Update);  
		
		
  }  
}
/*********************************************END OF FILE**********************/
