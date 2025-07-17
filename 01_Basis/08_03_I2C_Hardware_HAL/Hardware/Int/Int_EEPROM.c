#include "Int_EEPROM.h"

// ��̬��������һ��ҳ��д�����ݣ�ָ��λ��ָ�����ȣ�
// @param addr Ҫд��ĵ�ַ
// @param write_datas Ҫд�������
// @param write_len Ҫд��ĳ���
static void Int_EEPROM_WriteIntoPage(uint8_t addr, uint8_t *write_datas, uint16_t write_len)
{
    HAL_I2C_Mem_Write(&hi2c2,DEV_ADDR,addr,I2C_MEMADD_SIZE_8BIT,write_datas,write_len,HAL_MAX_DELAY);

    // ����д������ʱ5ms
    HAL_Delay(5);
}

/**
 * @brief ��ָ��λ��д��ָ�����ȵ�����
 *
 * @param addr          Ҫд���λ��
 * @param write_datas   Ҫд�������
 * @param write_len     Ҫд��ĳ���
 */
void Int_EEPROM_WriteData(uint8_t addr, uint8_t *write_datas, uint16_t write_len)
{

    uint8_t page_remain;

    while (1)
    {
        // ���㵱ǰҳʣ�೤��
        page_remain = PAGE_SIZE - addr % PAGE_SIZE;

        // �жϵ�ǰҳʣ�೤���Ƿ��㹻, ����㹻ֱ��д�벢����ѭ��
        if (write_len <= page_remain)
        {
            Int_EEPROM_WriteIntoPage(addr, write_datas, write_len);
            return;
        }
        // �����ǰҳʣ�೤�Ȳ�������д����ǰҳ���ٻ�ҳ
        else
        {
            // д����ǰҳʣ�೤��
            Int_EEPROM_WriteIntoPage(addr, write_datas, page_remain);
            // ��д�볤�ȼ�ȥ�Ѿ�д��ĳ���
            write_len -= page_remain;
            // д���ַҪ������д�볤��
            addr += page_remain;
            // д�����ݵ�ַ������д�볤��
            write_datas += page_remain;
        }
    }
}

/**
 * @brief ��ָ��λ�ö�ȡָ�����ȵ�����
 *
 * @param addr          Ҫ��ȡ��λ��
 * @param read_datas    ��ȡ�������ݱ��浽�˴�
 * @param read_len      Ҫ��ȡ�ĳ���
 */
void Int_EEPROM_ReadData(uint8_t addr, uint8_t *read_datas, uint16_t read_len)
{
    HAL_I2C_Mem_Read(&hi2c2,DEV_ADDR,addr,I2C_MEMADD_SIZE_8BIT,read_datas,read_len,HAL_MAX_DELAY);
}
