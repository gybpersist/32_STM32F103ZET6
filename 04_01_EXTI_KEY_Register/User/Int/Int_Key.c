#include "Int_Key.h"

void Int_Key_Init(void)
{
    // 1 时钟使能 -----------------------------------------
    // 1.1 对 GPIOF 进行时钟使能
    RCC->APB2ENR |= RCC_APB2ENR_IOPFEN;
    // 1.2 对 AFIO 时钟使能
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;

    // 2 设置 GPIO 引脚模式
    // 2.1 将 PF8 设置为上下拉输入模式 MODE=00,CNF=10
    GPIOF->CRH &= ~GPIO_CRH_MODE8;
    GPIOF->CRH |= GPIO_CRH_CNF8_1;
    GPIOF->CRH &= ~GPIO_CRH_CNF8_0;
    // 2.2 设置 PF8 为上拉模式
    GPIOF->ODR |= GPIO_ODR_ODR8;

    // 3 设置 AFIO -----------------------------------------
    // EXTI8 选择 PF8,在 AFIO_EXTICR3 寄存器中进行配置
    AFIO->EXTICR[2] &= ~AFIO_EXTICR3_EXTI8;   //将用于设置EXTI8的4个位全设置为0
    AFIO->EXTICR[2] |= AFIO_EXTICR3_EXTI8_PF; //EXTI8的4个位设置为 0101

    // 4 设置 EXTI -----------------------------------------
    // 4.1 设置下降沿触发 EXTI8
    EXTI->FTSR |= EXTI_FTSR_TR8;
    // 4.2 设置开放 EXTI8 的中断请求
    EXTI->IMR |= EXTI_IMR_MR8;

    // 5 设置 NVIC -----------------------------------------
    // 5.1 设置优先级的4个bit全是抢占优先级,将 SCB->AIRCR 寄存器中的对应的控制位设置成(3)011,表示4位全是抢占优先级 (全局设置)    
    NVIC_SetPriorityGrouping(3);  
    // 5.2 对 EXTI9_5 中断向量使能
    NVIC_EnableIRQ(EXTI9_5_IRQn);
    // 5.3 设置 EXTI9_5 的优先级为 10(随便设置)
    NVIC_SetPriority(EXTI9_5_IRQn,10);
}

/**
 * @brief 定义弱函数
 * 
 * @return __weak 
 */
__weak void Int_Key_SW3Callback(void){

}

/**
 * @brief EXTI9_5 的中断向量服务程序
 * 
 */
void EXTI9_5_IRQHandler(){
    
    // 判断是否是 EXTI8 发送的中断请求 (SW2是否按下) 中断标志位是:EXTI_PR
    if (EXTI->PR & EXTI_PR_PR8)
    {
        // 清除中断标志位 (置1才清除)
        EXTI->PR |= EXTI_PR_PR8;

        // 通过延时消抖
        Com_Delay_ms(10);

        // 延时结束再次判断PF8是否处于低电平
        if ((GPIOF->IDR & GPIO_IDR_IDR8) == 0)
        {
            // while ((GPIOF->IDR & GPIO_IDR_IDR8) == 0)
            // {  //加上是松手才亮,没有是立即亮
            // }
            
            // 调用弱函数
            Int_Key_SW3Callback();
        }
    }
}

