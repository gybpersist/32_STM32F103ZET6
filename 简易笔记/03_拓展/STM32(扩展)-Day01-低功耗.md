## 第 1 章 电源控制

### 1.1 供电模块架构

![%E7%94%B5%E6%BA%90%E6%A1%86%E5%9B%BE.png](https://p.sda1.dev/22/61edc55ca89984f53824b5e0ae65e1c0/电源框图.png)

**引脚说明：**

![MCU_PWR_Pin.png](https://p.sda1.dev/24/5bac02f30744e0bd2a1d39e4e053ce3e/MCU_PWR_Pin.png)

| **引脚名称** | **解释**                                                     |
| ------------ | ------------------------------------------------------------ |
| **VDD**      | 数字电路部分电源引脚，电压在2.0V~3.6V之间                    |
| **VSS**      | 数字电路部分共地引脚                                         |
| **VDDA**     | 模拟电路部分电源引脚，电压在2.0V~3.6V之间                    |
| **VSSA**     | 模拟电路部分共地引脚                                         |
| **VREF-**    | ADC/DAC参考地引脚，必须与VSSA相连                            |
| **VREF+**    | ADC/DAC参考正电压引脚，电压在2.4V~ VDDA之间，精度要求不高可直连VDDA |
| **VBAT**     | 后备电源引脚，电压在2.0V~3.6V之间                            |

**VDDA 供电区域： **

VDDA提供正电压，VSSA提供负电压，主要负责模拟部分的供电，包括ADC/DAC、温度传感器、复位模块和锁相环PLL。

**VDD供电区域：**

VDD是数字电路的正极，VSS是数字电路的负极，为I/O电路、待机电路和电压调节器供电。

**1.8V 供电区域：**

电压调节器将VDD转化为1.8V电压，提供给1.8V供电区域，该区域包括CPU内核、存储器和内置数字外设。

**电压调节器：**

电压调节器有3种不同的工作模式：

* 运转模式：调节器以正常功耗模式提供1.8V电源。
* 停止模式：调节器以低功耗模式提供1.8V电源，仅保存寄存器和SRAM的内容。
* 待机模式：调节器停止供电。寄存器和SRAM的内容全部丢失。

**后备供电区域：**

后备供电区域包括外部时钟LSE（需要外接32.768KHz的晶振）、后备寄存器、RCC 的 BDCR 寄存器和实时时钟RTC模块。

当VDD供电时，它会同时为后备供电区域供电，当它断开时，硬件会自动将该区域的供电切换为 VBAT。

### 1.2 上电复位和掉电复位（了解）

![%E4%B8%8A%E7%94%B5%E5%A4%8D%E4%BD%8D%E5%92%8C%E6%8E%89%E7%94%B5%E5%A4%8D%E4%BD%8D01.png](https://p.sda1.dev/22/0c9797d42f696118993d30f875baf512/上电复位和掉电复位01.png)

当VDD/VDDA低于指定的限位电压VPOR/VPDR时，系统保持为复位状态，而无需外部复位电路。

复位和解除复位有一个40mv的迟滞电压。当电压大于VPOR时解除复位，当电压小于VPDR时进入复位。设置2个阈值的作用是当电压在附近抖动的时候防止频繁的复位和解除复位。

![%E4%B8%8A%E7%94%B5%E5%A4%8D%E4%BD%8D%E5%92%8C%E6%8E%89%E7%94%B5%E5%A4%8D%E4%BD%8D02.png](https://p.sda1.dev/22/76060337c41cee7d31912e12b53d39fe/上电复位和掉电复位02.png)

### 1.3 可编程电压检测器（了解）

可编程电压检测器，简称 PVD（Programmable voltage detector），用于监测VDD/VDDA的供电电压，它的阈值电压可以编程。

![%E5%8F%AF%E7%BC%96%E7%A8%8B%E7%94%B5%E5%8E%8B%E6%A3%80%E6%B5%8B%E5%99%A801.png](https://p.sda1.dev/22/6b17b084a34b8a0ab22fe867cf561a3b/可编程电压检测器01.png)

电压与电源控制寄存器(PWR_CR)中的PLS[2:0]位  可以设置 PVD 阈值

![%E5%8F%AF%E7%BC%96%E7%A8%8B%E7%94%B5%E5%8E%8B%E6%A3%80%E6%B5%8B%E5%99%A802.png](https://p.sda1.dev/22/8ff6edf0f92ac8204a3331e418d3913f/可编程电压检测器02.png)

### 1.4 低功耗的三种模式

STM32F10xxx有三种低功耗模式：

![%E4%BD%8E%E5%8A%9F%E8%80%97%E4%B8%89%E7%A7%8D%E6%A8%A1%E5%BC%8F.png](https://p.sda1.dev/22/df372e8175f3970062cd35e7bfc697b1/低功耗三种模式.png)

### 1.5 低功耗案例1：睡眠模式

#### ① 需求描述

让MCU进入睡眠模式，然后通过串口发送消息来唤醒MCU退出睡眠模式。观察LED在进入休眠模式后是否仍然开启。

#### ② 睡眠模式介绍

![%E7%9D%A1%E7%9C%A0%E6%A8%A1%E5%BC%8F-SLEEP-NOW.png](https://p.sda1.dev/22/66dda02f92b084879992f44b277a53cc/睡眠模式-SLEEP-NOW.png)
![%E7%9D%A1%E7%9C%A0%E6%A8%A1%E5%BC%8F-SLEEP-ON_EXTI.png](https://p.sda1.dev/22/0253968551f59bb6678df27b3b732228/睡眠模式-SLEEP-ON_EXTI.png)

##### (1) WFI 和 WFE

WFI 指令使系统立即进入低功耗模式，并等待中断触发唤醒。唤醒条件为任意一个被NVIC（嵌套向量中断控制器）响应的外设中断。

WFE 指令使系统立即进入低功耗模式，并等待事件触发唤醒。唤醒事件可以是外部中断线信号（如GPIO电平变化）或内部事件（如DMA传输完成）

> **注：** CMSIS 规范提供了封装了 WFI 和 WFE 指令的 C 语言函数 `__WFE()` 和 `__WFI()`。

##### (2) SLEEP-NOW模式

**睡眠模式的进入: **

SCB 寄存器的 SLEEPDEEP 和 SLEEPONEXIT 均设置为0，然后执行WFI或WFE指令。

**睡眠模式的退出（唤醒处理器）:**

如果执行WFI指令进入睡眠模式则任意中断可唤醒；如果执行WFE指令进入睡眠模式则可以通过事件唤醒。

##### (3) SLEEP-ON-EXIT模式（了解）

**睡眠模式的进入: **

当 SLEEPONEXIT 为1时执行WFI指令。

> **注意：**官方手册告诉我们，当前模式下只能通过WFI进入睡眠模式。然而，实测发现其它配置不变的情况下调用WFE也可以。不过，这种模式是专为中断参与的场景设计的，调用WFE本就不合理，不必深究。

**睡眠模式的退出： **

任意中断。

##### (4) SLEEP-NOW 与 SLEEP-ON-EXIT 区别

SLEEP-ON-EXIT模式下，系统被中断唤醒后，中断服务函数执行完毕就会再次进入睡眠模式，主程序的代码在下次复位前不会再被执行。

SLEEP-NOW模式只有再次调用WFI或WFE才会再次进入睡眠模式。

#### ③ 寄存器方式实现

**修改Keil 配置：**

单片机处于低功耗模式时是无法烧录的，此时需要唤醒或复位后方可烧录，我们可以将烧写器的连接方式设置为 under Reset，这样每次烧录前Keil会自动执行复位，即便在低功耗模式下也可以直接烧录。

![Example01-SleepMode-Register-keil.png](https://p.sda1.dev/22/df87ca01a886c0ec3510cf85518cd12e/Example01-SleepMode-Register-keil.png)

**代码实现：**

```c
void Enter_SleepMode(void)
{
    // // 开启  SLEEP-ON-EXIT 模式
    // SCB->SCR |= SCB_SCR_SLEEPONEXIT;

    // 进入睡眠模式（使用中断唤醒）
    __WFI();
}
```



#### ④ HAL 库方式实现

**工程创建：**

```
1. 配置USART1，并开启中断
2. 配置 LED 的引脚
```

**相关 HAL 库函数：**

```
HAL_SuspendTick()				关闭滴答定时器
HAL_ResumeTick()				回复滴答定时器
HAL_PWR_EnableSleepOnExit()		开启 SleepOnExit
HAL_PWR_DisableSleepOnExit()	关闭 SleepOnExit
HAL_PWR_EnterSLEEPMode()		进入Sleep模式
HAL_UART_Receive_IT()			开启串口接收中断
```

**代码：**

> **注意：** 进入睡眠模式前必须关闭系统滴答定时器中断，唤醒后再回复滴答定时器中断。

```c
// 进入中断之前关闭滴答定时器
HAL_SuspendTick();
// 进入中断
printf ("进入中断... \n");
HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI); // 进入低功耗模式


// 唤醒后执行的代码
printf ("唤醒后执行的代码... \n");
// 唤醒之后开启滴答定时器
HAL_ResumeTick();
```



### 1.6 低功耗案例2：停止模式

#### ① 需求描述

让MCU进入停机模式。停机模式需要外部中断才能唤醒，所以要开启外部中断（外部中断是指EXTI外设的中断）。

#### ② 停止模式介绍

**进入：**

```
1. SCB_SCR.SLEEPDEEP 置 1 (Stop 和 Standby 都属于 SleepDeep)
2. PWR_CR.PDDS 置 0 (0 表示 Stop，1 表示 Standby)
3. PWR_CR.LPDS 置 0 或 1 (1 表示开启电压调节器的低功耗， 低功耗主要影响唤醒时间)
4. WFI 或 WFE
```

**唤醒：**

```c
WFI进入： 任意一个 EXTI 中断
WFE进入： 任意一个 EXTI 事件
```

**停止模式下电源工作情况：**

```
1. 1.8V 区域的时钟关闭，供电选择正常或低功耗，SRAM和寄存器数据被保留
2. PLL 停止工作
```

**停止模式下时钟树工作情况：**

```
1. 系统时钟(HSE、HSI、PLL)停止工作
2. RCC 中相关时钟树的配置丢失
```

#### ③ 寄存器方式实现

```c
void Enter_StopMode(void)
{
    // 1. 开启深睡眠模式（SCB_SCR.SLEEPDEEP=1, 该寄存器位于内核）
    SCB->SCR |= SCB_SCR_SLEEPDEEP;

    // 2. 对 PWR 时钟使能
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;

    // 3. 选择设置为深睡眠模式中的 STOP 模式（PWR_CR.PDDS=0, 默认值）
    PWR->CR &= ~PWR_CR_PDDS;

    // 4. 选择设置进入STOP模式的时候，1.8V供电区使用低功耗模式（PWR_CR.LPDS=1）
    PWR->CR |= PWR_CR_LPDS;

    // 5. 进入低功耗模式
    __WFI();
}
```

```c
// 进入睡眠模式（使用中断唤醒），进入睡眠模式后时钟配置会丢失
Enter_StopMode();

// 唤醒后，首先重新配置时钟系统
SystemInit();
```



#### ④ HAL 库方式实现

**工程创建：**

```
设置PF9为EXIT中断，上升沿触发，中断向量 EXTI_9_5
```

**相关 HAL 库函数：**

```
HAL_PWR_EnterSTOPMode()     进入STOP模式
SystemClock_Config()		配置系统时钟为72MHz
```



### 1.8 低功耗案例3：待机模式 (Standby Mode)

#### ① 需求描述

待机模式的唤醒方式比较有限，我们使用WKUP引脚的**上升沿**唤醒，PA0就是WKUP引脚。

我们电路中PA0已经连接了LED1，所以要产生上升沿需要先用杜邦线连接GND，然后再连接3.3V电源。

#### ② 待机模式介绍

##### (1) 进入

```
1. SCB_SCR 寄存器中的 SLEEPDEEP 位置 1。
2. PWR_CR 寄存器中的 PDDS 位置 1。
3. 如果使用功 WKUP 唤醒，需要使能 WKUP 引脚
4. 清除 PWR_CSR 寄存器中的 WUF 标志位。
5. 执行 WFI 或 WFE 指令。
```

##### (2) 唤醒

```
方式一：WKUP 引脚的上升沿。
方式二：RTC 闹钟事件的上升沿。
方式三：来自 NRST 引脚的外部复位。
方式四：独立看门狗 IWDG 的复位。
```

##### (3) 从待机模式唤醒后的行为

除 PWR_CSR 寄存器外的所有寄存器都被复位。实际上，从待机模式苏醒的过程和系统上电后的复位过程是一样的。PWR_CSR 寄存器中的 SBF 状态标志位可以指明 MCU 复位前是否处于待机模式。

##### (4) 待机模式下 I/O 引脚状态

在待机模式下，所有的I/O引脚处于高阻态（即断路），除了以下的引脚：

```
1. 复位引脚
2. 当被设置为防侵入或校准输出时的 TAMPER 引脚。
3. 被使能的唤醒引脚。
```

##### (5) 待机模式下供电模块工作情况

```
1. 电压调节器停止工作，1.8V 区域停止供电，SRAM、寄存器数据丢失
2. I/O引脚处于高阻态
3. 除了待机电路和后备区域，全部断电
```

#### ③ 寄存器方式实现

**Dri_LowPower.c**

```c
void Enter_StandbyMode(void)
{
    // 1. 开启深睡眠模式（SCB_SCR.SLEEPDEEP=1, 该寄存器位于内核）
    SCB->SCR |= SCB_SCR_SLEEPDEEP;

    // 2. 对 PWR 时钟使能
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;

    // 3. 选择设置为深睡眠模式中的 Standby 模式（PWR_CR.PDDS=1）
    PWR->CR |= PWR_CR_PDDS;

    // 4. 使能 WKUP 引脚
    PWR->CSR |= PWR_CSR_EWUP;

    // 5. 清除 WUF 标志位
    PWR->CR |= PWR_CR_CWUF;

    // 6. 调用指令，进入待机模式
    __WFI();
}
```

**main.c**

```c
// 对PWR时钟使能
RCC->APB1ENR |= RCC_APB1ENR_PWREN;
// 判断 WUF 标志位, 如果WUF为1，说明有待机模式唤醒，否则复位重新上电
if (PWR->CSR & PWR_CSR_WUF)
{
    printf("WakeUp From Standby\n");
    // 清除 WUF 标志位
    PWR->CR |= PWR_CR_CWUF;
}
else
{
    printf("Application Start \n");
}

// LED 初始化
Int_LED_Init();
// 点亮 LED3(绿灯)
Int_LED_On(LED3);

printf("3000ms 后进入 Standby 模式...\n");
// 延时 3000ms
Delay_ms(3000);

// 进入待机模
printf("进入 Standby 模式...\n");
Enter_StandbyMode();


// 唤醒后执行的代码（该代码永远不会执行）
printf("唤醒后执行的代码... \n");
```



#### ④ HAL 库方式实现

**STM32 Cube MX 配置：**

![%E5%BE%85%E6%9C%BA%E6%A8%A1%E5%BC%8F-HAL.png](https://p.sda1.dev/22/bc357ca7adcb3146d37db407a01d80e2/待机模式-HAL.png)

**相关 HAL 库函数：**

```
HAL_PWR_EnterSTANDBYMode()  进入待机模式
HAL_PWR_EnableWakeUpPin()	启动WKUP引脚
__HAL_PWR_CLEAR_FLAG();		清除PWR相关寄存器标志位
__HAL_PWR_GET_FLAG()		获取PWR相关寄存器标志位
```

**代码：**

```c
// 判断 WUF 标志位
if (__HAL_PWR_GET_FLAG(PWR_FLAG_WU) == SET) {
    printf ("Wakeup from Standby Mode \n");
    // 清除 WUF 标志位
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
}
else
{
    printf ("Application Start \n");
}

printf ("5000ms 后进入 Standby 模式... \n");
HAL_Delay(5000); // 3秒后进入中断

// 进入 Standby 模式前，使能WUP引脚
HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);

// 进入 Standby 模式
printf ("进入 Standby 模式... \n");
HAL_PWR_EnterSTANDBYMode();
```



## 附录

### 单词

```
intrinsic	adj.内在的，固有的
tamper		vi. 篡改；干预；损害；削弱；玩弄；贿赂
detector	n.探测器，检测器；发现者
hysteresis	n.滞后现象
regulator	n.调节器
```

### 名词缩写

```
PVD		Programmable voltage detector,可编程电压检测器
```

