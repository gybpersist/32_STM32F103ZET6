## 第 2 章 BKP 备份寄存器

### 2.1 BKP 介绍

BKP（backup register，备份寄存器），由于 42 个 16 位的寄存器组成，可用来存储84个字节的用户应用程序数据。当VDD电源被切断，BKP 仍然由 VBAT 维持供电。

从待机模式下被唤醒、或系统复位、电源复位时，BKP也不会被复位。

此外，BKP控制寄存器用来管理**侵入检测**和**RTC校准**功能。

复位后，默认对 BKP 和 RTC 的访问被禁止，执行以下操作可以使能 BKP 和 RTC 的访问：

（1）通过设置寄存器 RCC_APB1ENR 的 PWREN 和 BKPEN 位来打开 PWR 和 BKP 的时钟。

（2）电源控制寄存器(PWR_CR) 的 DBP 位来使能对 BKP 和 RTC 的访问

### 2.2 BKP 相关寄存器

**PWR:**

| 寄存器名称 | 描述       | 相关控制位/标志位            |
| ---------- | ---------- | ---------------------------- |
| `PWR->CR`  | 控制寄存器 | DBP: 使能对 RTC和 BKP 的访问 |

**RCC：**

| 寄存器名称     | 描述                       | 相关控制位/标志位    |
| -------------- | -------------------------- | -------------------- |
| `RCC->BDCR`    | 备份域控制寄存器           | BDRST: 置1复位备份域 |
| `RCC->APB1ENR` | APB1总线外设时钟使能寄存器 | BKPEN: 对BKP时钟使能 |

**BKP：**

```
BKP->DRx(x=1..42)	数据寄存器，共 42 个, 每个寄存器 16 位
BKP->RTCCR			RTC 校准寄存器
BKP->CR				控制寄存器
BKP->CSR			控制和状态寄存器
```

### 2.3 BKP 案例

#### ① 需求描述

```
1. 程序启动后向后备寄存器写入值，按下按键3从备份寄存器读出数据，验证是否一致。
2. 复位设备，然后按下按键3是否能读到数据，验证掉电不丢失。
3. 关机再启动，然后按下按键3是否能读到数据，验证掉电不丢失。
```

#### ② 寄存器方式实现

**Dri_BKP.c**

```c
void Dri_BKP_Init(void)
{
    // 1. 对 BKP 和 PWR 时钟使能
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    RCC->APB1ENR |= RCC_APB1ENR_BKPEN;

    // 2. 关闭备份区域写保护
    PWR->CR |= PWR_CR_DBP;

    // 3. 清空整个备份区域
    // RCC->BDCR |= RCC_BDCR_BDRST;
}
```

**main.c**

```c
// 主函数
int main()
{
	// USART1 初始化
	Dri_USART1_Init();
	// 打印信息
	printf("STM32 BKP Example: \n\n");

	// 按键初始化
	Int_Key_Init();

	// 备份区域初始化
	Dri_BKP_Init();

	// 向备份寄存器写入数据
	// BKP->DR1 = 999;

	// 死循环
	while (1)
		;
}

// 重定按键按下后（触发EXTI中断）调用的弱函数
void Int_Key_SW5Callback(void)
{
	printf("BKP DR1: %d \n", BKP->DR1);
}
```

#### ③ HAL 库方式实现

**STM32CuteMX 配置：**

> **注:** 需要启动并配置 RTC 模块，BKR 是依赖于 RTC 模块的。

![HAL01.png](https://p.sda1.dev/24/6d13dff6bd9752f7f1d618f385aa5e48/HAL01.png)

![HAL02.png](https://p.sda1.dev/22/e60be2c9aca96fa7b09b6e2e69fc088f/HAL02.png)

**HAL库相关函数：**

```
HAL_RTCEx_BKUPWrite()	向 BKP 写入数据
HAL_RTCEx_BKUPRead()	读取 BKP 的数据
```

**代码：**

```c
// 重定义 SW5 按键按下后调用的弱函数
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == SW5_Pin) {
       // 延时，用于消抖
       HAL_Delay(10);
       // 再次检测 SW5 按键是否按下
       if (HAL_GPIO_ReadPin(SW5_GPIO_Port, SW5_Pin) == GPIO_PIN_SET) {
          // 读取 BKP 数据
          uint32_t data = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1);
          // 打印 BKP 数据到串口
          printf("BKP data: %d\n", data);
        }
    }
}

int main(void)
{
  printf("STM32 HAL BKP Example: \n");

  // 向BKP写入数据
  // HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 250);


  while (1)
  	;
}
```





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

    // 2. RTC 时钟配置 ---------------------------------
    // 2.1 使能 LSE 时钟
    RCC->BDCR |= RCC_BDCR_LSEON;
    // 2.2 等待 LSE 稳定
    while ((RCC->BDCR & RCC_BDCR_LSERDY) == 0)
        ;
    // 2.3 选择 LSE 作为 RTC 时钟 (RCC_BDCR.RTCSEL = 01)
    RCC->BDCR &= ~RCC_BDCR_RTCSEL_1;
    RCC->BDCR |= RCC_BDCR_RTCSEL_0;	//RCC->BDCR |= RCC_BDCR_RTCSEL_LSE;
    // 2.4 对 RTC 时钟使能(在备份域复位之后)
    RCC->BDCR |= RCC_BDCR_RTCEN;

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



### 4.2 RTC 案例2-实时时钟

#### ① 需求分析

显示时间。通过串口把时间发送给电脑显示。即使关机很多天，再启动后也能正确显示时间。



#### ② 寄存器方式实现

```c

```



#### ③ HAL 库方式实现

**STM32CubeMX 配置：**

![Example02-HAL.png](https://p.sda1.dev/22/e768a6fe1ef084be52593e6a4d001c49/Example02-HAL.png)

**相关HAL库函数：**

```
HAL_RTC_SetDate()
HAL_RTC_SetTime()
HAL_RTC_GetDate()
HAL_RTC_GetTime()
```





