#include "platform.h"

#define PWM_MOTOR_MIN 100
#define PWM_MOTOR_MED 1250
#define PWM_MOTOR_MAX 2400

#define PWM1  TIM3->CCR1
#define PWM2  TIM3->CCR2
#define PWM3  TIM3->CCR4
#define PWM4  TIM1->CCR2
#define PWM5  TIM1->CCR3

void MotoInit(void){
	GPIO_InitTypeDef GPIO_InitStruct;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
    TIM_OCInitTypeDef TIM_OCInitStruct;
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9,  GPIO_AF_2); // TIM1 CH2
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_2); // TIM1 CH3
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6,  GPIO_AF_1); // TIM3 CH1
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7,  GPIO_AF_1); // TIM3 CH2
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource1,  GPIO_AF_1); // TIM3 CH4
    
    /* TIM1 CH2 PA9  */ /* TIM1 CH3 PA10 */ /* TIM3 CH1 PA6  */ /* TIM3 CH2 PA7  */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6  | GPIO_Pin_7  | GPIO_Pin_9  | GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP ;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    /************************** PWM Output **************************************/
    /* TIM1 Time Base */
    TIM_TimeBaseStruct.TIM_Period = (uint16_t)(2500-1);         // Period = 2.5ms => Freq = 400Hz
    TIM_TimeBaseStruct.TIM_Prescaler = (uint16_t)(48-1);        // fCK_PSC = APB1*2 = 42*2 = 84, fCK_PSC /84 = 1M ( 1us )
    TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;    // Count Up
    TIM_TimeBaseStruct.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStruct);
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStruct);
    
    /* Channel 1, 2, 3 and 4 Configuration in PWM mode */
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCNPolarity_High;
    TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCIdleState_Reset;
    TIM_OCInitStruct.TIM_Pulse = PWM_MOTOR_MIN;
    TIM_OC2Init(TIM1, &TIM_OCInitStruct);
    TIM_OC3Init(TIM1, &TIM_OCInitStruct);
    TIM_OC1Init(TIM3, &TIM_OCInitStruct);
    TIM_OC2Init(TIM3, &TIM_OCInitStruct);
//    TIM_OC4Init(TIM3, &TIM_OCInitStruct);
    
    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
//    TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM1, ENABLE);
    TIM_ARRPreloadConfig(TIM3, ENABLE);
    
    /* Enable */
    TIM_Cmd(TIM1, ENABLE);
    TIM_Cmd(TIM3, ENABLE);
    
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
    TIM_CtrlPWMOutputs(TIM3, ENABLE);
    
    TIM1->CCR2 = PWM_MOTOR_MED;
    TIM1->CCR3 = PWM_MOTOR_MED;
    TIM3->CCR1 = PWM_MOTOR_MED;
    TIM3->CCR2 = PWM_MOTOR_MED;
	
    gpio_mode(5,1,0);
    gpio_write(5,1);
}

void MotoSleepSet(int left,int right) {
	if(left > 0) {
		TIM1->CCR2 = left; 
		TIM1->CCR3 = 0x00;  
	} else {
		TIM1->CCR2 = 0x00; 
		TIM1->CCR3 = 0-left;  
	}
	
	if(right > 0) {
		TIM3->CCR1 = right; 
		TIM3->CCR2 = 0x00;  
	} else {
		TIM3->CCR1 = 0x00; 
		TIM3->CCR2 = 0-right;  
	}
}

