/*
 * Memory
 *
 *  Created on: 07.03.2019
 *      Author: Kajetan
 */

#ifndef MODULES_MEMORY_H_
#define MODULES_MEMORY_H_

#include "stm32f0xx_hal.h"

#define MEMORY_RECORDS_COUNT	8

#define DIR_LEFT 		0
#define DIR_RIGHT 		1

typedef  uint8_t RotateDir_t;
typedef  uint16_t RotateAngle_t;

typedef struct{

	RotateDir_t 	direction;		/* Kierunek obrotu */
	RotateAngle_t	f_angle;		/* K¹t skrêcenia */
	RotateAngle_t	r_angle;		/* K¹t cofniecia */

}Memory_t;


#endif /* MODULES_MEMORY_H_ */
