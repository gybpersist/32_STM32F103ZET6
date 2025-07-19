#include "Int_Key.h"

void Int_Key_Init(void)
{
    // 1 时钟使能 
    // 1.1 使能 GPIOF
    RCC->APB2ENR |= RCC_APB2ENR_IOPFEN;
    // 1.2 使能 AFIO 
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;

    // 2 GPIO 配置
    // 2.1 PF8 PF9 PF10 PF11 上下拉输入模式 MODE:00 CNF:10
    GPIOF->CRH &= ~(GPIO_CRH_MODE8 | GPIO_CRH_MODE9 | GPIO_CRH_MODE10 | GPIO_CRH_MODE11);
    GPIOF->CRH |= (GPIO_CRH_CNF8_1 | GPIO_CRH_CNF9_1 | GPIO_CRH_CNF10_1 | GPIO_CRH_CNF11_1);
    GPIOF->CRH &= ~(GPIO_CRH_CNF8_0 | GPIO_CRH_CNF9_0 | GPIO_CRH_CNF10_0 | GPIO_CRH_CNF11_0);
    // 2.2 在 ODR 寄存器中选择 上拉(1)/下拉(0)
    GPIOF->ODR |= (GPIO_ODR_ODR8 | GPIO_ODR_ODR9);      // PF8 PF9 选择上拉 
    GPIOF->ODR &= ~(GPIO_ODR_ODR10 | GPIO_ODR_ODR11);   // PF10 PF11 选择下拉 

    // 3 AFIO 配置
    // 3.1 AFIO 选择 PF8 PF9 PF10 PF11
    AFIO->EXTICR[2] &= ~(AFIO_EXTICR3_EXTI8 | AFIO_EXTICR3_EXTI9 | AFIO_EXTICR3_EXTI10 | AFIO_EXTICR3_EXTI11);     // 先清0
    AFIO->EXTICR[2] |= (AFIO_EXTICR3_EXTI8_PF | AFIO_EXTICR3_EXTI9_PF | AFIO_EXTICR3_EXTI10_PF | AFIO_EXTICR3_EXTI11_PF);   // 再配置PF8 PF9 PF10 PF11
    
    // 4 EXTI 配置
    // 4.1 选择 上升/下降沿 触发
    EXTI->FTSR |= (EXTI_FTSR_TR8 | EXTI_FTSR_TR9);      // PF8 PF9 下降沿触发
    EXTI->RTSR |= (EXTI_RTSR_TR10 | EXTI_RTSR_TR11);      // PF10 PF11 上降沿触发
    // 4.2 开启 EXTI 中断
    EXTI->IMR |= (EXTI_IMR_MR8 | EXTI_IMR_MR9 | EXTI_IMR_MR10 | EXTI_IMR_MR11);

    // 5 NVIC 配置
    NVIC_SetPriorityGrouping(3);            // 全部是抢占优先级
    NVIC_SetPriority(EXTI9_5_IRQn,2);       
    NVIC_SetPriority(EXTI15_10_IRQn,2);
    NVIC_EnableIRQ(EXTI9_5_IRQn);           //使能开启
    NVIC_EnableIRQ(EXTI15_10_IRQn);
}

/**************** 按键回调函数 ****************************/
// 定义 SW3 按下弱函数 ,触发中断后将会被调用
__weak void Int_Key_IsSW3Callback(void){
}
// 定义 SW4 按下弱函数 ,触发中断后将会被调用
__weak void Int_Key_IsSW4Callback(void){
}
// 定义 SW5 按下弱函数 ,触发中断后将会被调用
__weak void Int_Key_IsSW5Callback(void){
}
// 定义 SW6 按下弱函数 ,触发中断后将会被调用
__weak void Int_Key_IsSW6Callback(void){
}
/**************** 按键回调函数 ****************************/

/**************** 中断服务程序 ****************************/
// SW3(PF8) SW4(PF9) 的中断服务程序
void EXTI9_5_IRQHandler(void)
{
    // 判断是否是 EXTI8(SW3) 触发的
    if (EXTI->PR & EXTI_PR_PR8)
    {
        // 清除 挂起标志位 (写入’1’可以清除它)
        EXTI->PR |= EXTI_PR_PR8;

        // 消抖
        Com_Delay_ms(10);
        if ((GPIOF->IDR & GPIO_IDR_IDR8) == 0)
        {
            Int_Key_IsSW3Callback();
        }
    }

     // 判断是否是 EXTI9(SW4) 触发的
    if (EXTI->PR & EXTI_PR_PR9)
    {
        // 清除 挂起标志位 (写入’1’可以清除它)
        EXTI->PR |= EXTI_PR_PR9;

        // 消抖
        Com_Delay_ms(10);
        if ((GPIOF->IDR & GPIO_IDR_IDR9) == 0)
        {
            Int_Key_IsSW4Callback();
        }
    }
}

// SW5(PF10) SW6(PF11) 的中断服务程序
void EXTI15_10_IRQHandler(void)
{
    // 判断是否是 EXTI110(SW5) 触发的
    if (EXTI->PR & EXTI_PR_PR10)
    {
        // 清除 挂起标志位 (写入’1’可以清除它)
        EXTI->PR |= EXTI_PR_PR10;

        // 消抖
        Com_Delay_ms(10);
        if (GPIOF->IDR & GPIO_IDR_IDR10)
        {
            Int_Key_IsSW5Callback();
        }
    }

     // 判断是否是 EXTI11(SW6) 触发的
    if (EXTI->PR & EXTI_PR_PR11)
    {
        // 清除 挂起标志位 (写入’1’可以清除它)
        EXTI->PR |= EXTI_PR_PR11;

        // 消抖
        Com_Delay_ms(10);
        if (GPIOF->IDR & GPIO_IDR_IDR11)
        {
            Int_Key_IsSW6Callback();
        }
    }
}
/**************** 中断服务程序 ****************************/
