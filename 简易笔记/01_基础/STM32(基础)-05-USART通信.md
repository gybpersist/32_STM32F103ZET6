## 第 8 章 USART 串口通信

### 8.1 通信基础知识

#### ① 串行和并行

```
串行：一次传输一个位
并行：同时传输多个位
```

|                | **串行** | **并行** |
| -------------- | -------- | -------- |
| **传输速率**   | 慢       | 快       |
| **硬件成本**   | 低       | 高       |
| **抗干扰能力** | 强       | 弱       |
| **通讯距离**   | 远       | 近       |

#### ② 单工、半双工、全双工

```
单工： 传输方向是单向
半双工： 传输方向是双向的，但两个方向不能同时进行
全双工： 传输方向是双向的，两个方向可以同时进行
```

#### ③ 同步、异步

```
同步：有时钟信号线
异步：无时钟信号线
```

#### ③ USART/UART 串口通信

**全称：**

```
英文：Universal Synchronous/Asynchronous Receiver/Transmitter
中文：通用 同步/异步 收/发 协议
```

**数据帧：**

![UART-%E6%95%B0%E6%8D%AE%E5%B8%A7.png](https://p.sda1.dev/22/41d3912c38f37b7b9fbebb39b83095b4/UART-数据帧.png)

```
起始位：低电平
数据位：5~9位，一般8位
校验位：分为奇校验、偶校验
停止位：高电平

空闲状态：持续高电平
```

> **注意：**USART/UART 通信先发低位！

**波特率：**

```
波特率是指在通信系统中每秒钟传输的符号（或信号变化）数量， UART 中波特率等价于比特率。
```



## 8.2 STM32 的串口模块

#### ① 5个串口相关片上外设

|                   | USART1 | USART2 | USART3 | UART4 | UART5 |
| ----------------- | ------ | ------ | ------ | ----- | ----- |
| 异步模式          | √      | √      | √      | √     | √     |
| 硬件流控制        | √      | √      | √      | ×     | ×     |
| DMA               | √      | √      | √      | √     | ×     |
| 多处理器通讯      | √      | √      | √      | √     | √     |
| 同步              | √      | √      | √      | ×     | ×     |
| 智能卡            | √      | √      | √      | ×     | ×     |
| 半双工 (单线模式) | √      | √      | √      | √     | √     |
| IrDA              | √      | √      | √      | √     | √     |
| LIN               | √      | √      | √      | √     | √     |

```
APB1 总线： USART2、USART3、UART4、UART5
APB2 总线： USART1
```

#### ② 功能框图

![UART-%E5%8A%9F%E8%83%BD%E6%A1%86%E5%9B%BE.png](https://p.sda1.dev/22/f60993423ec1fea2fd899c524fc659f4/UART-功能框图.png)

```
发送流程：
	1. 将要发送的数据写入TDR
	2. TDR将数据放入发送移位数据寄存器
	3. 发送移位数据寄存器从低位开始逐bit发送

接收流程
	1. RX引脚的数据逐位放入接收数据移位寄存器
	2. 接收数据移位寄存器接完，放入RDR
	3. 从RDR读取数据
```

#### ③ 波特率计算

```
波特率 = 时钟频率 / 16 / USARTDIV
```

```
1. USARTDIV 是一个分频值，需存入 USART_BRR 寄存器
2. 发送和接收的波特率需要一致
```

#### ④ 相关寄存器

```
寄存器组 USART1
	SR	状态寄存器，常用标志位：TXE、TC、RXNE、IDLE 等
	DR	数据寄存器，TXD和RXD共用一个寄存器名称（地址）
	CR1	控制寄存器1，常用控制位：TE、RE、UE、PCE、M 等
	CR2 控制寄存器2，STOP 等
	BRR	波特率寄存器，高16位保留、中间12位存整数、低4位存小数

寄存器组 USART2

寄存器组 USART3

寄存器组 UART4

寄存器组 UART5
```

#### ⑤ 单片机引脚

|  串口模块  |       默认引脚        |                重映射引脚                 |
| :--------: | :-------------------: | :---------------------------------------: |
| **USART1** | PA9 (TX) / PA10 (RX)  |            PB6 (TX) / PB7 (RX)            |
| **USART2** |  PA2 (TX) / PA3 (RX)  |            PD5 (TX) / PD6 (RX)            |
| **USART3** | PB10 (TX) / PB11 (RX) | PC10 (TX) / PC11 (RX) PD8 (TX) / PD9 (RX) |
| **UART4**  | PC10 (TX) / PC11 (RX) |               无重映射功能                |
| **UART5**  | PC12 (TX) / PD2 (RX)  |               无重映射功能                |

### 8.3 案例1：MCU使用串口与计算机通信，轮询方式(阻塞方式)接收数据

#### ① 需求分析和硬件原理图

**需求：**

![%E6%A1%88%E4%BE%8B%E9%9C%80%E6%B1%82.png](https://p.sda1.dev/22/453998f28eeaead11b9f009d6ae46109/案例需求.png)

```
电脑通过串口向STM32发送数据，STM32原封不动的再发送过来。电脑可以借助串口助手来发送或接收数据。
```

**硬件电路原理图：**

![%E7%94%B5%E8%B7%AF%E5%8E%9F%E7%90%86%E5%9B%BE.png](https://p.sda1.dev/22/00d2cefa1c33d232a8492ff83d798ffd/电路原理图.png)



#### ② 寄存器方式实现

```
初始化函数：
	1. 时钟使能，对GPIOA、USART1 时钟使能
	2. GPIO 引脚设置，TX引脚(PA9)设置为复用推挽输出，RX引脚(PA10)设置为浮空输入或者复用推挽输出
	3. USART1 设置
	   3.1 使能设置: 串口外设使能
	   3.2 使能设置: 发送使能
	   3.3 使能设置: 接收使能
	   3.4 数据帧设置: 设置数据位数是8位 (CR1.M=0，默认值可省略) 
	   3.5 数据帧设置: 禁止校验 (CR1.PCE=0，默认值可省略)
	   3.6 数据帧设置: 设置停止位是1位 (CR2.STOP=00，默认值可省略)
	   3.7 波特率设置: 设置除数因子是39.0625，实现115200的波特率
		
发送一个字节函数：
	1. 如果 SR.TXE 标志位是0，就一直循环
	2. 将待发送数据写入 DR

发送多个字节函数:
	循环调用发送一个字节函数
	
接收一个字节函数：
	1. 如果 SR.RXNE 标志位是0，就一直循环
	2. 读取 DR
	
接收多个字节函数：
	1. 循环接收
	2. 在判断RXNE的同时需要判断IDLE标志位
	   RXNE为1说明接收到1个字节，继续接收下一个
	   IDLE为1说明接收完成，直接结束函数
```

#### ③ HAL 库方式实现









## 附录

### 单词

```
universal		普遍的、通用的
Synchronous		同步的
Asynchronous	异步的
parity			奇偶校验
Infrared		红外线
idle			闲置的
Transimit		发送
receive			接收
```

### 名词缩写

```
IrDA:	Infrared Data Association, 红外线数据协会
LIN:	Local Interconnect Network，本地互联网络 
```



```
[RCC->APB2ENR：Advanced Peripheral Bus 2 Peripheral Clock Enable Register，即 APB2 外设时钟使能寄存器，用于使能连接在 APB2 总线上的外设时钟。]
1. 时钟使能
	1.1 为确保 GPIO 端口和 USART1 外设能正常工作，需要先使能它们对应的时钟。
        使能 GPIOA 端口的时钟，对应 RCC_APB2ENR_IOPAEN 位。
        使能 USART1 外设的时钟，对应 RCC_APB2ENR_USART1EN 位。
        
[GPIOA->CRH：General - Purpose Input/Output Port A Configuration Register High，即 GPIOA 端口高配置寄存器，用于配置 GPIOA 端口 8 - 15 引脚的工作模式和输出类型。]
2. GPIO 引脚配置
    2.1 配置 GPIOA 端口的 PA9 和 PA10 引脚，分别作为 USART1 的发送（TX）和接收（RX）引脚。
        PA9（TX 引脚）：设置为复用推挽输出模式，最大速度 50MHz。
            MODE 位设置为 11，表示输出模式，最大速度 50MHz。
            CNF 位设置为 10，表示复用推挽输出。
        PA10（RX 引脚）：设置为浮空输入模式。
            MODE 位设置为 00，表示输入模式。
            CNF 位设置为 01，表示浮空输入。
        
[USART1->CR1：Universal Synchronous Asynchronous Receiver Transmitter 1 Control Register 1，即 USART1 控制寄存器 1，用来控制 USART1 的基本功能，像使能外设、发送接收使能、数据位长度、校验位等.]

[USART1->CR2：Universal Synchronous Asynchronous Receiver Transmitter 1 Control Register 2，即 USART1 控制寄存器 2，用于控制 USART1 的一些额外功能，例如停止位设置。]

[USART1->BRR：Universal Synchronous Asynchronous Receiver Transmitter 1 Baud Rate Register，即 USART1 波特率寄存器，用于设置 USART1 的通信波特率。]

[USART1->SR：Universal Synchronous Asynchronous Receiver Transmitter 1 Status Register，即 USART1 状态寄存器，反映 USART1 的工作状态，如发送完成、接收缓冲区非空等状态标志。]

[USART1->DR：Universal Synchronous Asynchronous Receiver Transmitter 1 Data Register，即 USART1 数据寄存器，用于存放要发送或接收到的数据 。]
3. USART1 外设配置
    3.1 对 USART1 外设进行一系列参数配置，使其能正常工作。
        使能 USART1 外设：将 USART_CR1_UE 位置 1，开启 USART1 外设。
        使能发送功能：将 USART_CR1_TE 位置 1，允许通过 TX 引脚发送数据。
        使能接收功能：将 USART_CR1_RE 位置 1，允许通过 RX 引脚接收数据。
        设置数据位长度：将 USART_CR1_M 位清零，设置数据帧的数据位长度为 8 位。
        设置校验位：将 USART_CR1_PCE 位清零，不启用奇偶校验功能。
        设置停止位：将 USART_CR2_STOP 位清零，设置停止位为 1 位。
        设置波特率：计算出除数因子为 39.0625，将整数部分 39 和小数部分 1 组合后写入 USART1->BRR 寄存器，设置波特率为 19200。
```

