#include "Com_Util.h"
#include "Dri_USART1.h"
#include "Dri_I2C2.h"
#include "Int_EEPROM.h"

uint8_t write_datas[] = "12345678901234565201314HUIHUI";
uint8_t read_datas[256] = {0};

int main(void)
{	
	// USART1 初始化
	Dri_USART1_Init();

	printf("5201314");
	printf("HUIHUI\n");

	// I2C2 初始化
    Dri_I2C2_Init();

	// 向指定位置写入指定长度的数据
    // Int_EEPROM_WriteData(0x20, write_datas, sizeof(write_datas) - 1);
	// 从指定位置读取指定长度的数据
    Int_EEPROM_ReadData(0x20, read_datas, sizeof(write_datas) - 1);
	// // 打印读取到的数据
    printf("读取到的数据: %s\n", read_datas);

	// 防止跑飞
	while (1)
	{

	}
}
