
#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_AdvanceTim.h"  
#include "485.h"
#include "delay.h"
#include "sys.h"
#include "LTC2944.h"
#include "LTCIIC.h"
#include  "commu.h"


static void Delay(__IO u32 nCount); 

//u8 adc_code ;
//extern u8 b[2];
//extern u16 volot;
//char answer[100];

extern char tranfserOK ;
extern u8  LTC2944_value[7];
uint8_t flag;
 
 
int main(void)
{	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  
	LED_GPIO_Config();  
	RS485_Config();
	delay_init();	    	 //延时函数初始化	 
	LTC2944_Init();      //LTC2944引脚初始化
	ADVANCE_TIM_Mode_Config(); //LTC2944 100ms读取一次电压值
	WakeUp_AnalogPart();
	
   while(1)
	{  
//		LTC2944_read_16_bits(LTC2944_I2C_ADDRESS,LTC2944_VOLTAGE_MSB_REG);
		//轮询上位 发来的指令

		if(tranfserOK ==1)
		{		
			flag=datadispose();
			switch(flag)
			{	
				case 0x02:
				{
					RS485_TX_EN();
					U485_SendData(LTC2944_value,7);
					Delay(0xFFF);  //试验证明对于SP3485一个延迟都不能少不能改变否则数据错乱
					RS485_RX_EN();
					Delay(0xFFFF); //对于SP3485一个延迟都不能少不能改变
					break;
				}
				default :
					WakeUp_AnalogPart();
					break;
			}
			
			tranfserOK =0;
		}
		
		WakeUp_AnalogPart();
		
//		if((volot >0) && (volot!=99 )&& (volot!=65535))
//		{

//			delay_ms(1000);
//			delay_ms(1000);
//			delay_ms(1000);
//			sprintf(answer,"电压为--- %d\n",volot);
//			Usart_SendString(USART1,answer);
//		}
//		else
//		{
//			Delay(0xFFFFF5);

//			Usart_SendString(USART1,"error\n");
//		}

  }
}

static void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}
/*********************************************END OF FILE**********************/

//			LIIC_Start();
//			LIIC_Stop();
//		   i2c_read_byte(LTC2944_I2C_ADDRESS,ack);
//			RS485_SendString("now1");
//		  LTC2944_write(LTC2944_I2C_ADDRESS,LTC2944_ACCUM_CHARGE_LSB_REG,0xF1);
//			i2c_read_block_data(LTC2944_I2C_ADDRESS,0x08,4,values);
//			if(LTC2944_read(LTC2944_I2C_ADDRESS,0X02)== 1)
//			{
////				RS485_SendString("44444\r\n");
//			}
//			else
//				RS485_SendString("error");
//	
				
//		  LTC2944_write_16_bits(LTC2944_I2C_ADDRESS,LTC2944_VOLTAGE_MSB_REG,0x0000);
//			LTC2944_read_16_bits(LTC2944_I2C_ADDRESS,LTC2944_VOLTAGE_MSB_REG);
//			if(LTC2944_read_16_bits(LTC2944_I2C_ADDRESS,LTC2944_VOLTAGE_MSB_REG)==1)
//			{
////				ack =(b[0]<<8) | b[1];

//				sprintf(answer,"电压为--- %d\n",ack);
//				Usart_SendString(USART1,answer);
//		
//			}
//			else
//			{
////				Usart_SendString(USART1,"error\n");
////				Delay(0xFFFFFF);
//			}
					/*加短暂延时，保证485发送数据完毕*/

//					Delay(0xFFFFFFFA);
//					RS485_RX_EN();
//					Delay(0xFFFF);
//					clean_rebuff();
//				sprintf(answer,"电压为--- %d\n",ack);
//				Usart_SendString(USART1,answer);

