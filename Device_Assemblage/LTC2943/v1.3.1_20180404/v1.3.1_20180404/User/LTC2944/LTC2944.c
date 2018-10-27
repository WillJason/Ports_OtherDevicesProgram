#include "LTC2944.h"
#include "LTCIIC.h"
#include "stm32f10x.h"
#include "485.h"
#include "delay.h"
#include  "commu.h"



/*!
| Conversion Constants                              |  Value   |
| :------------------------------------------------ | :------: |
| LTC2944_CHARGE_lsb                                | 0.34  mAh|
| LTC2944_VOLTAGE_lsb                               | 1.068  mV|
| LTC2944_CURRENT_lsb                               |  29.3  uV|
| LTC2944_TEMPERATURE_lsb                           | 0.25    C|
| LTC2944_FULLSCALE_VOLTAGE                         |  23.6   V|
| LTC2944_FULLSCALE_CURRENT                         |  60    mV|
| LTC2944_FULLSCALE_TEMPERATURE                     | 510     K|

*/
const float LTC2944_CHARGE_lsb = 0.34E-3;
const float LTC2944_VOLTAGE_lsb = 1.068E-3;
const float LTC2944_CURRENT_lsb = 29.3E-6;
const float LTC2944_TEMPERATURE_lsb = 0.25;
const float LTC2944_FULLSCALE_VOLTAGE = 70;
const float LTC2944_FULLSCALE_CURRENT = 60E-3;
const float LTC2944_FULLSCALE_TEMPERATURE = 510;





void LTC2944_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD ;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	LIIC_SCL=1;
	LIIC_SDA=1;
}

/*********************************************************************************************************************/
//读
/*********************************************************************************************************************/

// Read a byte of data at register specified by "command"
u8 data=0;
int8_t i2c_read_byte_data(uint8_t address, uint8_t command)
{
  	LIIC_Start();                           //I2C START
                   
   LIIC_Send_Byte((address<<1)|I2C_WRITE_BIT);        // Write 7 bit address with W bit
//	 LIIC_Send_Byte(0xC8);    
	if (LIIC_Wait_Ack()!=0)                                
		goto cmd_fail;	    

   LIIC_Send_Byte(command);                 // Set register to be read to command
 	if (LIIC_Wait_Ack()!=0)                              
		goto cmd_fail;	  

	LIIC_Start();  
	delay_us(1);
   LIIC_Send_Byte((address<<1)|I2C_READ_BIT);     // Write 7 bit address with R bit
//	LIIC_Send_Byte(0xC9);   
	if (LIIC_Wait_Ack()!=0)                               
	  goto cmd_fail;	  
//	RS485_SendString("11111\n"); 	
	        
	data = LIIC_Read_Byte(WITH_NACK);     // Read byte from buffer with NAK
//	RS485_SendString("33333\n"); 	
    LIIC_Stop();                            // I2C STOP

	return 1;   /* 执行成功 */
	
	cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	LIIC_Stop();
	return 0;

}

// Read a 16-bit word of data from register specified by "command" 
//extern u16 data16;
u8  LTC2944_value[7]={0x01,0x02,0x02,0xFF,0xFF,0x00,0x00};
//u16 volot =99;
u32 crc;
u16 LTC_CHARGE ,LTC_CHARGE_L;

int8_t i2c_read_word_data(uint8_t address, uint8_t command)
{

  	LIIC_Start();                                     //I2C START
	
   LIIC_Send_Byte((address<<1)|I2C_WRITE_BIT);        // Write 7 bit address with W bit

   if (LIIC_Wait_Ack()!=0)                               
		goto cmd_fail;	   
		
//	Usart_SendString(USART1,"----->1\n");
	
   LIIC_Send_Byte(command);                 // Set register to be read to command
   if (LIIC_Wait_Ack()!=0)                               
		goto cmd_fail;	 
	
	LIIC_Start();  
	delay_us(1);
   LIIC_Send_Byte((address<<1)|I2C_READ_BIT);     // Write 7 bit address with R bit
	if (LIIC_Wait_Ack()!=0)                               
		goto cmd_fail;	
	
   LTC_CHARGE  =  LIIC_Read_Byte(WITH_ACK);                        // Read MSB from buffer
	LTC_CHARGE_L=  LIIC_Read_Byte(WITH_NACK);                  // Read LSB from buffer
//	volot=(LTC_CHARGE <<8) |LTC_CHARGE_L;

   LIIC_Stop();                        			 //I2C STOP
	
	return 1;   									   	/* 执行成功 */
	
 	cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	LIIC_Stop();
	return 0;                                 
}

// Read a block of data, starting at register specified by "command" and ending at (command + length - 1)
int8_t i2c_read_block_data(uint8_t address, uint8_t command, uint8_t length, uint8_t *values)
{
  uint8_t i = (length-1);


  	LIIC_Start();                                   //Stop and return 0 if START fail
	LIIC_Send_Byte((address<<1)|I2C_WRITE_BIT);       //Write 7-bit address with W bit
	if (LIIC_Wait_Ack()!=0)                               
		goto cmd_fail;	
  LIIC_Send_Byte(command);                           //Write 8 bit command word
	if (LIIC_Wait_Ack()!=0)                               
		goto cmd_fail;	 
	
	LIIC_Start();  
	delay_us(1);
   
	LIIC_Send_Byte((address<<1)|I2C_READ_BIT);        //Write 7-bit address with R bit
	if (LIIC_Wait_Ack()!=0)                               
		goto cmd_fail;	 

  while (i>0)                         //Begin read loop
  {
    values[i] = LIIC_Read_Byte(WITH_ACK); //Read from bus with ACK
    i--;
  }

  values[0] = LIIC_Read_Byte(WITH_NACK);    //Read from bus with NACK for the last one;

  LIIC_Stop();                         //I2C STOP


  return(1);                           // Success!
  
   cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	LIIC_Stop();
	return 0;  
}

// Reads an 8-bit adc_code from LTC2943
int8_t LTC2944_read(uint8_t i2c_address, uint8_t adc_command)
// The function returns the state of the acknowledge bit after the I2C address write. 0=acknowledge, 1=no acknowledge.
{
  int8_t ack;

  ack = i2c_read_byte_data(i2c_address, adc_command);

  return(ack);
}


// Reads a 16-bit adc_code from LTC2943
int8_t LTC2944_read_16_bits(uint8_t i2c_address, uint8_t adc_command)
// The function returns the state of the acknowledge bit after the I2C address write. 0=acknowledge, 1=no acknowledge.
{
  int8_t ack;

  ack = i2c_read_word_data(i2c_address, adc_command);
	
	LTC2944_value[3] =LTC_CHARGE;
	LTC2944_value[4] =LTC_CHARGE_L;
   
	crc = N_CRC16(LTC2944_value,5);      
	LTC2944_value[5]= (u8)crc;  					 //CRC_L
	LTC2944_value[6]= (u8)(crc >> 8) ;        //CRC_H 	

  return(ack);
}


/*********************************************************************************************************************/
//写
/*********************************************************************************************************************/
// Write a byte of data to register specified by "command"
int8_t i2c_write_byte_data(uint8_t address, uint8_t command, uint8_t value)
{
    LIIC_Start()   ;                            //I2C START
                     
    LIIC_Send_Byte((address<<1)|I2C_WRITE_BIT);        // Write 7 bit address with W bit
	
	 if (LIIC_Wait_Ack()!=0)                               
		goto cmd_fail;	
	 
   LIIC_Send_Byte(command);                 // Set register to be read to command
	 
	 if (LIIC_Wait_Ack()!=0)                               
		goto cmd_fail;	 
	 
  LIIC_Send_Byte(value);
  LIIC_Stop();                        // I2C STOP
	 
  return(1);                     // Return success
  
  cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	LIIC_Stop();
	return 0;     
}

// Write a 16-bit word of data to register specified by "command"
int8_t i2c_write_word_data(uint8_t address, uint8_t command, uint16_t value)
{

  union
  {
    uint8_t b[2];
    uint16_t w;
  } data;
  
  
  data.w = value;

   LIIC_Start()   ;                                    //Stop and return 0 if START fail
   LIIC_Send_Byte((address<<1)|I2C_WRITE_BIT);       // Write 7 bit address with W bit
   if (LIIC_Wait_Ack()!=0)                               
		goto cmd_fail;	 
	
   LIIC_Send_Byte(command);                 // Set register to be read to command
   if (LIIC_Wait_Ack()!=0)                               
		goto cmd_fail;	 
	
	 LIIC_Send_Byte(data.b[1]);         //Write MSB
	   if (LIIC_Wait_Ack()!=0)                               
		goto cmd_fail;	 
    LIIC_Send_Byte(data.b[0]);         //Write LSB;
		   if (LIIC_Wait_Ack()!=0)                               
		goto cmd_fail;	 

     LIIC_Stop();                        // I2C STOP
              
     return(1);

    cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	LIIC_Stop();
	return 0;   


}
// Write a block of data, starting at register specified by "command" and ending at (command + length - 1)
int8_t i2c_write_block_data(uint8_t address, uint8_t command, uint8_t length, uint8_t *values)
{
  int8_t i = length-1;

   LIIC_Start()   ;                                    //Stop and return 0 if START fail
   LIIC_Send_Byte((address<<1)|I2C_WRITE_BIT);        // Write 7 bit address with W bit
  if (LIIC_Wait_Ack()!=0)                               
		goto cmd_fail;	 
	
  LIIC_Send_Byte(command);           // Set register to be read to command
	if (LIIC_Wait_Ack()!=0)                               
		goto cmd_fail;	 
  while (i>=0)
  {
    LIIC_Send_Byte(values[i]);     //Write Value
    i--;
	 if (LIIC_Wait_Ack()!=0)                               
	 goto cmd_fail;	 
  }
  
  LIIC_Stop();                        // I2C STOP
  return(1);
  
   cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	LIIC_Stop();
	return 0;   
}

// Write an 8-bit code to the LTC2944.
int8_t LTC2944_write(uint8_t i2c_address, uint8_t adc_command, uint8_t code)
// The function returns the state of the acknowledge bit after the I2C address write. 0=acknowledge, 1=no acknowledge.
{
  int32_t ack;

  ack = i2c_write_byte_data(i2c_address, adc_command, code);
  return(ack);
}
// Write a 16-bit code to the LTC2944.
int8_t LTC2944_write_16_bits(uint8_t i2c_address, uint8_t adc_command, uint16_t code)
// The function returns the state of the acknowledge bit after the I2C address write. 0=acknowledge, 1=no acknowledge.
{
  int8_t ack;

  ack = i2c_write_word_data(i2c_address, adc_command, code);
  return(ack);
}

u8 WakeUp_AnalogPart(void)
{

	int32_t ack;

  ack = i2c_write_byte_data(LTC2944_I2C_ADDRESS, LTC2944_CONTROL_REG, 0xFC);  //3C->FC 睡眠模式---> 自动模式  30ms 4.5ms
  return(ack);

}


// The function converts the 16-bit RAW adc_code to Volts
float LTC2944_code_to_voltage(uint16_t adc_code)
{
  float voltage;
  voltage = ((float)adc_code/(65535))*LTC2944_FULLSCALE_VOLTAGE;
  return(voltage);
}

