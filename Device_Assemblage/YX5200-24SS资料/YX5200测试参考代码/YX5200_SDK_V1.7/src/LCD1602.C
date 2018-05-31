#include <intrins.h>
#include "config.h"

/******************************************************************************************************************
        
						+--------------------------------------------------+
						                  LCD1602底层驱动
						+--------------------------------------------------+

 - 实现功能：

 - 目前进展：

 - 备注    ：使用的液晶为带字库型的液晶
******************************************************************************************************************/
void ReturnLcdInit(void);
void LCD1602_DispDat( INT8U x,  INT8U y, INT8U ch);
void LCD1602_DispStr( INT8U x, INT8U y, INT8U *str);
/********************************************************************************************
 - 功能描述： 检测忙
 - 隶属模块： 内部
 - 参数说明：
 - 返回说明：
 - 注：	     
*******************************************************************************************/
BOOL LCD1602_bz()
{							// 测试LCD忙碌状态
	BOOL result;
	rs = 0;
	rw = 1;
	ep = 1;
    Delay_Us(5) ;
	result = (BOOL)(LCDDATAPORT & 0x80);
	ep = 0;
	return result;	
}
/********************************************************************************************
 - 功能描述： 写命令
 - 隶属模块： 内部
 - 参数说明：
 - 返回说明：
 - 注：	     
*******************************************************************************************/
void LCD1602_wcmd(INT8U cmd)
{							// 写入指令数据到LCD
	while(LCD1602_bz());
	rs = 0;
	rw = 0;
	ep = 0;
    Delay_Us(5) ;	
	LCDDATAPORT = cmd;
    Delay_Us(5) ;
	ep = 1;
    Delay_Us(5) ;
	ep = 0;		
}
/********************************************************************************************
 - 功能描述： 写数据
 - 隶属模块： 内部
 - 参数说明：
 - 返回说明：
 - 注：	     
*******************************************************************************************/
void LCD1602_wdat(INT8U dat)	
{							//写入字符显示数据到LCD
	while(LCD1602_bz());
	rs = 1;
	rw = 0;
	ep = 0;
	LCDDATAPORT = dat;
    Delay_Us(5) ;
	ep = 1;
    Delay_Us(5) ;
	ep = 0;	
}

/********************************************************************************************
 - 功能描述： 指定位置[指定的列的地址]
 - 隶属模块： 内部
 - 参数说明：
 - 返回说明：
 - 注：	     
*******************************************************************************************/
void LCD1602_Pos(INT8U pos)
{							//设定显示位置
	LCD1602_wcmd(pos | 0x80);
}


/********************************************************************************************
 - 功能描述： 清屏
 - 隶属模块： 内部
 - 参数说明：
 - 返回说明：
 - 注：	     
*******************************************************************************************/
void LCD1602_Clear(void)
{
    LCD1602_wcmd(0x01);
    Delay_Ms(1);  
}

/********************************************************************************************
 - 功能描述： 清除指定的行
 - 隶属模块： 内部
 - 参数说明：
 - 返回说明：
 - 注：	     
*******************************************************************************************/
void LCD1602_ClearLine(INT8U x)
{
    LCD1602_DispStr(x, 0, "                ") ;   
}

/********************************************************************************************
 - 功能描述： 指定位置显示字符
 - 隶属模块： 内部
 - 参数说明： x:代表显示的行[0:第一行][1:第二行]
              y:代表显示的列[0~15]
 
 - 返回说明：
 - 注：	     
*******************************************************************************************/
void LCD1602_DispChar( INT8U x,  INT8U y, INT8U ch)
{
      if(x != 0)
      {
            LCD1602_Pos(y | 0x40); 
      }
      else
      {
            LCD1602_Pos(y);
      }
      LCD1602_wdat(ch); 
}

/********************************************************************************************
 - 功能描述： 指定位置显示字符
 - 隶属模块： 内部
 - 参数说明： x:代表显示的行[0:第一行][1:第二行]
              y:代表显示的列[0~15]
 
 - 返回说明：
 - 注：	     
*******************************************************************************************/
void LCD1602_DispDat( INT8U x,  INT8U y, INT8U ch)
{
      if(x != 0)
      {
            LCD1602_Pos(y | 0x40); 
      }
      else
      {
            LCD1602_Pos(y);
      }
      LCD1602_wdat(ch + 0x30); 
}

/********************************************************************************************
 - 功能描述： 指定位置显示字符串
 - 隶属模块： 内部
 - 参数说明： x:代表显示的行[0:第一行][1:第二行]
              y:代表显示的列[0~15]
 - 返回说明：
 - 注：	     
*******************************************************************************************/
void LCD1602_DispStr( INT8U x, INT8U y, INT8U *str)
{
    if(x > 1 || y > 15)
    {
          return ;
    } 
    for( ; *str != 0; )
    {
        LCD1602_DispChar( x, y,*str);
        str++;
        y++;
    }
}
/********************************************************************************************
 - 功能描述： 显示当前的播放模式
 - 隶属模块： 内部
 - 参数说明： 
 - 返回说明：
 - 注：	     
*******************************************************************************************/
static void DispDevice(INT8U device)
{
    switch(device)
    {

        case PLAYDEVICE_UDISK :
                            LCD1602_DispStr(0 , 9  , "U");     
                            break ;
                
        case PLAYDEVICE_TFCARD :
                            LCD1602_DispStr(0 , 9  , "T");  
                            break ;
                            
        case PLAYDEVICE_PC:
                            LCD1602_DispStr(0 , 9  , "P");  
                            break ;   
                            
        case PLAYDEVICE_FLASH:
                            LCD1602_DispStr(0 , 9  , "F");  
                            break ;

        case PLAYDEVICE_SLEEP:  
                            LCD1602_DispStr(0 , 9  , "S");  
                            break ;
                
        default:
                            LCD1602_DispStr(0 , 9  , "N"); 
                            break ;
    }
}

/********************************************************************************************
 - 功能描述： 返回液晶显示界面
 - 隶属模块： 内部
 - 参数说明： 
 - 返回说明：
 - 注：	     
*******************************************************************************************/
void ReturnLcdInit(void)
{
    DispDevice(PlayDevice) ;
    if(FEEDBACK)
    {
        LCD1602_DispStr(0 , 13  , "ACK");
    }
    else
    {
        LCD1602_DispStr(0 , 12  , "NACK");   
    } 
#if 0
    if(TrigEnable)
    {
        LCD1602_DispStr(1 , 5  , "TRIG");//触发模式          
    }
    else
    {
        LCD1602_DispStr(1 , 5  , "CONT");//连续模式    
    } 
#endif
    if(PointFunc == POINTNUM_ONE)
    {
        LCD1602_DispStr(1 , 5  , "ONE ");
    }
    else if(PointFunc == POINTNUM_LOOP)
    {
        LCD1602_DispStr(1 , 5  , "LOOP");   
    }
    else if(PointFunc == POINTNUM_FOLDER)
    {
        LCD1602_DispStr(1 , 5  , "FOLD");    
    }
    if(PointFolNum_Flag)
    {
        LCD1602_DispStr(0 , 7  , "F");//指定文件夹
    }
    else
    {
        LCD1602_DispStr(0 , 7  , "N");//正常指定曲目        
    }
}

/********************************************************************************************
 - 功能描述： 显示播放到的设备
 - 隶属模块： 内部
 - 参数说明： 
 - 返回说明：
 - 注：	     
*******************************************************************************************/
void Disp_PlayDevice(INT8U device)
{
    if(device == PLAYDEVICE_UDISK)
    {
        LCD1602_DispStr(1 , 0  , "PLAYD:UDISK");
    }
    else if(device == PLAYDEVICE_TFCARD)
    {
        LCD1602_DispStr(1 , 0  , "PLAYD:TFCARD");
    }
    else if(device == PLAYDEVICE_PC)
    {
        LCD1602_DispStr(1 , 0  , "PLAYD:PC");
    }
    else if(device == PLAYDEVICE_FLASH)
    {
        LCD1602_DispStr(1 , 0  , "PLAYD:FLASH");
    }
    else if(device == PLAYDEVICE_SLEEP)
    {
        LCD1602_DispStr(1 , 0  , "PLAYD:SLEEP");
    }    
}

/********************************************************************************************
 - 功能描述： 显示播放到的模式
 - 隶属模块： 内部
 - 参数说明： 
 - 返回说明：
 - 注：	     
*******************************************************************************************/
void Disp_PlayMode(INT8U mode)
{
    if(mode == REPEAT_ALL)
    {
        LCD1602_DispStr(1 , 0  , "PLAYM:ALL_0");
    }
    else if(mode == REPEAT_FOLDER)
    {
        LCD1602_DispStr(1 , 0  , "PLAYM:FOLDER_1");
    }  
    else if(mode == REPEAT_ONE)
    {
        LCD1602_DispStr(1 , 0  , "PLAYM:ONE_2");
    }
    else if(mode == REPEAT_RANDOM)
    {
        LCD1602_DispStr(1 , 0  , "PLAYM:RANDOM_3");
    }    
}

/********************************************************************************************
 - 功能描述： 显示播放到的EQ
 - 隶属模块： 内部
 - 参数说明： 
 - 返回说明：
 - 注：	     
*******************************************************************************************/
void Disp_PlayEQ(INT8U EQ)
{
    if(EQ == EQ_NORMAL)
    {
        LCD1602_DispStr(1 , 0  , "EQ:NORMAL_0");
    }
    else if(EQ == EQ_POP)
    {
        LCD1602_DispStr(1 , 0  , "EQ:POP_1");
    }
    else if(EQ == EQ_ROCK)
    {
        LCD1602_DispStr(1 , 0  , "EQ:ROCK_2");
    }
    else if(EQ == EQ_JAZZ)
    {
        LCD1602_DispStr(1 , 0  , "EQ:JAZZ_3");
    }
    else if(EQ == EQ_CLASSIC)
    {
        LCD1602_DispStr(1 , 0  , "EQ:CLASSIC_4");
    }
    else if(EQ == EQ_BASS)
    {
        LCD1602_DispStr(1 , 0  , "EQ:BASS_5");
    }    
}

/********************************************************************************************
 - 功能描述： 显示模块返回的在线设备
 - 隶属模块： 内部
 - 参数说明： 
 - 返回说明：
 - 注：	     
*******************************************************************************************/
void Disp_OnlineDevice(INT8U Online)
{
    INT8U temp ;
    temp = Online & 0x01 ;
    if(temp)//UDISK
    {
        LCD1602_DispStr(1 , 12  , "U");
    }
    temp = Online & 0x02 ;
    if(temp)//TF
    {
        LCD1602_DispStr(1 , 13  , "T");
    }
    temp = Online & 0x04 ;
    if(temp)//PC
    {
        LCD1602_DispStr(1 , 14  , "P");
    }   
    temp = Online & 0x08 ;
    if(temp)//FLASH
    {
        LCD1602_DispStr(1 , 15  , "F");
    }  
}

/********************************************************************************************
 - 功能描述： 上电跳转至设备状态
 - 隶属模块： 外部
 - 参数说明： 
 - 返回说明：
 - 注：	     
*******************************************************************************************/
void GoInitDevice(INT8U Online)
{
    INT8U temp ;
    if(0 == Online)
    {
        PlayDevice = PLAYDEVICE_SLEEP ; 
        return ;
    }
    temp = Online & 0x01 ;
    if(temp)
    {
        PlayDevice = PLAYDEVICE_UDISK ; 
        return ;
    }
    temp = Online & 0x02 ;
    if(temp)//TF
    {
        PlayDevice = PLAYDEVICE_TFCARD ; 
        return ;
    }
    temp = Online & 0x04 ;
    if(temp)//PC
    {
        PlayDevice = PLAYDEVICE_PC ; 
        return ;
    }   
    temp = Online & 0x08 ;
    if(temp)//FLASH
    {
        PlayDevice = PLAYDEVICE_FLASH ; 
        return ;
    }
}

/********************************************************************************************
 - 功能描述： 设备拔出跳至下一个设备
 - 隶属模块： 外部
 - 参数说明： 
 - 返回说明：
 - 注：	     
*******************************************************************************************/
void GotoNextDevice(INT8U device)
{
    if(device == UDISK)
    {
        if(OnlineDevice & BIT(TFCARD))
        {
            PlayDevice = PLAYDEVICE_TFCARD ;
        }
        else
        {
            if(OnlineDevice & BIT(FLASH))
            {
                PlayDevice = PLAYDEVICE_FLASH ;
            }
            else
            {
                PlayDevice = PLAYDEVICE_SLEEP ;
            }
        }
    }
    else if(device == TFCARD)
    {
        if(OnlineDevice & BIT(UDISK))
        {
            PlayDevice = PLAYDEVICE_UDISK ;
        }
        else
        {
            if(OnlineDevice & BIT(FLASH))
            {
                PlayDevice = PLAYDEVICE_FLASH ;
            }
            else
            {
                PlayDevice = PLAYDEVICE_SLEEP ;
            }
        }  
    }
    else if(device == PC)
    {
        if(OnlineDevice & BIT(FLASH))
        {
            PlayDevice = PLAYDEVICE_FLASH ;
        }
        else
        {
            PlayDevice = PLAYDEVICE_SLEEP ;
        }
    }
       
}

void ChangeDevice(INT8U dev)
{
    if(dev == PLAYDEVICE_UDISK)
    {
        if(0 == (OnlineDevice & BIT(UDISK)))
        {
            if(OnlineDevice & BIT(TFCARD))
            {
                PlayDevice = PLAYDEVICE_TFCARD ;
            }
            else
            {
                if(OnlineDevice & BIT(FLASH))
                {
                    PlayDevice = PLAYDEVICE_FLASH ;
                }
                else
                {
                    PlayDevice = PLAYDEVICE_SLEEP ;
                }
            }
        }
    }
    if(dev == PLAYDEVICE_TFCARD)
    {
        if(0 == (OnlineDevice & BIT(TFCARD)))
        {
            if(OnlineDevice & BIT(UDISK))
            {
                PlayDevice = PLAYDEVICE_UDISK ;
            }
            else
            {
                if(OnlineDevice & BIT(FLASH))
                {
                    PlayDevice = PLAYDEVICE_FLASH ;
                }
                else
                {
                    PlayDevice = PLAYDEVICE_SLEEP ;
                }
            }
        }
    }
    else if(dev == PLAYDEVICE_PC)
    {
        if(0 == (OnlineDevice & BIT(PC)))
        {    
            if(OnlineDevice & BIT(FLASH))
            {
                PlayDevice = PLAYDEVICE_FLASH ;
            }
            else
            {
                PlayDevice = PLAYDEVICE_SLEEP ;
            }
        }
    }
    else if(dev == PLAYDEVICE_FLASH)
    {
        if(0 == (OnlineDevice & BIT(FLASH)))
        {    
             PlayDevice = PLAYDEVICE_SLEEP ;
        }  
    }
}

/********************************************************************************************
 - 功能描述： LCD1602液晶初始化
 - 隶属模块： 内部
 - 参数说明： 
 - 返回说明：
 - 注：	     
*******************************************************************************************/
void LCD1602_Init()
{							//LCD初始化设定
	LCD1602_wcmd(0x38);			//
	Delay_Ms(1);
	LCD1602_wcmd(0x0c);			//
	Delay_Ms(1);
	LCD1602_wcmd(0x06);			//
	Delay_Ms(1);
	LCD1602_wcmd(0x01);			//清除LCD的显示内容
	Delay_Ms(1);
    LCD1602_DispDat( 0,  0, 0);
    LCD1602_DispStr( 0 , 0 , "QYMxFS----------");//
    LCD1602_DispStr( 1 , 0 , "PLAY:           ");//
    ReturnLcdInit();
    Disp_OnlineDevice(OnlineDevice);
}



		