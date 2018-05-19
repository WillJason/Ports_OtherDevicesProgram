#include  "CAT9555.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//IIC驱动 代码
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/9
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
/////////////////////////////////////////////////////////////////////////////////


//初始化IIC
u16 CAT9555Mirror0,CAT9555Mirror1;
void IIC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7); 	//PB10,PB11 输出高

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
//产生IIC起始信号
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA=1;
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low
	delay_us(4);
	IIC_SCL=0;//钳住I2C总线，准备发送或接收数据
}
//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL=1;
	IIC_SDA=1;//发送I2C总线结束信号
	delay_us(4);
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA设置为输入
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
	IIC_SCL=0;//时钟输出0
	return 0;
}
//产生ACK应答
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
//不产生ACK应答
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
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答
void IIC_Send_Byte(u8 txd)
{
    u8 t;
	SDA_OUT();
    IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {
        //IIC_SDA=(txd&0x80)>>7;
		if((txd&0x80)>>7)
			IIC_SDA=1;
		else
			IIC_SDA=0;
		txd<<=1;
		delay_us(2);   //根据实际应用调节，必须满足！！
		IIC_SCL=1;
		delay_us(2);
		IIC_SCL=0;
		delay_us(2);
    }
}
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
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
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK
    return receive;
}

//写IIC配置
void CAT9555_WriteCFG(u8 CAT9555Addr,u8 CAT9555_WR0,u8 CAT9555_WR1)
{
    IIC_Start();
	IIC_Send_Byte(CAT9555Addr&0XFE);   //发送器件地址
	IIC_Wait_Ack();
    IIC_Send_Byte(0X06);   //发送命令字节,端口0,命令字节是在写数据发送过程中紧跟地址字节之后的第一个字节，它作为一个指针指向要进行写或读操作的寄存器。当指针指向端口0配置寄存器，写完该寄存器后，指针自动指向下一地址即端口1配置寄存器
	IIC_Wait_Ack();
	IIC_Send_Byte(CAT9555_WR0);     //发送配置字节,端口0
	IIC_Wait_Ack();
	IIC_Send_Byte(CAT9555_WR1);     //发送配置字节，端口1
	IIC_Wait_Ack();
    IIC_Stop();//产生一个停止条件
	delay_ms(10);
	IWDG_Feed();//喂狗
}

//写IIC数据,端口0、端口1分别写
void CAT9555_Write_OneByte(u8 CAT9555Addr,u8 CAT9555_WrData0,u8 CAT9555_WrData1)
{
    IIC_Start();
	IIC_Send_Byte(CAT9555Addr&0XFE);   //发送器件地址
	IIC_Wait_Ack();
    IIC_Send_Byte(0X02);   //发送命令字节,端口0
	IIC_Wait_Ack();
	IIC_Send_Byte(CAT9555_WrData0);     //发送配置字节,端口0
	IIC_Wait_Ack();
	IIC_Send_Byte(CAT9555_WrData1);     //发送配置字节，端口1
	IIC_Wait_Ack();
    IIC_Stop();//产生一个停止条件
	delay_ms(10);
	IWDG_Feed();//喂狗
}
