#ifndef __MYLIB_H__
#define __MYLIB_H__


/************************常用函数的声明*************************/
#define BIT(n)		(1L << (n))


#define  DIR_IN   0xff
#define  DIR_OUT  0x00
#define  HIGH2LOW(Uc_Data)   (((Uc_Data&BIT(7))>>7)|((Uc_Data&BIT(6))>>5)|((Uc_Data&BIT(5))>>3)|((Uc_Data&BIT(4))>>1)|((Uc_Data&BIT(3))<<1)|((Uc_Data&BIT(2))<<3)|((Uc_Data&BIT(1))<<5)|((Uc_Data&BIT(0))<<7))
#define  DATA_X_MIRROR(Uc_Data)(((Uc_Data&BIT(0))>>0)|((Uc_Data&BIT(1))<<4)|((Uc_Data&BIT(2))<<2)|((Uc_Data&BIT(3))>>0)|((Uc_Data&BIT(4))>>2)|((Uc_Data&BIT(5))>>4)|((Uc_Data&BIT(6))<<0))


typedef struct aa
{
    INT8U a:1 ;
    INT8U b:1 ;
    INT8U c:1 ;
    INT8U d:1 ; 
    INT8U e:1 ;
    INT8U f:1 ;
    INT8U g:1 ;
    INT8U h:1 ;
}cc ;

extern cc Status0 , Status1  ;

#define System_Flag_2Ms	        Status0.a 
#define System_Flag_10Ms        Status0.b
#define System_Flag_100Ms       Status0.c
#define System_Flag_500Ms       Status0.d
#define RecvOver_Flag		    Status0.e
#define Resend_Flag             Status0.f //重发标志位
#define Resend_Flag_Enable      Status0.g //重发功能标志位
#define RecvError_Flag          Status0.h


#define Busy_Flag               Status1.a
#define RecvBusy_Flag           Status1.b
#define Resend_Enable_Flag      Status1.c
#define TrigEnable              Status1.d
#define MainEnable              Status1.e
#define PointFolNum_Flag        Status1.f
#define PlayAllFlag             Status1.g

extern INT8U bcd_number[5];

void Delay_Us(INT32U z);
void Delay_Ms(INT32U z);		//长时间延时, 测试用
void itoa1(INT8U i);
void itoa2(INT8U i);
void itoa3(INT16U i);
void itoa4(INT16U i);

void DS2MS_CNT(void)   ;
void DS10MS_CNT(void)  ;
void DS100MS_CNT(void) ;
void DS500MS_CNT(void)    ;
				  

#endif
