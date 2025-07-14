#include <stdint.h>
#include "stm32f10x.h"
#include "Com_Util.h"
#include "Dri_USART1.h"
#include "Dri_I2C2.h"
#include "Int_EEPROM.h"

// ������
int main()
{
	// USART1 ��ʼ��
	Dri_USART1_Init();
	// ��ӡ��Ϣ
	printf("STM32 I2C Hardware Example: \n");

	// ��ʼ��I2C
	Dri_I2C2_Init();

	// ����Ҫд�������
	uint8_t write_bytes[] = "ABCDEFGHIGKLMNOPQRSTUVWXYZ";
	// ʹ��I2C��EEPROMд������
	Int_EEPROM_WriteData(0x0E, write_bytes, 26);

	// // ��������������ȡ������
	// uint8_t read_bytes[27];
	// Int_EEPROM_ReadData(0x0E, read_bytes, 26);
	// // ��ӡ��ȡ��������
	// printf("Read Data: %s\n", read_bytes);

	// ��ѭ��
	while (1)
		;
}

