#include "Int_EEPROM.h"

// 静态函数：向一个页内写入数据（指定位置指定长度）
// @param addr 要写入的地址
// @param write_datas 要写入的数据
// @param write_len 要写入的长度
static void Int_EEPROM_WriteIntoPage(uint8_t addr, uint8_t *write_datas, uint16_t write_len)
{
    // 发送起始信号
    Dri_I2C2_Start();

    // 发送从设备地址+写标识, 并接收应答信号
    Dri_I2C2_TransmitSlaveAddr(DEV_ADDR_W);
    Dri_I2C2_ReceiveACK();

    // 发送EEPROM内部地址，并接收应答信号
    Dri_I2C2_TransmitByte(addr);
    Dri_I2C2_ReceiveACK();

    // 循环向EEPROM发送多个字节，每发送一个字节就接收一个应答信号
    for (uint16_t i = 0; i < write_len; i++)
    {
        Dri_I2C2_TransmitByte(write_datas[i]);
        Dri_I2C2_ReceiveACK();
    }

    // 发送停止信号
    Dri_I2C2_Stop();

    // 进入写周期延时5ms
    Com_Delay_ms(5);
}

/**
 * @brief 向指定位置写入指定长度的数据
 *
 * @param addr          要写入的位置
 * @param write_datas   要写入的数据
 * @param write_len     要写入的长度
 */
void Int_EEPROM_WriteData(uint8_t addr, uint8_t *write_datas, uint16_t write_len)
{

    uint8_t page_remain;

    while (1)
    {
        // 计算当前页剩余长度
        page_remain = PAGE_SIZE - addr % PAGE_SIZE;

        // 判断当前页剩余长度是否足够, 如果足够直接写入并结束循环
        if (write_len <= page_remain)
        {
            Int_EEPROM_WriteIntoPage(addr, write_datas, write_len);
            return;
        }
        // 如果当前页剩余长度不够，先写满当前页，再换页
        else
        {
            // 写满当前页剩余长度
            Int_EEPROM_WriteIntoPage(addr, write_datas, page_remain);
            // 待写入长度减去已经写入的长度
            write_len -= page_remain;
            // 写入地址要后移已写入长度
            addr += page_remain;
            // 写入数据地址后移已写入长度
            write_datas += page_remain;
        }
    }
}

/**
 * @brief 从指定位置读取指定长度的数据
 *
 * @param addr          要读取的位置
 * @param read_datas    读取到的数据保存到此处
 * @param read_len      要读取的长度
 */
void Int_EEPROM_ReadData(uint8_t addr, uint8_t *read_datas, uint16_t read_len)
{

    // -----------------------伪写-------------------------
    // 发送起始信号
    Dri_I2C2_Start();

    // 发送从设备地址+写标识, 并接收应答信号
    Dri_I2C2_TransmitSlaveAddr(DEV_ADDR_W);
    Dri_I2C2_ReceiveACK();
    
    // 发送EEPROM内部地址，并接收应答信号
    Dri_I2C2_TransmitByte(addr);
    Dri_I2C2_ReceiveACK();

    // -----------------------正式读-------------------------
    // 发送起始信号
    Dri_I2C2_Start();
    
    // 发送从设备地址+读标识, 并接收应答信号
    Dri_I2C2_TransmitSlaveAddr(DEV_ADDR_R);
    Dri_I2C2_ReceiveACK();

    // 循环读取多个字节，每读取一个字节就发送一个应答信号
    for (uint16_t i = 0; i < read_len; i++)
    {
        // 最后一个字节发送非应答信号
        if (i == read_len - 1)
        {
            Dri_I2C2_TransmitACK(1);

            // 发送停止信号,需要接收最后一个字节前提前设置
            Dri_I2C2_Stop();
        }
        // 其他字节发送应答信号
        else
        {
            Dri_I2C2_TransmitACK(0);
        }
        read_datas[i] = Dri_I2C2_ReceiveByte();
    }
}
