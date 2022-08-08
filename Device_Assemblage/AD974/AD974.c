#include "config.h"
//////////////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//SPI���� ����
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/9
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////

//������SPIģ��ĳ�ʼ�����룬���ó�����ģʽ������SD Card/W25Q64/NRF24L01
//SPI�ڳ�ʼ��
//�������Ƕ�SPI2�ĳ�ʼ��



void SPI2_Init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );//PORTBʱ��ʹ��

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //PB13/14/�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB

 	GPIO_ResetBits(GPIOB,GPIO_Pin_13);  //PB13/14/����
 	GPIO_SetBits(GPIOB,GPIO_Pin_12);  //PB13/14/����

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14;//PD10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOD10
}


//AD974��ʼ��
void AD974_Init(void)
{
	SPI2_Init();
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOD, ENABLE );//PORTDʱ��ʹ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //PD8/9�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

 	GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��GPIOB
 	GPIO_SetBits(GPIOD,GPIO_Pin_8 | GPIO_Pin_9);  //PD8/9����

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;//PD10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��GPIOD10
}

//����һ��ADCת��
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

//��ȡһ��ADC����
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


//��ȡAD974����
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


