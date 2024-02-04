/*
 * RunningMode.c
 *
 *  Created on: 10.04.2019
 *      Author: Kajetan
 */

#include "Menu.h"
#include "Settings.h"
#include "MenuControls.h"
#include "i2c-lcd.h"
#include "Encoder.h"
#include "Sounds.h"

#define ISR_PERIOD_MS 			100

#define BEEP_CYCLES 			20

#define MACHINE_BTN_START		BTN_R
#define MACHINE_BTN_BAZUJ		BTN_U
#define MACHINE_BTN_COFNIJ		BTN_D
#define MACHINE_BUTTONS 		MACHINE_BTN_START | MENU_BTN_BACK | MACHINE_BTN_BAZUJ | MACHINE_BTN_COFNIJ


static volatile uint8_t initFlag = 1;
static volatile uint8_t blinkFlag = 1;

static volatile uint8_t currMem = MANUAL_MEMORY;

static volatile uint16_t currAngle;

#define ACTION_NONE 			0

#define ACTION_BAZUJ 			1	/* Trwa jakis cykl */
#define ACTION_COFAJ 			2
#define ACTION_WYGINAJ 			3

#define ACTION_WAIT_BAZUJ 		4	/* Trwa jakies opoznienie */
#define ACTION_WAIT_COFAJ 		5
#define ACTION_WAIT_WYGINAJ 	6

#define ACTION_END_WAIT_BAZUJ 		7	/* Skonczylo sie jakies opoznienie */
#define ACTION_END_WAIT_COFAJ 		8
#define ACTION_END_WAIT_WYGINAJ 	9

static volatile uint8_t currAction = 0;
static volatile uint16_t currDelay = 0;	/* Aktualnie ustawione opoznienie w cyklach ISR_PERIOD_MS*/

void OnMachineBtnClick(ButtonsData_t state);
void OnMemoryBtnClick(ButtonsData_t state);

void Machine_DeInit(void){
	initFlag = 1;
	currAction = ACTION_NONE;
	currDelay = 0;
	Buttons_RemoveClickCallback(BTN_CB_MACHINE);
	Buttons_RemoveClickCallback(BTN_CB_MEMORY);
}

void Machine_Init(void){
	initFlag = 0;
	currMem = 0;
	currAction = ACTION_NONE;
	currDelay = 0;

	Buttons_SetClickCallback(BTN_CB_MACHINE, MACHINE_BUTTONS, OnMachineBtnClick);
	Buttons_SetClickCallback(BTN_CB_MEMORY, BTN_MEMORY_ALL, OnMemoryBtnClick);
	lcd_clear();
}

/*
 * 					Cykle pracy
 */
void Machine_Bazuj(){
	currAction = ACTION_BAZUJ;

	Encoder_Calibrate();
}
void Machine_Cofaj(){
	currAction = ACTION_COFAJ;

	if(Settings.m[currMem].direction == DIR_LEFT)
		Encoder_RunRight(Settings.m[currMem].r_angle);
	else
		Encoder_RunLeft(Settings.m[currMem].r_angle);
}
void Machine_Wyginaj(){
	currAction = ACTION_WYGINAJ;

	if(Settings.m[currMem].direction == DIR_RIGHT)
		Encoder_RunRight(Settings.m[currMem].f_angle);
	else
		Encoder_RunLeft(Settings.m[currMem].f_angle);
}
/*
 *
 * 					Przyciski maszyny
 *
 */
void OnMachineBtnClick(ButtonsData_t state){

	switch(state){

		case MENU_BTN_BACK:
			Encoder_Stop();
			Machine_DeInit();
			break;

		case MACHINE_BTN_START:
			Machine_Wyginaj();
			PlayBeep2();
			break;

		case MACHINE_BTN_BAZUJ:
			Machine_Bazuj();
			PlayBeep2();
			break;

		case MACHINE_BTN_COFNIJ:
			Machine_Cofaj();
			PlayBeep2();
			break;
	}
}


/*
 *
 * 					Przyciski pamieci
 *
 */
void OnMemoryBtnClick(ButtonsData_t state){

	PlayBeep3();

	switch(state){

		case BTN_M1:
			currMem = 0;
			break;
		case BTN_M2:
			currMem = 1;
			break;
		case BTN_M3:
			currMem = 2;
			break;
		case BTN_M4:
			currMem = 3;
			break;
		case BTN_M5:
			currMem = 4;
			break;
		case BTN_M6:
			currMem = 5;
			break;
		case BTN_M7:
			currMem = 6;
			break;
		case BTN_M8:
			currMem = 7;
			break;

	}
}


/*
 * 					Zegar odliczajacy czas pomiedzy cyklami i migajacy literkami A / B / C
 */
void RunningMode_ISR_Process(){
	static uint16_t cnt = ISR_PERIOD_MS;

	if(!(cnt--)){
		cnt = ISR_PERIOD_MS;

		blinkFlag ^= 1;

		if(currDelay){
			currDelay--;

			if(currDelay < BEEP_CYCLES)
				PlayBeep3();


			if(!currDelay){

				if(currAction == ACTION_WAIT_BAZUJ)
					currAction = ACTION_END_WAIT_BAZUJ;

				else if(currAction == ACTION_WAIT_COFAJ)
					currAction = ACTION_END_WAIT_COFAJ;

				else if(currAction == ACTION_WAIT_WYGINAJ)
					currAction = ACTION_END_WAIT_WYGINAJ;

				PlayBeep2();

			}

		}

	}
}


/*
 *
 * 					Praca maszyny
 *
 */
void Running_Loop(void){

	if(initFlag)		/* Inicjalizacja jesli trzeba */
		Machine_Init();

	int16_t currAngle = Encoder_GetAngle();		/* Aktualne dane o k¹cie */

	/* Aktualne nastawy */
	uint16_t fAngle = Settings.m[currMem].f_angle;
	uint16_t rAngle = Settings.m[currMem].r_angle;
	char cDir = (Settings.m[currMem].direction) ? 'P' : 'L';

	char cMem = 'r';

	if(currMem < 8)
		cMem = currMem + '1';

	/* Automatyzacja */
	char sAS[3] = "AS";
	char sAB[3] = "AB";
	char sAC[3] = "AC";

	if(!Settings.AS.enable)						/* Jesli jakis tryb jest wylaczony to literke 'A' zastepujemy kropka */
		sAS[0] = '.';
	else if(!blinkFlag && currAction == ACTION_WAIT_WYGINAJ)	/* Jesli czekamy na rozpoczecie cyklu to migamy literka oznaczajaca tryb automatyczny */
		sAS[0] = ' ';

	if(!Settings.AB.enable)
		sAB[0] = '.';
	else if(!blinkFlag && currAction == ACTION_WAIT_BAZUJ)
		sAB[0] = ' ';

	if(!Settings.AC.enable)
		sAC[0] = '.';
	else if(!blinkFlag && currAction == ACTION_WAIT_COFAJ)
		sAC[0] = ' ';


	if(!blinkFlag && Encoder_IsBusy()){			/* Jesli maszyna jest w trakcie jakiegos cyklu - to migaj odpowiednia literka */
		if(currAction == ACTION_WYGINAJ)
			sAS[1] = ' ';

		else if(currAction == ACTION_BAZUJ)
			sAB[1] = ' ';

		else if(currAction == ACTION_COFAJ)
			sAC[1] = ' ';
	}


	/* Wyswietlamy wszystko */
	lcd_xy(0,0);
	lcd_printf("I=%5d %s %s %s", currAngle, sAS, sAB, sAC);
	lcd_xy(0,1);
	lcd_printf("M%c %c %4d %4d", cMem, cDir, fAngle, rAngle);


	/* Przelaczanie miedzy cyklami w trybach automatycznych */
	if(!Encoder_IsBusy()){	/* Jesli Maszyna nie wykonuje cyklu */

		if(currAction == ACTION_WYGINAJ && Settings.AC.enable){
			currDelay = Settings.AC.time * 10;
			currAction = ACTION_WAIT_COFAJ;	/* Czekaj na cofanie (autocofanie) */
		}

		else if(currAction == ACTION_COFAJ && Settings.AB.enable){
			currDelay = Settings.AB.time * 10;
			currAction = ACTION_WAIT_BAZUJ;	/* Czekaj na bazowanie (autobazowanie) */
		}

		else if(currAction == ACTION_BAZUJ && Settings.AS.enable){
			currDelay = Settings.AS.time * 10;
			currAction = ACTION_WAIT_WYGINAJ;	/* Czekaj na wyginanie (autostart) */
		}

		else if(currAction == ACTION_END_WAIT_WYGINAJ)
			Machine_Wyginaj();

		else if(currAction == ACTION_END_WAIT_COFAJ)
			Machine_Cofaj();

		else if(currAction == ACTION_END_WAIT_BAZUJ)
			Machine_Bazuj();

	}

}










