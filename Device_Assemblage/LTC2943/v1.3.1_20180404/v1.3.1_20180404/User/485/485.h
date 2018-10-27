#ifndef _RS485_H
#define	_RS485_H

#include "stm32f10x.h"
#include <stdio.h>


/*USART号、时钟、波特率*/
#define RS485_USART                             USART1
#define RS485_USART_CLK                         RCC_APB2Periph_USART1
#define RS485_USART_BAUDRATE                    9600

/*RX引脚*/
#define RS485_USART_RX_GPIO_PORT                GPIOA
#define RS485_USART_RX_GPIO_CLK                 RCC_APB2Periph_GPIOA
#define RS485_USART_RX_PIN                      GPIO_Pin_10

/*TX引脚*/
#define RS485_USART_TX_GPIO_PORT                GPIOA
#define RS485_USART_TX_GPIO_CLK                 RCC_APB2Periph_GPIOA
#define RS485_USART_TX_PIN                      GPIO_Pin_9

/*485收发控制引脚*/
#define RS485_RE_GPIO_PORT								GPIOA
#define RS485_RE_GPIO_CLK								RCC_APB2Periph_GPIOA
#define RS485_RE_PIN										GPIO_Pin_8

/*中断相关*/
#define RS485_INT_IRQ                 				USART1_IRQn
#define RS485_IRQHandler                        USART1_IRQHandler


	/// 不精确的延时
static void RS485_delay(__IO u32 nCount)
{
	for(; nCount != 0; nCount--);
} 


/*控制收发引脚*/
//进入接收模式,必须要有延时等待485处理完数据
#define RS485_RX_EN()			RS485_delay(1000); GPIO_ResetBits(RS485_RE_GPIO_PORT,RS485_RE_PIN);  RS485_delay(1000);
//进入发送模式,必须要有延时等待485处理完数据
#define RS485_TX_EN()			RS485_delay(1000); GPIO_SetBits(RS485_RE_GPIO_PORT,RS485_RE_PIN);  RS485_delay(1000);



void RS485_Config(void);
void RS485_SendByte(  uint8_t ch );
void RS485_SendStr_length( uint8_t *str,uint32_t strlen );
void RS485_SendString(  char *str);

void bsp_RS485_IRQHandler(void);
uint8_t  *get_rebuff(uint16_t *len);
void clean_rebuff(void);
void U485_SendData(u8 * dat,int datlen);
void Usart_SendString( USART_TypeDef * pUSARTx, char *str);
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);

void ptest(char num);
void USART_OUT(USART_TypeDef* USARTx, uint8_t *Data,...);
#endif /* _RS485_H */
