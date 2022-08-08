#define	DS18B20_GLOBLES
#include <ds18b20.h>
void	DS18B20_Init()    //发送复位脉冲，检测存在脉冲 ---已调试
{
	uC_18B20 = 1;
	Delay(1);
	uC_18B20 = 0 ;
	Delay(500);		//延时480~960us，
	uC_18B20 = 1 ;
	Delay(50);
//	while(uC_18B20);//等待存在脉冲	//此处不判断存在脉冲，改为直接使用延时，避免因芯片损坏使整个系统停止工作
	Delay(180);		//延时250us
}

void	WriteBit(bit	DQ)
{
	uC_18B20 = 1;
	Delay(1);
	uC_18B20 = 0 ;
	Delay(10);//稍做延时，1us以上
	uC_18B20 = DQ ;
	Delay(40);
	uC_18B20 = 1 ;	//释放总线
	Delay(20);		//延时一段时间不大于50us，需调试
}

bit		ReadBit()
{
	bit temp;
	uC_18B20 = 1;
	Delay(1);
	uC_18B20 = 0 ;
	Delay(3);//约4us
	uC_18B20 = 1 ;
	Delay(1);	//稍作延时
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
	uint8	DQ8 = 0;		//实际低12位有效
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
	Delay(60000);					//延时，等待温度转换完成
	Delay(60000);
	Delay(60000);
	Delay(60000);
	Delay(60000);
	Delay(60000);
	DS18B20_Init();
	WriteByte(SkipRom);
	WriteByte(ReadScratchpad);

	*(temp+1) = ReadByte();//读内部RAM（LSB）
	*temp = ReadByte();	//读内部RAM（MSB）
}

void StartConvertTemp()
{
	DS18B20_Init();
	WriteByte(SkipRom);
	WriteByte(ConvertTemperature); 
//	Delay(60000);					//延时，等待温度转换完成
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

	*(temp+1) = ReadByte();	//读内部RAM（LSB）
	*temp = ReadByte();	//读内部RAM（MSB）
}