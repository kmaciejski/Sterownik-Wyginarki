/*
 * Sounds.c
 *
 *  Created on: 27.02.2019
 *      Author: Kajetan
 */

#include "Sounds.h"
#include "stm32f0xx_hal.h"

extern TIM_HandleTypeDef htim14;
static TIM_OC_InitTypeDef sConfigOC;

static uint16_t soundDuration = 0;

/*
 * 				Inicjalizacja
 */
void Sounds_Init(void){
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 500;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
}


/*
 * 				Wylacza buzzer
 */
static void Sounds_Buzzer_Off(void){
	HAL_TIM_PWM_Stop_IT(&htim14, TIM_CHANNEL_1);
}


/*
 * 				Wlacza buzzer
 */
static void Sounds_Buzzer_On(uint16_t period_us){
	htim14.Init.Period = period_us;			// period
	sConfigOC.Pulse = period_us >> 1;		// period / 2

	HAL_TIM_Base_Init(&htim14);
	HAL_TIM_PWM_ConfigChannel(&htim14, &sConfigOC, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start_IT(&htim14, TIM_CHANNEL_1);
}


/*
 * 				Przerwanie timera 1ms
 */
void Sounds_ISR_Process(void){

	if(soundDuration){
		soundDuration--;
		if(!soundDuration){
			Sounds_Buzzer_Off();
		}
	}

}


/*
 * 				Dzwiek 1
 */
void PlayBeep1(void){
	Sounds_Buzzer_On(1000);
	soundDuration = 40;

}


/*
 * 				Dzwiek 2
 */
void PlayBeep2(void){
	Sounds_Buzzer_On(500);
	soundDuration = 40;
}


/*
 * 				Dzwiek 3
 */
void PlayBeep3(void){
	Sounds_Buzzer_On(300);
	soundDuration = 35;
}
