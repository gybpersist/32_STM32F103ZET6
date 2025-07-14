#include <stdint.h>
#include "stm32f10x.h"
#include "Com_Util.h"
#include "Dri_USART1.h"
#include "Dri_I2C2.h"
#include "Int_EEPROM.h"

// 主函数
int main()
{
	// USART1 初始化
	Dri_USART1_Init();
	// 打印信息
	printf("STM32 I2C Hardware Example: \n");

	// 初始化I2C
	Dri_I2C2_Init();

	// 定义要写入的数据
	uint8_t write_bytes[] = "ABCDEFGHIGKLMNOPQRSTUVWXYZ";
	// 使用I2C向EEPROM写入数据
	Int_EEPROM_WriteData(0x0E, write_bytes, 26);

	// // 定义变量，保存读取到数据
	// uint8_t read_bytes[27];
	// Int_EEPROM_ReadData(0x0E, read_bytes, 26);
	// // 打印读取到的数据
	// printf("Read Data: %s\n", read_bytes);

	// 死循环
	while (1)
		;
}

