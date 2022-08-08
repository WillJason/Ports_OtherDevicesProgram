#include	<spi0.h>  
/******************************************************************************	   
	在全双工操作中，当SPI主器件在MOSI线向从器件发送数据时，被寻址的SPI从器件可以
同时在MISO线上向主器件发送其移位寄存器中的内容。
	芯片C8051F340------SPIF标志即作为发送完成标志又作为接收完成标志。
	无论是MOSI,还是MISO，数据传输时都是最高位在先。
*******************************************************************************/
#define SPI_CLK 800000  																	//1000000
void SendSPIByte(uint8 ch)
{
	#ifdef WD_EN
		Feed_Wd();
	#endif
	SPI0DAT = ch;
	while (SPIF == 0);	 																		// 等待写结束，传输结束后SPIF（SPIOCN.7）标志被置为逻辑1；
	SPIF = 0;																								 //必须用软件清零
} 
uint8 GetSPIByte(void)
{
	#ifdef WD_EN
		Feed_Wd();
	#endif
	SPI0DAT = 0;                       											//产生读回时钟
	while (SPIF == 0);
	SPIF = 0;
	return  SPI0DAT;                   		// 等待读结束
}

void SPI0_Init(void)
{
//	XBR0    |= 0x02;                     //交叉开关连接spi
//	P0MDOUT   |= 0x0D;                  	//SPI口推挽输出* 开漏输出的（若上拉）上升沿太缓
    SPI0CFG  = 0x40;                     // data sampled on 1st SCK rising edge		 将主允许标志（MSTEN,SPI0CFG.6）置1将SPI0置主动方式。
                                        	// 8-bit data words
								        //Master mode
    SPI0CN   = 0x03;                     // ; SPI enabled
    SPI0CKR  = SYSCLK/2/SPI_CLK-1;       // SPI clock <= 、
}