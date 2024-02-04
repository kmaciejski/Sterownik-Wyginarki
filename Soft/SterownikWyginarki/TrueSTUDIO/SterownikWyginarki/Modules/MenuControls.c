/*
 * MenuControls.c
 *
 *  Created on: 12.03.2019
 *      Author: Kajetan
 */

#include "MenuControls.h"
#include "Buttons.h"
#include "Sounds.h"
#include "Memory.h"
#include "i2c-lcd.h"
#include "eeprom24xx.h"


#define BLINK_SHOW 1
#define BLINK_HIDE 0

static volatile uint8_t blinkState = BLINK_SHOW;
static volatile uint8_t blinkEnable = 1;

static volatile uint8_t initFlag = 1;

#define ACTION_TAB 		(100)

#define ACTION_INC 		(1)
#define ACTION_INC5 	(5)
#define ACTION_INC50 	(50)

#define ACTION_DEC 		(-1)
#define ACTION_DEC5 	(-5)
#define ACTION_DEC50 	(-50)

static volatile int8_t actionState = 0;
static volatile uint8_t actionEnable = 1;

static volatile uint8_t pos = 0;

void OnMCBtnClick(ButtonsData_t state);

/*
 * 					Watek kontrolek ISR
 */
void MC_ISR_Process(){
	static uint16_t cnt = 0;

	if(cnt == BLINK_OFF_PERIOD_MS){
		blinkState = 1;
	}
	else if(cnt == BLINK_OFF_PERIOD_MS + BLINK_ON_PERIOD_MS){
		blinkState = 0;
		cnt = 0;
	}

	cnt++;

}


/*
 * 					Inicjalizacja modulu
 */
void MC_Init(void){
	initFlag = 0;

	blinkEnable = 1;
	actionEnable = 1;
	pos = 0;

	Buttons_SetClickCallback(BTN_CB_MENU_CONTROLS, MENU_CONTROLS_BUTTONS, OnMCBtnClick);
	Buttons_EnableAutorepetition(BTN_AR_MENU_CONTROLS_INC, MENU_CONTROLS_BTN_INC);
	Buttons_EnableAutorepetition(BTN_AR_MENU_CONTROLS_DEC, MENU_CONTROLS_BTN_DEC);

}

void MC_DeInit(void){
	initFlag = 1;

	Buttons_RemoveClickCallback(BTN_CB_MENU_CONTROLS);
	Buttons_DisableAutorepetition(BTN_AR_MENU_CONTROLS_INC);
	Buttons_DisableAutorepetition(BTN_AR_MENU_CONTROLS_DEC);

	// Zapisujemy ustawienia do eeprom!
	EEPROM24XX_Save(0, &Settings.bytes, sizeof(Settings.bytes));
}


/*
 * 					Wcisnieto przycisk sterujacy kontrolka
 */
void OnMCBtnClick(ButtonsData_t state){

	static uint16_t ar_cnt = 0;

	switch(state){

		case MENU_BTN_BACK:

			MC_DeInit();

			break;
		case MENU_CONTROLS_BTN_INC:

			actionState = ACTION_INC;

			if(!Buttons_IsAutorepetitionClick(MENU_CONTROLS_BTN_INC)){
				ar_cnt = 0;
				PlayBeep3();
			}
			else{
				actionState = ACTION_INC5;
				ar_cnt++;
				if(ar_cnt > 30)
					actionState = ACTION_INC50;
			}

			break;
		case MENU_CONTROLS_BTN_DEC:

			actionState = ACTION_DEC;

			if(!Buttons_IsAutorepetitionClick(MENU_CONTROLS_BTN_DEC)){
				ar_cnt = 0;
				PlayBeep3();
			}
			else{
				actionState = ACTION_DEC5;
				ar_cnt++;
				if(ar_cnt > 30)
					actionState = ACTION_DEC50;
			}

			break;

		case MENU_CONTROLS_BTN_TAB:

			actionState = ACTION_TAB;
			PlayBeep2();

			break;
	}
}


/*
 * 				Rysuje przycisk TAK/NIE i reaguje na odpowiednie przyciski
 */
void MC_drawSwitch(uint8_t x, uint8_t y, uint8_t *state){

	if(initFlag)
		MC_Init();

	if(actionEnable && (actionState == ACTION_INC || actionState == ACTION_DEC)){
		actionState = 0;

		if (*state)
			*state = 0;
		else
			*state = 1;

	}

	lcd_xy(x,y);

	if(blinkState == BLINK_SHOW || !blinkEnable){
		if(*state == 0)
			lcd_printf("NIE");
		else
			lcd_printf("TAK");
	}
	else
		lcd_printf("   ");

}


/*
 * 				Rysuje przycisk Lewo/Prawo i reaguje na odpowiednie przyciski
 */
void MC_drawSwitchLP(uint8_t x, uint8_t y, uint8_t *state){

	if(initFlag)
		MC_Init();

	if(actionEnable && (actionState == ACTION_INC || actionState == ACTION_DEC)){
		actionState = 0;

		if (*state)
			*state = 0;
		else
			*state = 1;
	}

	lcd_xy(x,y);

	if(blinkState == BLINK_SHOW || !blinkEnable){
		if(*state == DIR_LEFT)
			lcd_printf("Lewo ");
		else
			lcd_printf("Prawo");
	}
	else
		lcd_printf("          ");

}


/*
 * 				Rysuje przycisk Lewo/Prawo i reaguje na odpowiednie przyciski
 */
void MC_drawAngleSelector(uint8_t x, uint8_t y, int16_t *state){

	if(initFlag)
		MC_Init();

	if(actionEnable && actionState && actionState != ACTION_TAB){

		(*state) += actionState;
		actionState = 0;

		if((*state) < MIN_ANGLE)
			(*state) = MIN_ANGLE;

		if((*state) > MAX_ANGLE)
			(*state) = MAX_ANGLE;

	}

	lcd_xy(x,y);

	if(blinkState == BLINK_SHOW || !blinkEnable){
		lcd_printf("%dimp", *state);
	}
	else
		lcd_printf("          ");

}


/*
 * 				Kontrolka wyboru czasu w sekundach
 */
void MC_drawTimeSelector(uint8_t x, uint8_t y, int16_t *state){

	if(initFlag)
		MC_Init();

	if(actionEnable && actionState && actionState != ACTION_TAB){

		(*state) += actionState;
		actionState = 0;

		if((*state) < MIN_TIME_S)
			(*state) = MIN_TIME_S;

		if((*state) > MAX_TIME_S)
			(*state) = MAX_TIME_S;

	}

	lcd_xy(x,y);

	if(blinkState == BLINK_SHOW || !blinkEnable){
		lcd_printf("%ds", *state);
	}
	else
		lcd_printf("          ");

}


/*
 * 				Kontrolka wyboru czasu w sekundach i aktywacji (TAK/NIE)
 */
void MC_drawAutoModeSelector(uint8_t x, uint8_t y, AutoRecord_t *state){

	if(actionState == ACTION_TAB){
		pos ^= 1;
		actionState = 0;
	}

	blinkEnable = 0;
	actionEnable = 0;
	if(pos == 0){
		blinkEnable = 1;
		actionEnable = 1;
	}
	MC_drawSwitch(x, y, &state->enable);
	
	blinkEnable = 0;
	actionEnable = 0;
	if(pos == 1){
		blinkEnable = 1;
		actionEnable = 1;
	}
	MC_drawTimeSelector(x+5, y, &state->time);

}


