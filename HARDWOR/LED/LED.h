
#ifndef  __LED_H
#define __LED_H

#include "sys.h"
//LED位带操作端口定义
#define LED0 PBout(5)
#define LED1 PEout(5)

void LED_Init(void);

#endif
