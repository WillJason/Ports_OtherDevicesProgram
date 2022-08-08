#define  I2C_GLOBALES
#include <i2c.h>
// ������ͨ��2010.06.25
/************************************************************************************************************
SMBus��û�б�Ҫ��һ��ϵͳ��ָ��ĳ��������Ϊ���������κ�һ��������ʼ������START���ʹ�������ַ�������ͳ�Ϊ�ô����ݴ������������
	 ��������������������ͬһʱ���������ݴ��䣬����ʹ���ٲû�����ʹһ���������������ߡ�
*SCL�͵�ƽ��ʱ��SMBusЭ��涨���μ�һ�����ݴ��������������ʱ�ӵ͵�ƽʱ�䣬������25ms����Ϊ�ǡ���ʱ������⵽��ʱ����������������10ms���ڸ�λͨ�ŵ�·��
	��SMB0CF�е�SMBTOEλ����λʱ����ʱ��3�����ڼ��SCL�͵�ƽ��ʱ������SCL�͵�ƽ��ʱ�¼��ö�ʱ��3�жϷ�������SMBus��λ����ֹ������ʹ�ܣ�
*SCL�ߵ�ƽ��SMBus���У���ʱ�����һ����������SCL��SDA��Ϊ�ߵ�ƽ��ʱ�䳬��50΢�����Ϊ���ߴ��ڿ���״̬����SMBFTEλ���á�1��ʱ�����SCL��SDA���ָߵ�ƽ��ʱ�䳬��10��
	SMBusʱ�����ڣ����߽�����Ϊ���С�				 
*ÿ�������ֽڻ�ӵ�ַ���䶼����SMBus�жϡ�						   
*��SYSCLK����10MHZʱ��ͨ����Ҫ��չ�����ͱ���ʱ�䡣
*��ACKRQ��λʱ�����Ӧ�����SI֮ǰ��ACKλд��Ҫ������ACKֵ��
*��SMBus��ʹ�ܵ�SI��־����Ϊ�߼���0��ʱ�����Ӧ����SMB0DAT�Ĵ�������ΪӲ���������ڶԸüĴ����е������ֽڽ���������Ƴ�������
 ֻҪSI�����жϱ�־λ��SMB0CN.0��Ϊ�߼���1�����üĴ����ڵĴ������ݾ����ȶ��ġ�
*SMB0DAT�е������������Ƴ�MSB��
*************************************************************************************************************/
#define  WRITE 	0x00                 // SMBus д����
#define  READ 		0x01                 // SMBus ������

// #define  EEPROM_ADDR    0xA0                 // ����洢���ĵ�ַ
// #define  SMB_BUFF_SIZE  0x08                 // ���崫���ַ


#define  SMB_MTSTA      0xE0                 // ��ʼ���� 1110 ������	��������
#define  SMB_MTDB       0xC0                 // ���ݴ��� 1100 ������	�����ֽڷ���
#define  SMB_MRDB       0x80                 // ���ݽ��� 1000 ������	�����ֽڽ���


uint8* pSMB_DATA_IN;                 // ����ȫ�ֱ���     ���н��յ�������д������
uint8 SMB_SINGLEBYTE_OUT;            // ���������ź�	 �����ֽ�д

uint8* pSMB_DATA_OUT;                // ���ݴ洢����	 ����Ҫ���͵����ݴ������ȡ
// uint32 q;
uint8 SMB_DATA_LEN;                  // SMBUS�������ݳ���	��ǰ��Ҫ���ͻ���յ���������

uint8 WORD_ADDR;                     // ���ݵ�ַ	   ���豸�ڲ���ַ

uint8 TARGET;                        //Ŀ��ӵ�ַ	   ���豸Ŀ���ַ
// uint8 temp_char;
uint8 retval;                        //����ֵ����
bit SMB_BUSY = 0;                            // ����SMBUSæ��־λ	����æ��־������д����߶�ȡ��ʱ��SMB_BUSY=1;

bit SMB_RW;                                  // ��д�����־	��ǰ���ߴ��䷽��ѡ���־

bit SMB_SENDWORDADDR;                        // �����ַλ	  �˱�־λ��λ���������ڷ�������豸��ַ�󣬷��������ڲ���ַ

bit SMB_RANDOMREAD;                          // ����������Ҫд��ַ������Ϊ1 �����	 �˱�־λ��λ�����ڷ����������ڲ���ַ�����·���һ������λ

bit SMB_ACKPOLL;                             // ����Ӧ��   �˱�־λ��λ����һֱ���������źţ�ֱ�����豸����һ��ȷ�����ַ�ź�
bit FAIL=0;						 //����ʧ�ܵı�־

// 16-bit SFR declarations sfr�Ƕ���8λ�����⹦�ܼĴ�������sfr16������������16�����⹦�ܼĴ���
sfr16    TMR3RL   = 0x92;                    // Timer3 reload registers
sfr16    TMR3     = 0x94;                    // Timer3 counter registers

void Timer0_Init (void);					 //��ʱ��0��ʼ������  ʱ��
void Timer3_Init (void);					 //��ʱ��3��ʼ������  �͵�ƽ��ʱ


/*
**********************************************************************************************************
** �������ƣ�I2C_ByteWrite()
** �������ܣ������ӵ�ַ���������ֽ����ݡ�
** ��ڲ�����target_addr		������ַ
**               dat		                Ҫ���͵�����
** ���ڲ�����
** ˵����
*********************************************************************************************************
*/

void I2c_WriteByte(uint8 target_addr, uint8* src_addr,uint8 len)
{
	while (SMB_BUSY)
	{
		#ifdef WD_EN
		Feed_Wd();
		#endif
	};

	SMB_BUSY = 1;

	TARGET = target_addr;
	SMB_RW = WRITE;
	SMB_SENDWORDADDR = 0;
	SMB_RANDOMREAD = 0;
	SMB_ACKPOLL = 1;
	// SMB_SINGLEBYTE_OUT = dat;
	pSMB_DATA_OUT = (uint8 *)src_addr;  //??    pSMB_DATA_OUT = &dat;
	SMB_DATA_LEN = len;

	STA = 1;   // ��������

	while (SMB_BUSY)
	{
		#ifdef WD_EN
			Feed_Wd();
		#endif
	}
}

/*********************************************************************************************************
** �������ƣ�I2C_ByteRead()
** �������ܣ������ӵ�ַ������ȡ�ֽ����ݡ�
** ��ڲ�����target_addr	������ַ
**
** ���ڲ��������ص�����
** ˵����
*********************************************************************************************************/
void I2c_ReadByte(uint8 target_addr, uint8* dst_addr,uint8 len)
{
	#ifdef WD_EN
		Feed_Wd();
	#endif
	while (SMB_BUSY)
	{
		#ifdef WD_EN
			Feed_Wd();
		#endif
	};
	SMB_BUSY = 1;
	TARGET = target_addr;
	SMB_RW = READ;

	SMB_SENDWORDADDR = 0;
	SMB_RANDOMREAD = 0;  // д���ַ�������
	SMB_ACKPOLL = 1;
	// pSMB_DATA_IN = &retval;
	pSMB_DATA_IN = (uint8 *)dst_addr;
	SMB_DATA_LEN = len;

	STA = 1;

	while (SMB_BUSY)
	{
		#ifdef WD_EN
			Feed_Wd();
		#endif
	};
}

/*********************************************************************************************************
** �������� I2C_WriteSubByte()
** �������� �������ӵ�ַ���������ַ��ʼ��ȡN�ֽ�����
** ��ڲ��� ��		target_addr		�����ӵ�ַ
**				suba_type	 �ӵ�ַ�ṹ	1�����ֽڵ�ַ	3��8+X�ṹ	2��˫�ֽڵ�ַ
**				suba		 �����ӵ�ַ
**				src_addr			���ݽ��ջ�����ָ��
**				len			��ȡ�ĸ���
** ���ڲ��� ��
**
*********************************************************************************************************/
void I2c_WriteSubByte (uint8 target_addr, uint8 suba_type, uint16 suba, uint8* src_addr,
                        uint8 len)
{
  while (SMB_BUSY);
   SMB_BUSY = 1;

   TARGET = target_addr;
   SMB_RW = WRITE;
   if(suba_type == ONE_BYTE_SUBA)
   {
	   SMB_SENDWORDADDR = 1;
   }
   else
   {
	   SMB_SENDWORDADDR = 0;
   }
   SMB_RANDOMREAD = 0;
   SMB_ACKPOLL = 1;
   WORD_ADDR = suba;
   //SMB_SINGLEBYTE_OUT = dat;
   pSMB_DATA_OUT = (uint8 *)src_addr;  //??
   SMB_DATA_LEN = len;

   STA = 1;   // ��������

   while(SMB_BUSY);
} 
/**********************************************************************************************************
** �������� ��I2C_ReadSubByte()
** �������� �������ӵ�ַ���������ַ��ʼ��ȡN�ֽ�����
** ��ڲ��� ��	target_addr			�����ӵ�ַ
**				suba_type	�ӵ�ַ�ṹ	1�����ֽڵ�ַ	2��8+X�ṹ	2��˫�ֽڵ�ַ
**				suba		�����ӵ�ַ
**				scr_addr			���ݽ��ջ�����ָ��
**				len			��ȡ�ĸ���
** ���ڲ��� ��
**
**********************************************************************************************************/
 void I2C_ReadSubByte (uint8 target_addr, uint8 suba_type, uint16 suba, uint8* scr_addr,
                       uint8 len)
{
   while (SMB_BUSY);
   SMB_BUSY = 1;
   TARGET = target_addr;
   SMB_RW = WRITE;

   if(suba_type == ONE_BYTE_SUBA)
   {
	   SMB_SENDWORDADDR = 1;
	   SMB_RANDOMREAD = 1;  // д���ַ�������
   }
   else
   {
	   SMB_SENDWORDADDR = 1;
	   SMB_RANDOMREAD = 1;  // д���ַ�������
   }

   SMB_ACKPOLL = 1;
   WORD_ADDR = suba;
    pSMB_DATA_IN = (uint8 *) scr_addr;

   SMB_DATA_LEN = len;
   STA = 1;
   while(SMB_BUSY);
} 

/*******************************************************************************
��������:SMBUS��ʼ��
*******************************************************************************/
void I2c_Init (void)
{
	//XBR0    |= 0x04;//���濪������SMBus
	SMB0CF = 0x5C;     // 0101 1100   ��ʱ��0���Ϊʱ��Դ  ����ʱ��3��ʱ���25ms
	SMB0CF |= 0x80;   //ʹ��IIC��ʹ��SMBus
	EIE1   |= 0x01;    // SMBus interrupt enable��SMBus�ж�����λ������SMB0���ж�����
	Timer0_Init();					 //��ʱ��1��ʼ������
	Timer3_Init();					 //��ʱ��3��ʼ������
}

/*******************************************************************************
��������:��ʱ��3��ʼ�� 25ms//���Բ��õ�8λ����жϲ���1���ж�
*******************************************************************************/
static void Timer3_Init (void)
{
	TMR3CN = 0x00;  // Timer3 configured for 16-bit
	// auto-reload,
	// low-byte interrupt disabled
	TMR3 = -(SYSCLK/12/40);                   // Timer3 configured to overflow after
	TMR3RL = -(SYSCLK/12/40);                 // ~25ms (for SMBus low timeout detect)

	CKCON &= ~0x40;                           // Timer3 uses SYSCLK/12	��ʱ��3���ֽ�ʹ��TMR3CN�е�T3XCLKλ�����ʱ��
	TMR3CN |= 0x04;                             // Start Timer3	   ��ʱ��3������˫8λ�Զ���װ�ض�ʱ����ʽ
	EIE1 |= 0x80;                             // Timer3 �ж�����
}

/*******************************************************************************
��������:��ʱ��0��ʼ����SCL
*******************************************************************************/
static void Timer0_Init (void)
{
	CKCON |= 0x01;							   // Timer0 clock source = SYSCLK / 4  ʹ�÷�Ƶʱ��
	//CKCON &= ~0x07;                           // Timer0 clock source = SYSCLK / 12
	//ע��UART0��ʱ��
	// CKCON |= 0x01;                           // Timer0 clock source = SYSCLK / 4
	TMOD |= 0x02;                              // Timer0 in 8-bit auto-reload mode	  �Զ���װ�ص�8λ������/��ʱ��

	TH0 = -(SYSCLK/4/SMB_FREQUENCY/3);       // Timer0 configured to overflow at 1/3
	// the rate defined by SMB_FREQUENCY

	TL0 = -(SYSCLK/4/SMB_FREQUENCY/3);       // Timer0 preloaded to overflow at 1/3       SMB_FREQUENCYΪSMBƵ��200000 Ϊ200KHZ
	// the rate defined by SMB_FREQUENCY
	TR0 = 1;                                  // Timer0 enabled
}

/*******************************************************************************
��������:	SMBUS������,�ⲿ�ֺ�������SMBUSͨѶЭ������жϴ���.������μ�F340��
		���ֲ�SMBUSͨѶ����.
*******************************************************************************/
void SMBus_ISR (void) interrupt INTERRUPT_SMBUS0			 //�жϷ������
{
	// Used by the ISR to flag failed
	// transfers
	
	static uint8 idata i;                     // Used by the ISR to count the	���ͻ������������
	// number of data bytes sent or
	// received
	static bit SEND_START = 0;  		   // Send a start ������־λ

	switch (SMB0CN & 0xF0)                    // Status vector	״̬����
	{
		// Master Transmitter/Receiver: START condition transmitted.
	    //�����ͻ���գ����������ѷ���
	case SMB_MTSTA: //1110

		SMB0DAT = TARGET | SMB_RW;          // Load address of the target slave	  װ��Ŀ���ַ
		STA = 0;                            // Manually clear START bit	�ֶ��������λ
		i = 0;							   //��������λ������
		_nop_();
		break;

		// Master Transmitter: Data byte (or Slave Address) transmitted
		//�����ͣ�����λ��ӵ�ַ�ѷ���
	case SMB_MTDB: // 1100			
		if (ACK)                            // Slave Address or Data Byte  �ӵ�ַ������λӦ��
		{                                   // Acknowledged?
		  
			if (SEND_START)     // �����ظ���ʼ����
			{
				STA = 1;
				SEND_START = 0;
				break;
			}
			if (SMB_SENDWORDADDR)            // Are we sending the word address?  �Ƿ��������ڲ���ַ��
			{
				SMB_SENDWORDADDR = 0;         // Clear flag		 ���־λ
				SMB0DAT = WORD_ADDR;          // send word address	  ���͵�8λ�����ڲ���ַ

				if (SMB_RANDOMREAD)			   //�Ƿ���Ҫ����һ������λ
				{
					SEND_START = 1;            // send a START after the next ACK cycle
					SMB_RW = READ;
				}

				break;
			}

			if (SMB_RW==WRITE)               // Is this transfer a WRITE?
			{
				if (i < SMB_DATA_LEN)         // Is there data to send?	  ���ݷ��ꣿ
				{
					SMB0DAT = *pSMB_DATA_OUT;  // send data byte		   ������
					pSMB_DATA_OUT++;           // increment data out pointer	  ���͵����ݵ�ַ���ӣ�����д������
					i++;                       // increment number of bytes sent
				}
				else
				{
					STO = 1;                    // set STO to terminte transfer	   ��λSTO�Խ����˴δ���
					SMB_BUSY = 0;               // clear software busy flag		���æ��־λ
				}
			}
			else {}                          // If this transfer is a READ,	 ����˴��Ǹ���������ʾ������
			// then take no action. Slave
			// address was transmitted. A
			// separate 'case' is defined
			// for data byte recieved.
		}
		else                                // If slave NACK,
		{
			if (SMB_ACKPOLL)				 
			{
				STA = 1;                      // Restart transfer ��������
			}
			else
			{
				FAIL = 1;                     // Indicate failed transfer
			}                                // and handle at end of ISR
		}
		break;

		// Master Receiver: byte received	�����գ��ֽڽ���
	case SMB_MRDB: // 1000
//	Spectrum_LED1=!Spectrum_LED1;
		if ( i < SMB_DATA_LEN )             // Is there any data remaining?
		{
			*pSMB_DATA_IN = SMB0DAT;         // Store received byte
			pSMB_DATA_IN++;                  // Increment data in pointer
			i++;                             // Increment number of bytes received
			ACK = 1;                         // Set ACK bit (may be cleared later
			// in the code)

		}

		if (i == SMB_DATA_LEN)              // This is the last byte
		{
			SMB_BUSY = 0;                    // Free SMBus interface
			ACK = 0;                         // Send NACK to indicate last byte
			// of this transfer
			STO = 1;                         // Send STOP to terminate transfer
		}
		break;

	default:
		FAIL = 1;                           // Indicate failed transfer
		// and handle at end of ISR
		break;
	}

	if (FAIL)                                 // If the transfer failed,
	{
		SMB0CN &= ~0x40;                       // Reset communication
		SMB0CN |= 0x40;
		SMB_BUSY = 0;                          // Free SMBus  �ͷ�SMBus
	}

	SI=0;                                     // clear interrupt flag	����жϱ�־λ
}


/*******************************************************************************
��������:25MS���SMBUS���³�ʼ��
// A Timer3 interrupt indicates an SMBus SCL low timeout.
// The SMBus is disabled and re-enabled here
*******************************************************************************/
void Timer3_ISR (void) interrupt INTERRUPT_TIMER3
{ 
	SMB0CN &= ~0x40;                          // Disable SMBus	  ��ֹSMBus
	SMB0CN |= 0x40;                           // Re-enable SMBus  ����SMBus
	TMR3CN &= ~0x80;                          // Clear Timer3 interrupt-pending flag
}