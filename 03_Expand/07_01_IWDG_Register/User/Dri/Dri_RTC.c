#include "Dri_RTC.h"
#include "Dri_USART1.h"

void Dri_RTC_Init(void)
{
    // 1 备份域的配置 -------------------------------------------------------
    // 1.1 对 PWR BKP 时钟使能 (PWR:用于管理芯片的电源和低功耗模式  BKP:用于在主电源断电或系统复位时保存关键数据和维持实时时钟（RTC）运行)
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    RCC->APB1ENR |= RCC_APB1ENR_BKPEN;
    // 1.2 使能对备份寄存器的访问 (关闭备份寄存器的写保护)
    PWR->CR |= PWR_CR_DBP;
    // 1.3 复位备份寄存器 (使用日历功能时需要注释掉复位)
    // RCC->BDCR |= RCC_BDCR_BDRST;
    // Com_Delay_ms(10);
    // RCC->BDCR &= ~RCC_BDCR_BDRST;
    // Com_Delay_ms(10);

    // 2 RTC 时钟的配置 -------------------------------------------------------
    // 2.1 使能 LSE 时钟
    RCC->BDCR |= RCC_BDCR_LSEON;
    // 2.2 等待 LSE 就绪
    while((RCC->BDCR & RCC_BDCR_LSERDY) == 0){
    }
    // 2.3 选择 LSE 作为 RTC 时钟 (RTC 时钟源选择)
    RCC->BDCR &= ~RCC_BDCR_RTCSEL;
    RCC->BDCR |= RCC_BDCR_RTCSEL_LSE;
    // RCC->BDCR &= ~RCC_BDCR_RTCSEL_1;
    // RCC->BDCR |= RCC_BDCR_RTCSEL_0;
    // 2.4 对 RTC 时钟使能
    RCC->BDCR |= RCC_BDCR_RTCEN;

    // 3 RTC 寄存器的配置 -------------------------------------------------------
    // 3.1 等待上一步使能操作完成
    while((RTC->CRL & RTC_CRL_RTOFF) == 0){
    }
    // 3.2 使能 RTC 寄存器的写操作 (进入配置模式)
    RTC->CRL |= RTC_CRL_CNF;
    // 3.3 设置 RTC 寄存器的预分频: 32768分频 ,实现每1s 计1个数
    RTC->PRLH = (32768 - 1) >> 16;
    RTC->PRLL = (32768 - 1) & 0xFFFF;

    // 3.4 退出配置模式 ,正式执行写操作
    RTC->CRL &= ~RTC_CRL_CNF;
    // 3.5 等待上一步使能操作完成
    while((RTC->CRL & RTC_CRL_RTOFF) == 0){
    }
}

void Dri_RTC_SetAlarm(uint32_t seconds)
{
    // 0 等待上一步操作完成（RTOFF位置1表示上一步操作完成）
    while ((RTC->CRL & RTC_CRL_RTOFF) == 0)
        ;
    
    // 1 使能 RTC 寄存器的写操作 (进入配置模式)
    RTC->CRL |= RTC_CRL_CNF;

    // 2 设置 RTC 寄存器的闹钟时间
    RTC->ALRH = (seconds - 1) >> 16;
    RTC->ALRL = (seconds - 1) & 0xFFFF;

    // 3 退出配置模式 ,正式执行写操作
    RTC->CRL &= ~RTC_CRL_CNF;

    // 4 等待上一步使能操作完成
    while((RTC->CRL & RTC_CRL_RTOFF) == 0){
    }
}

void Dri_RTC_SetTime(DataTime_t datatime)
{
    // 将日期时间处理为时间戳
    struct tm tm_datatime;
    tm_datatime.tm_year = datatime.year - 1900;
    tm_datatime.tm_mon = datatime.month - 1;
    tm_datatime.tm_mday = datatime.day;
    tm_datatime.tm_hour = datatime.hour;
    tm_datatime.tm_min = datatime.minute;
    tm_datatime.tm_sec = datatime.second;
    tm_datatime.tm_isdst = 0;
    time_t timestamp = mktime(&tm_datatime);    // 将时间结构体转换为时间戳

    // 0 等待上一步操作完成（RTOFF位置1表示上一步操作完成）
    while ((RTC->CRL & RTC_CRL_RTOFF) == 0)
        ;
    // 1 使能 RTC 寄存器的写操作 (进入配置模式)
    RTC->CRL |= RTC_CRL_CNF;
    // 2 设置 RTC 寄存器的时间
    RTC->CNTH = timestamp >> 16;
    RTC->CNTL = timestamp & 0xFFFF;
    // 3 退出配置模式 ,正式执行写操作
    RTC->CRL &= ~RTC_CRL_CNF;
    // 4 等待上一步使能操作完成
    while((RTC->CRL & RTC_CRL_RTOFF) == 0){
    }
}

DataTime_t Dri_RTC_GetTime(void)
{
    // 0 等待寄存器同步完成
    while ((RTC->CRL & RTC_CRL_RSF) == 0)
    {
    }

    // 1 读取 RTC 寄存器的时间
    time_t timestamp = (RTC->CNTH << 16) | RTC->CNTL;

    // 2 将时间戳转换为时间结构体
    struct tm *tm_datatime = localtime(&timestamp);

    // 3 从时间结构体中提取日期时间信息
    DataTime_t datatime;
    datatime.year = tm_datatime->tm_year + 1900;
    datatime.month = tm_datatime->tm_mon + 1;
    datatime.day = tm_datatime->tm_mday;
    datatime.hour = tm_datatime->tm_hour;
    datatime.minute = tm_datatime->tm_min;
    datatime.second = tm_datatime->tm_sec;
    
    return datatime;
}
