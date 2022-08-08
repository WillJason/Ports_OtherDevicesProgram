
#include "485.h"
#include <stdarg.h>
#include "bsp_SysTick.h"
static void Delay(__IO uint32_t nCount)	;

/// ����USART�����ж����ȼ�
static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    /* Configure the NVIC Preemption Priority Bits */
//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

    /* Enable the USARTy Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = RS485_INT_IRQ; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/*
 * ��������RS485_Config
 * ����  ��USART GPIO ����,����ģʽ����
 * ����  ����
 * ���  : ��
 * ����  ���ⲿ����
 */
void RS485_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* config USART clock */
	RCC_APB2PeriphClockCmd(RS485_USART_RX_GPIO_CLK|RS485_USART_TX_GPIO_CLK|RS485_RE_GPIO_CLK, ENABLE);
	RCC_APB2PeriphClockCmd(RS485_USART_CLK, ENABLE); 	
	
	// ��USART Tx��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin = RS485_USART_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(RS485_USART_TX_GPIO_PORT, &GPIO_InitStructure);
  	
	// ��USART Rx��GPIO����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Pin = RS485_USART_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(RS485_USART_RX_GPIO_PORT, &GPIO_InitStructure);	
  
  /* 485�շ����ƹܽ� */
	GPIO_InitStructure.GPIO_Pin = RS485_RE_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 	
  GPIO_Init(RS485_RE_GPIO_PORT, &GPIO_InitStructure);
	  
	/* USART ģʽ����*/
	USART_InitStructure.USART_BaudRate = RS485_USART_BAUDRATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(RS485_USART, &USART_InitStructure); 
	/*ʹ��USART*/
  USART_Cmd(RS485_USART, ENABLE);
	
	/*�����ж����ȼ�*/
	NVIC_Configuration();
	/* ʹ�ܴ��ڽ����ж� */
	USART_ITConfig(RS485_USART, USART_IT_RXNE, ENABLE);
	
	/*����485оƬ�������ģʽ*/
	GPIO_ResetBits(RS485_RE_GPIO_PORT,RS485_RE_PIN);
//	GPIO_SetBits(RS485_RE_GPIO_PORT,RS485_RE_PIN);
}





//�жϻ��洮������
#define     UART_BUFF_SIZE      20
volatile    uint16_t uart_p = 0;
uint8_t     uart_buff[UART_BUFF_SIZE];

void bsp_RS485_IRQHandler(void)
{
    if(uart_p<UART_BUFF_SIZE)
    {
        if(USART_GetITStatus(RS485_USART, USART_IT_RXNE) != RESET)
        {
            uart_buff[uart_p] = USART_ReceiveData(RS485_USART);
            uart_p++;
						
						USART_ClearITPendingBit(RS485_USART, USART_IT_RXNE);
        }
				
    }
		else
		{
			USART_ClearITPendingBit(RS485_USART, USART_IT_RXNE);
//			clean_rebuff();       
		}
}



//��ȡ���յ������ݺͳ���
uint8_t*get_rebuff(uint16_t *len) 
{
    *len = uart_p;
    return (uint8_t *)&uart_buff;
}

//��ջ�����
void clean_rebuff(void) 
{

    uint16_t i=UART_BUFF_SIZE+1;
    uart_p = 0;
	while(i)
		uart_buff[--i]=0;

}

void U485_SendData(u8 * dat,int datlen)
{
	int i;
  for(i=0;i<datlen;i++)
	{
	   USART_SendData(USART1, dat[i]);
		 while (USART_GetFlagStatus(RS485_USART, USART_FLAG_TXE) == RESET);
	}
	clean_rebuff();
}



/***************** ����һ���ַ�  **********************/
//ʹ�õ��ֽ����ݷ���ǰҪʹ�ܷ������ţ����ͺ�Ҫʹ�ܽ������š�
void RS485_SendByte(  uint8_t ch )
{
	/* ����һ���ֽ����ݵ�USART1 */
	USART_SendData(RS485_USART,ch);
		
	/* �ȴ�������� */
	while (USART_GetFlagStatus(RS485_USART, USART_FLAG_TXE) == RESET);	
	
}
/*****************  ����ָ�����ȵ��ַ��� **********************/
void RS485_SendStr_length( uint8_t *str,uint32_t strlen )
{
	unsigned int k=0;

	RS485_TX_EN()	;//	ʹ�ܷ�������	
    do 
    {
        RS485_SendByte( *(str + k) );
        k++;
    } while(k < strlen);
		
	/*�Ӷ�����ʱ����֤485�����������*/
	Delay(0xFFF);
		
	RS485_RX_EN()	;//	ʹ�ܽ�������
}


/*****************  �����ַ��� **********************/
void RS485_SendString(  char *str)
{
	unsigned int k=0;
	
	RS485_TX_EN()	;//	ʹ�ܷ�������
	
    do 
    {
        RS485_SendByte(  *(str + k) );
        k++;
    } while(*(str + k)!='\0');
	
	/*�Ӷ�����ʱ����֤485�����������*/
	Delay(0xFFF);
		
	RS485_RX_EN()	;//	ʹ�ܽ�������
}

/*****************  ����һ���ַ� **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* ����һ���ֽ����ݵ�USART */
	USART_SendData(pUSARTx,ch);
		
	/* �ȴ��������ݼĴ���Ϊ�� */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/*****************  �����ַ��� **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* �ȴ�������� */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}

/******************************************************
		��������ת�ַ�������
        char *itoa(int value, char *string, int radix)
		radix=10 ��ʾ��10����	��ʮ���ƣ�ת�����Ϊ0;  

	    ����d=-379;
		ִ��	itoa(d, buf, 10); ��
		
		buf="-379"							   			  
**********************************************************/
char *itoa(int value, char *string, int radix)
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }

    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;
    }

    //for (i = 10000; i > 0; i /= 10)
		for (i = 1000000000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;

    return string;

} /* NCL_Itoa */

/****************************************************************************
* ��    �ƣ�void USART_OUT(USART_TypeDef* USARTx, uint8_t *Data,...)
* ��    �ܣ���ʽ�������������
* ��ڲ�����USARTx:  ָ������
			Data  :  ��������
			...   :  ��������
* ���ڲ�������
* ˵    ������ʽ�������������
        	"\r"	�س���	   USART_OUT(USART1, "abcdefg\r")   
			"\n"	���з�	   USART_OUT(USART1, "abcdefg\r\n")
			"%s"	�ַ���	   USART_OUT(USART1, "�ַ����ǣ�%s","abcdefg")
			"%d"	ʮ����	   USART_OUT(USART1, "a=%d",10)
* ���÷������� 
****************************************************************************/
void USART_OUT(USART_TypeDef* USARTx, uint8_t *Data,...)
{ 

	const char *s;
    int d;
   
    char buf[16];
    va_list ap;
    va_start(ap, Data);
	
	RS485_TX_EN()	;//	ʹ�ܷ�������
	while(*Data!=0){				                          //�ж��Ƿ񵽴��ַ���������
		if(*Data==0x5c){									  //'\'
			switch (*++Data){
				case 'r':							          //�س���
					USART_SendData(USARTx, 0x0d);	   

					Data++;
					break;
				case 'n':							          //���з�
					USART_SendData(USARTx, 0x0a);	
					Data++;
					break;
				
				default:
					Data++;
				    break;
			}
		}
		else if(*Data=='%'){									  //
			switch (*++Data){				
				case 's':										  //�ַ���
                	s = va_arg(ap, const char *);
                	for ( ; *s; s++) {
                    	USART_SendData(USARTx,*s);
						while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
                	}
					Data++;
                	break;
            	case 'd':										  //ʮ����
                	d = va_arg(ap, int);
                	itoa(d, buf, 10);
                	for (s = buf; *s; s++) {
                    	USART_SendData(USARTx,*s);
						while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
                	}
					Data++;
                	break;
				default:
					Data++;
				    break;
			}		 
		}
		else USART_SendData(USARTx, *Data++);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
	}
	
		RS485_RX_EN()	;//	ʹ�ܽ�������
}

static void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}
