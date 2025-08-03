#include "Int_NorFlash.h"
#include "Dri_SPI1.h"

// 静态函数 ----------------------------------------------------------------------------------------------
// 打开 Norflash 写使能
static void Int_NorFlash_OpenWriteEnable(void)
{
    // 1 开始 SPI 通信
    Dri_SPI1_Start();

    // 2 向 SPI 发送命令
    Dri_SPI1_SwapByte(0x06);

    // 3 停止 SPI 通信
    Dri_SPI1_Stop();
}

// 关闭 NorFlash 写使能
static void Int_NorFlash_CloseWriteEnable(void)
{
    // 1 开始 SPI 通信
    Dri_SPI1_Start();

    // 2 向 SPI 发送命令
    Dri_SPI1_SwapByte(0x04);

    // 3 停止 SPI 通信
    Dri_SPI1_Stop();
}

// 等待 NorFlash 不忙
static void Int_NorFlash_WaitNotBusy(void)
{
    uint8_t status;
    do
    {
        // 1 开始 SPI 通信
        Dri_SPI1_Start();

        // 2 向 SPI 发送命令
        Dri_SPI1_SwapByte(0x05);

        // 3 接收状态寄存器
        status = Dri_SPI1_SwapByte(0x00);

        // 4 停止 SPI 通信
        Dri_SPI1_Stop();

        // 5 延时 1ms
        Com_Delay_ms(1);
    } while (status & 0x01);
}

// 擦除 NorFlash 的一个扇区
static void Int_NorFlash_EraseSector(uint8_t block_addr, uint8_t sector_addr)
{
    // 1 等待 NorFlash 不忙
    Int_NorFlash_WaitNotBusy();

    // 2 打开写使能
    Int_NorFlash_OpenWriteEnable();
    
    // 3 扇区擦除 -----------------------------------
    // 开始 SPI 通信
    Dri_SPI1_Start();

    // 向 SPI 发送命令 扇区擦除命令
    Dri_SPI1_SwapByte(0x20);

    // 发送地址
    Dri_SPI1_SwapByte(block_addr);   // 块地址
    Dri_SPI1_SwapByte(sector_addr << 4);  // 扇区地址
    Dri_SPI1_SwapByte(0x00);         // 字节地址

    // 停止 SPI 通信
    Dri_SPI1_Stop();
    // 扇区擦除 -----------------------------------

    // 4 关闭写使能
    Int_NorFlash_CloseWriteEnable();
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

void Int_NorFlash_WritePageData(uint8_t block_addr, uint8_t sector_addr, uint8_t page_addr, uint8_t *write_datas, uint16_t len)
{
    // 0 清除扇区
    Int_NorFlash_EraseSector(block_addr, sector_addr);

    // 1 等待 NorFlash 不忙
    Int_NorFlash_WaitNotBusy();

    // 2 打开写使能
    Int_NorFlash_OpenWriteEnable();

    // 3 页编程 -----------------------------------
    // 开始 SPI 通信
    Dri_SPI1_Start();

    // 向 SPI 发送命令 页编程命令
    Dri_SPI1_SwapByte(0x02);

    // 发送地址
    Dri_SPI1_SwapByte(block_addr);   // 块地址
    Dri_SPI1_SwapByte((sector_addr << 4) + (page_addr & 0x0F));  // 扇区地址 + 页地址
    Dri_SPI1_SwapByte(0x00);    // 字节地址

    // 发送数据
    for (uint16_t i = 0; i < len; i++)
    {
        Dri_SPI1_SwapByte(write_datas[i]);
    }

    // 停止 SPI 通信
    Dri_SPI1_Stop();
    // 页编程 -----------------------------------

    // 4 关闭写使能
    Int_NorFlash_CloseWriteEnable();
}

void Int_NorFlash_ReadPageData(uint8_t block_addr, uint8_t sector_addr, uint8_t page_addr, uint8_t byte_addr, uint8_t *read_datas, uint16_t len)

{
    // 1 等待 NorFlash 不忙
    Int_NorFlash_WaitNotBusy();

    // 2 页读取 -----------------------------------
    // 开始 SPI 通信
    Dri_SPI1_Start();

    // 向 SPI 发送命令 页读取命令
    Dri_SPI1_SwapByte(0x03);

    // 发送地址
    Dri_SPI1_SwapByte(block_addr);   // 块地址
    Dri_SPI1_SwapByte((sector_addr << 4) + (page_addr & 0x0F));  // 扇区地址 + 页地址
    Dri_SPI1_SwapByte(byte_addr);    // 字节地址

    // 接收数据
    for (uint16_t i = 0; i < len; i++)
    {
        read_datas[i] = Dri_SPI1_SwapByte(0x00);
    }

    // 停止 SPI 通信
    Dri_SPI1_Stop();
    // 页读取 -----------------------------------
}
