#ifndef	__DS18B20_H_
#define	__DF18B20_H_

sbit	uC_18B20= 	P1^4;

#ifdef	DS18B20_GLOBLES
#define	DS18B20_EXT
#else
#define	DS18B20_EXT	extern
#endif

#define	ReadRom		0x33
#define	MatchRom		0x55
#define	SkipRom		0xcc
#define	SearchRom	0xf0
#define	AlarmSearch	0xec

#define	WriteScratchpad		0x4e		//д?ݴ??洢??
#define	ReadScratchpad		0xbe		//???ݴ??洢??
#define	CopyScratchpad		0x48		//?????ݴ??洢??
#define	ConvertTemperature	0x44		//?¶ȱ任
#define	RecalEPROM		0xb8		//???µ???
#define	ReadPowerSupply	0xb4		//????Դ

DS18B20_EXT	void	DS18B20_Init(void);
DS18B20_EXT	bit		ReadBit();
DS18B20_EXT	void	WriteBit(bit	DQ);
DS18B20_EXT	uint8	ReadByte();
DS18B20_EXT	void	WriteByte(uint8	Cmd);
DS18B20_EXT	void	GetTemperature(uint8* temp);
DS18B20_EXT void StartConvertTemp();
DS18B20_EXT void GetTemperature_Once(uint8* temp);


#endif