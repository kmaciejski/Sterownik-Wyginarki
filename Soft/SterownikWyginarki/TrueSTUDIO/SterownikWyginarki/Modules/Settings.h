/*
 * Settings.h
 *
 *  Created on: 12.03.2019
 *      Author: Kajetan
 */

#ifndef MODULES_SETTINGS_H_
#define MODULES_SETTINGS_H_

#include "stm32f0xx_hal.h"
#include "Memory.h"

#define MANUAL_MEMORY 8

typedef struct{

	/* Aktywacja */
	uint8_t enable;

	/* Czasy */
	uint16_t time;

}AutoRecord_t;

typedef union
{

	struct{
	/* Tryby automatyczne */
	AutoRecord_t AS;	/* Autostart */
	AutoRecord_t AC;	/* Autocofanie */
	AutoRecord_t AB;	/* Autobazowanie */

	/* Pamiec */
	Memory_t m[MEMORY_RECORDS_COUNT];
	};

	uint8_t bytes[60];

} Settings_t;


extern volatile Settings_t Settings;



#endif /* MODULES_SETTINGS_H_ */
