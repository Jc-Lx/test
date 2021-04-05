#ifndef __KEY_H
#define __KEY_H
#include "sys.h"

#define KEY0 PEin(4)//��ȡ����0
#define KEY1 PEin(3)//��ȡ����1
#define WK_UP PAin(0)//��ȡ����3

#define KEY0_PRES 1//���尴�����·���ֵ������swicth case
#define KEY1_PRES 2
#define WKUP_PRES 3

void  KEY_Init(void);
u8 KEY_Scan(u8);


#endif
