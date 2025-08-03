#include "Com_Util.h"
#include "Dri_USART1.h"
#include "Dri_SPI1.h"
#include "Int_NorFlash.h"

int main(void)
{
	// USART1 初始化
	Dri_USART1_Init();

	printf(" SPI1 Hardware :\n");

	// SPI1 初始化
	Dri_SPI1_Init();

	// 读取 NOR  Flash 厂商 ID 和产品 ID
    uint8_t manu_id = 0;
    uint16_t product_id = 0;
    Int_NorFlash_GetProductID(&manu_id, &product_id);
    printf("manu_id: 0x%02X, product_id: 0x%04X\n", manu_id, product_id);

	// 向 NOR Flash 写入数据
    uint8_t data[8] = "ABCDEFGH";
    Int_NorFlash_WriteData(0, 0, 0, data, 8);
	// 从 NOR Flash 读取数据
    uint8_t read_data[128] = {0};
    Int_NorFlash_ReadData(0, 0, 0, read_data, 8);
    printf("read_data: %s\n", read_data);

	// 防止跑飞
	while (1)
	{
	}
}
