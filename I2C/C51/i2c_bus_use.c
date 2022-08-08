/*
I2C Agreement

Introduction:
Two way serial line, one data line SDA, one clock line SCL.
SDA transmission data is big end transmission, 8 bits per transmission, i.e. one byte.
Multiple masters are supported. There can only be one master at any time point.
Each device on the bus has its own addr, 7 bits in total, and the broadcast address is all 0
There may be more than one chip of the same kind in the system, so addr is 
divided into fixed part and programmable part. The details depend on the chip. See datasheet.

Start SIGNAL:When SCL is high, SDA will jump from high level to low level. 
End SIGNAL: when SCL is high level, SDA jumps from low level to high level.
ACK SIGNAL:
	All data on I2C bus is transmitted in 8-bit bytes. Each byte sent by 
	transmitter releases data line during clock pulse 9, and receiver feeds 
	back a response signal. When the response signal is low level, it is 
	specified as the effective response bit (ACK for short) to indicate 
	that the receiver has successfully received the byte; when the response
	 signal is high level, it is specified as the non response bit (NACK),
	which generally indicates that the receiver has not successfully received
	the byte. The requirement for the feedback effective response bit ack is 
	that the receiver is in the low level period before the 9th clock pulse
	Pull down the SDA line and ensure that it is a stable low level during 
	the high level of the clock. If the receiver is the main controller, 
	after it receives the last byte, it sends a NACK signal to inform the 
	controlled transmitter to end data transmission and release the SDA line 
	so that the main control receiver sends a stop signal P.
ACK time sequence:
	SDA is the high level wait for receiving response bit, SCL is the high 
	level wait for 4us, SDA pulled low represents ack response bit, if SDA 
	stays high, it represents NACK non response bit.
Data bit Send:
	Each bit of data transmitted on I2C bus has a clock pulse corresponding 
	(or synchronous control), that is to say, with the cooperation of SCL 
	serial clock, each bit of data is serially transmitted on SDA bit by bit. 
	During data transmission, during the high level of SCL, the level on SDA 
	must be stable, the low level is data 0, and the high level is data 1. 
	The level change state on the SDA is only allowed when the SCL is low. 
	The level of logic 0 is low, while that of logic 1 is high.
I2C write process:
	1. Master initiates start(Note that after I2C is turned on, there is no need
	  to wait for the response signal. If the wait for the response signal is added, 
	  the data may not be normal)
	2. Master sends I2C addr (7bit) and W operation 0 (1bit) and waits for ACK
	3. devve sends ACK
	4. Master sends reg addr (8 bit) to wait for ACK
	5. devve sends ACK
	6. Master sends data (8bit), that is, to write the data in the register and wait for ACK
	7. devve sends ACK
	8. Steps 6 and 7 can be repeated many times, that is, write multiple registers in sequence
	9. Master initiates stop
I2C reading process:
	1.Master initiates start
	2.Master sends I2C addr (7bit) and W operation 1 (1bit) and waits for ACK
	3.devve send ACK
	4.Master sends reg addr (8bit) and waits for ACK
	5.devve send ACK
	6.Master initiates start(Note that I2C must be reopened here)
	7.Master sends I2C addr (7bit) and R operation 1 (1bit), waiting for ACK
	8.devve send ACK
	9.devve sends data (8bit), which is the value in the register
	10.Master sends ACK(If all reception is completed (including received a byte), send NACK)
	11.Steps 8 and 9 can be repeated many times, i.e. reading multiple registers in sequence

NOTE:
	After each read operation or write operation function, a delay of several milliseconds 
	is required, otherwise data reading error will be caused	
*/

/*************************This part is the driver of I2C bus*************************************/
#include<reg52.h>
#include <intrins.h>

#define  NOP()  _nop_()   /*Null instruction*/

sbit     SCL=P2^0;     //I2C  
sbit     SDA=P2^1;     //I2C  

bit ack;                 /*Response flag bit*/
/*******************************************************************
Fuction:  The bus Start   
Description:  Start I2C bus, i.e. send I2C starting condition   . 
Input: void           
Output:void
Return:void
Others: 
********************************************************************/
void Start_I2c()
{
  SDA=1;     /*Send data signal of starting condition*/
  NOP();
  SCL=1;
  NOP();    /*Starting condition establishment time is greater than 4.7us, delay*/
  NOP();
  NOP();
  NOP();
  NOP();    
  SDA=0;     /*Send start signal*/
  NOP();    /*Starting condition locking time greater than 4 ¦Ìs */
  NOP();
  NOP();
  NOP();
  NOP();       
  SCL=0;     /* Clamp I2C bus and prepare to send or receive data*/
  NOP();
  NOP();
}

/*******************************************************************
Fuction:   The bus End              
Description:  End I2C bus, i.e. send I2C end condition
Input: void           
Output:void
Return:void
Others:     
********************************************************************/
void Stop_I2c()
{
  SDA=0;      /*Send data signal of end condition*/
  NOP();     /*Send clock signal of end condition*/
  SCL=1;      /*The establishment time of end condition is more than 4¦Ìs*/
  NOP();
  NOP();
  NOP();
  NOP();
  NOP();
  SDA=1;      /*Send I2C bus end signal*/
  NOP();
  NOP();
  NOP();
  NOP();
}

/*******************************************************************
Fuction:   Byte data transmission             
Description:  Send out data C, which can be address or data, and wait 
					for response after sending, and operate on this status bit. 
					(ACK = 0 for non response or non response). Send data normally, 
					ACK = 1; ack = 0 means the controlled device has no response or damage.
Input:  c : send byte;          
Output:void
Return:void
Others:    
********************************************************************/
void  SendByte(unsigned char  c)
{
 unsigned char  BitCnt;
 
 for(BitCnt=0;BitCnt<8;BitCnt++)  /*The length of data to be transmitted is 8 bits*/
    {
     if((c<<BitCnt)&0x80)SDA=1;   /*Judge sending bit*/
       else  SDA=0;                
     NOP();
     SCL=1;      /*Set the clock line to high, and inform the controlled device to start receiving data bits*/
      NOP(); 
      NOP();    /*Ensure that the high level cycle of clock is greater than 4¦Ìs*/
      NOP();
      NOP();
      NOP();         
     SCL=0; 
    }
    
    NOP();
    NOP();
    SDA=1;       /*Release the data line after 8-bit transmission and prepare to receive the response bit*/
    NOP();
    NOP();   
    SCL=1;
    NOP();
    NOP();
    NOP();
    if(SDA==1)ack=0;     
       else ack=1; /*Judge whether the response signal is received*/
    SCL=0;
    NOP();
    NOP();
}

/******************************************************************* 
Fuction:   Byte data receiving             
Description:  It is used to receive the data from the device and judge 
					the bus error (no response signal is sent). After sending, 
					please use the response function to answer the devve.
Input: void         
Output:void
Return:Receive byte
Others:    
********************************************************************/    
unsigned char   RcvByte()
{
  unsigned char  retc;
  unsigned char  BitCnt;
  
  retc=0; 
  SDA=1;               /*Set data line as input mode*/
  for(BitCnt=0;BitCnt<8;BitCnt++)
      {
        NOP();           
        SCL=0;         /*Set clock line to low, ready to receive data bits*/
        NOP();
        NOP();        /*Clock low level period is greater than 4.7¦Ìs*/
        NOP();
        NOP();
        NOP();
        SCL=1;         /*Set clock line high to make data on data line effective*/
        NOP();
        NOP();
        retc=retc<<1;
        if(SDA==1)retc=retc+1;  /* Read the data bit and put the received data bit into RETC*/
        NOP();
        NOP(); 
      }
  SCL=0;    
  NOP();
  NOP();
  return(retc);
}

/********************************************************************
Fuction:   Reply ACK            
Description:  The master controller carries out response signal 
							(it can be response or non response signal, which is 
							determined by bit parameter a)
Input:  a : 0 - ACK,1 - NACK;         
Output:void
Return:void
Others:    
********************************************************************/
void Ack_I2c(bit a)
{
  
  if(a==0)SDA=0;    /*Send reply or non reply signal here */
  else SDA=1;
  NOP();
  NOP();
  NOP();      
  SCL=1;
  NOP();
  NOP();           /*Clock low level period is greater than 4 ¦Ì s*/
  NOP();
  NOP();
  NOP();  
  SCL=0;           /*Clear the clock line and clamp I2C bus to continue receiving*/
  NOP();
  NOP();    
}


/*******************************************************************
Fuction:   Send multibyte data to device with register address (internal address)           
Description:  The whole process from starting the bus to sending address, 
						internal address, data and ending the bus
Input:  dev : Device address
				reg:	register address (internal address)
				s:	Send data
				Count:   Count of bytes to send     
Output:void
Return:1 means the operation is successful, otherwise the operation is wrong
Others:  The bus must have ended before use.  
********************************************************************/
bit ISendStr(unsigned char  dev,unsigned char  reg,unsigned char  *s,unsigned char  Count)
{
   unsigned char i;

   Start_I2c();               /*Start up bus*/
   SendByte(dev);             /*send Device address*/
   if(ack==0)return(0);
   SendByte(reg);            /*send reg address or internal address*/
   if(ack==0)return(0);

   for(i=0;i<Count;i++)
   {   
     SendByte(*s);            /*send data*/
     if(ack==0)return(0);
     s++;
   } 
   Stop_I2c();                /*end of bus*/ 

   return(1);
}

/*******************************************************************
Fuction:   Receive multibyte data to device with register address (internal address)           
Description:  From starting the bus to sending address, internal address,
					 reading data, and ending the whole process of the bus
Input:  dev : Device address
				reg:	register address (internal address)
				Count:   Count of bytes to receive    
Output:   s : Put the read-out content into the storage area pointed to by S
Return:1 means the operation is successful, otherwise the operation is wrong
Others:  The bus must have ended before use.  
********************************************************************/
bit IRcvStr(unsigned char  dev,unsigned char  reg,unsigned char  *s,unsigned char  Count)
{
   unsigned char i;

   Start_I2c();                  /*Start up bus*/
   SendByte(dev);                /*send Device address*/
   if(ack==0)return(0);
   SendByte(reg);               /*send reg address or internal address*/
   if(ack==0)return(0);

   Start_I2c();                 /*Restart up bus*/
   SendByte(dev+1);
   if(ack==0)return(0);
   for(i=0;i<Count-1;i++)
   {   
     *s=RcvByte();               /*Receive data*/
      Ack_I2c(0);                /*Send ACK*/  
     s++;
   } 
   *s=RcvByte();
   Ack_I2c(1);                   /*Send NACK,All received*/
   Stop_I2c();                   /*End of bus*/ 
   return(1);
}












































