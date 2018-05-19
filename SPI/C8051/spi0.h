#ifndef  __SPI0_H
#define __SPI0_H

extern void SPI0_Init (void);

extern void SendSPIByte(uint8 ch); 

extern uint8 GetSPIByte(void);

#endif
