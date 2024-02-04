/*
 * MenuControls.h
 *
 *  Created on: 12.03.2019
 *      Author: Kajetan
 */

#ifndef MODULES_MENUCONTROLS_H_
#define MODULES_MENUCONTROLS_H_

#include "stm32f0xx_hal.h"
#include "Buttons.h"
#include "Menu.h"
#include "Settings.h"


#define BLINK_ON_PERIOD_MS  90
#define BLINK_OFF_PERIOD_MS 40

#define MIN_ANGLE	0
#define MAX_ANGLE	7200

#define MIN_TIME_S	1
#define MAX_TIME_S	99

#define MENU_CONTROLS_BTN_INC		BTN_U
#define MENU_CONTROLS_BTN_DEC		BTN_D
#define MENU_CONTROLS_BTN_TAB		BTN_R

#define MENU_CONTROLS_BUTTONS 		MENU_CONTROLS_BTN_INC | MENU_CONTROLS_BTN_DEC | MENU_BTN_BACK | MENU_CONTROLS_BTN_TAB

void MC_ISR_Process();
void MC_drawSwitch(uint8_t x, uint8_t y, uint8_t *state);
void MC_drawSwitchLP(uint8_t x, uint8_t y, uint8_t *state);
void MC_drawAngleSelector(uint8_t x, uint8_t y, int16_t *state);
void MC_drawTimeSelector(uint8_t x, uint8_t y, int16_t *state);
void MC_drawAutoModeSelector(uint8_t x, uint8_t y, AutoRecord_t *state);


#endif /* MODULES_MENUCONTROLS_H_ */
