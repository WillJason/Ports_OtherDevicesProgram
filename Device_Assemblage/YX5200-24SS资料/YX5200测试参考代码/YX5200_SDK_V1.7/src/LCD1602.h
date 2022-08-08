#ifndef __LCD1602_H__
#define __LCD1602_H__



void LCD1602_Init() ;
void LCD1602_DispDat( INT8U x,  INT8U y, INT8U ch);
void LCD1602_DispStr( INT8U x, INT8U y, INT8U *str);
void LCD1602_ClearLine(INT8U x);
void LCD1602_Clear(void);
void ReturnLcdInit(void);
void Disp_PlayEQ(INT8U EQ);
void Disp_PlayMode(INT8U mode);
void Disp_PlayDevice(INT8U device);
void Disp_OnlineDevice(INT8U Online);
void GoInitDevice(INT8U Online) ;
void GotoNextDevice(INT8U device) ;
void ChangeDevice(INT8U dev);
#endif
