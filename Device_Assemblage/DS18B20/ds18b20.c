#define	DS18B20_GLOBLES
#include <ds18b20.h>
void	DS18B20_Init()    //���͸�λ���壬���������� ---�ѵ���
{
	uC_18B20 = 1;
	Delay(1);
	uC_18B20 = 0 ;
	Delay(500);		//��ʱ480~960us��
	uC_18B20 = 1 ;
	Delay(50);
//	while(uC_18B20);//�ȴ���������	//�˴����жϴ������壬��Ϊֱ��ʹ����ʱ��������оƬ��ʹ����ϵͳֹͣ����
	Delay(180);		//��ʱ250us
}

void	WriteBit(bit	DQ)
{
	uC_18B20 = 1;
	Delay(1);
	uC_18B20 = 0 ;
	Delay(10);//������ʱ��1us����
	uC_18B20 = DQ ;
	Delay(40);
	uC_18B20 = 1 ;	//�ͷ�����
	Delay(20);		//��ʱһ��ʱ�䲻����50us�������
}

bit		ReadBit()
{
	bit temp;
	uC_18B20 = 1;
	Delay(1);
	uC_18B20 = 0 ;
	Delay(3);//Լ4us
	uC_18B20 = 1 ;
	Delay(1);	//������ʱ
	temp = uC_18B20;
	Delay(30);
	return  temp;
}

void	WriteByte(uint8	Cmd)
{
	uint8	i;
	for(i = 0;i<8;i++)
	{
		((Cmd>>i)&0x01 ==1)?WriteBit(1):WriteBit(0);
	}
}

uint8	ReadByte()
{
	uint8	DQ8 = 0;		//ʵ�ʵ�12λ��Ч
	uint8	i;
	for (i = 0; i < 8; i++)
	{
		if(1 == ReadBit())
			DQ8 |= (0x01 << i);
	}
	return	DQ8;
}

void GetTemperature_Once(uint8* temp)
{
	DS18B20_Init();
	WriteByte(SkipRom);
	WriteByte(ConvertTemperature); 
	Delay(60000);					//��ʱ���ȴ��¶�ת�����
	Delay(60000);
	Delay(60000);
	Delay(60000);
	Delay(60000);
	Delay(60000);
	DS18B20_Init();
	WriteByte(SkipRom);
	WriteByte(ReadScratchpad);

	*(temp+1) = ReadByte();//���ڲ�RAM��LSB��
	*temp = ReadByte();	//���ڲ�RAM��MSB��
}

void StartConvertTemp()
{
	DS18B20_Init();
	WriteByte(SkipRom);
	WriteByte(ConvertTemperature); 
//	Delay(60000);					//��ʱ���ȴ��¶�ת�����
//	Delay(60000);
//	Delay(60000);
//	Delay(60000);
//	Delay(60000);
//	Delay(60000);	
}

void GetTemperature(uint8* temp)
{
	DS18B20_Init();
	WriteByte(SkipRom);
	WriteByte(ReadScratchpad);

	*(temp+1) = ReadByte();	//���ڲ�RAM��LSB��
	*temp = ReadByte();	//���ڲ�RAM��MSB��
}