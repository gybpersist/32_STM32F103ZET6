## 第 3 章 RTC 实时时钟

### 3.1 RTC 概述

RTC（Real Time Clock，实时时钟），是一个**掉电**后仍然可以继续**运行**的**独立定时器**。

RTC模块拥有一个连续计数的计数器，在相应的软件配置下，可以提供时钟日历的功能。修改计数器的值可以重新设置当前时间和日期。RTC还包含用于管理低功耗模式的自动唤醒单元。

### 3.2 RTC 功能框图和功能描述

![RTC.png](https://p.sda1.dev/22/c27fe8a5310d38214f66dbae72ce40cb/RTC.png)

##### RTC 时钟来源：

![RTC-Clock.png](https://p.sda1.dev/24/e22da25c6f05392e80c5332bb2462470/RTC-Clock.png)

RTC 有 3 路时钟来源：HSE（8MHz）/128、LSE（32.768KHz）、LSI（40KHz）。

如果使用 HSE 或 LSI ，主电源掉电的话，这两个始终都会受到影响，RTC无法正常工作，所以，一般的通用做法是使用 **LSE**。  

##### RTC 预分频模块

RTC 预分频模块，属于后备区域，VDD 掉电后，可以在 VBAT 下继续运行。

包含了一个 20 位的可编程分频器（RTC 预分频器）。它可编程产生 1 秒的 RTC 时间基准 TR_CLK。如果在RTC_CR 寄存器中设置了相应的允许位，则在每个 TR_CLK 周期中 RTC 产生一个中断（秒中断）。

##### 32位可编程计数器

这个模块属于后备区域，是一个 32 位的可编程计数器，可被初始化为当前的系统时间。

32 位的时钟计数器，按秒计算，可以记录 4294967296 秒，约合 136 年左右，作为一般应用，这是足够的。

##### 中断

包括三个中断：

```
1. 秒中断：每计时 1s 产生一次中断。
2. 计数器溢出中断：136 年才会产生溢出。
3. RTC 闹钟中断：RCT_CN 和 RTC_ALR 比较，如果相等表示闹钟时间到，产生闹钟中断。
```

![%E4%B8%AD%E6%96%AD.png](https://p.sda1.dev/22/b0486d72352e7fff8e385fc11a9867ea/中断.png)

### 3.3 RTC 相关寄存器

##### RCC 寄存器

| 寄存器名称  | 描述             | 相关控制位/标志位                                            |
| ----------- | ---------------- | ------------------------------------------------------------ |
| `RCC->BDCR` | 备份域控制寄存器 | BDRST:  BKP和RTC复位<br>RTCEN: RTC 时钟使能<br>RTCSEL: RTC 时钟源选择<br>LSEON: LSE 使能<br>LSERDY: LSE 就绪标志位 |


##### RTC 寄存器

| 寄存器名称  | 描述                 | 相关控制位/标志位                                            |
| ----------- | -------------------- | ------------------------------------------------------------ |
| `RTC->CRH`  | 控制寄存器高位       | OWIE: 允许溢出中断<br>ALEIE: 允许闹钟中断<br>SECIE: 允许秒中断 |
| `RTC->CRL`  | 控制寄存器低位       | RTOFF: RTC操作完成标志位<br>CNF: 置1进入配置模式<br>RSF: RTC 相关寄存器已同步标志位<br>OWF: 溢出标志位<br>ALRF: 闹钟标志位<br>SECF: 秒标志位 |
| `RTC->PRLH` | 预分频装载寄存器高位 | [19:16]                                                      |
| `RTC->PRLL` | 预分频装载寄存器地位 | [15:0]                                                       |
| `RTC->DIVH` | 预分频余数寄存器高位 | [19:16]                                                      |
| `RTC->DIVL` | 预分频余数寄存器地位 | [15:0]                                                       |
| `RTC->CNTH` | 计数寄存器高位       | [31:16]                                                      |
| `RTC->CNTL` | 计数寄存器地位       | [15:0]                                                       |
| `RTC->ALRH` | 闹钟寄存器高位       | [31:16]                                                      |
| `RTC->ALRL` | 闹钟寄存器地位       | [15:0]                                                       |

##### RTC 寄存器的复位

RTC_PRL、RTC_ALR、RTC_CNT 和 RTC_DIV 寄存器仅能通过备份域复位信号（RCC_BDCR 寄存器中的 BDRST 位）复位，其他寄存都由系统复位或电源复位进行异步复位。

##### 读 RTC 寄存器

以下情况，软件在读取 RTC 寄存器前，必须检查 `RTC_CRL`寄存器 的 RSF 标志位，需等待 **RSF** 置1以避免读取错误数据。

```
1. 系统复位或电源复位
2. 从 Standby/Stop 模式唤醒
3. APB1时钟曾被关闭
```

##### 写 RTC 寄存器

必须设置 `RTC_CRL` 寄存器中的 `CNF` 位，使 RTC 进入配置模式后，才能写入 `RTC_PRL`、`RTC_CNT`、`RTC_ALR` 寄存器。

另外，对RTC任何寄存器的写操作，都必须在前一次写操作结束后进行。可以通过查询 `RTC_CR` 寄存器中的 RTOFF 标志位，判断 RTC 寄存器是否处于更新中。

写 RTC 寄存器的过程如下：

```
1. 查询 RTOFF 位，直到 RTOFF 的值变为1 
2. 将 CNF 位置1，进入配置模式
3. 对一个或多个 RTC 寄存器进行写操作
4. 清除CNF标志位，退出配置模式
5. 查询 RTOFF，直至RTOFF位变为1以确认写操作已经完成。
```

### 4.3 RTC案例1-闹钟唤醒待机

#### ① 需求分析

MCU进入待机模式，设置闹钟，自动让MCU从待机模式中被唤醒。可以用LED点亮熄灭显示是否唤醒。

应用场景：比如设计一个野外温度自动采集的设备，规定每小时采集一次温度，就可以定义一个1小时的闹钟，定时唤醒，采集温度，采集完进入待机模式，可以大大降低设备功耗。

#### ② 寄存器方式实现

**Dri_RTC.c**

```c
#include "Dri_RTC.h"
#include <stdio.h>

/**
 * @brief RTC 初始化
 */
void Dri_RTC_Init(void)
{
    // 1. 备份域配置 ------------------------------------
    // 1.1 对 PWR、BKP 时钟使能
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    RCC->APB1ENR |= RCC_APB1ENR_BKPEN;
    // 1.2 关闭备份域写保护
    PWR->CR |= PWR_CR_DBP;
    // 1.3 复位备份域
    RCC->BDCR |= RCC_BDCR_BDRST;
    Delay_ms(10);
    RCC->BDCR &= ~RCC_BDCR_BDRST;
    Delay_ms(10);
     // 1.4 对 RTC 时钟使能(在备份域复位之后)
    RCC->BDCR |= RCC_BDCR_RTCEN;


    // 2. RTC 时钟配置 ---------------------------------
    // 2.1 使能 LSE
    RCC->BDCR |= RCC_BDCR_LSEON;
    // 2.2 等待 LSE 稳定
    while ((RCC->BDCR & RCC_BDCR_LSERDY) == 0)
        ;
    // 2.3 选择 LSE 作为 RTC 时钟 (RCC_BDCR.RTCSEL = 01)
    RCC->BDCR &= ~RCC_BDCR_RTCSEL_1;
    RCC->BDCR |= RCC_BDCR_RTCSEL_0;

    // 3. RTC 配置 -------------------------------------
    // 3.1 等待上一步操作完成（RTOFF位置1表示上一步操作完成）
    while ((RTC->CRL & RTC_CRL_RTOFF) == 0)
        ;
    // 3.2 进入配置模式（CNF位置1）
    RTC->CRL |= RTC_CRL_CNF;
    // 3.3 设置 RTC 预分进行 32768 预分频，实现1s计1个数，将 32767 存入 PRLH、PRLL 寄存器
    RTC->PRLH = 32767 >> 16;
    RTC->PRLL = 32767 & 0xFFFF;
    // 3.4 退出配置模式，正式执行写操作
    RTC->CRL &= ~RTC_CRL_CNF;
    // 3.5 等待上一步操作完成（RTOFF位置1表示上一步操作完成）
    while ((RTC->CRL & RTC_CRL_RTOFF) == 0)
        ;
}

/**
 * @brief 设置闹钟时间，多少秒后产生闹钟事件
 * @param 时间单位是秒
 */
void Dri_RTC_SetAlarm(uint32_t seconds)
{
    // 1. 等待上一步操作完成（RTOFF位置1表示上一步操作完成）
    while ((RTC->CRL & RTC_CRL_RTOFF) == 0)
        ;
    // 2. 进入配置模式（CNF位置1）
    RTC->CRL |= RTC_CRL_CNF;
    // 3. 设置闹钟时间，将秒数存入 ALRH、ALRL 寄存器
    RTC->ALRH = (seconds - 1) >> 16;
    RTC->ALRL = (seconds - 1) & 0xFFFF;
    // 4. 退出配置模式，正式执行写操作
    RTC->CRL &= ~RTC_CRL_CNF;
    // 5. 等待上一步操作完成（RTOFF位置1表示上一步操作完成）
    while ((RTC->CRL & RTC_CRL_RTOFF) == 0)
        ;
}
```

**main.c**

> **注意：** 如果使用 RTC 闹钟唤醒待机，需要关闭 WKUP 引脚的使能。

```c
int main()
{
	// USART1 初始化
	Dri_USART1_Init();
	// 打印信息
	// printf("STM32 RTC Alarm Example: \n\n");
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	if (PWR->CSR & PWR_CSR_WUF)
	{	
		printf("Wakeup from Standby Mode \n");
		PWR->CR |= PWR_CR_CWUF;
	}
	else
	{
		printf("Application Start \n");
	}

	
	// LED 初始化
	Int_LED_Init();
	// 点亮 LED3
	Int_LED_On(LED3);

	printf("3s 之后进入Standby模式 ...\n");
	Delay_ms(3000);
	
	// RTC 初始化
	Dri_RTC_Init();
	// 设置闹钟时间，5s后产生闹钟事件
	Dri_RTC_SetAlarm(5);

	// 进入Standby模式
	printf("进入Standby模式 ... \n");
	printf("5s之后唤醒 ...\n");
	Enter_StandbyMode();

	// 死循环
	while (1)
		;
}
```

#### ③ HAL 方式实现

**STM32CubeMX 配置：**

![Example01-HAL.png](https://p.sda1.dev/22/51df5a52e7f443af7d56e72c6cc72794/Example01-HAL.png)

**相关HAL库函数：**

````
HAL_RTC_GetTime()
HAL_RTC_SetAlarm()

RTC_TimeTypeDef
RTC_AlarmTypeDef
````

**代码：**

```c
// 函数原型：设置RTC闹钟，多少秒之后产生闹钟事件
void RTC_SetAlarm(uint32_t seconds);


int main(void)
{

  /* 前面代码省略... */
    
  /* USER CODE BEGIN 2 */
  // 点亮LED2（蓝灯）
  HAL_GPIO_WritePin(GPIOA, LED2_Pin, GPIO_PIN_RESET);

  // printf("STM32 HAL Lowpower StandbyMode Example: \n");

  // 判断 WUF 标志位
  if (__HAL_PWR_GET_FLAG(PWR_FLAG_WU) == SET)
  {
    printf("Wakeup from Standby Mode \n");
    // 清除 WUF 标志位
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
  }
  else
  {
    printf("Application Start \n");
  }

  printf("5000ms 后进入 Standby 模式... \n");
  HAL_Delay(5000); 

  // 进入 Standby 模式前设置 RTC 闹钟， 5s之后产生闹钟事件
  RTC_SetAlarm(5);

  // 进入 Standby 模式
  printf("进入 Standby 模式... \n");
  printf("5s 之后唤醒... \n");
  HAL_PWR_EnterSTANDBYMode();
  /* USER CODE END 2 */
	
  /* 后面代码省略... */
}

// 函数实现：设置RTC闹钟，多少秒之后产生闹钟事件
void RTC_SetAlarm(uint32_t seconds)
{
  // 获取当前RTC计数器的时间
  RTC_TimeTypeDef sTime;
  HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);

  // 设置RTC闹钟时间
  sTime.Seconds += (seconds - 1);
  // 定义结构体变量，用于存储RTC闹钟时间
  RTC_AlarmTypeDef sAlarm = {sTime, 1};
  // 设置RTC闹钟
  HAL_RTC_SetAlarm(&hrtc, &sAlarm, RTC_FORMAT_BIN);
}
```



### 4.2 RTC 案例2-实时时钟日历

#### ① 需求分析

显示时间。通过串口把时间发送给电脑显示，即使关机很多天，再启动后也能正确显示时间。

#### ② 寄存器方式实现

**Dri_RTC.c 添加如下函数：**

> **注意：** RTC_Init() 函数中要去掉备份域复位操作！

```c
#include <time.h>

// 定义日期时间的类型结构体 /* 可在 Dri_RTC.h 中定义 */
typedef struct
{
    uint16_t year;   // 年，范围：2000-2099/  
    uint8_t month;   // 月，范围：1-12
    uint8_t day;     // 日，范围：1-31   
    uint8_t hour;    // 时，范围：0-23   
    uint8_t minute;  // 分，范围：0-59
    uint8_t second;  // 秒，范围：0-59
} Def_Datetime_Struct;

/* 省略 void Dri_RTC_Init(void) 部分的代码 */

/**
 * @brief 设置起始时间
 * @param dt 日期时间的结构体变量
 */
void Dri_RTC_SetTime(Def_Datetime_Struct dt)
{
    // 将日期时间处理为时间戳
    struct tm tm_dt;
    tm_dt.tm_year = dt.year - 1900;
    tm_dt.tm_mon = dt.month - 1;
    tm_dt.tm_mday = dt.day;
    tm_dt.tm_hour = dt.hour;
    tm_dt.tm_min = dt.minute;
    tm_dt.tm_sec = dt.second;
    tm_dt.tm_isdst = 0; // 夏令时标志，0 表示不使用夏令时
    // 生成时间戳
    time_t timestamp = mktime(&tm_dt);

    // --------------------------------------------
    // 1. 等待上一步操作完成（RTOFF位置1表示上一步操作完成）
    while ((RTC->CRL & RTC_CRL_RTOFF) == 0)
        ;
    // 2. 进入配置模式（CNF位置1）
    RTC->CRL |= RTC_CRL_CNF;
    // 3. 将时间戳写入CNT寄存器
    RTC->CNTH = timestamp >> 16;
    RTC->CNTL = timestamp & 0xFFFF;
    // 4. 退出配置模式，正式执行写操作
    RTC->CRL &= ~RTC_CRL_CNF;
    // 5. 等待上一步操作完成（RTOFF位置1表示上一步操作完成）
    while ((RTC->CRL & RTC_CRL_RTOFF) == 0)
        ;
}

/**
 * @brief 获取当前时间
 * @return Def_Datetime_Struct 当前日期时间
 */
Def_Datetime_Struct Dri_RTC_GetTime(void)
{
    // 1. 等待寄存器同步完成
    while ((RTC->CRL & RTC_CRL_RSF) == 0)
        ;

    // 2. 读取CNT寄存器的值
    time_t timestamp = (RTC->CNTH << 16) | RTC->CNTL;

    // 将时间戳转为 tm 类型结构体
    struct tm *tm_dt = localtime(&timestamp);

    // 定义 Def_Datetime_Struct 结构体变量
    Def_Datetime_Struct dt;
    dt.year = tm_dt->tm_year + 1900;
    dt.month = tm_dt->tm_mon + 1;
    dt.day = tm_dt->tm_mday;
    dt.hour = tm_dt->tm_hour;
    dt.minute = tm_dt->tm_min;
    dt.second = tm_dt->tm_sec;

    return dt;
}
```

**main.c**

```c

int main()
{
	// USART1 初始化
	Dri_USART1_Init();
	// 打印信息
	printf("STM32 RTC Calendar Example: \n\n");
	
	// RTC 初始化
	Dri_RTC_Init();
	
	// 设置起始的RTC时间
	// Def_Datetime_Struct start_datetime = {2025, 7, 30, 10, 25, 0};
	// Dri_RTC_SetTime(start_datetime);

	// 死循环
	while (1)
	{
		// 获取当前的日期时间
		Def_Datetime_Struct dt = Dri_RTC_GetTime();
		// 打印日期时间
		printf("%d-%02d-%02d %02d:%02d:%02d \n", dt.year, dt.month, dt.day, dt.hour, dt.minute, dt.second);
		// 延时
		Delay_s(1);
	}
}

```

#### ③ HAL 库方式实现

**STM32CubeMX 配置：**

![Example02-HAL.png](https://p.sda1.dev/24/c8550afe83e9edb0eb1e1fb38d293fcf/Example02-HAL.png)

**相关HAL库函数：**

```
HAL_RTC_SetDate()
HAL_RTC_SetTime()
HAL_RTC_GetDate()
HAL_RTC_GetTime()

RTC_DateTypeDef		日期类型结构体
RTC_TimeTypeDef		时间类型结构体
```

**注意事项：**

```
1. HAL 中，只有时间部分在 RTC 中计数，日期部分用的变量进行计数(位于RAM)，复位、断电之后，只有时间部分能保持
2. 获取日期时间的时候，先获取时间，再获取日期，原因是获取时间的时候可以锁定此时此刻的日期，避免日期交替时获取错误，
```

**代码：**

```c
int main(void)
{

  /* 其他代码省略 */
    
  /* USER CODE BEGIN 2 */
  printf("STM32 HAL RTC Calender Example: \n");

  // 定义RTC日期的结构体变量
  RTC_DateTypeDef sDate;
  // 定义RTC时间的结构体变量
  RTC_TimeTypeDef sTime;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

    // 获取RTC时间
    HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);

    // 获取RTC日期
    HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

    // 打印RTC日期和时间
    printf("%d-%02d-%02d %02d:%02d:%02d \n", sDate.Year + 2000, sDate.Month, sDate.Date, sTime.Hours, sTime.Minutes, sTime.Seconds);

    // 延时 1000 ms
    HAL_Delay(1000);
  }
  /* USER CODE END 3 */
}
```

**程序运行一次之后，`MX_RTC_Init` 函数中时间设置的部分需要注释掉：**

```c
RTC_TimeTypeDef sTime = {0};

/** Initialize RTC and set the Time and Date
*/
sTime.Hours = 11;
sTime.Minutes = 29;
sTime.Seconds = 0;

if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
{
   Error_Handler();
}
```



## 第 5 章 看门狗

### 5.1 看门够介绍

#### ① 概述

看门狗是一种**计时硬件电路**。

当系统环境比较恶劣复杂的时候，程序可能会出现一些不可预料的错误，导致程序卡死，崩溃等情况。在这种情况下，看门狗可以及时的重置程序，使程序重新从头开始运行。

STM32 有2个看门狗，**独立看门狗**和**窗口看门狗**。独立看门狗号称宠物狗，窗口看门狗号称警犬。

#### ② 独立看门狗

IWDG（Independent watchdog，独立看门狗）。独立看门狗用通俗一点的话来解释就是一个 **12** **位**的**递减**计数器，当计数器的值从某个值一直减到 0 的时候，系统就会产生一个复位信号，即 IWDG_RESET。如果在计数减到 0 之前，刷新了计数器的值的话，那么就不会产生复位信号，这个刷新计数器值的动作就是我们经常说的**喂狗**。

![IWDG%E6%A1%86%E5%9B%BE.png](https://p.sda1.dev/22/7a6042fd848d1ccf9732c0c8da1578c3/IWDG框图.png)

#### ③ 窗口看门狗

WWDG（Window watchdog，窗口看门狗）。窗口看门狗跟独立看门狗一样，也是一个递减计数器不断的往下递减计数。

当计数器减到一个固定值 0x40（T6位减到0）时还不喂狗的话，产生复位，这个值叫窗口的**下限**，是固定的值，不能改变。这个是跟独立看门狗类似的地方。

不同的地方是窗口看门狗的计数器的值在减到某一个数之前喂狗的话也会产生复位，这个值叫窗口的上限，**上限值由用户**独立设置。窗口看门狗计数器的值必须在**上窗口和下窗口之间**才可以喂狗，喂早了喂晚了都不行。这就是窗口看门狗中窗口两个字的含义。

![WWDG%E6%A1%86%E5%9B%BE.png](https://p.sda1.dev/22/9990aa3f2c7d5bfa9a01536b1f987f8b/WWDG框图.png)

#### ④ 独立看门狗和窗口看门狗区别

|                | **独立看门狗**                    | **窗口看门狗**                    |
| -------------- | --------------------------------- | --------------------------------- |
| **复位**       | 计数减到0                         | 计数器T[5,0]减到0和过早重装计数器 |
| **喂狗方式**   | 键寄存器写入0xAAAA，重装固定值RLR | 直接写入寄存器，写多少重装多少    |
| **中断**       | 无                                | 死前中断                          |
| **时钟源**     | LSI（40KHz）                      | PCLK1（36MHz）                    |
| **预分频系数** | 4，8 ，32， 64 ，128， 256        | 1，2， 4， 8                      |
| **计数器**     | 12位                              | 6位                               |
| **超时时间**   | 0.1ms到26214.4ms                  | 113us到58.25ms                    |
| **用途**       | 独立工作，对时间精度要求比较低    | 要求看门狗在计时窗口起作用        |

### 5.2 独立看门狗相关寄存器

| 寄存器名称  | 描述             | 相关控制位/标志位               |
| ----------- | ---------------- | ------------------------------- |
| `IWDG->KR`  | 键寄存器（只写） | KEY[15:0]                       |
| `IWDG->PR`  | 预分频寄存器     | PR: 预分配因子，3个bit          |
| `IWDG->RLR` | 重装载值         | RL[11:0]                        |
| `IWDG->SR`  | 状态寄存器       | RVU: RLR值更新<br>PVU: PR值更新 |

**KR 寄存器：**

```
1. 喂狗：必须以一定的间隔写入 0xAAAA（喂狗），否则，当计数器为0时，看门狗会产生复位。
2. 写入0x5555表示允许访问IWDG_PR和IWDG_RLR寄存器。 
3. 写入0xCCCC，启动看门狗工作
```

**预分配因子：**

```
000: 预分频因子=4 
001: 预分频因子=8 
010: 预分频因子=16 
011: 预分频因子=32 
100: 预分频因子=64 
101: 预分频因子=128 
110: 预分频因子=256 
111: 预分频因子=256 
```

### 5.3 独立看门狗案例

#### ① 需求分析

验证独立看门狗是否能复位程序。假设程序正常执行需要3s，执行完毕之后喂狗，我们把超时时间设置为4s。当检测到按键1按下后，我们再延长3s，那么喂狗超时，就会产生复位。

#### ② 寄存器方式实现

**Dri_IWDG.c**

```c
/**
 * @brief 初始化独立看门狗
 */
void Dri_IWDG_Init(void)
{
    // 1. 写入Key，允许写入IWDG_PR和IWDG_RLR寄存器
    IWDG->KR = 0x5555;

    // 2. 设置预分频值，进行64分频，实现1.6ms计一个数，PR.PR[2:0] = 100
    IWDG->PR = 0x0004;

    // 3. 设置重装载值，实现4s之后触发复位，重装载值需要设置为2500
    IWDG->RLR = 2500;

    // 4. 写入Key，启动独立看门狗
    IWDG->KR = 0xCCCC;
}

/**
 * @brief 刷新IWDG计数器（喂狗）
 */
void Dri_IWDG_Refresh(void)
{
    // 写入Key，刷新独立看门狗
    IWDG->KR = 0xAAAA;
}
```

**main.c**

```c
int main()
{
	// USART1 初始化
	Dri_USART1_Init();
	// 打印信息
	printf("STM32 IWDG Example: \n\n");

	// 初始化按键
	Int_Key_Init();
	
	// 初始化独立看门狗
	Dri_IWDG_Init();

	// 死循环
	while (1)
	{
		// 正常代码
		printf("正常代码...\n");

		// 喂狗
		Dri_IWDG_Refresh();

		// 延时2s
		Delay_s(2);
	}
}

// 重定义SW5按键被按下后调用的弱函数
void Int_Key_SW5Callback(void)
{
	printf("SW5按键被按下...\n");
	Delay_s(5);
}
```



#### ③ HAL 库方式实现

**STM32CubeMX配置：**

![IWDG-HAL.png](https://p.sda1.dev/22/50fe73a2fda1180f2ee6f139c87dc7cc/IWDG-HAL.png)

**HAL库相关函数：**

```
HAL_IWDG_Refresh()   
```

**代码：**

```c
/* USER CODE BEGIN 0 */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == SW5_Pin)
  {
    // 延时10ms，用于消抖
    HAL_Delay(10);
    // 再次判断引脚是否是高电平
    if (HAL_GPIO_ReadPin(SW5_GPIO_Port, SW5_Pin) == GPIO_PIN_SET)
    {
      printf("SW5 按键按下...\n");
      // 延时5s，阻挠喂狗
      HAL_Delay(5000); 
    }
  }
}

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{

  /* 前面代码省略 */

  /* USER CODE BEGIN 2 */
  printf("STM32 HAL IWDG Example Start ...\n");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    /* USER CODE BEGIN 3 */

    // 打印内容
    printf("正常内容...\n");

    // 喂狗
    HAL_IWDG_Refresh(&hiwdg);

    // 延时2s
    HAL_Delay(2000);
  }
  /* USER CODE END 3 */
}
```