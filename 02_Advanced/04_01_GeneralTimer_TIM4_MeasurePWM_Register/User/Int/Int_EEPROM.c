#include "Int_EEPROM.h"

// ��̬��������һ��ҳ��д�����ݣ�ָ��λ��ָ�����ȣ�
// @param addr Ҫд��ĵ�ַ
// @param write_datas Ҫд�������
// @param write_len Ҫд��ĳ���
static void Int_EEPROM_WriteIntoPage(uint8_t addr, uint8_t *write_datas, uint16_t write_len)
{
    // ������ʼ�ź�
    Dri_I2C2_Start();

    // ���ʹ��豸��ַ+д��ʶ, ������Ӧ���ź�
    Dri_I2C2_TransmitSlaveAddr(DEV_ADDR_W);
    Dri_I2C2_ReceiveACK();

    // ����EEPROM�ڲ���ַ��������Ӧ���ź�
    Dri_I2C2_TransmitByte(addr);
    Dri_I2C2_ReceiveACK();

    // ѭ����EEPROM���Ͷ���ֽڣ�ÿ����һ���ֽھͽ���һ��Ӧ���ź�
    for (uint16_t i = 0; i < write_len; i++)
    {
        Dri_I2C2_TransmitByte(write_datas[i]);
        Dri_I2C2_ReceiveACK();
    }

    // ����ֹͣ�ź�
    Dri_I2C2_Stop();

    // ����д������ʱ5ms
    Com_Delay_ms(5);
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

    // -----------------------αд-------------------------
    // ������ʼ�ź�
    Dri_I2C2_Start();

    // ���ʹ��豸��ַ+д��ʶ, ������Ӧ���ź�
    Dri_I2C2_TransmitSlaveAddr(DEV_ADDR_W);
    Dri_I2C2_ReceiveACK();
    
    // ����EEPROM�ڲ���ַ��������Ӧ���ź�
    Dri_I2C2_TransmitByte(addr);
    Dri_I2C2_ReceiveACK();

    // -----------------------��ʽ��-------------------------
    // ������ʼ�ź�
    Dri_I2C2_Start();
    
    // ���ʹ��豸��ַ+����ʶ, ������Ӧ���ź�
    Dri_I2C2_TransmitSlaveAddr(DEV_ADDR_R);
    Dri_I2C2_ReceiveACK();

    // ѭ����ȡ����ֽڣ�ÿ��ȡһ���ֽھͷ���һ��Ӧ���ź�
    for (uint16_t i = 0; i < read_len; i++)
    {
        // ���һ���ֽڷ��ͷ�Ӧ���ź�
        if (i == read_len - 1)
        {
            Dri_I2C2_TransmitACK(1);

            // ����ֹͣ�ź�,��Ҫ�������һ���ֽ�ǰ��ǰ����
            Dri_I2C2_Stop();
        }
        // �����ֽڷ���Ӧ���ź�
        else
        {
            Dri_I2C2_TransmitACK(0);
        }
        read_datas[i] = Dri_I2C2_ReceiveByte();
    }
}
