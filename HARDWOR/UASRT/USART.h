#ifndef __USART_H
#define __USART_H

#include "stdio.h"
#include "sys.h"

#define USART_REC_LEN 200//定义最大接受字节数200
#define EN_USART1_RX 1//使能（1）/禁止（0）   串口1接受

extern u8 USART_RX_BUF[USART_REC_LEN];//接受缓冲，最大USART_REN_LEN个字节，末字节为换行符
extern u16 USART_RX_STA;//接受状态标记

void uart_init(u32 bound);//定义串口初始化函数


#endif
