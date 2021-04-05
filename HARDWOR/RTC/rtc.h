#ifndef __RTC_H
#define __RTC_H     
#include "stm32f10x.h"
//定义一个时间结构体//
typedef struct 
{
 vu8 hour;     //时
 vu8 min;      //分
 vu8 sec;      //秒
 vu16 w_year;  //年
 vu8  w_month; //月
 vu8  w_date;  //日
 vu8  week;    //周 
}_calendar_obj;      
extern _calendar_obj calendar;                //日历结构体，已在rtc.c中申明了//
extern u8 const mon_table[12];                //月份日期数据表，常数
void Disp_Time(u8 x,u8 y,u8 size);            //在制定位置开始显示时间
void Disp_Week(u8 x,u8 y,u8 size,u8 lang);    //在指定位置显示星期
u8 RTC_Init(void);                            //初始化RTC,返回0,失败;1,成功;
u8 Is_Leap_Year(u16 year);                    //平年,闰年判断，返回1是，0不是
u8 RTC_Alarm_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);  //闹钟设定
u8 RTC_Get(void);                             //获取时间   
u8 RTC_Get_Week(u16 year,u8 month,u8 day);    //获取日期，返回0成功，其他失败
u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);        //设置时间    
#endif
