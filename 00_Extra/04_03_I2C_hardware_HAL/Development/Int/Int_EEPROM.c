#include "Int_EEPROM.h"

/**
 * @brief 向 EEPROM 的 某页 写入数据
 *
 * @param addr 页的某个地址
 * @param write_datas 要写入的数据
 * @param write_len 要写入的数据长度
 */
static void Int_EEPROM_WriteIntoPage(uint8_t addr, uint8_t *write_datas, uint8_t write_len)
{
    HAL_I2C_Mem_Write(&hi2c2, DEV_ADDR, addr, I2C_MEMADD_SIZE_8BIT,write_datas, write_len, HAL_MAX_DELAY);
    // 写周期 5ms
    HAL_Delay(5);
}

void Int_EEPROM_WriteData(uint8_t addr, uint8_t *write_datas, uint16_t write_len)
{
    uint8_t page_remain = 0; // 页内剩余的字节

    while (write_len > 0) // 一直写入
    {
        // 1 计算页内剩余的字节
        page_remain = EEPROM_PAGE_SIZE - (addr % EEPROM_PAGE_SIZE);

        // 2 如果写入长度大于页内剩余的字节，需要分页写入
        if (write_len > page_remain)
        {
            // 2.1 写入页内剩余的字节
            Int_EEPROM_WriteIntoPage(addr, write_datas, page_remain);
            // 2.2 写入剩余的字节
            addr += page_remain;
            write_datas += page_remain;
            write_len -= page_remain;
        }
        else
        {
            // 2.1 可以直接写入页内
            Int_EEPROM_WriteIntoPage(addr, write_datas, write_len);
            // 2.2 写入完成，退出循环
            break;
        }
    }
}

void Int_EEPROM_ReadData(uint8_t addr, uint8_t *read_datas, uint16_t read_len)
{
    HAL_I2C_Mem_Read(&hi2c2, DEV_ADDR, addr ,I2C_MEMADD_SIZE_8BIT ,read_datas, read_len, HAL_MAX_DELAY);
}
