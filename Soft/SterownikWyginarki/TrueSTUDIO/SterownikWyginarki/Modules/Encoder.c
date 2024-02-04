/*
 * Encoder.c
 *
 *  Created on: 26.02.2019
 *      Author: Kajetan
 */

#include "Encoder.h"
#include "i2c-lcd.h"
#include "Sounds.h"

extern TIM_HandleTypeDef htim3;

static volatile Encoder_t Encoder = {0};


void Motor_StartLeft(void){
	HAL_GPIO_WritePin(MOTOR_LEFT_GPIO_Port, MOTOR_LEFT_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MOTOR_RIGHT_GPIO_Port, MOTOR_RIGHT_Pin, GPIO_PIN_RESET);
}
void Motor_StartRight(void){
	HAL_GPIO_WritePin(MOTOR_LEFT_GPIO_Port, MOTOR_LEFT_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOTOR_RIGHT_GPIO_Port, MOTOR_RIGHT_Pin, GPIO_PIN_SET);
}
void Motor_Stop(void){
	HAL_GPIO_WritePin(MOTOR_LEFT_GPIO_Port, MOTOR_LEFT_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOTOR_RIGHT_GPIO_Port, MOTOR_RIGHT_Pin, GPIO_PIN_RESET);
}

inline void CounterZero(void){
	TIM3->CNT = (ENCODER_MAX * 4);
}
inline void CounterVal(int16_t angle){
	TIM3->CNT = (ENCODER_MAX * 4) + (angle * 4);
}


/*
 * 				Pobiera k¹t obrotu
 */
int16_t Encoder_GetAngle(void){
	return (TIM3->CNT / 4) - ENCODER_MAX;
}


/*
 * 				komendy zewnetrzne
 */
void Encoder_Calibrate(void){
	Motor_StartRight();
	HAL_Delay(50);
	Encoder.startCalibrate = 1;
}


void Encoder_RunRight(int16_t angle){
	if((Encoder_GetAngle() + angle) < ENCODER_ANGLE_MAX){
		Encoder.setpoint = (Encoder_GetAngle() + angle);
		Encoder.runRight = 1;
		Motor_StartRight();
	}
}


void Encoder_RunLeft(int16_t angle){
	if((Encoder_GetAngle() - angle) > ENCODER_ANGLE_MIN){
		Encoder.setpoint = (Encoder_GetAngle() - angle);
		Encoder.runLeft = 1;
		Motor_StartLeft();
	}
}


uint8_t Encoder_IsBusy(void){
	return (Encoder.endCalibrate || Encoder.endRun || Encoder.runLeft || Encoder.runRight || Encoder.startCalibrate);
}

void Encoder_Stop(void){
	Encoder.endCalibrate = 0;
	Encoder.endRun = 0;
	Encoder.runLeft = 0;
	Encoder.runRight = 0;
	Encoder.startCalibrate = 0;

	Motor_Stop();

}
/*
 * 				Inicjalizacja enkodera
 */
void Encoder_Init(void){

	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
	HAL_TIM_Base_Start_IT(&htim3);
	Motor_Stop();
	CounterZero();

}


/*
 * 				Watek enkodera
 */
void Encoder_Process(void){

	if(Encoder.endCalibrate){
		HAL_Delay(200);
		CounterZero();
		PlayBeep2();
		Encoder.endCalibrate = 0;
	}
	if(Encoder.endRun){
		HAL_Delay(200);
		CounterVal(Encoder.setpoint);
		PlayBeep2();
		Encoder.endRun = 0;
	}
}


/*
 * 				Watek ISR
 */
void	Encoder_ISR_Process(void){

	if(Encoder.runLeft){
		if(Encoder_GetAngle() <= Encoder.setpoint){
			Motor_Stop();
			Encoder.runLeft = 0;
			Encoder.endRun = 1;
		}
	}
	if(Encoder.runRight){
		if(Encoder_GetAngle() >= Encoder.setpoint){
			Motor_Stop();
			Encoder.runRight = 0;
			Encoder.endRun = 1;
		}
	}
}


/*
 * 				Impuls linii Z
 */
void	Encoder_Z_Pulse(void){

	if(Encoder.startCalibrate){
		Motor_Stop();
		Encoder.startCalibrate = 0;
		Encoder.endCalibrate = 1;
	}

}

