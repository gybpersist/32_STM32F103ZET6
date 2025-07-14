#include <stdint.h>
#include "stm32f10x.h"
#include "Com_Util.h"
#include "Dri_USART1.h"
#include "Dri_I2C.h"
#include "Int_EEPROM.h"

// ������
int main()
{
	// USART1 ��ʼ��
	Dri_USART1_Init();
	// ��ӡ��Ϣ
	printf("STM32 I2C Software Example: \n");

	// ��ʼ��I2C
	Dri_I2C_Init();
	
	// // ����һ��I2Cͨ��
	// // ������ʼ�ź�
	// Dri_I2C_Start();
	// // ���ʹ��豸��ַ+��д��ʶ   1010000 + 0, ����Ӧ���ź�
	// Dri_I2C_TransmitByte(0xA0);
	// Dri_I2C_ReceiveACK();
	// // ����һ���ֽ�,������Ӧ���ź�
	// Dri_I2C_TransmitByte(0xBF);
	// Dri_I2C_ReceiveACK();
	// // ����ֹͣ�ź�
	// Dri_I2C_Stop();

	// ����Ҫд�������
	// uint8_t write_bytes[] = "ABCDEFGHIGKLMNOPQRSTUVWXYZ";
	// ʹ��I2C��EEPROMд������
	// Int_EEPROM_WriteData(0x0E, write_bytes, 26);

	// ��������������ȡ������
	uint8_t read_bytes[27];
	Int_EEPROM_ReadData(0x0E, read_bytes, 26);
	// ��ӡ��ȡ��������
	printf("Read Data: %s\n", read_bytes);

	// ��ѭ��
	while (1)
		;
}

