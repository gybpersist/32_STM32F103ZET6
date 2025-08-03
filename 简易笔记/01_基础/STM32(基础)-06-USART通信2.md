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

**STM32 Cube MX 配置：**

![%E6%A1%88%E4%BE%8B01-HAL01.png](https://p.sda1.dev/22/d531fdb9c382b8bd2dc7a2fc196be7d8/案例01-HAL01.png)
![%E6%A1%88%E4%BE%8B01-HAL02.png](https://p.sda1.dev/22/eea51b023e41df71c5cc2e1aedbe48d7/案例01-HAL02.png)
![%E6%A1%88%E4%BE%8B01-HAL03.png](https://p.sda1.dev/22/c9fd1d5f508a8da178a6765aaba2aaa5/案例01-HAL03.png)

**相关函数：**

```
HAL_UART_Transmit()	发送指定长度的字节数据
HAL_UART_Receive（） 接收定长数据（接收到指定长度即停止）
HAL_UARTEx_ReceiveToIdle() 接收非定长数据（检测到Idle标志位或指定的最大长度停止接收）
```



### 8.4 案例2：MCU使用串口与计算机通信，中断方式(非阻塞方式)接收数据

#### ① USART 相关中断

![USART%E4%B8%AD%E6%96%AD%E4%BD%BF%E8%83%BD%E4%BD%8D.png](https://p.sda1.dev/22/484873dcfd97fdf95d0a86d57ab18003/USART中断使能位.png)

```
USART的各种中断事件被连接到同一个中断向量(见下图)，有以下各种中断事件：
发送期间：发送完成（TC）、清除发送(仅硬件流控制)（CTS）、发送数据寄存器空（TXE）。
接收期间：空闲总线检测(IDLE)、溢出错误(ORE)、接收数据寄存器非空(RXNE)、校验错误(PE)、LIN断开符号检测(LBD)、噪音标志(仅在DMA通信)和帧错误(仅在DMA通信)(NE)。
```

![USART%E4%B8%AD%E6%96%AD%E6%B5%81%E7%A8%8B.png](https://p.sda1.dev/22/c956cf5560afe6d2aab3bed8215f17ac/USART中断流程.png)

#### ② 寄存器方式实现

**USART1驱动程序：**

```
初始化函数;
	添加NVIC设置

添加 Dri_USART1_ReceiveITStart 函数
	对RXNE、IDLE中断进行使能
	
添加 Dri_USART1_ReceiveITStop 函数
	关闭RXNE、IDLE中断进行使能
	
定义USART1_IRQHandler 中断服务程序：
	如果是RXNE中断，将DR添加到数组，长度加一
	如果是IDLE中断，调用弱函数并将数组和长度传出，清零长度，清除IDLE标志位
	
```

**主程序：**

```
1. USART 初始化
2. Dri_USART1_ReceiveITStart()
3. 重定义弱函数
```

#### ③ HAL 库方式实现

**STM32 Cube MX 配置：** 基于案例1添加中断配置

![%E6%A1%88%E4%BE%8B2-HAL.png](https://p.sda1.dev/22/a9b1512b2fe7073c283e41324ec1859d/案例2-HAL.png)

**中断触发后的调用链：**

![%E6%A1%88%E4%BE%8B2-HAL%E5%BA%93-%E8%B0%83%E7%94%A8%E9%93%BE-%E5%AE%9A%E9%95%BF%E4%B8%AD%E6%96%AD.jpg](https://p.sda1.dev/22/266fe53819fc973c1fa3bf0f1ebd8abe/案例2-HAL库-调用链-定长中断.jpg)

![%E6%A1%88%E4%BE%8B2-HAL%E5%BA%93-%E8%B0%83%E7%94%A8%E9%93%BE-%E4%B8%8D%E5%AE%9A%E9%95%BF%E4%B8%AD%E6%96%AD.jpg](https://p.sda1.dev/22/3daa7357108791b312a83d2b970eab26/案例2-HAL库-调用链-不定长中断.jpg)

 **相关函数：**

```
HAL_UART_Receive_IT()			开启中断，定长方式接收数据，非阻塞方式, 处理完接收数据后需要重新调用
HAL_UARTEx_ReceiveToIdle_IT()   开启中断，非定长方式接收数据，非阻塞方式，处理完接收数据后需要重新调用

__weak HAL_UART_RxCpltCallback()     定长数据接收完成后自动调用
__weak HAL_UARTEx_RxEventCallback()  非定长数据接收完成后自动调用
```





### 8.5 案例3 重定向 printf

#### ① 需求分析

```
C语言中经常使用printf来输出调试信息，打印到屏幕。由于在单片机中没有屏幕，但是我们可以重定向printf，把数据打印到串口，从而在电脑端接收调试信息。
```

#### ② 实现原理

**第一步** 修改 Keil 配置

![printf-keil.png](https://p.sda1.dev/22/e04acc7107ee7b870bed6912b67ca697/printf-keil.png)

MicroLIB 是 Keil MDK 中一个高度优化的轻量级 C 库，代码量小但功能精简，提供了有限的 stdio 子系统，支持未缓冲的 `stdin`、`stdout` 和 `stderr`，适用于资源受限的嵌入式系统 。

**第二步：** 重定义 `fputc()` 函数

#### ② 寄存器方式实现

重定义 `fputc()` 函数如下：

```c
// 重定义 fputc
int fputc( int ch, FILE* stream )
{
    // 使用串口将 ch 发送出去
    Dri_USART1_TransmitByte(ch);
    // 返回
    return ch;
}
```

#### ③ HAL 库方式实现

重定义 `fputc()` 函数如下：

```c
// 重定义 fputc
int fputc(int ch, FILE *stream)
{
  // 使用串口发送 ch
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 1000);
  // 返回
  return ch;
}
```

### 8.6 使用 Keil 断点调试

![Keil-%E6%96%AD%E7%82%B9%E8%B0%83%E8%AF%95.png](https://p.sda1.dev/22/8b89d2cf9bafef54fef5bc22c1073d6f/Keil-断点调试.png)

快捷键：

```
F10 		Step, 进入函数内部
F11 		Step Over，不会进入函数内
Ctrl + F11	Step Out，从函数中跳出
Ctrl + F10	Run to Cursor Line，跳到下一个断点
```







## 附录

### 单词

```
universal		普遍的、通用的
Synchronous		同步的
Asynchronous	异�
```