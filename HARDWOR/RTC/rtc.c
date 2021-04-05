#include "beep.h"
#include "rtc.h"  
#include "sys.h"
#include "delay.h"
#include "usart.h"
//时钟结构体//
_calendar_obj calendar;                                      
static void RTC_NVIC_Config(void)
{ 
 NVIC_InitTypeDef NVIC_InitStructure;
 NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;             //RTC全局中断
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级1位,从优先级3位
 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;         //抢占优先级0位,从优先级4位
 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            //使能该通道中断
 NVIC_Init(&NVIC_InitStructure);  
}
//实时时钟配置
//初始化RTC时钟,同时检测时钟是否工作正常
//BKP->DR1用于保存是否第一次配置的设置
//返回0:正常; 其他:错误代码
u8 RTC_Init(void)
{
 //检查是不是第一次配置时钟//
 u8 temp=0;
 //*****第一步*****//
 //电源控制PWR，当主电源VDD掉电后，通过VBAT脚为实时时钟(RTC)和备份寄存器(BKP)提供电源//
 //使能PWR和BKP外设时钟//  
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE); 
 //*****第二步*****// 
 //使能后备寄存器访问，位带操作，实际对电源控制寄存器PWR_CR的第八位DBP写1，允许写入RTC和BKP// 
 PWR_BackupAccessCmd(ENABLE); 
 //读取BKP_DR1寄存器0~15位的值，判断是否与0x5050一致//
 if (BKP_ReadBackupRegister(BKP_DR1) != 0x5050)  
  { 
   //如果不一致，说明第一次配置，则进行如下操作//   
   BKP_DeInit();                           //复位备份区域//  
   RCC_HSEConfig(RCC_HSE_ON);              //设置外部低速晶振(HSE),使用外设低速晶振//
   //备份域控制寄存器RCC_BDCR，位1，LSERDY为0时未就绪，为1时就绪//
   while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET&&temp<250)
    {
     temp++;
     delay_ms(10);
    }
   if(temp>=250)return 1;                   //初始化时钟失败,晶振有问题，结束u8 RTC_Init(void)函数// 
   //若temp＜250，则继续// 
   //*****第三步*****//   
   RCC_RTCCLKConfig(RCC_RTCCLKSource_HSE_Div128);  //设置RTC时钟(RTCCLK),选择HSE作为RTC时钟//    
   RCC_RTCCLKCmd(ENABLE);                   //使能RTC时钟//  
   //*****配置RTC寄存器开始*****//   
   //*****第六步*****//
   //RTC控制寄存器低位(RTC_CRL)，位5，RTOFF为0时仍在写操作，为1时写操作完成，该函数循环执行，直到RTOFF为1// 
   RTC_WaitForLastTask();    
   //RTC控制寄存器低位(RTC_CRL)，位3，RSF为0时寄存器未同步，为1时寄存器同步，该函数循环执行，直到 RSF 为1//
   //因为修改CRH/CRL寄存器，必须先判断RSF位，确定已经同步//  
   RTC_WaitForSynchro();                     
   RTC_ITConfig(RTC_IT_SEC | RTC_IT_ALR, ENABLE);     //对控制寄存器高位(RTC_CRH)操作，使能RTC秒中断//
   RTC_WaitForLastTask();                             //等待最近一次对RTC寄存器的写操作完成//   
   //*****第四步和第五步*****//
   //RTC控制寄存器低位(RTC_CRL)，位4，CNF置1，进入配置模式//
   RTC_EnterConfigMode();                    
   RTC_SetPrescaler(195312);                           //设置RTC预分频的值//
   RTC_WaitForLastTask();                             //等待最近一次对RTC寄存器的写操作完成//
   RTC_Set(2015,1,14,17,42,55);                       //设置时间，纯c语言代码// 
   //RTC控制寄存器低位(RTC_CRL)，位4，CNF置0，退出配置模式//
   RTC_ExitConfigMode();                      
   //写入BKP_DR1寄存器0~15位的值//
   BKP_WriteBackupRegister(BKP_DR1, 0X5050);          //向指定的后备寄存器中写入用户程序数据//
  }
 else                                                 //如果，不是第一次配置，系统继续计时//
  {
    RTC_WaitForSynchro();                             //等待寄存器同步// 
    RTC_ITConfig(RTC_IT_SEC | RTC_IT_ALR, ENABLE);         
    RTC_WaitForLastTask();                            //等待最近一次对RTC寄存器的写操作完成//
  }
 RTC_NVIC_Config();                                   //RCT中断分组设置//               
 RTC_Get();                                           //更新时间//
 return 0;                                            //ok//
}           
//*****第七步，编写中断服务函数*****//
//秒中断发生，更新时间，闹钟中断发生，更新时间并向串口发送时间//
void RTC_IRQHandler(void)
{   
 if(RTC_GetITStatus(RTC_IT_ALR)!= RESET)              //判断闹钟中断是否发生//
 {
    //RTC控制寄存器低位(RTC_CRL)，位1，ALRF置0，清闹钟中断// 
    RTC_ClearITPendingBit(RTC_IT_ALR);           
    RTC_Get();                                          //更新时间//  
    printf("alarm time:%d-%d-%d %d:%d:%d\r\n",calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,calendar.min,calendar.sec);
    BEEP=1;
    delay_ms(1000);
    BEEP=0;      
 }   
 if (RTC_GetITStatus(RTC_IT_SEC) != RESET)            //判断秒钟中断是否发生//
 {       
  RTC_Get(); 
  printf("alarm time:%d-%d-%d %d:%d:%d\r\n",calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,calendar.min,calendar.sec); 
 }   
            
 RTC_ClearITPendingBit(RTC_IT_SEC|RTC_IT_OW);         //清闹钟中断和溢出标志//
 RTC_WaitForLastTask();                               //等待最近一次对RTC寄存器的写操作完成//    
}
//判断是否是闰年函数，能被4整除，但不能被100整除的年是闰年，能被400整除的年是闰年//
//月份        1  2  3  4  5  6  7  8  9  10 11 12
//闰年        31 29 31 30 31 30 31 31 30 31 30 31
//非闰年/平年 31 28 31 30 31 30 31 31 30 31 30 31
//输入:年份
//输出:该年份是不是闰年.1,是.0,不是
u8 Is_Leap_Year(u16 year)
{     
 if(year%4==0)                              //判断是否能被4整除，能则继续；不能则不是闰年，且返回0//
 { 
  if(year%100==0)                           //判断是否能被100整除,能则继续；不能则是闰年，且返回1// 
  { 
   if(year%400==0)return 1;                 //判断是否能被400整除，能则是闰年，且返回1；不能则不是闰年，且返回0//    
   else return 0;   
  }else return 1;   
 }else return 0; 
}        
//设置时钟，把输入的时钟转换为秒钟，以1970年1月1日为基准，1970~2099年为合法年份//
//并将值传至RTC计数器寄存器高位(RTC_CNTH)和RTC计数器寄存器低位(RTC_CNTL)中// 
//返回值:0,成功;其他:错误代码//           
//平年的月份日期表，闰年的话，就把28改29//
const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};
u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec)
{
 u16 t;
 u32 seccount=0;
 if(syear<1970||syear>2099)return 1;               //如果年份不在1970~2099年之内，则报错，函数停止//
 for(t=1970;t<syear;t++)                           //把所有年份的秒钟相加//
 {
  if(Is_Leap_Year(t))seccount+=31622400;           //闰年的秒钟数//
  else seccount+=31536000;                         //平年的秒钟数，少一天//
 }
 smon-=1;
 for(t=0;t<smon;t++)                               //把前面月份的秒钟数相加//
 {
  seccount+=(u32)mon_table[t]*86400;               //月份秒钟数相加//
  if(Is_Leap_Year(syear)&&t==1)seccount+=86400;    //闰年2月份增加一天的秒钟数 //   
 }
 seccount+=(u32)(sday-1)*86400;                    //把前面日期的秒钟数相加// 
 seccount+=(u32)hour*3600;                         //小时秒钟数//
 seccount+=(u32)min*60;                            //分钟秒钟数//
 seccount+=sec;                                    //最后的秒钟加上去//
 //以上，已经把某年某月某日某时某分某秒的时间，用秒计数起来//
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE); //使能PWR和BKP外设时钟//  
 PWR_BackupAccessCmd(ENABLE);                                             //使能RTC和BKP访问//
 RTC_SetCounter(seccount);                                                //设置RTC计数器的值//
 RTC_WaitForLastTask();    
 return 0;     
}
//初始化闹钟，以1970年1月1日为基准，1970~2099年为合法年份，syear,smon,sday,hour,min,sec：年月日时分秒//   
//返回值:0,成功;其他:错误代码.//
u8 RTC_Alarm_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec)
{
 u16 t;
 u32 seccount=0;
 if(syear<1970||syear>2099)return 1;    
 for(t=1970;t<syear;t++)                        //把所有年份的秒钟相加//
 {
  if(Is_Leap_Year(t))seccount+=31622400;        //闰年的秒钟数//
  else seccount+=31536000;                      //平年的秒钟数//
 }
 smon-=1;
 for(t=0;t<smon;t++)                            //把前面月份的秒钟数相加//
 {
  seccount+=(u32)mon_table[t]*86400;            //月份秒钟数相加//
  if(Is_Leap_Year(syear)&&t==1)seccount+=86400; //闰年2月份增加一天的秒钟数//    
 }
 seccount+=(u32)(sday-1)*86400;                 //把前面日期的秒钟数相加//
 seccount+=(u32)hour*3600;                      //小时秒钟数//
    seccount+=(u32)min*60;                      //分钟秒钟数//
 seccount+=sec;                                 //最后的秒钟加上去//       
 //设置时钟//
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);  //使能PWR和BKP外设时钟//   
 PWR_BackupAccessCmd(ENABLE);                                              //使能RTC和BKP访问//  
 RTC_SetAlarm(seccount);                                                   //设置RTC闹钟寄存器值//
 RTC_WaitForLastTask();   
 return 0;     
}
//得到当前的时间，将秒转换成年月日时分秒//
//返回值:0,成功;其他:错误代码.//
u8 RTC_Get(void)
{
 static u16 daycnt=0;
 u32 timecount=0; 
 u32 temp=0;
 u16 temp1=0;   
  timecount=RTC_GetCounter();   //获取16位RTC计数器寄存器高位RTC_CNTH和低位RTC_CNTL的值，返回32位数据//
  temp=timecount/86400;         //得到天数//
 if(daycnt!=temp)               //超过一天了
 {   
  daycnt=temp;
  temp1=1970;                   //从1970年开始//
  while(temp>=365)              //当temp≥365天时//
  {     
   if(Is_Leap_Year(temp1))      //闰年判断，闰年366天，平年365天//
   {
    if(temp>=366)
     temp-=366;                 //闰年，temp减366天//
    else break;                 //当年是闰年，且是365天时，跳出循环//
   }
   else  temp-=365;             //平年，temp减365天// 
   temp1++;                     //平年，temp1加1年//
  }   
  calendar.w_year=temp1;        //得到年份temp1//
  temp1=0;                      //temp1至0，开始计算月份//
  while(temp>=28)               //当28≤temp＜365天时//
  {
   //temp1从0至11分别对应1月到12月//
   if(Is_Leap_Year(calendar.w_year)&&temp1==1)         //当是闰年，且为2月份时，执行下面语句//
   {
    if(temp>=29)temp-=29;                              
    else break; 
   }
   else                                                //平年，执行else//
   {
    if(temp>=mon_table[temp1])                         
     temp-=mon_table[temp1];                          
    else break;
   }
   temp1++;  
  }
  calendar.w_month=temp1+1;                            //得到月份//
  calendar.w_date=temp+1;                              //得到日期// 
 }
 temp=timecount%86400;                                 //得到秒钟数//      
 calendar.hour=temp/3600;                              //小时//
 calendar.min=(temp%3600)/60;                          //分钟// 
 calendar.sec=(temp%3600)%60;                          //秒钟//
 calendar.week=RTC_Get_Week(calendar.w_year,calendar.w_month,calendar.w_date); //获取星期//   
 return 0;
}  
//获得现在是星期几
//功能描述:输入公历日期得到星期(只允许1901-2099年)
//输入参数：公历年月日 
//返回值：星期号  
u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5};        //月修正数据表?//
u8 RTC_Get_Week(u16 year,u8 month,u8 day)
{ 
 u16 temp2;
 u8 yearH,yearL;
 yearH=year/100; yearL=year%100; 
 //如果为21世纪,年份数加100//  
 if (yearH>19)
  yearL+=100;
 //所过闰年数只算1900年之后的//  
 temp2=yearL+yearL/4;
 temp2=temp2%7; 
 temp2=temp2+day+table_week[month-1];
 if (yearL%4==0&&month<3)temp2--;
 return(temp2%7);
}
  


