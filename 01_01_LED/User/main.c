#include "stm32f10x.h"                  // Device header

int main(void)
{
//	// 1 对 GPIOA 时钟使能  
//	// (uint32_t *)把后面数字变成地址 *(uint32_t *)解引用 
//	*(uint32_t *)(0x40021000 + 0x18) = 0x00000004;
//	
//	// 2 设置 PA0 的引脚模式
//	*(uint32_t *)(0x40010800 + 0x00) = 0x04444443;
//	
//	// 3 设置 PA0 输出低电平
//	*(uint32_t *)(0x40010800 + 0x0C) = 0x0000FFFE;
	
	RCC->APB2ENR = 0x00000004; //1 对 GPIOA 时钟使能 
	GPIOA->CRH = 0x44444443;   //2 设置 PA8 的引脚模式
	GPIOA->CRL = 0x44444433;   //2 设置 PA1 PA0 的引脚模式
	GPIOA->ODR = 0x0000fefc;   //3 设置 PA* 输出低电平
	
	
	
	
	
	
	// 防止跑飞
	while (1)
	{
	}
}
