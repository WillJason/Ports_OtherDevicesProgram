#include "LTCIIC.h"
#include "delay.h"
#include "485.h"



//����IIC��ʼ�ź�
void LIIC_Start(void)
{
	LSDA_OUT();     //sda�����
	LIIC_SDA=1;	  	  
	LIIC_SCL=1;
	delay_us(4);
	
 	LIIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	LIIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	

//����IICֹͣ�ź�
void LIIC_Stop(void)
{
	LSDA_OUT();//sda�����
	LIIC_SCL=0;
	LIIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	LIIC_SCL=1; 
	LIIC_SDA=1;//����I2C���߽����ź�
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 LIIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	
	LIIC_SDA=1;delay_us(1);	
	
	LIIC_SCL=1;delay_us(1);	 /*SCLΪ�� ���ж�ȡ���� */  
   LSDA_IN();               //SDA����Ϊ����  
//	Usart_SendString(USART1,"----->Ӧ��\n");
	while(LREAD_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			LIIC_Stop();
			return 1;
		}
	}

	LIIC_SCL=0;//ʱ�����0 	  	
	return 0;  
} 


//����ACKӦ��
void LIIC_Ack(void)
{
	LIIC_SCL=0;
	LSDA_OUT();
	LIIC_SDA=0;
	delay_us(2);
	LIIC_SCL=1;
	delay_us(2);
	LIIC_SCL=0;
}
//������ACKӦ��		    
void LIIC_NAck(void)
{
	LIIC_SCL=0;
	LSDA_OUT();
	LIIC_SDA=1;
	delay_us(2);
	LIIC_SCL=1;
	delay_us(2);
	LIIC_SCL=0;
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void LIIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	 LSDA_OUT(); 	    
    LIIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
	
    for(t=0;t<8;t++)
    {     
		/* I2C���ݰ��մ�˴���ģ�Ҳ���Ǹ�λ�ȴ���*/  
       LIIC_SDA=(txd&0x80)>>7;
		  
        txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		LIIC_SCL=1;
		delay_us(2); 
		LIIC_SCL=0;	
		delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 LIIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	LSDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        LIIC_SCL=0; 
        delay_us(2);
			LIIC_SCL=1;
		
		receive <<= 1;
		
		if(LREAD_SDA)
		   receive++;
//		else
//			receive=receive&0xFE;
		
		delay_us(1); 
    }	

    if (!ack)
        LIIC_Ack(); //����ACK  
    else
       LIIC_NAck();//����nACK
//	 
//	 RS485_SendString("22222\n"); 		
    return receive;
}







