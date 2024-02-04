/*
 * Buttons.h
 *
 *  Created on: 26.02.2019
 *      Author: Kajetan
 */

#ifndef MODULES_BUTTONS_H_
#define MODULES_BUTTONS_H_

#include "SN74LS166.h"

#define REFRESH_PERIOD_MS 				30

#define AUTOREPETITION_PERIOD_CYCLES 	20		/* okres autorepetycji w cyklach odswiezania */
#define AUTOREPETITION_DELAY_CYCLES 	100		/* czas po ktorym nastapi autorepetycja */

#define MAX_BUTTONS 			16
#define MAX_CALLBACKS 			16
#define MAX_AUTOREPETITIONS 	4

#define SHIFT_REG_DATA_SIZE 	2
#define BUTTONS_ACTIVE_STATE 	0


typedef uint16_t ButtonsData_t;
typedef void(*ButtonsCallback_t)(ButtonsData_t state);


/*
 * 				Kody przyciskow
 */
#define BTN_M1		(ButtonsData_t)0x0001
#define BTN_M2		(ButtonsData_t)0x0002
#define BTN_M3		(ButtonsData_t)0x0004
#define BTN_M4		(ButtonsData_t)0x0008
#define BTN_M5		(ButtonsData_t)0x0010
#define BTN_M6		(ButtonsData_t)0x0020
#define BTN_M7		(ButtonsData_t)0x0040
#define BTN_M8		(ButtonsData_t)0x0080
#define BTN_D		(ButtonsData_t)0x0200 //0x0100   B³¹d na p³ytce
#define BTN_L		(ButtonsData_t)0x0400 //0x0200
#define BTN_U		(ButtonsData_t)0x0100 //0x0400
#define BTN_R		(ButtonsData_t)0x0800
#define BTN_13		(ButtonsData_t)0x1000
#define BTN_14		(ButtonsData_t)0x2000
#define BTN_15		(ButtonsData_t)0x4000
#define BTN_16		(ButtonsData_t)0x8000

#define BTN_MEMORY_ALL	(BTN_M1|BTN_M2|BTN_M3|BTN_M4|BTN_M5|BTN_M6|BTN_M7|BTN_M8)
#define BTN_KEYPAD_ALL	(BTN_D|BTN_L|BTN_U|BTN_R)


/*
 * 				ID callback'ów
 */
/* ID rekordow callbackow  */
#define BTN_CB_MENU 			0
#define BTN_CB_MENU_CONTROLS 	1
#define BTN_CB_MACHINE 			2
#define BTN_CB_MEMORY 			3

/* ID rekordow autorepetycji  */
#define BTN_AR_MENU_CONTROLS_INC 0
#define BTN_AR_MENU_CONTROLS_DEC 1
/*
 * 				Glowna struktura modulu przyciskow
 */
typedef struct{

	ButtonsData_t prevState;
	ButtonsData_t currState;
	ButtonsData_t clicksDetected;
	ButtonsData_t arOccurred;		/* Czy wywolanie callbacka bylo spowodowane autorepetycj¹ */

	ButtonsCallback_t cbs[MAX_CALLBACKS];
	ButtonsData_t cbs_masks[MAX_CALLBACKS];

	uint8_t ar_cnts[MAX_AUTOREPETITIONS];
	ButtonsData_t ar_keys[MAX_AUTOREPETITIONS];

}Buttons_t;


void Buttons_Init(void);
void Buttons_Process(void);
void Buttons_ISR_Process(void);
ButtonsData_t Buttons_GetState(void);

void Buttons_SetClickCallback(uint8_t id, ButtonsData_t mask, ButtonsCallback_t cb);
void Buttons_RemoveClickCallback(uint8_t id);

void Buttons_EnableAutorepetition(uint8_t id, ButtonsData_t key);
void Buttons_DisableAutorepetition(uint8_t id);

uint8_t Buttons_IsAutorepetitionClick(ButtonsData_t key);


#endif /* MODULES_BUTTONS_H_ */
