#include "Int_NorFlash.h"

// 静态函数 ----------------------------------------------------------------------------------------------
// 打开 NorFlash 的写使能
static void Int_NorFlash_WriteEnable(void)
{
    Dri_SPI1_Start();
    Dri_SPI1_SwapByte(0x06); // 打开写使能指令
    Dri_SPI1_Stop();
}

// 关闭 NorFlash 的写使能
static void Int_NorFlash_WriteDisable(void)
{
    Dri_SPI1_Start();
    Dri_SPI1_SwapByte(0x04); // 关闭写使能指令
    Dri_SPI1_Stop();
}

// 等待是否处于 BUSY 状态
static void Int_NorFlash_WaitNotBusy(void)
{
    uint8_t status = 0;
    do
    {
        Dri_SPI1_Start();
        Dri_SPI1_SwapByte(0x05); // 读取状态寄存器指令
        status = Dri_SPI1_SwapByte(0x00);
        Dri_SPI1_Stop();
        // 延时 1ms
        Com_Delay_ms(1);
    } while (status & 0x01);
}

// 擦除整个扇区
static void Int_NorFlash_EraseSector(uint8_t block_addr, uint8_t sector_addr)
{
    // 1 等待 NOR Flash 不忙
    Int_NorFlash_WaitNotBusy();

    // 2 发送写使能指令
    Int_NorFlash_WriteEnable();

    // 3 发送扇区擦除指令
    Dri_SPI1_Start();
    Dri_SPI1_SwapByte(0x20); // 扇区擦除指令
    Dri_SPI1_SwapByte(block_addr);                   //块地址
    Dri_SPI1_SwapByte((sector_addr << 4) | 0x00);    //扇区地址+页地址
    Dri_SPI1_SwapByte(0x00);                         //字节地址
    Dri_SPI1_Stop();

    // 4 关闭写使能
    Int_NorFlash_WriteDisable();

    // 5 等待 NOR Flash 不忙
    Int_NorFlash_WaitNotBusy();
}
// 静态函数 ----------------------------------------------------------------------------------------------

void Int_NorFlash_GetProductID(uint8_t *manu_id, uint16_t *product_id)
{
    // 1 开始 SPI 通信
    Dri_SPI1_Start();

    // 2 向 SPI 发送命令
    Dri_SPI1_SwapByte(0x9F);

    // 3 接收厂商编号
    *manu_id = Dri_SPI1_SwapByte(0x00);
    // 4 接收产品 ID
    *product_id = Dri_SPI1_SwapByte(0x00) << 8;
    *product_id |= Dri_SPI1_SwapByte(0x00);
    
    // 5 停止 SPI 通信
    Dri_SPI1_Stop();
}

void Int_NorFlash_WriteData(uint8_t block_addr, uint8_t sector_addr, uint8_t page_addr, uint8_t *data, uint16_t data_len)
{
    // 0 擦除指定扇区
    Int_NorFlash_EraseSector(block_addr, sector_addr);

    // 1 等待 NOR Flash 不忙
    Int_NorFlash_WaitNotBusy();

    // 2 发送写使能指令
    Int_NorFlash_WriteEnable();

    // 3 发送页写入指令
    Dri_SPI1_Start();
    Dri_SPI1_SwapByte(0x02); // 页写入指令
    Dri_SPI1_SwapByte(block_addr);                   //块地址
    Dri_SPI1_SwapByte((sector_addr << 4) | (page_addr & 0x0F)); //扇区地址+页地址
    Dri_SPI1_SwapByte(0x00);                         //字节地址

    // 4 发送数据
    for (uint16_t i = 0; i < data_len; i++)
    {
        Dri_SPI1_SwapByte(data[i]);
    }

    // 5 停止 SPI 通信
    Dri_SPI1_Stop();

    // 6 关闭写使能
    Int_NorFlash_WriteDisable();
    
    // 7 等待 NOR Flash 不忙
    Int_NorFlash_WaitNotBusy();
}

void Int_NorFlash_ReadData(uint8_t block_addr, uint8_t sector_addr, uint8_t page_addr, uint8_t *data, uint16_t data_len)
{
    // 1 等待 NOR Flash 不忙
    Int_NorFlash_WaitNotBusy();

    // 2 发送页读取指令
    Dri_SPI1_Start();
    Dri_SPI1_SwapByte(0x03); // 页读取指令
    Dri_SPI1_SwapByte(block_addr);                   //块地址
    Dri_SPI1_SwapByte((sector_addr << 4) | (page_addr & 0x0F)); //扇区地址+页地址
    Dri_SPI1_SwapByte(0x00);                         //字节地址

    // 3 接收数据
    for (uint16_t i = 0; i < data_len; i++)
    {
        data[i] = Dri_SPI1_SwapByte(0x00);
    }

    // 4 停止 SPI 通信
    Dri_SPI1_Stop();

    // 5 等待 NOR Flash 不忙
    Int_NorFlash_WaitNotBusy();
}
