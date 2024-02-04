/*
 * Encoder.h
 *
 *  Created on: 26.02.2019
 *      Author: Kajetan
 */

#ifndef MODULES_ENCODER_H_
#define MODULES_ENCODER_H_

#include "stm32f0xx_hal.h"


#define ENCODER_MAX (8000)
#define ENCODER_MIN (-8000)

#define ENCODER_ANGLE_MIN (-7200)
#define ENCODER_ANGLE_MAX (7200)

/*
 * 				Glowna struktura modulu enkodera
 */
typedef struct{

	uint8_t  startCalibrate;
	uint8_t  endCalibrate;

	int16_t  setpoint;

	uint8_t  runLeft;
	uint8_t  runRight;

	uint8_t  endRun;

}Encoder_t;


uint8_t Encoder_IsBusy(void);
void Encoder_Init(void);
void Encoder_Process(void);
void Encoder_ISR_Process(void);
void Encoder_Z_Pulse(void);

int16_t Encoder_GetAngle(void);

void Encoder_Stop(void);				// Przerywa prace maszyny (silnika)
void Encoder_Calibrate(void);			// Bazowanie
void Encoder_RunLeft(int16_t angle);	// Kreci w lewo o zadany k¹t
void Encoder_RunRight(int16_t angle);	// Kreci w prawo o zadany k¹t

#endif /* MODULES_ENCODER_H_ */
