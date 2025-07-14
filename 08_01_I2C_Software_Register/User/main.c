#include <stdint.h>
#include "stm32f10x.h"
#include "Com_Util.h"
#include "Dri_USART1.h"
#include "Dri_I2C.h"
#include "Int_EEPROM.h"

// 主函数
int main()
{
	// USART1 初始化
	Dri_USART1_Init();
	// 打印信息
	printf("STM32 I2C Software Example: \n");

	// 初始化I2C
	Dri_I2C_Init();
	
	// // 进行一次I2C通信
	// // 发送起始信号
	// Dri_I2C_Start();
	// // 发送从设备地址+读写标识   1010000 + 0, 接收应答信号
	// Dri_I2C_TransmitByte(0xA0);
	// Dri_I2C_ReceiveACK();
	// // 发送一个字节,并接收应答信号
	// Dri_I2C_TransmitByte(0xBF);
	// Dri_I2C_ReceiveACK();
	// // 发送停止信号
	// Dri_I2C_Stop();

	// 定义要写入的数据
	// uint8_t write_bytes[] = "ABCDEFGHIGKLMNOPQRSTUVWXYZ";
	// 使用I2C向EEPROM写入数据
	// Int_EEPROM_WriteData(0x0E, write_bytes, 26);

	// 定义变量，保存读取到数据
	uint8_t read_bytes[27];
	Int_EEPROM_ReadData(0x0E, read_bytes, 26);
	// 打印读取到的数据
	printf("Read Data: %s\n", read_bytes);

	// 死循环
	while (1)
		;
}

