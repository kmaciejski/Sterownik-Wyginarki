/*
 * Buttons.c
 *
 *  Created on: 26.02.2019
 *      Author: Kajetan
 */

#include "Buttons.h"
#include "i2c-lcd.h"

static volatile Buttons_t Buttons = {0};
extern TIM_HandleTypeDef htim17;


inline ButtonsData_t Buttons_GetState(void){
	return Buttons.currState;
}

/*
 * 				Inicjalizacja przyciskow
 */
void Buttons_Init(void){

  SN74LS166_Init();

	HAL_TIM_Base_Start_IT(&htim17);

}


/*
 * 				Wylowywanie callback'ow w petli glownej
 */
void Buttons_Process(void){

	uint8_t i;

    /* Przepisujemy stan przyciskow */
	__disable_irq();
	ButtonsData_t clicksDetected = Buttons.clicksDetected;
	ButtonsData_t currState = Buttons.currState;
	Buttons.clicksDetected = 0;
	Buttons.arOccurred = 0;
    __enable_irq();


    /* Sprawdzamy autorepetycje */
	for(i = 0; i < MAX_AUTOREPETITIONS; i++){

		if(Buttons.ar_keys[i]){	// Czy dla danego rekordu autorepetycja jest wlaczona

			if(! (currState & Buttons.ar_keys[i])){	// i-ty przycisk wcisniety
				Buttons.ar_cnts[i] ++;	// Zwiekszamy licznik

				if(Buttons.ar_cnts[i] >= (AUTOREPETITION_DELAY_CYCLES + AUTOREPETITION_PERIOD_CYCLES)){	// Sprawdzamy czy wykryto autorepetycje
					Buttons.ar_cnts[i] = AUTOREPETITION_DELAY_CYCLES;
					clicksDetected |= Buttons.ar_keys[i];					// Jesli tak to rejestrujemy odpowiednie klikniecie
					Buttons.arOccurred |= Buttons.ar_keys[i];
				}

			}else{						// i-ty przycisk puszczony
				Buttons.ar_cnts[i] = 0;	// Zerujemy licznik
			}

		}

	}


    /* Wywolujemy callbacki do wykrytych klikniec */
	for(i = 0; i < MAX_CALLBACKS; i++){

		if(Buttons.cbs[i]){

			ButtonsData_t clickDetected = clicksDetected & Buttons.cbs_masks[i];

			if(clickDetected){
				Buttons.cbs[i](clickDetected);
			}

		}

	}

}


/*
 * 				Przerwanie Timera 1ms (odswierzanie stanu przyciskow)
 */
void Buttons_ISR_Process(void){

	static uint16_t cnt = 0;

	if(!cnt--){							/* Odswiezamy z okreslona czestotliwoscia */
		cnt = REFRESH_PERIOD_MS;

		ButtonsData_t diffState;	/* roznica pomiedzy stanem poprzednim a aktualnym */

		SN74LS166_Read((uint8_t*)&Buttons.currState, SHIFT_REG_DATA_SIZE);

		//lcd_xy(9,1);
		//lcd_printf("%04X", Buttons.currState);

		diffState = Buttons.currState ^ Buttons.prevState;	/* obliczamy roznice */

		#if (BUTTONS_ACTIVE_STATE == 1)
			Buttons.clicksDetected |= diffState & Buttons.currState;
		#else
			Buttons.clicksDetected |= diffState & ~Buttons.currState;
		#endif

		Buttons.prevState = Buttons.currState;

	}
}


/*
 * 				Ustawienie callback'a
 */
void Buttons_SetClickCallback(uint8_t id, ButtonsData_t mask, ButtonsCallback_t cb){
	//if(id<MAX_CALLBACKS){
		Buttons.cbs[id] = cb;
		Buttons.cbs_masks[id] = mask;
	//}
}


/*
 * 				Usuniecie callback'a
 */
void Buttons_RemoveClickCallback(uint8_t id){
	//if(id<MAX_CALLBACKS){
		Buttons.cbs[id] = 0;
	//}
}


/*
 * 				Ustawienie autorepetycji
 */
void Buttons_EnableAutorepetition(uint8_t id, ButtonsData_t key){
	//if(id<MAX_AUTOREPETITIONS){
		Buttons.ar_keys[id] = key;
	//}
}


/*
 * 				Usuniecie autorepetycji
 */
void Buttons_DisableAutorepetition(uint8_t id){
	//if(id<MAX_AUTOREPETITIONS){
		Buttons.ar_keys[id] = 0;
	//}
}


/*
 * 				Sprawdza czy wywolanie callbacka dla podanego przycisku
 * 				zostalo spowodowane autorepetycj¹
 */
uint8_t Buttons_IsAutorepetitionClick(ButtonsData_t key){
	return (Buttons.arOccurred & key) != 0;
}
