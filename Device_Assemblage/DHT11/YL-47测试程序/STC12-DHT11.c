/*********************************************************************************************
硬件支持：　　STC12C5A60S2  DHT11
接口说明：　　DHT11：VCC GND DATA 
*********************************************************************************************
说明：1、单片机是STC12C5A60S2，晶振频率12MHz，实验成功。换用12T单片机或别的晶振的时候要修改
          延时函数和JUDGE。程序里判断超时的变量t的阀值也要改。
          2、好东西要共享，大家随意折腾~~
*********************************************************************************************/
#include <STC12C5A60S2.H>

typedef unsigned char        uchar;
typedef unsigned int        uint;

#define JUDGE        30                //用于判断通信的0和1，与单片机速度和晶振频率有关

sbit DATA = P1^0;                //定义数据引脚

uchar dat_r[5];                        //用于存放从DHT11读取到的数值

/*********************************************************************************************
函数名：延时函数
调  用：Delay30us();/Delay20ms();/Delay1s();
参  数：无
返回值：无 
结  果：延时相应时间
备  注：振荡晶体为12MHz(所有延时函数Copy自STC-ISP(v6.85).exe)
**********************************************************************************************/
void Delay30us()                //@12.000MHz
{
        unsigned char i;

        i = 87;
        while (--i);
}


void Delay20ms()                //@12.000MHz
{
        unsigned char i, j, k;

        i = 1;
        j = 234;
        k = 113;
        do
        {
                do
                {
                        while (--k);
                } while (--j);
        } while (--i);
}

void Delay1s()                //@12.000MHz
{
        unsigned char i, j, k;

        i = 46;
        j = 153;
        k = 245;
        do
        {
                do
                {
                        while (--k);
                } while (--j);
        } while (--i);
}

/*********************************************************************************************
函数名：UART串口初始化函数
调  用：UART_Init();
参  数：无
返回值：无
结  果：启动UART串口接收中断，允许串口接收，启动T/C1产生波特率（占用）
备  注：振荡晶体为12MHz，PC串口端设置 [ 4800，8，无，1，无 ]
**********************************************************************************************/
void UART_Init (void){
        EA = 1; //允许总中断（如不使用中断，可用//屏蔽）
        ES = 1; //允许UART串口的中断

        TMOD = 0x20;        //定时器T/C1工作方式2
        SCON = 0x50;        //串口工作方式1，允许串口接收（SCON = 0x40 时禁止串口接收）
        TH1 = 0xF3;        //定时器初值高8位设置
        TL1 = 0xF3;        //定时器初值低8位设置
        PCON = 0x80;        //波特率倍频（屏蔽本句波特率为2400）
        TR1 = 1;        //定时器启动    
}

/*********************************************************************************************
函数名：UART串口发送函数
调  用：UART_T (?);
参  数：需要UART串口发送的数据（8位/1字节）
返回值：无 
结  果：将参数中的数据发送给UART串口，确认发送完成后退出
备  注：
**********************************************************************************************/
void UART_T (uchar UART_data){ //定义串口发送数据变量
        SBUF = UART_data;        //将接收的数据发送回去
        while(TI == 0);                //检查发送中断标志位
        TI = 0;                        //令发送中断标志位为0（软件清零）
}

/*********************************************************************************************
函数名：DHT11读数据函数
调  用：? = GetData();
参  数：无
返回值：失败->-1,-2,-3,-4;成功->0
结  果：读DHT11数据并保存到数组dat_r[]
备  注：
**********************************************************************************************/
char GetData (void)
{
        uchar i,j;                                //for循环变量
        uchar t;                                //超时判断
        uchar dat8=0;                        //一次读取的8位数据，需要读5次

        DATA = 0;                                //主机发起始信号
        Delay20ms();                        //主机拉低总线至少18ms
        DATA = 1;                                //主机拉高总线20~40us
        Delay30us();

        t = 80;                                        //设置超时等待时间
        while(DATA && t--);                //等待DHT11拉低总线
        if(t == 0)                                //超时
        {
                DATA = 1;
                return -1;                        //通信错误退出，返回错误信号：-1
        }

        //等80us响应信号
        t = 250;                                //设置超时等待时间
        while(!DATA && t--);        //等待DHT11拉高总线
        if(t == 0)                                //超时
        {
                DATA = 1;
                return -2;                        //通信错误退出，返回错误信号：-2
        }

        //等80us响应信号
        t = 250;                                //设置超时等待时间
        while(DATA && t--);                //等待DHT11拉低总线
        if(t == 0)                                //超时
        {
                DATA = 1;
                return -3;                        //通信错误退出，返回错误信号：-3
        }

        for(j=0; j<5; j++)                //5次读取
        {
                for(i=0; i<8; i++)                        //1次8个位
                {
                        //等待50us开始时隙
                        t = 150;                                //设置超时等待时间
                        while(!DATA && t--);        //等待DHT11拉高总线
                        if(t == 0)                                //超时
                        {
                                DATA = 1;
                                return -4;                        //通信错误退出，返回错误信号：-4
                        }
                        t = 0;                                        //记录时间清零
                        while(DATA && ++t);                //等待并记录高电平持续时间
                        dat8 <<= 1;
                        if(t > JUDGE)                        //高电平持续时间较长(70us)
                                dat8 += 1;                        //传输值为1
                        //else dat8 += 0;
                }
                dat_r[j] = dat8;
        }

        Delay30us();                //等待DHT11拉低50us
        Delay30us();
        DATA = 1;                        //结束，拉高总线
        return 0;                        //返回成功信号
}

/*********************************************************************************************
函数名：数据校验函数
调  用：? = Check();
参  数：无
返回值：成功->0;失败->-1;
结  果：数据校验
备  注：数据传送正确时校验和数据等于“8bit湿度整数数据+8bit湿度小数数据+8bi温度整数数据+8bit温度小数数据”所得结果的末8位。
**********************************************************************************************/
char Check (void)
{
        uchar i;                        //for循环变量
        uint chk=0;                        //校验和
        for(i=0; i<4; i++)
                chk += dat_r[i];                //累加
        if((uchar)chk != dat_r[4])        //比对
                return -1;                                //返回错误信号：-1
        else
                return 0;                                //返回正确信号：0
}

/*********************************************************************************************
函数名：主函数
调  用：无
参  数：无
返回值：无
结  果：读DHT11数据并送到串口显示
备  注：
**********************************************************************************************/
void main (void)
{
        char i;
        UART_Init();                //串口初始化
        Delay1s();                        //等待DHT11稳定
        do
        {
                i = GetData();        //读数据
                if(i != 0)                //有错误
                {
                        UART_T(-i);        //返回错误值
                        break;                //重新开始
                }

                i = Check();        //数据校验
                if(i != 0)                //有错误
                {
                        UART_T(-i);        //返回错误值
                        break;                //重新开始
                }

                UART_T(dat_r[0]);                //湿度整数
                //UART_T(dat_r[1]);                //湿度小数(根据数据手册，DHT11小数部分一直是0没用)
                UART_T(dat_r[2]);                //温度整数
                //UART_T(dat_r[3]);                //温度小数(根据数据手册，DHT11小数部分一直是0没用)
                UART_T(dat_r[4]);                //校验值

                Delay1s();                                //经测试，两次连读要至少延时80ms        
        }while(1);
}//main