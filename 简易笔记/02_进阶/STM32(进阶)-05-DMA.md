## 第 2 章 DMA 直接存储访问

### 2. 1 DMA 介绍

直接存储器访问（Direct Memory Access，DMA），用于直接在外设与存储器之间实现高速数据传输，无需CPU干预，从而节省CPU资源。 

2 个 DMA 控制器共 12 个通道（DMA1 有 7 个通道，DMA2 有 5 个通道），每个通道专门用来管理来自于一个或多个外设对存储器访问的请求，还有一个仲裁器来协调各个DMA请求的优先权。

DMA控制器和Cortex™-M3核心共享系统数据总线，执行直接存储器数据传输。当CPU和DMA同时访问相同的目标（RAM或外设）时，DMA请求会暂停CPU访问系统总线达若干个周期，总线仲裁器执行循环调度，以保证CPU至少可以得到一半的系统总线（存储器或外设）带宽。

要注意的是DMA2只存在于@@

### 2.2 DMA 功能框图和功能描述

![DMA%E6%A1%86%E5%9B%BEpng.png](https://p.sda1.dev/22/df1f6cc682a018fec0b33c1c8f41a5b4/DMA框图png.png)

#### ① DMA 请求

如果外设要想通过DMA来传输数据，必须先给DMA控制器发送DMA请求，DMA控制器收到请求信号之后，控制器会给外设一个应答信号，当外设得到控制器的应答信号后，外设会立即释放它的请求。

DMA有DMA1和DMA2两个控制器，不同DMA控制器的通道对应着不同的外设请求，这决定了我们在软件编程上该怎么设置。

**各个通道的 DMA1 请求：**

![DMA1%E8%AF%B7%E6%B1%82.png](https://p.sda1.dev/22/34089a71cecc6d4873de2b09eb0e8d57/DMA1请求.png)

**各个通道的 DMA2 请求：**

![DMA2%E8%AF%B7%E6%B1%82.png](https://p.sda1.dev/22/868bd96e91c843b20407e2bef16d7656/DMA2请求.png)

#### ② DMA 通道

DMA具有12个独立可编程的通道，其中 DMA1有7个通道，DMA2有5个通道，每个通道对应不同的外设的DMA请求。虽然每个通道可以接收多个外设的请求，但是**同一时间只能接收一个**，不能同时接收多个。

#### ③ 仲裁器

当多个DMA通道请求同时发生时，需要**仲裁器**进行管理，分为两个阶段管理响应顺序：

1. **软件阶段**：通过配置`DMA_CCRx`寄存器设置优先级，分为4个等级：非常高、高、中、低。
2. **硬件阶段**：若多个通道优先级相同，则按通道编号决定，编号越低优先级越高（如通道0优先于通道1）。

**注意**：在大容量和互联型产品中，DMA1控制器的优先级高于DMA2。

#### ④ 传输方向

DMA传输方向支持：

1. **存储器到外设**
2. **外设到存储器**
3. **存储器到存储器**

**注意**：

- 存储器指ROM（如Flash）和RAM。
- DMA无法将数据从RAM传输到ROM（Flash）。

### 2.3 DMA 执行流程图

#### ① DMA1 控制器

![DMA1%E6%B5%81%E7%A8%8B%E5%9B%BE.png](https://p.sda1.dev/22/5d5865d0d82466163869508f475a4de1/DMA1流程图.png)

#### ② DMA2 控制器

![DMA2%E6%B5%81%E7%A8%8B%E5%9B%BE.png](https://p.sda1.dev/22/c4f7d8482e8e10d8376cf05856a91fd1/DMA2流程图.png)

### 2.4 DMA 相关寄存器

| 寄存器名称           | 描述                  | 相关控制位/标志位                                            |
| -------------------- | --------------------- | ------------------------------------------------------------ |
| DMAx->ISR            | 中断状态寄存器        | 每个通道有TEIF、HTIF、TCIF、GIF 4个中断标志位                |
| DMAx->IFCR           | 中断标志清除寄存器    | 与 ISR 寄存器对应，相应的位写1可清除中标标志                 |
| DMAx_Chancelx->CCR   | 通道x配置寄存器       | PL: 2个bit，设置通道优先级<br>MSIZE: 2个bit，存储器数据宽度<br/>PSIZE: 2个bit，外设数据宽度<br>MINC: 存储器地址增量模式，置1生效<br>PINC: 外设地址增量模式，置1生效<br>CIRC: 循环模式，置1生效<br>DIR: 数据传输方向,0从外设读.1从存储器读<br/>TEIE:  传输错误中断使能<br>HTIE:  传输一半中断使能<br>TCIE：传输完成中断使能<br>EN: 通道开启，置1生效 |
| DMAx_Chancelx->CNDTR | 通道x数据数量寄存器   | 值范围0~65535，需要通道开启前写入                            |
| DMAx_Chancelx->CPAR  | 通道x外设地址寄存器   | 32 bit                                                       |
| DMAx_Chancelx->CMAR  | 通道x存储器地址寄存器 | 32 bit                                                       |

> **注意：**
>
> DMA后面的x取值范围1 ~ 2； Chancel后面的x取值范围 1 ~ 7。

> **注意：**
>
> 设置存储器数据宽度（CCR 寄存器中的 MSIZE 位），00 表示 8位；01 表示 16位，10 表示 32 位；11保留。
>
> 设置外设数据宽度（CCR 寄存器中的 PSIZE 位）, 00 表示 8位；01 表示 16位，10 表示 32 位；11保留。

### 2.5 DMA案例1: RAM 到外设（串口）

#### ①  需求描述和硬件电路

![P01-%E9%9C%80%E6%B1%82.png](https://p.sda1.dev/24/53d172e406e03c044b5a6a823c07adbc/P01-需求.png)

需求：实现把RAM中的数据直接传输到usart1的Tx引脚（TDR 发送数据寄存器），然后数据被发送到电脑端。

电路：USART1_TX 连接 DMA1 通道4。

#### ② 寄存器方式实现

**DMA 驱动程序：**

```
初始化函数 Driver_DMA_Init(void)：
	1. 时钟使能 DMA1 (DMA1位于AHB总线)
	2. DMA 设置
		2.1 设置传输方向为存储器到外设（CCR.DIR=1）
		2.2 设置存储器数据宽度为8位（CCR.MSIZE=00, 默认值）
		2.3 设置外设数据宽度为8位（CCR.PSIZE=00, 默认值）
		2.4 设置存储器开启地址增量模式（CCR.MINC=1）
		2.5 设置外设关闭地址增量模式(CCR.PINC=0, 默认值)
		2.6 开启传输完成（TC）中断 (CCR.TCIE=1)
	3. USART1 使能 DMA 发送 （USART1->CR3.DMAT=1）
	4. NVIC 设置
	
传输数据函数 Driver_DMA_Transfer(uint32_t mem_addr, uint32_t periph_addr, uint16_t len)：
	1. 关闭通道使能，并延时
	2. 设置存储器地址（CMAR寄存器）
	3. 设置外设地址（CPAR寄存器）
	4. 设置传输长度（CNDTR寄存器，该寄存器只有通道关闭的时候赋值才能生效）
	5. 开启通道使能
	
中断服务程序 DMA1_Channel4_IRQHandler：
	1. 清除TCIF标志位（IFCR.CTCIF=1）
	2. 调弱函数
```

**main.c 主程序：**

```c
int main()
{
    // DMA 初始化
    Dri_DMA_Init();
    // 启动DMA传输
    Dri_DMA_Transfer((uint32_t)rx_data, (uint32_t)&(USART1->DR), 13);
}

// 重定义DMA传输完毕后调用的弱函数
void Dri_DMA_TC_Callback(void)
{
	printf("DMA transfer complete!\n");

	// 再次发送
	// Dri_DMA_Transfer((uint32_t)rx_data, (uint32_t)&(USART1->DR), 13);
}

```

#### ③ HAL 库方式实现

**STM32CubeMX 配置：**

![P01-RAM%E5%88%B0%E5%A4%96%E8%AE%BE-HAL01.png](https://p.sda1.dev/22/96c5feaa8e1db56a6f62a477e617485f/P01-RAM到外设-HAL01.png)

![P01-RAM%E5%88%B0%E5%A4%96%E8%AE%BE-HAL02.png](https://p.sda1.dev/24/d25f866a81c6f08b2b8d22991025c886/P01-RAM到外设-HAL02.png)

**相关HAL库函数：**

```
HAL_UART_Transmit_DMA()				USART将DMA输出的数据通过串口发送
__weak HAL_UART_TxCpltCallback()	输出完成自动调用
```

**代码：**

发送：

```c
// 定义要传输的数据
uint8_t tx_data[] = "Hello, DMA! \n";

// 使用DMA传输数据，RAM到USART1_TX
// USART1发送DMA传输过来的数据
HAL_UART_Transmit_DMA(&huart1, tx_data, sizeof(tx_data)-1);
```

重定义弱函数：

```c
// 重定义 USART 发送完成后所调用的弱函数
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
        printf("DMA Finished \n");
    }
}
```

### 2.6 DMA案例2: 外设（串口）到 RAM

#### ① 需求描述和硬件电路

![P02-%E9%9C%80%E6%B1%82.png](https://p.sda1.dev/24/5c8e93918b3cfb69f0acd3edd5d1b638/P02-需求.png)

需求：使用寄存器实现把串口Rx引脚接收到的数据通过DMA直接传输到内存中

电路：USART1_RX 连接 DMA1 通道5。

#### ② 寄存器方式实现

**DMA 驱动程序与案例1不同的地方：**

```
1. 使用 DMA1 的通道5
2. 传输方向是外设到RAM （CCR.DIR=0）
3. 开启循环模式实现一直可以接收数据 （CCR.CIRC=1）
```

#### ③ HAL 库方式实现

**STM32CubeMX 配置：**

![P02-%E5%A4%96%E8%AE%BE%E5%88%B0RAM-HAL-01.png](https://p.sda1.dev/22/c678f75a407d81e80519574f0bd02801/P02-外设到RAM-HAL-01.png)
![P01-RAM%E5%88%B0%E5%A4%96%E8%AE%BE-HAL02.png](https://p.sda1.dev/24/d25f866a81c6f08b2b8d22991025c886/P01-RAM到外设-HAL02.png)

**相关HAL库函数：**

```
HAL_UART_Receive_DMA()				USART将串口接收的数据通过DAM传输给存储器
__weak HAL_UART_RxCpltCallback() 	USART接收完成回调函数
```

**代码：**

开始接收：

```c
// 定义数组，用于接收数据
uint8_t rx_buffer[100]
// 配置 DMA 传输
HAL_UART_Receive_DMA(&huart1, rx_buffer, 12);
```

重定义弱函数：

```c
// 重定义 USART1 的接收完成回调函数
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1)
  {
    // 处理接收到的数据
    printf("Received data: %s\n", rx_buffer);
  }
}
```



## 附录

#### 单词

```
direct		adj.直接的
memory		n.存储器
access		v.访问
Channel		n.管道，通道
transfer	v.传输
half		n.一半，二分之一
complete	adj.完成的
priority	n.优先级
Peripheral	n.（计算机的）外部设备
circular	adj.圆形的，环形的；循环的，
```

#### DMA 相关HAL 库函数总结

```
HAL_UART_Transmit_DMA()		USART将DMA输出的数据通过串口发送
HAL_UART_Receive_DMA()		USART将串口接收的数据通过DAM传输给存储器
__weak HAL_UART_TxCpltCallback()  输出完成自动调用
__weak HAL_UART_RxCpltCallback() U
```