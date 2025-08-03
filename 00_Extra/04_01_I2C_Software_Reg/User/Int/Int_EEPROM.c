#include "Int_EEPROM.h"
#include "Dri_I2C.h"
#include "Dri_USART1.h"

/**
 * @brief 向 EEPROM 的 某页 写入数据
 *
 * @param addr 页的某个地址
 * @param write_datas 要写入的数据
 * @param write_len 要写入的数据长度
 */
static void Int_EEPROM_WriteIntoPage(uint8_t addr, uint8_t *write_datas, uint8_t write_len)
{
    uint8_t ACK = 0;

    // 0 发送起始信号
    Dri_I2C_Start();

    // 1 发送设备地址 + 读写标识
    Dri_I2C_TransmitByte(DEV_ADDR_W);
    ACK = Dri_I2C_ReceiveAck(); // 接收应答信号
    if (ACK != 0)
    {
        printf("EEPROM_WriteIntoPage1 设备地址发送失败\n");
        return;
    }

    // 2 发送页地址
    Dri_I2C_TransmitByte(addr);
    ACK = Dri_I2C_ReceiveAck(); // 接收应答信号
    if (ACK != 0)
    {
        printf("EEPROM_WriteIntoPage2 页地址发送失败\n");
        return;
    }

    // 3 发送数据 (从addr处开始写入)
    for (uint8_t i = 0; i < write_len; i++)
    {
        Dri_I2C_TransmitByte(write_datas[i]);
        ACK = Dri_I2C_ReceiveAck(); // 接收应答信号
        if (ACK != 0)
        {
            printf("EEPROM_WriteIntoPage3 数据发送失败\n");
            return;
        }
    }

    // 4 发送停止信号
    Dri_I2C_Stop();

    // 5 写周期 5ms
    Com_Delay_ms(5);
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
    uint8_t ACK = 0;

    // 1 发送设备地址 + 读写标识 ---------------------- 伪写 ---------------------------
    Dri_I2C_Start();
    Dri_I2C_TransmitByte(DEV_ADDR_W);
    ACK = Dri_I2C_ReceiveAck(); // 接收应答信号
    if (ACK != 0)
    {
        printf("EEPROM_ReadData1 设备地址发送失败\n");
        return;
    }

    // 2 发送页地址
    Dri_I2C_TransmitByte(addr);
    ACK = Dri_I2C_ReceiveAck(); // 接收应答信号
    if (ACK != 0)
    {
        printf("EEPROM_ReadData2 页地址发送失败\n");
        return;
    }

    // 3 发送重新启动信号 ---------------------- 伪写结束 ---------------------------
    Dri_I2C_Start();
    Dri_I2C_TransmitByte(DEV_ADDR_R); // 发送设备地址 + 读写标识
    ACK = Dri_I2C_ReceiveAck();       // 接收应答信号
    if (ACK != 0)
    {
        printf("EEPROM_ReadData3 设备地址发送失败\n");
        return;
    }

    // 4 读取数据
    for (uint16_t i = 0; i < read_len; i++)
    {
        read_datas[i] = Dri_I2C_ReceiveByte();
        if (i == read_len - 1)
        {
            // 4.1 最后一个字节发送NACK
            Dri_I2C_TransmitACK(1);
        }
        else
        {
            // 4.2 其他字节发送ACK
            Dri_I2C_TransmitACK(0);
        }
    }

    // 5 发送停止信号
    Dri_I2C_Stop();
}
