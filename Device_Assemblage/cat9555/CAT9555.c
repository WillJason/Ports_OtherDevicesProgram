#include  "CAT9555.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//ALIENTEKս��STM32������
//IIC���� ����
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/9
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) �������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
/////////////////////////////////////////////////////////////////////////////////


//��ʼ��IIC
u16 CAT9555Mirror0,CAT9555Mirror1;
void IIC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7); 	//PB10,PB11 �����

}
void CAT9555_Init(void)
{
	IIC_Init();
	CAT9555Mirror0=0xFFFF;
	CAT9555Mirror1=0xFFFF;
	CAT9555_WriteCFG(CAT9555_ADDR0,0x00,0x00);
	CAT9555_WriteCFG(CAT9555_ADDR1,0x00,0x00);
	CAT9555_Write_OneByte(CAT9555_ADDR0,0xff,0xff);
	CAT9555_Write_OneByte(CAT9555_ADDR1,0xff,0xff);
}
//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA=1;
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low
	delay_us(4);
	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ��������
}
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL=1;
	IIC_SDA=1;//����I2C���߽����ź�
	delay_us(4);
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����
	IIC_SDA=1;delay_us(1);
	IIC_SCL=1;delay_us(1);
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;//ʱ�����0
	return 0;
}
//����ACKӦ��
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}
//������ACKӦ��
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��
void IIC_Send_Byte(u8 txd)
{
    u8 t;
	SDA_OUT();
    IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {
        //IIC_SDA=(txd&0x80)>>7;
		if((txd&0x80)>>7)
			IIC_SDA=1;
		else
			IIC_SDA=0;
		txd<<=1;
		delay_us(2);   //����ʵ��Ӧ�õ��ڣ��������㣡��
		IIC_SCL=1;
		delay_us(2);
		IIC_SCL=0;
		delay_us(2);
    }
}
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0;
        delay_us(2);
		IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;
		delay_us(1);
    }
    if (!ack)
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK
    return receive;
}

//дIIC����
void CAT9555_WriteCFG(u8 CAT9555Addr,u8 CAT9555_WR0,u8 CAT9555_WR1)
{
    IIC_Start();
	IIC_Send_Byte(CAT9555Addr&0XFE);   //����������ַ
	IIC_Wait_Ack();
    IIC_Send_Byte(0X06);   //���������ֽ�,�˿�0,�����ֽ�����д���ݷ��͹����н�����ַ�ֽ�֮��ĵ�һ���ֽڣ�����Ϊһ��ָ��ָ��Ҫ����д��������ļĴ�������ָ��ָ��˿�0���üĴ�����д��üĴ�����ָ���Զ�ָ����һ��ַ���˿�1���üĴ���
	IIC_Wait_Ack();
	IIC_Send_Byte(CAT9555_WR0);     //���������ֽ�,�˿�0
	IIC_Wait_Ack();
	IIC_Send_Byte(CAT9555_WR1);     //���������ֽڣ��˿�1
	IIC_Wait_Ack();
    IIC_Stop();//����һ��ֹͣ����
	delay_ms(10);
	IWDG_Feed();//ι��
}

//дIIC����,�˿�0���˿�1�ֱ�д
void CAT9555_Write_OneByte(u8 CAT9555Addr,u8 CAT9555_WrData0,u8 CAT9555_WrData1)
{
    IIC_Start();
	IIC_Send_Byte(CAT9555Addr&0XFE);   //����������ַ
	IIC_Wait_Ack();
    IIC_Send_Byte(0X02);   //���������ֽ�,�˿�0
	IIC_Wait_Ack();
	IIC_Send_Byte(CAT9555_WrData0);     //���������ֽ�,�˿�0
	IIC_Wait_Ack();
	IIC_Send_Byte(CAT9555_WrData1);     //���������ֽڣ��˿�1
	IIC_Wait_Ack();
    IIC_Stop();//����һ��ֹͣ����
	delay_ms(10);
	IWDG_Feed();//ι��
}