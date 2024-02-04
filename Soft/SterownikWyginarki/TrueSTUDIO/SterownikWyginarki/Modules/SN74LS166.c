/*
 * SN74LS166.c
 *
 *  Created on: 26.02.2019
 *      Author: Kajetan
 */

#include "SN74LS166.h"

static SN74LS166_t SN74LS166 = {0};


/*
 * 				Funkcja przeprowadza inicjalizacje modulu (wypelnia strukture)
 */
inline void SN74LS166_Init(void)
{
	SN74LS166.GPIO_LOAD = LS166_LOAD_GPIO_Port;
	SN74LS166.GPIO_SCK = LS166_SCK_GPIO_Port;
	SN74LS166.GPIO_MISO = LS166_MISO_GPIO_Port;
	SN74LS166.GPIO_LOAD_Pin = LS166_LOAD_Pin;
	SN74LS166.GPIO_SCK_Pin = LS166_SCK_Pin;
	SN74LS166.GPIO_MISO_Pin = LS166_MISO_Pin;
}

void _delay(){
	uint8_t i = 200;
	while(i--);
}

/*
 * 				Funkcja czyta dane z wszystkich chipów i zapisuje w tablicy
 */
void SN74LS166_Read(uint8_t *pData, uint8_t nOfChips){

	uint8_t i, temp;

	HAL_GPIO_WritePin(SN74LS166.GPIO_LOAD, SN74LS166.GPIO_LOAD_Pin, GPIO_PIN_RESET);	// LOAD: LO

	HAL_GPIO_WritePin(SN74LS166.GPIO_SCK, SN74LS166.GPIO_SCK_Pin, GPIO_PIN_RESET);		// SCK: Low -> High
	HAL_GPIO_WritePin(SN74LS166.GPIO_SCK, SN74LS166.GPIO_SCK_Pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(SN74LS166.GPIO_LOAD, SN74LS166.GPIO_LOAD_Pin, GPIO_PIN_SET);		// LOAD: HIGH


	while(nOfChips--){

		temp = 0;

		for(i=0; i<8; i++){

			temp <<= 1;

			if(HAL_GPIO_ReadPin(SN74LS166.GPIO_MISO, SN74LS166.GPIO_MISO_Pin) == GPIO_PIN_SET)	// MISO: read
				temp |= 0x01;		/* set bit */
			else
				temp &= ~0x01;	/* clear bit */

			HAL_GPIO_WritePin(SN74LS166.GPIO_SCK, SN74LS166.GPIO_SCK_Pin, GPIO_PIN_RESET);		// SCK: Low -> High
			HAL_GPIO_WritePin(SN74LS166.GPIO_SCK, SN74LS166.GPIO_SCK_Pin, GPIO_PIN_SET);

		}

		*pData = temp;
		pData++;

	}

}
