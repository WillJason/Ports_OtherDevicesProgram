#ifndef    _LTCIIC_H
#define	  _LTCIIC_H

#include "sys.h"
#include "stm32f10x.h"

//IO方向             8 上拉/下拉输入 4 浮空输入
#define LSDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)4<<28;}  //   8 上拉/下拉输入     |  4 浮空输入
#define LSDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)7<<28;}  //   3通用推挽输出50MHZ  | 6开漏2MHZ|   7 开漏输出50MHZ

#define EEPROM_GPIO_PORT_I2C	GPIOB		/* GPIO端口 */
#define EEPROM_RCC_I2C_PORT 	RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define EEPROM_I2C_SCL_PIN		GPIO_Pin_6			/* 连接到SCL时钟线的GPIO */
#define EEPROM_I2C_SDA_PIN		GPIO_Pin_7			/* 连接到SDA数据线的GPIO */

#define EEPROM_I2C_SCL_1()  EEPROM_GPIO_PORT_I2C->BSRR = EEPROM_I2C_SCL_PIN				/* SCL = 1 */
#define EEPROM_I2C_SCL_0()  EEPROM_GPIO_PORT_I2C->BRR = EEPROM_I2C_SCL_PIN				/* SCL = 0 */
	
#define EEPROM_I2C_SDA_1()  EEPROM_GPIO_PORT_I2C->BSRR = EEPROM_I2C_SDA_PIN				/* SDA = 1 */
#define EEPROM_I2C_SDA_0()  EEPROM_GPIO_PORT_I2C->BRR = EEPROM_I2C_SDA_PIN				/* SDA = 0 */
	

//IO操作函数	 
#define LIIC_SCL      PBout(6) //SCL
#define LIIC_SDA      PBout(7) //SDA	 
#define LREAD_SDA     PBin(7)  //输入SDA 

#define LTC2944_I2C_SDA_READ()  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)	/* 读SDA口线状态 */

void LIIC_Start(void);
void LIIC_Stop(void);
u8   LIIC_Wait_Ack(void);
void LIIC_Ack(void);
void LIIC_NAck(void);
void LIIC_Send_Byte(u8 txd);
u8   LIIC_Read_Byte(unsigned char ack);

#endif
