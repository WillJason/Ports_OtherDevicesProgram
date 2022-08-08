#ifndef _RS485_H
#define	_RS485_H

#include "stm32f10x.h"
#include <stdio.h>


/*USART�š�ʱ�ӡ�������*/
#define RS485_USART                             USART1
#define RS485_USART_CLK                         RCC_APB2Periph_USART1
#define RS485_USART_BAUDRATE                    9600

/*RX����*/
#define RS485_USART_RX_GPIO_PORT                GPIOA
#define RS485_USART_RX_GPIO_CLK                 RCC_APB2Periph_GPIOA
#define RS485_USART_RX_PIN                      GPIO_Pin_10

/*TX����*/
#define RS485_USART_TX_GPIO_PORT                GPIOA
#define RS485_USART_TX_GPIO_CLK                 RCC_APB2Periph_GPIOA
#define RS485_USART_TX_PIN                      GPIO_Pin_9

/*485�շ���������*/
#define RS485_RE_GPIO_PORT								GPIOA
#define RS485_RE_GPIO_CLK								RCC_APB2Periph_GPIOA
#define RS485_RE_PIN										GPIO_Pin_8

/*�ж����*/
#define RS485_INT_IRQ                 				USART1_IRQn
#define RS485_IRQHandler                        USART1_IRQHandler


	/// ����ȷ����ʱ
static void RS485_delay(__IO u32 nCount)
{
	for(; nCount != 0; nCount--);
} 


/*�����շ�����*/
//�������ģʽ,����Ҫ����ʱ�ȴ�485����������
#define RS485_RX_EN()			RS485_delay(1000); GPIO_ResetBits(RS485_RE_GPIO_PORT,RS485_RE_PIN);  RS485_delay(1000);
//���뷢��ģʽ,����Ҫ����ʱ�ȴ�485����������
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
