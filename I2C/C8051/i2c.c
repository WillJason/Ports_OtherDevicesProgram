#define  I2C_GLOBALES
#include <i2c.h>
// 调试已通过2010.06.25
/************************************************************************************************************
SMBus：没有必要再一个系统中指定某个器件作为主器件；任何一个发送起始条件（START）和从器件地址的器件就成为该次数据传输的主器件。
	 两个或多个主器件可能在同一时刻启动数据传输，所以使用仲裁机制迫使一个主器件放弃总线。
*SCL低电平超时：SMBus协议规定：参加一次数据传输的器件必须检查时钟低电平时间，若超过25ms则认为是“超时”。检测到超时条件的器件必须在10ms以内复位通信电路。
	当SMB0CF中的SMBTOE位被置位时，定时器3倍用于检测SCL低电平超时。发生SCL低电平超时事件用定时器3中断服务程序对SMBus复位（禁止后重新使能）
*SCL高电平（SMBus空闲）超时：如果一个器件保持SCL和SDA线为高电平的时间超过50微妙，则认为总线处于空闲状态。当SMBFTE位被置‘1’时，如果SCL和SDA保持高电平的时间超过10个
	SMBus时钟周期，总线将被视为空闲。				 
*每次数据字节或从地址传输都产生SMBus中断。						   
*当SYSCLK大于10MHZ时，通常需要扩展建立和保持时间。
*当ACKRQ置位时，软件应在清除SI之前向ACK位写入要发出的ACK值。
*当SMBus被使能丹SI标志被清为逻辑‘0’时软件不应访问SMB0DAT寄存器，因为硬件可能正在对该寄存器中的数据字节进行移入或移出操作。
 只要SI串行中断标志位（SMB0CN.0）为逻辑‘1’，该寄存器内的串行数据就是稳定的。
*SMB0DAT中的数据总是先移出MSB。
*************************************************************************************************************/
#define  WRITE 	0x00                 // SMBus 写命令
#define  READ 		0x01                 // SMBus 读命令

// #define  EEPROM_ADDR    0xA0                 // 铁电存储器的地址
// #define  SMB_BUFF_SIZE  0x08                 // 定义传输地址


#define  SMB_MTSTA      0xE0                 // 开始传输 1110 主发送	启动传输
#define  SMB_MTDB       0xC0                 // 数据传输 1100 主发送	数据字节发送
#define  SMB_MRDB       0x80                 // 数据接收 1000 主接收	数据字节接收


uint8* pSMB_DATA_IN;                 // 定义全局变量     所有接收到的数据写到这里
uint8 SMB_SINGLEBYTE_OUT;            // 定义握手信号	 单个字节写

uint8* pSMB_DATA_OUT;                // 数据存储变量	 所有要发送的数据从这里读取
// uint32 q;
uint8 SMB_DATA_LEN;                  // SMBUS传输数据长度	当前需要发送或接收的数据数量

uint8 WORD_ADDR;                     // 数据地址	   从设备内部地址

uint8 TARGET;                        //目标从地址	   从设备目标地址
// uint8 temp_char;
uint8 retval;                        //读回值缓存
bit SMB_BUSY = 0;                            // 定义SMBUS忙标志位	传输忙标志：正在写入或者读取的时候SMB_BUSY=1;

bit SMB_RW;                                  // 读写传输标志	当前总线传输方向选择标志

bit SMB_SENDWORDADDR;                        // 传输地址位	  此标志位置位，则允许在发送完从设备地址后，发送器件内部地址

bit SMB_RANDOMREAD;                          // 读操作，需要写地址，则设为1 随机读	 此标志位置位，则在发送完器件内部地址后，重新发送一个启动位

bit SMB_ACKPOLL;                             // 设置应答   此标志位置位，则一直发送重启信号，直到从设备发起一个确认其地址信号
bit FAIL=0;						 //传输失败的标志

// 16-bit SFR declarations sfr是定义8位的特殊功能寄存器，而sfr16则是用来定义16的特殊功能寄存器
sfr16    TMR3RL   = 0x92;                    // Timer3 reload registers
sfr16    TMR3     = 0x94;                    // Timer3 counter registers

void Timer0_Init (void);					 //定时器0初始化函数  时钟
void Timer3_Init (void);					 //定时器3初始化函数  低电平超时


/*
**********************************************************************************************************
** 函数名称：I2C_ByteWrite()
** 函数功能：向无子地址器件发送字节数据。
** 入口参数：target_addr		器件地址
**               dat		                要发送的数据
** 出口参数：
** 说明：
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

	STA = 1;   // 启动总线

	while (SMB_BUSY)
	{
		#ifdef WD_EN
			Feed_Wd();
		#endif
	}
}

/*********************************************************************************************************
** 函数名称：I2C_ByteRead()
** 函数功能：向无子地址器件读取字节数据。
** 入口参数：target_addr	器件地址
**
** 出口参数：读回的数据
** 说明：
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
	SMB_RANDOMREAD = 0;  // 写完地址后读数据
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
** 函数名称 I2C_WriteSubByte()
** 函数功能 ：从有子地址器件任意地址开始读取N字节数据
** 入口参数 ：		target_addr		器件从地址
**				suba_type	 子地址结构	1－单字节地址	3－8+X结构	2－双字节地址
**				suba		 器件子地址
**				src_addr			数据接收缓冲区指针
**				len			读取的个数
** 出口参数 ：
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

   STA = 1;   // 启动总线

   while(SMB_BUSY);
} 
/**********************************************************************************************************
** 函数名称 ：I2C_ReadSubByte()
** 函数功能 ：从有子地址器件任意地址开始读取N字节数据
** 入口参数 ：	target_addr			器件从地址
**				suba_type	子地址结构	1－单字节地址	2－8+X结构	2－双字节地址
**				suba		器件子地址
**				scr_addr			数据接收缓冲区指针
**				len			读取的个数
** 出口参数 ：
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
	   SMB_RANDOMREAD = 1;  // 写完地址后读数据
   }
   else
   {
	   SMB_SENDWORDADDR = 1;
	   SMB_RANDOMREAD = 1;  // 写完地址后读数据
   }

   SMB_ACKPOLL = 1;
   WORD_ADDR = suba;
    pSMB_DATA_IN = (uint8 *) scr_addr;

   SMB_DATA_LEN = len;
   STA = 1;
   while(SMB_BUSY);
} 

/*******************************************************************************
函数功能:SMBUS初始化
*******************************************************************************/
void I2c_Init (void)
{
	//XBR0    |= 0x04;//交叉开关连接SMBus
	SMB0CF = 0x5C;     // 0101 1100   定时器0溢出为时钟源  ，定时器3超时检测25ms
	SMB0CF |= 0x80;   //使能IIC、使能SMBus
	EIE1   |= 0x01;    // SMBus interrupt enable，SMBus中断允许位，允许SMB0的中断请求
	Timer0_Init();					 //定时器1初始化函数
	Timer3_Init();					 //定时器3初始化函数
}

/*******************************************************************************
函数功能:定时器3初始化 25ms//可以采用低8位溢出中断产生1秒中断
*******************************************************************************/
static void Timer3_Init (void)
{
	TMR3CN = 0x00;  // Timer3 configured for 16-bit
	// auto-reload,
	// low-byte interrupt disabled
	TMR3 = -(SYSCLK/12/40);                   // Timer3 configured to overflow after
	TMR3RL = -(SYSCLK/12/40);                 // ~25ms (for SMBus low timeout detect)

	CKCON &= ~0x40;                           // Timer3 uses SYSCLK/12	定时器3低字节使用TMR3CN中的T3XCLK位定义的时钟
	TMR3CN |= 0x04;                             // Start Timer3	   定时器3工作在双8位自动重装载定时器方式
	EIE1 |= 0x80;                             // Timer3 中断允许
}

/*******************************************************************************
函数功能:定时器0初始化，SCL
*******************************************************************************/
static void Timer0_Init (void)
{
	CKCON |= 0x01;							   // Timer0 clock source = SYSCLK / 4  使用分频时钟
	//CKCON &= ~0x07;                           // Timer0 clock source = SYSCLK / 12
	//注意UART0的时钟
	// CKCON |= 0x01;                           // Timer0 clock source = SYSCLK / 4
	TMOD |= 0x02;                              // Timer0 in 8-bit auto-reload mode	  自动重装载的8位计数器/定时器

	TH0 = -(SYSCLK/4/SMB_FREQUENCY/3);       // Timer0 configured to overflow at 1/3
	// the rate defined by SMB_FREQUENCY

	TL0 = -(SYSCLK/4/SMB_FREQUENCY/3);       // Timer0 preloaded to overflow at 1/3       SMB_FREQUENCY为SMB频率200000 为200KHZ
	// the rate defined by SMB_FREQUENCY
	TR0 = 1;                                  // Timer0 enabled
}

/*******************************************************************************
函数功能:	SMBUS处理函数,这部分函数按照SMBUS通讯协议进行判断处理.具体请参见F340数
		据手册SMBUS通讯部分.
*******************************************************************************/
void SMBus_ISR (void) interrupt INTERRUPT_SMBUS0			 //中断服务程序
{
	// Used by the ISR to flag failed
	// transfers
	
	static uint8 idata i;                     // Used by the ISR to count the	发送或接收数据数量
	// number of data bytes sent or
	// received
	static bit SEND_START = 0;  		   // Send a start 启动标志位

	switch (SMB0CN & 0xF0)                    // Status vector	状态变量
	{
		// Master Transmitter/Receiver: START condition transmitted.
	    //主发送或接收：启动条件已发送
	case SMB_MTSTA: //1110

		SMB0DAT = TARGET | SMB_RW;          // Load address of the target slave	  装载目标地址
		STA = 0;                            // Manually clear START bit	手动清除启动位
		i = 0;							   //重置数据位计数器
		_nop_();
		break;

		// Master Transmitter: Data byte (or Slave Address) transmitted
		//主发送：数据位或从地址已发送
	case SMB_MTDB: // 1100			
		if (ACK)                            // Slave Address or Data Byte  从地址或数据位应答？
		{                                   // Acknowledged?
		  
			if (SEND_START)     // 发送重复起始条件
			{
				STA = 1;
				SEND_START = 0;
				break;
			}
			if (SMB_SENDWORDADDR)            // Are we sending the word address?  是否发送器件内部地址？
			{
				SMB_SENDWORDADDR = 0;         // Clear flag		 清标志位
				SMB0DAT = WORD_ADDR;          // send word address	  发送低8位器件内部地址

				if (SMB_RANDOMREAD)			   //是否需要发送一个启动位
				{
					SEND_START = 1;            // send a START after the next ACK cycle
					SMB_RW = READ;
				}

				break;
			}

			if (SMB_RW==WRITE)               // Is this transfer a WRITE?
			{
				if (i < SMB_DATA_LEN)         // Is there data to send?	  数据发完？
				{
					SMB0DAT = *pSMB_DATA_OUT;  // send data byte		   发数据
					pSMB_DATA_OUT++;           // increment data out pointer	  发送的数据地址增加，连续写入数据
					i++;                       // increment number of bytes sent
				}
				else
				{
					STO = 1;                    // set STO to terminte transfer	   置位STO以结束此次传输
					SMB_BUSY = 0;               // clear software busy flag		清除忙标志位
				}
			}
			else {}                          // If this transfer is a READ,	 如果此次是个读操作表示不关心
			// then take no action. Slave
			// address was transmitted. A
			// separate 'case' is defined
			// for data byte recieved.
		}
		else                                // If slave NACK,
		{
			if (SMB_ACKPOLL)				 
			{
				STA = 1;                      // Restart transfer 重启发送
			}
			else
			{
				FAIL = 1;                     // Indicate failed transfer
			}                                // and handle at end of ISR
		}
		break;

		// Master Receiver: byte received	主接收：字节接收
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
		SMB_BUSY = 0;                          // Free SMBus  释放SMBus
	}

	SI=0;                                     // clear interrupt flag	清除中断标志位
}


/*******************************************************************************
函数功能:25MS后对SMBUS重新初始化
// A Timer3 interrupt indicates an SMBus SCL low timeout.
// The SMBus is disabled and re-enabled here
*******************************************************************************/
void Timer3_ISR (void) interrupt INTERRUPT_TIMER3
{ 
	SMB0CN &= ~0x40;                          // Disable SMBus	  禁止SMBus
	SMB0CN |= 0x40;                           // Re-enable SMBus  重启SMBus
	TMR3CN &= ~0x80;                          // Clear Timer3 interrupt-pending flag
}