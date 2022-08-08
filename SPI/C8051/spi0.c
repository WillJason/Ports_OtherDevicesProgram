#include	<spi0.h>  
/******************************************************************************	   
	��ȫ˫�������У���SPI��������MOSI�����������������ʱ����Ѱַ��SPI����������
ͬʱ��MISO��������������������λ�Ĵ����е����ݡ�
	оƬC8051F340------SPIF��־����Ϊ������ɱ�־����Ϊ������ɱ�־��
	������MOSI,����MISO�����ݴ���ʱ�������λ���ȡ�
*******************************************************************************/
#define SPI_CLK 800000  																	//1000000
void SendSPIByte(uint8 ch)
{
	#ifdef WD_EN
		Feed_Wd();
	#endif
	SPI0DAT = ch;
	while (SPIF == 0);	 																		// �ȴ�д���������������SPIF��SPIOCN.7����־����Ϊ�߼�1��
	SPIF = 0;																								 //�������������
} 
uint8 GetSPIByte(void)
{
	#ifdef WD_EN
		Feed_Wd();
	#endif
	SPI0DAT = 0;                       											//��������ʱ��
	while (SPIF == 0);
	SPIF = 0;
	return  SPI0DAT;                   		// �ȴ�������
}

void SPI0_Init(void)
{
//	XBR0    |= 0x02;                     //���濪������spi
//	P0MDOUT   |= 0x0D;                  	//SPI���������* ��©����ģ���������������̫��
    SPI0CFG  = 0x40;                     // data sampled on 1st SCK rising edge		 ���������־��MSTEN,SPI0CFG.6����1��SPI0��������ʽ��
                                        	// 8-bit data words
								        //Master mode
    SPI0CN   = 0x03;                     // ; SPI enabled
    SPI0CKR  = SYSCLK/2/SPI_CLK-1;       // SPI clock <= ��
}