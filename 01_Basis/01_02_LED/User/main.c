#include "stm32f10x.h"                  // Device header

int main(void)
{
//	// 1 �� GPIOA ʱ��ʹ��  
//	// (uint32_t *)�Ѻ������ֱ�ɵ�ַ *(uint32_t *)������ 
//	*(uint32_t *)(0x40021000 + 0x18) = 0x00000004;
	// RCC->APB2ENR |= (1 << 2); 
//	
//	// 2 ���� PA0 ������ģʽ
//	*(uint32_t *)(0x40010800 + 0x00) = 0x04444443;
//	
//	// 3 ���� PA0 ����͵�ƽ
//	*(uint32_t *)(0x40010800 + 0x0C) = 0x0000FFFE;
	

// ����LED3 PA8
	// RCC->APB2ENR = 0x00000004; //1 �� GPIOA ʱ��ʹ�� 
	// RCC->APB2ENR |= (1 << 2); 
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

	// GPIOA->CRH = 0x44444443;   //2 ���� PA8 ������ģʽ
	// GPIOA->CRH &= ~(1 << 3);
	GPIOA->CRH &= ~GPIO_CRH_CNF8_1;

	// GPIOA->CRH &= ~(1 << 2);
	GPIOA->CRH &= ~GPIO_CRH_CNF8_0;

	// GPIOA->CRH &= ~GPIO_CRH_CNF8; //һ����λ

	// GPIOA->CRH |= (1 << 1);
	GPIOA->CRH |= GPIO_CRH_MODE8_1;

	// GPIOA->CRH |= (1 << 0);
	GPIOA->CRH |= GPIO_CRH_MODE8_0;

	// GPIOA->CRH |= GPIO_CRH_MODE8; //һ����λ

	// GPIOA->ODR = 0x0000fefc;   //3 ���� PA* ����͵�ƽ
	// GPIOA->ODR &= ~(1 << 8);
	GPIOA->ODR &= ~ GPIO_ODR_ODR8;
	
	
	
	
	
	
	// ��ֹ�ܷ�
	while (1)
	{
	}
}
