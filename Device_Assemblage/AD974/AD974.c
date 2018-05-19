#include "config.h"
//////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//SPI驱动 代码
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/9
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////

//以下是SPI模块的初始化代码，配置成主机模式，访问SD Card/W25Q64/NRF24L01
//SPI口初始化
//这里针是对SPI2的初始化



void SPI2_Init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );//PORTB时钟使能

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //PB13/14/复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB

 	GPIO_ResetBits(GPIOB,GPIO_Pin_13);  //PB13/14/上拉
 	GPIO_SetBits(GPIOB,GPIO_Pin_12);  //PB13/14/上拉

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14;//PD10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOD10
}


//AD974初始化
void AD974_Init(void)
{
	SPI2_Init();
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOD, ENABLE );//PORTD时钟使能

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //PD8/9推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

 	GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化GPIOB
 	GPIO_SetBits(GPIOD,GPIO_Pin_8 | GPIO_Pin_9);  //PD8/9上拉

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;//PD10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化GPIOD10
}

//启动一次ADC转换
void SPI_Start(void)
{
	//SPI_SDA=1;
	SPI_SCL=0;
	delay_us(4);
	SPI_RC=0;//
	delay_us(2);
	SPI_RC=1;
	delay_us(1);
}

//读取一次ADC数据
u16	SPI_READ_DATA(void)
{
	u8 i=0;
	u16 receive_spi=0;
	for(i=0;i<16;i++)
	{
		SPI_SCL=0;
		delay_us(1);
		SPI_SCL=1;
		receive_spi<<=1;
        if(SPI_SDA)receive_spi++;
		delay_us(1);
	}
	return receive_spi;
}


//读取AD974数据
u16	AD974_ReadData(u8 AD974_Addr)
{
	u16 AD974_Data = 0;
	u16 i=0;
	u32 AD974_Temp = 0;
	switch(AD974_Addr)
		{
			case 0:
				AD974_Addr0();
				break;
			case 1:
				AD974_Addr1();
				break;
			case 2:
				AD974_Addr2();
				break;
			case 3:
				AD974_Addr3();
				break;
			default:
				break;

		}
	for(i=0;i<5000;i++)
	{
		WdogFeed();
		SPI_Start();
		while(int0_flag == FALSE)
		{
			if(++i > 10000*200) // 120
				break;
			WdogFeed();
		}
		WdogFeed();
		if(int0_flag == TRUE)
		{
			LED2=~LED2;
			AD974_Temp += SPI_READ_DATA();
			WdogFeed();
		}
		int0_flag =	FALSE;
		WdogFeed();
	}
	//AD974_Data = AD974_Temp/5000;
	AD974_Data = 10*AD974_Temp/65536/5.1-2731.5;
	return	AD974_Data;

}


