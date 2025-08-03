#include "Dri_USART1.h"
#include "Dri_ADC1.h"
#include "Dri_SPI1.h"
#include "Int_NorFlash.h"

// 主函数
int main()
{
    // USART1 初始化
    Dri_USART1_Init();

    // 打印信息
    printf("STM32 SPI Example: \n");
    // SPI 初始化
    Dri_SPI1_Init();

    // 读取 NOR  Flash 厂商 ID 和产品 ID
    uint8_t manu_id = 0;
    uint16_t product_id = 0;
    Int_NorFlash_GetProductID(&manu_id, &product_id);
    printf("manu_id: 0x%02X, product_id: 0x%04X\n", manu_id, product_id);

    // 用于写入数据的数组
    uint8_t write_datas[] = "Hello World !";
    // 用于读取数据的数组
    uint8_t read_datas[256];

    // 写入数据 (页编程) (每页最大 256 字节)
    Int_NorFlash_WritePageData(0x2F,0x0F,0x00,write_datas,sizeof(write_datas) - 1);
    // 读取数据 (页读取) (每页最大 256 字节)
    Int_NorFlash_ReadPageData(0x2F,0x0F,0x00,0x00,read_datas,sizeof(write_datas) - 1);

    // 打印读取数据
    printf("read_datas: %s\n", read_datas);

    // 死循环
    while (1)
    {
    }
}
