/*
 * SN74LS166.h
 *
 *  Created on: 26.02.2019
 *      Author: Kajetan
 */

#ifndef MODULES_SN74LS166_H_
#define MODULES_SN74LS166_H_

#include "stm32f0xx_hal.h"
#include "gpio.h"

/*
 * 				Glowna struktura modulu - przechowuje dane o konfiguracji SPI i GPIO
 */
typedef struct{

	GPIO_TypeDef* GPIO_SCK;			/* Pin SCK */
	uint16_t GPIO_SCK_Pin;

	GPIO_TypeDef* GPIO_LOAD;		/* Pin LOAD */
	uint16_t GPIO_LOAD_Pin;

	GPIO_TypeDef* GPIO_MISO;		/* Pin MISO */
	uint16_t GPIO_MISO_Pin;

}SN74LS166_t;


/*
 * 								Procedury
 */
void SN74LS166_Init(void);

void SN74LS166_Read(uint8_t *pData, uint8_t nOfChips);


#endif /* MODULES_SN74LS166_H_ */
