#include "stm32f10x.h"                  // Device header

int main(void)
{
//	// 1 �� GPIOA ʱ��ʹ��  
//	// (uint32_t *)�Ѻ������ֱ�ɵ�ַ *(uint32_t *)������ 
//	*(uint32_t *)(0x40021000 + 0x18) = 0x00000004;
//	
//	// 2 ���� PA0 ������ģʽ
//	*(uint32_t *)(0x40010800 + 0x00) = 0x04444443;
//	
//	// 3 ���� PA0 ����͵�ƽ
//	*(uint32_t *)(0x40010800 + 0x0C) = 0x0000FFFE;
	
	RCC->APB2ENR = 0x00000004; //1 �� GPIOA ʱ��ʹ�� 
	GPIOA->CRH = 0x44444443;   //2 ���� PA8 ������ģʽ
	GPIOA->CRL = 0x44444433;   //2 ���� PA1 PA0 ������ģʽ
	GPIOA->ODR = 0x0000fefc;   //3 ���� PA* ����͵�ƽ
	
	
	
	
	
	
	// ��ֹ�ܷ�
	while (1)
	{
	}
}
