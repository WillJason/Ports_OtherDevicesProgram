/********************************************************************************************
	文件: Ps2_Int.c
	作者: WillJason
	版本：V1.0
	备注：这个文件提供了外部中断和编码处理的函数库  	   
*********************************************************************************************/
#include "STC12C5A60S2.h"
#include "Ps2_Int.h"
#include "io_config.h"
#include "uart.h"
#include "global.h"
#include "beep.h"
#include "yx5200.h"
#include "timer.h"
#include "iap.h"

uint8_t xdata Password_Buf[80]=0;						  //玩家或配置密码人员输入的密码
uint8_t xdata Saved_Password_Buf[60]=0;		              //配置好的密码
uint8_t xdata Read_Password_Buf[60]=0;		              //配置好的密码
uint8_t xdata Administrator[12]={0x1B,0x23,0x1C,0x3A,0x72,0x45,0x16,0x36,0x45,0x16,0x16,0x36};	//进入配置程序密码sdam20160116

bit Gain_New=0;								  //接收到新的键码数据标志位
bit Key_Up = 1;								  //判断按键是否松开标志位
bit Over_Flag = 0;							  //

/********************************************************************************************
	函数功能：发送从键盘得到的按键码数据；调用操作函数
	入口参数：无 
	返回参数：无
	函数说明：判断按键的字符是否输入完毕，输入完毕将键值数据通过串口发送上位机。
********************************************************************************************/
void Send_Analysis(void)
{														  
	if(BF != 0)											  //接收到了一个键的键码
	{
		BF=0;
		//uart1_send_byte(0x33);						  //调试用
		uart1_send_byte(Key_Data);
		Password_Input(Key_Data);				          //调用操作函数（接收、保存密码、解码实现相应的功能）		
	}
}
/********************************************************************************************
	函数功能：按键中断处理函数
	入口参数：无 
	返回参数：无
	函数说明：获得键值并将将键值存入参数变量。每按一个键会产生如下数据，按下产生一组串行数据，松开产生两组串行数据
			  第一位为起始位0
			  第二位~第九位为数据位（即扫描码），低位开始传输
			  第十位奇偶校验位。如果扫描码中1的个数为奇数，该位取0，如果扫描码中1的个数为偶数，该位取1
			  第十一位结束位，高电平。
			  当键盘发送数据时，键盘先将DATA 拉低，通知主机准备接收数据。键盘在CLK为高电平时建立数据，主机在CLK为低
			  电平时读取数据。发送数据的时钟信号由键盘产生。
********************************************************************************************/
void Ps2_Out(void) interrupt 2                            //键盘中断处理 键值存储在 KeyV 中
{
	
	if ((IntNum>0) && (IntNum <9))						  //取出第二位到第九位总共一个字节的数据，第一位为起始位
	{			
		KeyV = KeyV >> 1;                                 //因键盘数据是低>>高，结合上一句所以右移一位
		if (Ps2_Data != 0)                                  //当键盘数据线为1时   
		{KeyV = KeyV | 0x80;}                             //存储一位
	}
	IntNum++;                                             //中断次数加一(中断一次接收一位数据)
	if (IntNum > 10)                                      //中断11次后数据发送完毕
	{   
		IntNum = 0;                                       //当中断11次后表示一帧数据收完，清变量准备下一次接收
		led_port = ~led_port;							  //调试用
		/*********************数据处理*********************/
		Make_Code=KeyV;									  //接收到的键盘码：通码，断码0xF0，等等（这里仅起了个通码Make_Code的名字而已）
		if(Make_Code == 0xF0)							  //如果接收到0xF0，单独留住，确定收到0xF0
		{
			isCode_F0=Make_Code;						  //断码第一位数据0xF0保存
		}
		//uart1_send_byte(Make_Code);					  //调试用
		Gain_New=1;										  //接收到新的字节的数据标志
	}	 							 	
}
/********************************************************************************************
	函数功能：外部中断函数中的变量清零
	入口参数：无
	返回参数：无
	函数说明：外部中断函数中的变量清零
********************************************************************************************/
void Ps2_Out_Free(void)
{
	KeyV=0;
	Make_Code=0;									  //接收到的键盘码：通码，断码0xF0，等等（这里仅起了个通码Make_Code的名字而已）
	isCode_F0=0;						  //断码第一位数据0xF0保存
	Gain_New=0;
}
/********************************************************************************************
	函数功能：获得键值
	入口参数：无 
	返回参数：无
	函数说明：获得键值
********************************************************************************************/
void Gain_Key_Data(void)
{
	if(Gain_New==1)										  //接收到新的字节数据，标志位置1
	{	if(Key_Up == 0)									  //判断按键是否抬起：0-未抬起；1-已抬起
		{	if(isCode_F0 == 0xF0)	   					  //按键抬起时
			{	
				isCode_F0=0;
				//relay_port =~relay_port;		  		  //调试用
				//uart1_send_byte(0xF0);				  //调试用
				Over_Flag = 1;
				BF=1;}									  //表示接收完成一个键值的数据
		}	
		else
		{  
		   Gain_New=0;									  //清除标志位，准备接收下一个数据
		   //uart1_send_byte(Make_Code);					  //调试用
		   Key_Data=Make_Code;							  //将得到的通码保存起来送给Password_Input函数（之后
		   												  //如果检测到松开该按键并且该按键有效，则保存为一位密码）
		   Decode();// 								  	  //判断键值是否有效，蜂鸣器鸣声
		   Key_Up=0;									  //标志位清零，等待松开按键（即抬起按键）
		}
	}	   
	if(Over_Flag == 1)	    						      //如果是F0 则滤掉剩余的结束码
	{
		delay_ms(10);									  //延时时间保证结束码要传递完了
		Over_Flag = 0;
		Key_Up = 1;		    							  //确定抬手后，重新开始准备结束有效码
		Gain_New = 0;		      						  //标识字符		
	}
}
/********************************************************************************************
	函数功能：判断键值是否有效，蜂鸣器鸣声
	入口参数：无 
	返回参数：无
	函数说明：判断键值是否有效
			  回车键：用来确定输入，输入密码后必须按下回车键确认。按下回车键后蜂鸣2声（响100ms、停50ms、响100ms）；
			  回格键：2个按键功能相同，用来清除所有输入，按下按键蜂鸣2声（响100ms、停50ms、响100ms）；
		 	  其他按键：用来表示密码，按下按键蜂鸣1声（100ms）；
********************************************************************************************/
void Decode(void)
{
	switch(Make_Code)									  //判断通码
	{
		/*********************运算符*******************************/
		case 0x79:								          //加法
		case 0x7B:								          //减法
		case 0x7C:								          //乘法
		//case 0xE0 0x4A:						          //除法
		/*********************小数点*******************************/
		//case 0x49:								          //主键盘小数点
		case 0x71:								          //小键盘小数点
		/*********************空格键*******************************/
		case 0x29:
		/******************主键盘数字区****************************/
		case 0x45:								          //数字0
		case 0x16:								          //数字1
		case 0x1E:								          //数字2
		case 0x26:								          //数字3
		case 0x25:								          //数字4
		case 0x2E:								          //数字5
		case 0x36:							   	          //数字6
		case 0x3D:								          //数字7
		case 0x3E:								          //数字8
		case 0x46:								          //数字9
		/******************小键盘数字区****************************/
		case 0x70:								          //数字0
		case 0x69:								          //数字1
		case 0x72:								          //数字2
		case 0x7A:								          //数字3
		case 0x6B:								          //数字4
		case 0x73:								          //数字5
		case 0x74:							   	          //数字6
		case 0x6C:								          //数字7
		case 0x75:								          //数字8
		case 0x7D:								          //数字9
		/******************字母区******************************/
		case 0x1C:								          //字母A
		case 0x32:								          //字母B
		case 0x21:								          //字母C
		case 0x23:								          //字母D
		case 0x24:								          //字母E
		case 0x2B:								          //字母F
		case 0x34:								          //字母G
		case 0x33:								          //字母H
		case 0x43:								          //字母I
		case 0x3B:								          //字母J
		case 0x42:								          //字母K
		case 0x4B:								          //字母L
		case 0x3A:								          //字母M
		case 0x31:								          //字母N
		case 0x44:								          //字母O
		case 0x4D:								          //字母P
		case 0x15:								          //字母Q
		case 0x2D:								          //字母R
		case 0x1B:								          //字母S
		case 0x2C:								          //字母T
		case 0x3C:								          //字母U		  
		case 0x2A:								          //字母V		  
		case 0x1D:								          //字母W		  
		case 0x22:								          //字母X		  
		case 0x35:								          //字母Y
		case 0x1A: Impactful_Key=1;Other_Beep();Make_Code=0;//	//蜂鸣器工作 //字母Z
				 break;
		case 0x5A: Enter_Beep();Make_Code=0;	          //回车键
				 break;
		case 0x66:Backspace_Beep();Make_Code=0;           //回格键
		default:
				break;

	}
}
/********************************************************************************************
	函数功能：接收、保存密码、解码实现相应的功能
	入口参数：键码数据-通码
	返回参数：无
	函数说明：接收、保存密码、解码实现相应的功能
********************************************************************************************/
void Password_Input(uint8_t Password_One)  
{
	uint16_t i;
	uint16_t passWordNum;
	TR0=0;
	
	if(Password_One == 0x5A)	                          //输入密码接收完成
	{	
		TR0=0;											  //关闭定时
		if(Config_Password_State != 0)				      //判断程序是否在配置密码程序状态
		{					  
			if(Player_Input_Num >= 60)					  //如果设置的密码大于60位，则保存前60位
			{
				iap_erase_sector(IAP_ADDRESS);			  //擦除地址为0x0000大小为0.5K字节的扇区
				for(i=60;i<Player_Input_Num;i++)		  //一定要清除输入60位之后的键值数据，否则会影响到后面判断密码是否一致的程序
				{
					Password_Buf[i]=0;				      //循环清除60位之后密码缓冲区的每一位数据
				}
				Player_Input_Num=60;					  //相当于仅接收到了前60位的密码数据
				for(i=0;i<Player_Input_Num;i++)			  //循环把前60位每一位密码保存起来
				{ 
					Saved_Password_Buf[i]=Password_Buf[i];//将输入的密码保存起来
					iap_program_byte(IAP_ADDRESS+i+1, Saved_Password_Buf[i]); //开始的第一个字节为密码数目
				}	
			}
			else if(!Player_Input_Num)					  //如果设置的密码为0位（密码设置的时候直接按Enter），保存原有的密码
			{
				//relay_port =~relay_port;				  //调试用
				Player_Input_Num=iap_read_byte(IAP_ADDRESS);//将原先保存的密码数据的个数取出，在EEPROM的0x0000地址的第一个字节
														    //如果不取出则相当于输入的密码的个数是0位，这样就不能保存原有的密码了
				if(Player_Input_Num >= 60)				  //第一次上电EEPROM的第一个字节为未知数据，此时如果设置密码为0位，那么系												
				{										  //统将会紊乱；解决措施：如果第一字节大于60位则清0，相当于原先保存的密码
					Player_Input_Num=0;				      //的个数为0个，保证系统不会紊乱
				}
			}		   
			else
			{
				iap_erase_sector(IAP_ADDRESS);			  //擦除地址为0x0000大小为0.5K字节的扇区
				for(i=0;i<Player_Input_Num;i++)			  //循环把每一位密码保存起来
				{ 
					Saved_Password_Buf[i]=Password_Buf[i];//将输入的密码保存起来
					iap_program_byte(IAP_ADDRESS+i+1, Saved_Password_Buf[i]); //开始的第一个字节为密码数目
				}
			}
																  
			i=0;  
			Password_Size=Player_Input_Num;				  //得到输入密码的位数
			iap_program_byte(IAP_ADDRESS, Password_Size); //将密码数目写入EEprom
			Finish_Config=1;							  //密码配置完成
			//relay_port =~relay_port;					  //调试用
		}
		else											  //接收密码进行开锁上锁；判断是否进入配置密码模式
		{
			//relay_port =~relay_port;					  //调试用
			passWordNum=iap_read_byte(IAP_ADDRESS);		  //获取密码位数
			for(i=0;i<passWordNum;i++)
			{
				Read_Password_Buf[i]=iap_read_byte(IAP_ADDRESS+i+1);//从EEPROM中取出正确的密码数据
			}
			isTruePassWord=Password_Determine(&Password_Buf[0],&Read_Password_Buf[0],passWordNum); //查看密码是否正确	  Password_Size
			isTruePassWord_In30s=Password_Determine(&Password_Buf[0],&Administrator[0],12);  //上电30s内输入输入配置密码：sdam20160116，输入正确可以配置
			if(isTruePassWord != 0)			 		      //查看密码是否正确
			{
				
				relay_port =~relay_port;				  //relay_port= 0为开锁状态
				
				music.folder_num= music.folder_num^0x0003;//异或运算，0x0101异或0x0003为0x0102,0x0102异或0x0003为0x0101，异或取反运算
				Play_Set_Sound(music.folder_num);		  //播放指定的音乐文件
				Ps2_Out_Free();							  //将中断服务函数中的变量清零，以准备接收之后输入的键值
				
			}
			isIn_30s();									  //判断计时时间是否到达30s
			if(!Finish_30s)							  	  //判断是否在开始的30s内,返回值为0代表还在30s内，30s内可以进入配置模式	
			{
				TR0=1;									  //启动定时器！如果开始的30s内，关闭定时器会影响30s的计时（30s计时用定时器0）
				//relay_port =~relay_port;				  //调试用				
				if(isTruePassWord_In30s != 0)			 	  //查看进入配置模式输入的密码是否正确
				{
					//relay_port =~relay_port;			  //调试用
					isTruePassWord_In30s=0;	      
					Allow_Set_Password=1;				  //进入密码配置模式，设置密码
				}
				else
				{Administrator_Ok=0;}					  //密码错误
				Clean_Time0_Paramater();				  //清零计时的参数,防止影响下一次计时的准确性
				Password_Free(0x3C);					  //60位密码空间全部清零（清除?
			}


		}		
		Password_Free(Player_Input_Num);				  //密码输入完成，清空密码输入的缓冲区
		Start_10s=0;									  //计时标志位清0，停止计时
		Player_Input_Num=0;								  //密码输入缓冲区中密码数目清零
	}	
	else if((Password_One == 0x66)||(Password_One == 0x71))	  //清除键和回格键：所有输入清除
	{
		Password_Free(Player_Input_Num);				  //清空密码输入的缓冲区
		TR0=1;											  //启动定时器！如果开始的30s内，关闭定时器会影响30s的计时（30s计时用定时器0）
		Start_10s=0;									  //一个10s判断标志位：如果10s内没有其他密码输入则清空已输入的密码：启动计时
		Clean_Time0_Paramater();						  //清零计时的参数
		Player_Input_Num=0;								  //密码输入缓冲区中密码数目清零
	}
	else
	{
		if(Impactful_Key != 0)							  //判断按键是否是有效按键，无效则不记录为输入的密码
		{
			if(Password_One == 0x70){Password_One=0x45;}  //小键盘数字0和主键盘数字0等效
			if(Password_One == 0x69){Password_One=0x16;}  //小键盘数字1和主键盘数字1等效
			if(Password_One == 0x72){Password_One=0x1E;}  //小键盘数字2和主键盘数字2等效
			if(Password_One == 0x7A){Password_One=0x26;}  //小键盘数字3和主键盘数字3等效
			if(Password_One == 0x6B){Password_One=0x25;}  //小键盘数字4和主键盘数字4等效
			if(Password_One == 0x73){Password_One=0x2E;}  //小键盘数字5和主键盘数字5等效
			if(Password_One == 0x74){Password_One=0x36;}  //小键盘数字6和主键盘数字6等效
			if(Password_One == 0x6C){Password_One=0x3D;}  //小键盘数字7和主键盘数字7等效
			if(Password_One == 0x75){Password_One=0x3E;}  //小键盘数字8和主键盘数字8等效
			if(Password_One == 0x7D){Password_One=0x46;}  //小键盘数字9和主键盘数字9等效

			Password_Save(Password_One,Player_Input_Num); //保存密码
			Player_Input_Num++;
			if(Player_Input_Num>=70)					  //密码不得超过六十位
			{
				Player_Input_Num=0x46;
				//Password_Free(0x3C);					  //60位密码空间全部清零（清除）
				//relay_port =~relay_port;				  //调试用
			}	
			Clean_Time0_Paramater();					  //清零计时的参数,重新计时
			TR0=1;										  //启动定时器
			Start_10s=1;								  //一个10s判断标志位：如果10s内没有其他密码输入则清空已输入的密码	
			Impactful_Key=0;							  //防止下一个非有效按键被误认为是有效
		}		
	}	
}

/********************************************************************************************
	函数功能：判断连续的10秒内是否有输入
	入口参数：无
	返回参数：无
	函数说明：当游戏玩家开始输入密码，判断连续10秒是否有输入；若没有则清除所有输入状态
********************************************************************************************/
void Determine_10s(void)
{
	if((Key_Data != 0)&&(Start_10s != 0))	      //有密码输入并且启动10s计时
	{
		if(timer0_10s() != 0)	   						  //如果为1，代表10s计时完成说明连续10s没有输入
		{
			Password_Free(0x3C);					      //60位密码空间全部清零（清除）
			//relay_port =~relay_port;					  //调试用
			Start_10s=0;								  //清除标志位
			Player_Input_Num=0;							  //密码输入缓冲区中密码数目清零
		}
	} 
}
/********************************************************************************************
	函数功能：判断计时时间是否到达30s
	入口参数：无
	返回参数：无
	函数说明：判断计时时间是否到达30s，到达30s则将标志位死锁住
********************************************************************************************/
void isIn_30s(void)
{										
	if(!Finish_30s)										  //返回值为0表示现在未到30s，则进入判断是否是否计时完毕；否则表示计时完毕
	{
		Finish_30s=timer0_30s();						  //接收返回值
	}
}					   
/********************************************************************************************
	函数功能：进入密码配置模式，由玩家设定配置的密码
	入口参数：无
	返回参数：无
	函数说明：设置密码
********************************************************************************************/
void Player_Set_Password(void)
{
	if(Allow_Set_Password)
	{
		Allow_Set_Password=0;
		Play_Set_Sound(0x0202);						      //播放指定的音乐文件
		Ps2_Out_Free();									  //将中断服务函数中的变量清零，以准备接收之后输入的键值
		while(!Finish_Config)
		{
			Config_Password_State=1;
			Send_Analysis();							  //调用操作函数（接收、保存密码、解码实现相应的功能）
			//Decode();									  //判断键值是否有效	蜂鸣器工作
			Gain_Key_Data();
	
		}
		Config_Password_State=0;						  //跳出密码配置模式
		Play_Set_Sound(0x0203);						      //播放指定的音乐文件
		Ps2_Out_Free();									  //将中断服务函数中的变量清零，以准备接收之后输入的键值
		Password_Free(0x3C);							  //60位密码空间全部清零（清除）
	}	
}
/********************************************************************************************
	函数功能：密码数值判断是否一致
	入口参数：玩家输入完成的密码--Player_Password；设置好的密码--Fix_Password；密码的数目
	返回参数：1--密码一致；0--密码不一致。
	函数说明：密码数值判断是否一致，判断输入的密码是否与设置好的
			  密码一致，一致的话返回1，不一致返回0
********************************************************************************************/
uint8_t Password_Determine(uint8_t *Player_Password, uint8_t *Fix_Password,uint8_t Num)
{
	uint8_t i=0;
	uint8_t j=0;
	for(i=0;i<Num;i++)
	{  
		if(*(Player_Password+j)==*(Fix_Password+j))
		{
			j++;
			if((j>=Num)&&(!(*(Player_Password+j))))		  //如果密码完全一致（Num位的密码一致并且之后玩家没有继续输入密码）
			{
				i=0;
				j=0;
				return 1;								  //返回1代表输入的密码和正确的密码一致
			}
		}
		else
		{i=0;j=0;return 0;}								  //返回0代表输入的密码和正确的密码不一致
	}
	i=0;j=0;return 0;
}

/********************************************************************************************
	函数功能：密码清空函数
	入口参数：清除缓冲区位数数目
	返回参数：无
	函数说明：对玩家输入的密码清除。
********************************************************************************************/
void Password_Free(uint8_t Num)
{
	uint8_t i;
	for(i=0;i<Num;i++)
	{
		Password_Buf[i]=0;								  //循环清除密码缓冲区的每一位数据
	}
}
/********************************************************************************************
	函数功能：密码保存函数
	入口参数：玩家一个键值密码数据；保存在缓冲区的位置坐标
	返回参数：无
	函数说明：将玩家输入的密码进行存储
********************************************************************************************/
void Password_Save(uint8_t Player_Password,uint8_t i)
{
	Password_Buf[i]=Player_Password;					  //将得到的键值数据保存到缓冲区
}


