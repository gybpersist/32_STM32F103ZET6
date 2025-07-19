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
    GPIOF->ODR |= GPIO_ODR_ODR8;    //设置 PF8 为上拉模式
    // 2.2 将 PF9 设置为上下拉输入模式 MODE=00,CNF=10
    GPIOF->CRH &= ~GPIO_CRH_MODE9;
    GPIOF->CRH |= GPIO_CRH_CNF9_1;
    GPIOF->CRH &= ~GPIO_CRH_CNF9_0;
    GPIOF->ODR |= GPIO_ODR_ODR9;    //设置 PF9 为上拉模式
    // 2.3 将 PF10 设置为上下拉输入模式 MODE=00,CNF=10
    GPIOF->CRH &= ~GPIO_CRH_MODE10;
    GPIOF->CRH |= GPIO_CRH_CNF10_1;
    GPIOF->CRH &= ~GPIO_CRH_CNF10_0;
    GPIOF->ODR &= ~GPIO_ODR_ODR10;    //设置 PF10 为下拉模式
    // 2.4 将 PF11 设置为上下拉输入模式 MODE=00,CNF=10
    GPIOF->CRH &= ~GPIO_CRH_MODE11;
    GPIOF->CRH |= GPIO_CRH_CNF11_1;
    GPIOF->CRH &= ~GPIO_CRH_CNF11_0;
    GPIOF->ODR &= ~GPIO_ODR_ODR11;    //设置 PF11 为下拉模式


    // 3 设置 AFIO -----------------------------------------
    // 3.1 EXTI8 选择 PF8,在 AFIO_EXTICR3 寄存器中进行配置
    AFIO->EXTICR[2] &= ~AFIO_EXTICR3_EXTI8;   //将用于设置EXTI8的4个位全设置为0
    AFIO->EXTICR[2] |= AFIO_EXTICR3_EXTI8_PF; //EXTI8的4个位设置为 0101 [0101：PF[x]引脚]
    // 3.2 EXTI9 选择 PF9,在 AFIO_EXTICR3 寄存器中进行配置
    AFIO->EXTICR[2] &= ~AFIO_EXTICR3_EXTI9; 
    AFIO->EXTICR[2] |= AFIO_EXTICR3_EXTI9_PF; 
    // 3.3 EXTI10 选择 PF10,在 AFIO_EXTICR3 寄存器中进行配置
    AFIO->EXTICR[2] &= ~AFIO_EXTICR3_EXTI10; 
    AFIO->EXTICR[2] |= AFIO_EXTICR3_EXTI10_PF;
    // 3.4 EXTI11 选择 PF11,在 AFIO_EXTICR3 寄存器中进行配置
    AFIO->EXTICR[2] &= ~AFIO_EXTICR3_EXTI11; 
    AFIO->EXTICR[2] |= AFIO_EXTICR3_EXTI11_PF;

    // 4 设置 EXTI -----------------------------------------
    // 4.1 设置下降沿触发 EXTI8 EXTI9
    EXTI->FTSR |= (EXTI_FTSR_TR8 | EXTI_FTSR_TR9);
    // 4.2 设置上升沿触发 EXTI10 EXTI11
    EXTI->RTSR |= (EXTI_RTSR_TR10 | EXTI_RTSR_TR11);
    // 4.3 设置开放 EXTI8 EXTI9 EXTI10 EXTI11 的中断请求
    EXTI->IMR |= (EXTI_IMR_MR8 | EXTI_IMR_MR9 | EXTI_IMR_MR10 | EXTI_IMR_MR11);
 

    // 5 设置 NVIC -----------------------------------------
    // 5.1 设置优先级的4个bit全是抢占优先级,将 SCB->AIRCR 寄存器中的对应的控制位设置成(3)011,表示4位全是抢占优先级 (全局设置)    
    NVIC_SetPriorityGrouping(3);  
    // 5.2 对 EXTI9_5 EXTI15_10 中断向量使能
    NVIC_EnableIRQ(EXTI9_5_IRQn);
    NVIC_EnableIRQ(EXTI15_10_IRQn);
    // 5.3 设置 EXTI9_5 EXTI15_10 的优先级为 10 11(随便设置)
    NVIC_SetPriority(EXTI9_5_IRQn,2);
    NVIC_SetPriority(EXTI15_10_IRQn,2);
}

/**
 * @brief 定义按下SW3的弱函数
 * 
 * @return __weak 
 */
__weak void Int_Key_SW3Callback(void){

}

/**
 * @brief 定义按下SW4的弱函数
 * 
 * @return __weak 
 */
__weak void Int_Key_SW4Callback(void){

}

/**
 * @brief 定义按下SW5的弱函数
 * 
 * @return __weak 
 */
__weak void Int_Key_SW5Callback(void){

}

/**
 * @brief 定义按下SW6的弱函数
 * 
 * @return __weak 
 */
__weak void Int_Key_SW6Callback(void){

}

/**
 * @brief EXTI9_5 的中断向量服务程序
 * 
 */
void EXTI9_5_IRQHandler(){
    
    // 判断是否是 EXTI8 发送的中断请求 (SW3是否按下) 中断标志位是:EXTI_PR
    if (EXTI->PR & EXTI_PR_PR8)
    {
        // 清除中断标志位 (置1才清除)
        EXTI->PR |= EXTI_PR_PR8;

        // 通过延时消抖
        Com_Delay_ms(10);

        // 延时结束再次判断PF8是否处于低电平
        if ((GPIOF->IDR & GPIO_IDR_IDR8) == 0)
        {
            // 调用弱函数
            Int_Key_SW3Callback();
        }
    }
    // 判断是否是 EXTI9 发送的中断请求 (SW4是否按下) 中断标志位是:EXTI_PR
    if (EXTI->PR & EXTI_PR_PR9)
    {
        // 清除中断标志位 (置1才清除)
        EXTI->PR |= EXTI_PR_PR9;

        // 通过延时消抖
        Com_Delay_ms(10);

        // 延时结束再次判断PF9是否处于低电平
        if ((GPIOF->IDR & GPIO_IDR_IDR9) == 0)
        {
            // 调用弱函数
            Int_Key_SW4Callback();
        }
        
    }
    
}

/**
 * @brief EXTI15_10 的中断向量服务程序
 * 
 */
void EXTI15_10_IRQHandler(){

    // 判断是否是 EXTI10 发送的中断请求 (SW2是否按下) 中断标志位是:EXTI_PR
    if (EXTI->PR & EXTI_PR_PR10)
    {
        // 清除中断标志位 (置1才清除)
        EXTI->PR |= EXTI_PR_PR10;

        // 通过延时消抖
        Com_Delay_ms(10);

        // 延时结束再次判断PF10是否处于高电平
        if (GPIOF->IDR & GPIO_IDR_IDR10)
        {
             // 调用弱函数
            Int_Key_SW5Callback();
        }     
    }
    // 判断是否是 EXTI11 发送的中断请求 (SW2是否按下) 中断标志位是:EXTI_PR
    if (EXTI->PR & EXTI_PR_PR11)
    {
        // 清除中断标志位 (置1才清除)
        EXTI->PR |= EXTI_PR_PR11;

        // 通过延时消抖
        Com_Delay_ms(10);

        // 延时结束再次判断PF11是否处于高电平
        if (GPIOF->IDR & GPIO_IDR_IDR11)
        {
            // 调用弱函数
            Int_Key_SW6Callback();
        }    
    }
}
