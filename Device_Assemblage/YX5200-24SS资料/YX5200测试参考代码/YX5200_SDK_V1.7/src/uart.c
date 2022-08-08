#include "config.h"

/******************************************************************************************************************

					+--------------------------------------------------+
					                   串口通讯模块
					+--------------------------------------------------+

 - 实现功能：

 - 目前进展：

 - 运行环境：STC   晶振：11.0592M   波特率:9600   [硬件串口操作]

 - 备注    ：在普中科技的51开发板上调试OK --- STC89C516RD+

******************************************************************************************************************/

static INT8U Send_buf[SENDLENTH] = {0} ;
static INT8U Recv_buf[SENDLENTH] = {0} ;

static INT8U SendDataLen = 0 ;
static INT8U ResendDataLen = 0 ;

code INT8U hex[]={"0123456789ABCDEF"};

void DoSum( INT8U *Str, INT8U len) ;
void Uart_Task(INT8U *pr);
/********************************************************************************************
 - 功能描述： 串口发送一个字节
 - 隶属模块： 外部
 - 参数说明：
 - 返回说明：
 - 注：	      
********************************************************************************************/
void Uart_PutByte(INT8U ch)
{
    SBUF  = ch;
    while(!TI){;}
    TI = 0;
}

/********************************************************************************************
 - 功能描述： 串口发送字符串
 - 隶属模块： 外部
 - 参数说明：
 - 返回说明：
 - 注：	      
********************************************************************************************/
#if FUNC_UARTDBG_EN
void Uart_PutString(INT8U* value)
{
	while((*value) != '\0')
	{
		Uart_PutByte(*value) ;
		value ++ ;
	}
}
#endif
/********************************************************************************************
 - 功能描述： 串口发送一个字节[调试用]
 - 隶属模块： 外部
 - 参数说明：
 - 返回说明：
 - 注：	      
*******************************************************************************************/
#if FUNC_UARTDBG_EN
void Uart_PutChar(INT8U dat)
{
	INT8U ch = 0 ;
	ch =(dat>>4) & 0x0f;
	Uart_PutByte(hex[ch]);
	ch = dat & 0x0f;
	Uart_PutByte(hex[ch]);
}
#endif

/*****************************************************************************************************
 - 功能描述： 串口发送数据
 - 隶属模块： 内部 
 - 参数说明： 
 - 返回说明： 
 - 注：无     
*****************************************************************************************************/
void SendCmd(INT8U len,bit flag,INT8U time)
{
    bit Status = 0 ;
    INT8U i = 0 ;

    Status   = ES;
    ES       = 0;
    TI       = 0;
    Uart_PutByte(0x7E); //起始
    for(i=0; i<len; i++)//数据
    {
		Uart_PutByte(Send_buf[i]) ;
    }
    Uart_PutByte(0xEF) ;//结束
    ES  = Status;
    if(flag)                  //设置重发标志  溢出时间
    {
        Resend_Flag  = 1; SendDataLen = len;  OutTimeCnt = time;
    }
    else
	{
        Resend_Flag  = 0; SendDataLen = 0; 	  ResendCount  = 1;
	}
}

/*******************************************************************************************
 - 功能描述：串口中断
 - 隶属模块：
 - 参数说明：
 - 返回说明：
 - 注：	    串口接收是采用状态机的方式
*******************************************************************************************/
void Serial_0_interrupt(void) interrupt   4
{
    
	INT8U UartTemp = 0 ;
	if(RI)
	{	
		RI = 0;
		UartTemp  = SBUF;
        if(Busy_Flag)
        {
            RecvBusy_Flag = 1 ;
        }
        switch(UartRecvStatus)
        {
            case UART_RECV_IDLE:
                                if(0x7E == UartTemp)
                                {
                                    UartRecv_100Ms = 10 ;
                                    UartRecvStatus = UART_RECV_VER ;
                                }
                                else
                                {
                                    UartRecvStatus = UART_RECV_IDLE ;
                                }
                                break ;
            case UART_RECV_VER:
                                Recv_buf[0] = UartTemp ;
                                UartRecvStatus = UART_RECV_LENTH ;
                                break ;
            case UART_RECV_LENTH:
                                Recv_buf[1] = UartTemp ;
                                UartRecvStatus = UART_RECV_CMD ;
                                break ;
            case UART_RECV_CMD:
                                Recv_buf[2] = UartTemp ;
                                UartRecvStatus = UART_RECV_FEEDBACK ;
                                break ;
            case UART_RECV_FEEDBACK:
                                Recv_buf[3] = UartTemp ;
                                UartRecvStatus = UART_RECV_DATAH ;
                                break ;
            case UART_RECV_DATAH:
                                Recv_buf[4] = UartTemp ;
                                UartRecvStatus = UART_RECV_DATAL ;
                                break ;
            case UART_RECV_DATAL:
                                Recv_buf[5] = UartTemp ;
                                UartRecvStatus = UART_RECV_CHECKSUMH ;
                                break ;
            case UART_RECV_CHECKSUMH:
                                Recv_buf[6] = UartTemp ;
                                UartRecvStatus = UART_RECV_CHECKSUML ;
                                break ;
            case UART_RECV_CHECKSUML:
                                Recv_buf[7] = UartTemp ;
                                UartRecvStatus = UART_RECV_OVER ;
                                break ;
            case UART_RECV_OVER :
                                if(0xEF == UartTemp)
                                {
                                    UartRecv_100Ms = 0 ;//清计时器
                                    RecvOver_Flag = 1 ;
                                    UartRecvStatus = UART_RECV_IDLE ;
                                }
                                else
                                {
                                    RecvError_Flag = 1 ;//错误处理
                                    UartRecvStatus = UART_RECV_IDLE ;
                                }
                                break ;
            default :
                                break ;
        }
	}
}

/********************************************************************************************
 - 功能描述： 重发处理函数
 - 隶属模块： 外部
 - 参数说明：
 - 返回说明：
 - 注：
********************************************************************************************/
void UartSend_Collide_Task(void)
{
    if(1 == Resend_Enable_Flag)
    {
        if(!Resend_Flag) return;	    //如果 Resend_Flag1 = 1 时间溢出还没有收到ACK 则重发

        ResendCount ++;

    	if(ResendCount>2)
        {
    	    ResendCount  = 1;
            Resend_Flag  = 0;
            return;
        }
    	SendCmd(ResendDataLen,1,25);    //重发数据
	}
}
/********************************************************************************************
 - 功能描述：争对接收的命令进行处理
 - 隶属模块：外部
 - 参数说明：无
 - 返回说明：无
 - 注：      
********************************************************************************************/
void Uart_communication(void)
{
	INT8U i, *pi ;

	INT16U  xorsum =0,xorsum1=0 ;

    if(1 == RecvError_Flag)//接收错误处理
    {
        RecvError_Flag = 0 ;
        UartRecvStatus = UART_RECV_IDLE ;
        Uart_SendCMD(0x40 , 0 , 1) ;//错误处理请求重发
        return ;
    }
    if(1 == RecvOver_Flag)//一帧数据接收完毕
    {
        RecvOver_Flag = 0 ;
    	pi = Recv_buf;
    	for(i=0; i<(*(pi+1)); i++)
    	{
    	    xorsum  = xorsum + pi[i] ;
    	}
    	xorsum1 = ((INT16U)((*(pi+i))<<8)) | (*(pi+i+1));
    	xorsum  = xorsum + xorsum1;

    	if(!xorsum)
    	{
    	    Uart_Task(pi);//串口处理
    	}
        else
        {
            RecvError_Flag = 1 ;//错误处理
        }
        UartRecvStatus = UART_RECV_IDLE ;
    }
}

/**********************************************************************************************
 - 功能描述： 串口处理函数
 - 隶属模块： 内部
 - 参数说明： *pr ：串口0需要接收的数据的地址指针
 - 返回说明： 无
 - 注：	
**********************************************************************************************/
void Uart_Task(INT8U *pr)
{
    INT16U Temp16 = 0 ;
    INT8U *pi ,tempH ,tempL , CMD , FeedBack;		
    pi = pr;

    tempH = *(pi+4) ;
    tempL = *(pi+5) ;
    Temp16 = (((INT16U)(tempH))<<8)|tempL ;
    CMD   = *(pi+2) ;
    FeedBack = *(pi+3) ;

    switch(CMD )
    {
        case (0x41)://收到应答ACK
                    UartRecvACK = 0 ;
                    PlayledCnt = PLAYLEDCNT ;
            		PlayledStatus = SET_PLAYLED_ON;
                    break ;
    /**********************************************************************
    - 1、设备插入拔出消息
    **********************************************************************/   
        case (0x3A)://设备插入
                    if(tempL == 0x01)
                    {
                        put_msg_lifo(MSG_UDISK_IN);
                    }
                    else if(tempL == 0x02)
                    {
                        put_msg_lifo(MSG_TF_IN);                    
                    }
                    else if(tempL == 0x04)
                    {
                        put_msg_lifo(MSG_PC_IN);
                    }
                    break;
        case (0x3B)://设备拔出
                    if(tempL == 0x01)
                    {
                        put_msg_lifo(MSG_UDISK_OUT);
                    }
                    else if(tempL == 0x02)
                    {
                        put_msg_lifo(MSG_TF_OUT);                    
                    }
                    else if(tempL == 0x04)
                    {
                        put_msg_lifo(MSG_PC_OUT);
                    }
                    break;
                    
    /**********************************************************************
    - 1、收到当前曲目播放完毕消息
    **********************************************************************/   
        case (0x3C)://U盘当前曲目播放完毕
                    if(PLAYDEVICE_UDISK == PlayDevice)
                    {
                        put_msg_lifo(MSG_MUSIC_OVER);
                    }
                    break;
        case (0x3D)://TF当前曲目播放完毕
                    if(PLAYDEVICE_TFCARD == PlayDevice)
                    {
                        put_msg_lifo(MSG_MUSIC_OVER);
                    }
                    break ;
        case (0x3E)://FLASH当前曲目播放完毕
                    if(PLAYDEVICE_FLASH == PlayDevice)
                    {
                        put_msg_lifo(MSG_MUSIC_OVER);
                    }
                    break ;
    /**********************************************************************
    - 1、模块上电返回的数据
    **********************************************************************/  
        case (0x3F):
                    OnlineDevice = tempL ;
                    put_msg_lifo(MSG_RETURN_MINIT);
                    break ;

    /**********************************************************************
    - 1、模块返回的错误
    **********************************************************************/  
        case (0x40):
                    ErrorStatus = tempL ;
                    put_msg_lifo(MSG_RECV_ERROR);
                    break ;
                    
    /**********************************************************************
    - 1、查询参数
    **********************************************************************/
        case (0x42)://查询当前状态
                  
                    break;
        case (0x43)://查询当前系统音量
                    input_vol = tempL ;
                    put_msg_lifo(MSG_RETURN_VOL);  
                    break;
        case (0x44)://查询当前EQ
                    CurrentEQ = tempL ;
                    put_msg_lifo(MSG_RETURN_EQ);  
                    break;
        case (0x45)://查询当前播放模式
                    PlayMode = tempL ;
                    put_msg_lifo(MSG_RETURN_PLAYMODE);  
                    break;
        case (0x46)://查询当前软件版本

                    break;
    /**********************************************************************
    - 1、查询参数 --- 设备总文件数
    **********************************************************************/
        case (0x47)://查询UDISK文件总数
                    UDiskTotal = Temp16 ;
                    put_msg_lifo(MSG_RETURN_NUMTOTAL); 
                    break ;
        case (0x48)://查询TFCARD文件总数
                    TFTotal = Temp16 ;
                    put_msg_lifo(MSG_RETURN_NUMTOTAL);
                    break ;
        case (0x49)://查询FLASH文件总数
                    FlashTotal = Temp16 ;
                    put_msg_lifo(MSG_RETURN_NUMTOTAL); 
                    break;

    /**********************************************************************
    - 1、查询参数 --- 设备当前播放的文件数
    **********************************************************************/   
        case (0x4B)://查询UDISK的当前曲目
                    UDiskCurFile = Temp16 ;
                    put_msg_lifo(MSG_RETURN_CURNUM);
                    break;
    
        case (0x4C)://查询TF卡的当前曲目
                    TFCurFile = Temp16 ;
                    put_msg_lifo(MSG_RETURN_CURNUM);
                    break ;
                  
        case (0x4D)://查询FLASH的当前曲目
                    FlashCurFile = Temp16 ;
                    put_msg_lifo(MSG_RETURN_CURNUM);
                    break;
        default:
                    break;
    }
}



/********************************************************************************************
 - 功能描述： 串口向外发送命令[包括控制和查询]
 - 隶属模块： 外部
 - 参数说明： CMD:表示控制指令，请查阅指令表，还包括查询的相关指令
              feedback:是否需要应答[0:不需要应答，1:需要应答]
              data:传送的参数
 - 返回说明：
 - 注：       
********************************************************************************************/
void Uart_SendCMD(INT8U CMD ,INT8U feedback , INT16U dat)
{
    Send_buf[0] = 0xff;    //保留字节 
    Send_buf[1] = 0x06;    //长度
    Send_buf[2] = CMD;     //控制指令
    Send_buf[3] = feedback;//是否需要反馈
    Send_buf[4] = (INT8U)(dat >> 8);//datah
    Send_buf[5] = (INT8U)(dat);     //datal
    DoSum(&Send_buf[0],6);        //校验
    SendCmd(8,1,30);       //发送此帧数据
    UartRecvACK = 3 ;//设定等待应答的时间[300ms]
}


/********************************************************************************************
 - 功能描述：求和校验
 - 隶属模块：
 - 参数说明：
 - 返回说明：
 - 注：      和校验的思路如下
             发送的指令，去掉起始和结束。将中间的6个字节进行累加，最后取反码
             接收端就将接收到的一帧数据，去掉起始和结束。将中间的数据累加，再加上接收到的校验
             字节。刚好为0.这样就代表接收到的数据完全正确。
********************************************************************************************/
void DoSum( INT8U *Str, INT8U len)
{
    INT16U xorsum = 0;
    INT8U i;

    for(i=0; i<len; i++)
    {
        xorsum  = xorsum + Str[i];
    }
	xorsum     = 0 -xorsum;
	*(Str+i)   = (INT8U)(xorsum >>8);
	*(Str+i+1) = (INT8U)(xorsum & 0x00ff);
}

































































