## 第 4 章 SPI 通信

### 4.1 SPI 通信概述

#### ① 概念

SPI，Serial Peripheral interface，串行外围设备接口。 

是一种**高速的，全双工，同步**的**串行**通信总线。  

#### ② 物理层

SPI设备一般有4条线：

**SCLK**：时钟线，主设备提供的时钟信号，用于同步数据传输。

**MOSI**：数据线，主设备发送数据，从设备接收数据。

**MISO**：数据线，从设备发送数据，主设备接收数据。

**SS**：片选线，主设备选择从设备（低电平有效），也称为 **NSS** 或 **CS** ， 每增加一个从设备，需增加一根 SS 线。 

![SPI.%E7%89%A9%E7%90%86%E5%B1%82.png](https://p.sda1.dev/22/c69dd22693b460ebf24bfa190dcd23cf/SPI.物理层.png)

注意： 只能主从设备之间进行通信，从与从之间是无法进行通信的。

#### ③ 协议层

##### (1) 数据交换

![SPI-%E5%8D%8F%E8%AE%AE%E5%B1%82-%E6%95%B0%E6%8D%AE%E4%BA%A4%E6%8D%A2.png](https://p.sda1.dev/22/4194731aebe0412993865a03f78e6916/SPI-协议层-数据交换.png)

主设备和从设备的**移位寄存器**临时存储要交换的数据。**输入寄存器**存储最终得到的值。

来一个时钟**上升沿**信号（模式3），主机和从机分别把自己高位的值**左移**出来(**一般高位先行**)。

来一个时钟**下降沿**信号（模式3），主机和从机分别读入数据，存储到移位寄存器的**低位。**

经过 8 次同样的操作，就完成了一个字节的交换**，**最后从输入寄存器就可以读出数据了。

##### (2) 时钟的极性

CPOL：时钟极性 (Clock polarity) 
置0： 空闲状态时，SCK保持低电平；
置1： 空闲状态时，SCK保持高电平。

![SPI-%E5%8D%8F%E8%AE%AE%E5%B1%82-%E6%97%B6%E9%92%9F%E6%9E%81%E6%80%A7.png](https://p.sda1.dev/22/cb8604e8da4b93993adae6e1cd284c15/SPI-协议层-时钟极性.png)

##### (3) 时钟的相位

CPHA：时钟相位 (Clock phase) 
置0： 数据采样从第一个时钟边沿开始；
置1： 数据采样从第二个时钟边沿开始。

![SPI-%E5%8D%8F%E8%AE%AE%E5%B1%82-%E6%97%B6%E9%92%9F%E7%9B%B8%E4%BD%8D.png](https://p.sda1.dev/22/f3967ad99ba3ec90e8bb77be199722af/SPI-协议层-时钟相位.png)



##### (4) SPI 的 4 种模式

时钟的2种极性和2种相位的不同组合，得到了SPI的4种工作模式：

| **模式**   | **CPOL (Clock Polarity)** | **CPHA (Clock Phase)** | **描述**                                                 |
| :--------- | :------------------------ | :--------------------- | :------------------------------------------------------- |
| **模式 0** | 0                         | 0                      | 时钟空闲时为低电平，<br>数据在时钟的第一个时钟边沿采样。 |
| **模式 1** | 0                         | 1                      | 时钟空闲时为低电平，<br>数据在时钟的第二个时钟边沿采样。 |
| **模式 2** | 1                         | 0                      | 时钟空闲时为高电平，<br>数据在时钟的第一个时钟边沿采样。 |
| **模式 3** | 1                         | 1                      | 时钟空闲时为高电平，<br>数据在时钟的第二个时钟边沿采样。 |

模式0和模式2：

![SPI-%E5%8D%8F%E8%AE%AE%E5%B1%82-4%E4%B8%AD%E6%A8%A1%E5%BC%8F-01.png](https://p.sda1.dev/22/b828d627cf65416921cc73c16f594750/SPI-协议层-4中模式-01.png)

模式1和模式3：

![SPI-%E5%8D%8F%E8%AE%AE%E5%B1%82-4%E4%B8%AD%E6%A8%A1%E5%BC%8F-02.png](https://p.sda1.dev/22/e8096a73d60bbce2af01993d00d8b7b5/SPI-协议层-4中模式-02.png)

> 注意：很多设备仅支持模式0和模式3。模式0和模式3都是上升沿采集数据。

### 4.2  W25Q32 （NorFlash）

#### ① 产品介绍和引脚说明

W25Q32是一种使用SPI通讯协议的**NOR FLASH**存储器 ， NOR 闪存的特点是**支持随机访问，读取速度快，适合存储和执行代码**，但写入和擦除速度较慢，成本较高。 

**引脚：**

![W23Q23-%E5%BC%95%E8%84%9A.png](https://p.sda1.dev/22/53738b6bc7061fb6689ca7e274405fc6/W23Q23-引脚.png)

| 引脚编号 | 引脚名称 | 类型 | 功能描述                                            |
| :------- | :------- | :--- | :-------------------------------------------------- |
| 1        | CS#      | 输入 | 片选引脚，低电平有效。                              |
| 2        | DO       | 输出 | 数据输出引脚，用于输出数据。                        |
| 3        | WP#      | 输入 | 写保护引脚，低电平有效。                            |
| 4        | GND      | 电源 | 接地引脚。                                          |
| 5        | DI       | 输入 | 数据输入引脚，用于输入数据。                        |
| 6        | CLK      | 输入 | 串行时钟引。                                        |
| 7        | HOLD#    | 输入 | 保持引脚，可暂停通讯，低电平有效。                  |
| 8        | VCC      | 电源 | 电源引脚，提供芯片工作电压（通常为 2.7V 至 3.6V）。 |

**注意事项：**

1. 这个flash芯片只支持**模式0**和**模式03**。
2. 写的时候必须是先擦除，擦除后再写入。
3. 移位是高位优先。

#### ② 功能框图和产品结构

![W23Q23-%E5%8A%9F%E8%83%BD%E6%A1%86%E5%9B%BE.png](https://p.sda1.dev/22/b76d166fa4c265783b370c5b03803dc3/W23Q23-功能框图.png)

***

**总容量**：32M-bit（4MB）。

**块（Block）**：64 个块，每个块 64KB。		// 1MB = 1024KB

**扇区（Sector）**：每个块 16 个扇区，每个扇区 4KB。总扇区数：64*16=1024 个。

**页（Page）**：每个扇区 16 个页，每个页 256 字节。总页数：1024*16=16384 个。

***

```
地址共24bit
23-22: 	保留
21-16： 块地址(6bit)
15-12:  扇区地址(4bit)
11-8：  页地址(4bit)
7-0:   	字节地址(8bit)
```

#### ③ 写操作注意事项

```
（1）写入操作前，必须先进行写使能。
（2）每个数据位只能由1改写为0，不能由0改写为1。
（3）写入数据前必须先檫除，檫除后，所有数据位变为1。擦除必须按最小擦除单元（扇区）进行。
（4）连续写入多字节时，最多写入一页的数据，超过页尾位置的数据，会回到页首覆盖写入。
（5）写入操作结束后，芯片进入忙状态，不响应新的读写操作。
```

#### ④ 读操作注意事项

```
（1）直接调用读取时序，无需读使能，无需额外操作，没有页的限制。
（2）读取操作结束后不会进入忙状态，但不能在忙状态时读取。
```

#### ④ 读写指令

![W23Q23-%E8%AF%BB%E5%86%99%E6%8C%87%E4%BB%A4.png](https://p.sda1.dev/22/850f97fef0854ed3af33892a2617ed4c/W23Q23-读写指令.png)

### 4.3 案例1：纯软件实现SPI通信读写Flash

#### ① SPI 软件驱动实现

```
初始化函数 Driver_SPI_Init()
	1. 对 GPIOA、GPIOC 时钟使能
	2. 引脚模式设置
	   2.1 SS(PC13)设置为通用推挽输出	
	   2.2 SCLK(PA5)设置为通用推挽输出
	   2.3 MISO(PA6)设置为浮空输入
	   2.4 MOSI(PA7)设置为通用推挽输出
	3. 拉低片选信号，默认不选中
	4. 拉低时钟，模式0的空闲状态
	
开始传输函数 Driver_SPI_Start()
	拉低片选信号

结束传输函数 Driver_SPI_Stop()
	拉高片选信号
	
交换一个字节的函数 Driver_SPI_SwapByte() 参数是要发送的字节，返回值是接收的字节
	1. 循环逐bit发送
	2. 每次循环：
		① 将byte当前的最高位发送到MOSI
		② byte左移1位，已发送的数据已经移走，最低位已经空出来
		③ 拉高SCLK，让从设备采样
		④ 读取MISO，放入byte的最低位
		⑤ 延时让高电平持续一小会
		④ 拉低并延时，让从设备准备数据，主设备下次循环直接修改MOSI
	3. 返回byte

```

#### ② W25Q32 Flash 程序

```c
// 静态函数：打开NorFlash的写使能
static void Int_NorFlash_OpenWriteEnable(void)

// 静态函数：关闭NorFlash的写使能
static void Int_NorFlash_CloseWriteEnable(void)

// 静态函数：等待NorFlash不忙
static void Int_NorFlash_WaitNotBusy(void)

// 静态函数：擦除NorFlash的一个扇区
static void Int_NorFlash_EraseSector(uint8_t block_addr, uint8_t sector_addr)
/*********************************************************************/


/**
 * @brief 获取产品ID
 *
 * @param mid 用于保存厂商编号
 * @param pid 用于保存产品ID
 */
void Int_NorFlash_GetProductionID(uint8_t *mid, uint16_t *pid);

/**
 * @brief 写入数据
 *
 * @param block_addr    块地址
 * @param sector_addr   扇区地址
 * @param page_addr     页地址
 * @param datas         待写入数据
 * @param len           待写入数据长度
 */
void Int_NorFlash_WriteData(uint8_t block_addr, uint8_t sector_addr, uint8_t page_addr, uint8_t *datas, uint16_t len);

/**
 * @brief 读取数据
 * 
 * @param block_addr    块地址 
 * @param sector_addr   扇区地址
 * @param page_addr     页地址
 * @param byte_addr     字节地址
 * @param datas         将读取到的数据保存到该地址处
 * @param len           读取数据长度
 */
void Int_NorFlash_ReadData(uint8_t block_addr, uint8_t sector_addr, uint8_t page_addr, uint8_t byte_addr, uint8_t *datas, uint16_t len);
```



### 4.4 STM32 的 SPI 片上外设

#### ① 概述

STM32 的 SPI 外设可用作通讯的主机及从机，支持最高的 SCK 时钟频率为 fpclk/2 （STM32F103 型号的芯片默认fpclk1为36MHz，fpclk2为72MHz。），完全支持 SPI 协议的 4 种模式，数据帧长度可设置为 8 位或 16 位，可设置数据 MSB 先行或 LSB 先行。它还支持双线全双工、双线单向以及单线模式。

STM32F103系列提供了 **3** 个SPI，SPI1挂在APB2总线，SPI2/3挂在APB1总线。

其中双线单向模式可以同时使用 MOSI 及 MISO 数据线向一个方向传输数据，可以加快一倍的传输速度。而单线模式则可以减少硬件接线，当然这样速率会受到影响。

用的比较多还是**双线全双工模式**。

#### ② 功能框图

![SPI%E5%8A%9F%E8%83%BD%E6%A1%86%E5%9B%BE.png](https://p.sda1.dev/22/0f56d379550f0d5687c1349d48ac20db/SPI功能框图.png)

#### ③ 相关寄存器

| 寄存器名称     | 描述            | 相关控制位/标志位                                            |
| -------------- | --------------- | ------------------------------------------------------------ |
| SPIx->CR1      | 控制寄存器1     | BIDIMODE: 双向数据模式使能，0表示双线双向，1表示双线单向<br>BIDIOE: 双向模式下输出使能，0表示只收，1表示只发。<br>DFF: 数据帧格式，0表示8为，1表示16位<br>RXONLY: 只接收，0表示全双工，1表示只接收<br>SSM: 软件从设备管理，置1表示NSS引脚电平由SSI位控制<br>SSI: 在SSM=1时有效，决定NSS引脚电平，外部I/O操作无效。<br>LSBFIRST: 0表示MSB先行，1表示LSB先行。<br>SPE: SPI 使能，置1生效<br>BR: 时钟分频（SPI频率）,SPI至少小于18MHZ<br>MSTR: 主设备选择，0配置为从设备，1配置为主设备<br>CPOL: 时钟极性，0表示空闲状态SCK低电平，1表示空闲状态SCK高电平<br>CPHA: 时钟相位，0表示第一个边沿开始，1表示第二个边沿开始 |
| SPIx->CR2      | 控制寄存器2     | TXEIE: 发送缓冲区空中断使能<br>RXNEIE: 接收缓冲区非空中断使能<br>SSOE: SS 输出使能。 |
| SPIx->SR       | 状态寄存器      | TXE: 发送缓冲区空，可写入下个字节<br>RXNE: 接收缓冲区非空，接收完成 |
| SPIx->DR       | 数据寄存器      | 16bit，如果数据帧设置为8位，只使用7~0位<br>发送缓冲区和接收缓冲区是两个寄存器，但名称和地址相同 |
| SPIx->CRCPR    | CRC多项式寄存器 |                                                              |
| SPIx->RXCRCR   | RxCRC寄存器     |                                                              |
| SPIx->TXCRCR   | TxCRC寄存器     |                                                              |
| SPIx->I2S_GFGR | I²S配置寄存器   | I2SMODE: I²S模式选择，0表示SPI，1表示I²S                     |
| SPIx->I2SPR    | I²S预分频寄存器 |                                                              |

**通信频率设置，CR1寄存器之BR[2:0]：**

```
000： fPCLK/2 
001： fPCLK/4 
010： fPCLK/8 
011： fPCLK/16 
100： fPCLK/32 
101： fPCLK/64 
110： fPCLK/128 
111： fPCLK/256 
```



### 4.5 案例：硬件实现SPI通信读写 NorFlash

#### ① 寄存器方式

**SPI 驱动程序：**

```
初始化函数 Dri_SPI_Init：
	1. 时钟使能 (对SPI1、GPIOA、GPIOC 时钟使能)
	2. 引脚模式设置
		2.1 SS (PC13) 设置为通用推挽输出
		2.2 SCLK (PA5) 设置为复用推挽输出
		2.3 MOSI (PA7) 设置为复用推挽输出
		2.2 MISO (PA6) 设置为浮空输入或者复用推挽输出
	3. 片选信号拉高
	4. SPI1 设置
		4.1 设置分频系数，至少进行4分频（CR.BR=001）
		4.2 设置 SPI 极性为0 (CPOL=0，默认值)；相位为0 (CPHA=0，默认值)
    	4.3 设置 SPI 数据格式为8位（CR1.DFF=0，默认值）
    	4.4 设置 SPI 高位先行（CR1.LSBFIRST=0，默认值）
    	4.5 强制设置NSS为高电平不受外部控制（CR1.SSM=1，CR1.SSI=1）
    	4.6 设置 SPI 为主模式（CR1.MSTR=1）
    	4.7 设置 SPI 使能（CR1.SPE=1）
        
开始通信函数 Dri_SPI_Start:
	拉低SS

结束通信函数 Dri_SPI_Start:
	拉高SS
		
交换数据的函数 Dri_SPI_SwapByte:
	1. 等待发送缓冲区为空(SR.TXE)
	2. 将发送的数据写入DR
	3. 等待接收缓冲区接收到数据(SR.RXNE)
	4. 返回DR寄存器的值
	
```

#### ② HAL 库方式

**STM32CubeMX 配置：**

![P01-HAL01.png](https://p.sda1.dev/22/06dbacf8c94b1acc09e27a1f9d80bfe1/P01-HAL01.png)
![P01-HAL02.png](https://p.sda1.dev/22/506af9d95699dac73bc776a6b95a61ea/P01-HAL02.png)

**HAL 库相关函数：**

```
HAL_SPI_TransmitReceive()		交换数据
```

**spi.c** 添加如下代码：

```c
// 函数实现：开始 SPI 通信
void SPI_Start(void)
{
    HAL_GPIO_WritePin(SS_GPIO_Port, SS_Pin, GPIO_PIN_RESET); // 拉低片选信号
}

// 函数实现：结束 SPI 通信
void SPI_Stop(void)
{
    HAL_GPIO_WritePin(SS_GPIO_Port, SS_Pin, GPIO_PIN_SET); // 拉高片选信号
}

// 函数实现：交换一个字节的数据
uint8_t SPI_SwapByte(uint8_t tx_byte)
{
    uint8_t rx_byte = 0;
    HAL_SPI_TransmitReceive(&hspi1, &tx_byte, &rx_byte, 1, HAL_MAX_DELAY);
    return rx_byte; 
}
```

norfalsh 相关操作的代码与寄存器方式相同。



## 附录

### 单词

```
sector	n. 部门；扇形，扇区；象限仪；函数尺
erase	v. 抹去，擦掉；删除
```

### SPI HAL 库相关函数

```
HAL_SPI_TransmitReceive()		交换数据
```






