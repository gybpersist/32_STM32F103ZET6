#include "stm32f10x.h"                  // Device header

int main(void)
{
//	// 1 对 GPIOA 时钟使能  
//	// (uint32_t *)把后面数字变成地址 *(uint32_t *)解引用 
//	*(uint32_t *)(0x40021000 + 0x18) = 0x00000004;
	// RCC->APB2ENR |= (1 << 2); 
//	
//	// 2 设置 PA0 的引脚模式
//	*(uint32_t *)(0x40010800 + 0x00) = 0x04444443;
//	
//	// 3 设置 PA0 输出低电平
//	*(uint32_t *)(0x40010800 + 0x0C) = 0x0000FFFE;
	

// 点亮LED3 PA8
	// RCC->APB2ENR = 0x00000004; //1 对 GPIOA 时钟使能 
	// RCC->APB2ENR |= (1 << 2); 
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

	// GPIOA->CRH = 0x44444443;   //2 设置 PA8 的引脚模式
	// GPIOA->CRH &= ~(1 << 3);
	GPIOA->CRH &= ~GPIO_CRH_CNF8_1;

	// GPIOA->CRH &= ~(1 << 2);
	GPIOA->CRH &= ~GPIO_CRH_CNF8_0;

	// GPIOA->CRH &= ~GPIO_CRH_CNF8; //一步到位

	// GPIOA->CRH |= (1 << 1);
	GPIOA->CRH |= GPIO_CRH_MODE8_1;

	// GPIOA->CRH |= (1 << 0);
	GPIOA->CRH |= GPIO_CRH_MODE8_0;

	// GPIOA->CRH |= GPIO_CRH_MODE8; //一步到位

	// GPIOA->ODR = 0x0000fefc;   //3 设置 PA* 输出低电平
	// GPIOA->ODR &= ~(1 << 8);
	GPIOA->ODR &= ~ GPIO_ODR_ODR8;
	
	
	
	
	
	
	// 防止跑飞
	while (1)
	{
	}
}
