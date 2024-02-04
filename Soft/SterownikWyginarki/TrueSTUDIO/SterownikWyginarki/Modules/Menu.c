/*
 * Menu.c
 *
 *  Created on: 07.03.2019
 *      Author: Kajetan
 */


#include "Menu.h"
#include "Buttons.h"
#include "Sounds.h"
#include "i2c-lcd.h"
#include "Settings.h"

void OnMenuBtnClick(ButtonsData_t state);

// Pozycja: menu glowne
extern MenuItem_t MainMenu;


/* Glowna struktura */
volatile MenuModule_t MenuModule = {0};

/* Pobiera aktualny element */
inline MenuItem_t * Menu_GetCurrElement(void){
	return MenuModule.pCurrentElement;
}


/*
 * 				Rysowanie pozycji w elemencie typu lista
 */
inline uint8_t Menu_IsNowListElement(){
	return MenuModule.pCurrentElement->childreen_count;
}
inline uint8_t Menu_IsListElement(MenuItem_t* element){
	return element->childreen_count;
}


/*
 * 				Inicjalizuje element (dodaje obsluge przyciskow)
 */
void Menu_InitElementWithPos(MenuItem_t* element, uint8_t pos){

	MenuModule.currMenuPos = pos;			/* Zerujemy pozycje menu */
	MenuModule.pCurrentElement = element;	/* Zmieniamy aktualny el. menu na podany w argumencie */

	if(Menu_IsListElement(element))
		Buttons_SetClickCallback(BTN_CB_MENU, MENU_BUTTONS, OnMenuBtnClick);
	else
		Buttons_SetClickCallback(BTN_CB_MENU, MENU_BTN_BACK, OnMenuBtnClick);

	MenuModule.needsToRedraw = 1; /* Zadanie przerysowania */

}
void Menu_InitElement(MenuItem_t* element){
	Menu_InitElementWithPos(element, 0);
}


/*
 * 				Rysowanie pozycji w elemencie typu lista
 */
void Menu_DrawList(){

	lcd_xy(0,1);
	char* pName = MenuModule.pCurrentElement->pChildreenList[MenuModule.currMenuPos]->pName;
	lcd_printf("%d. %s", (MenuModule.currMenuPos + 1), pName);

}


/*
 * 				Watek menu
 */
void Menu_Process(void){

	if(MenuModule.pCurrentElement){	/* Jesli jest wybrany ktorys el. menu */

		if(MenuModule.needsToRedraw){				/* Trzeba przerysowac cale menu */
			MenuModule.needsToRedraw = 0;

			lcd_clear();
			lcd_xy(0,0);
			lcd_printf("%16s", MenuModule.pCurrentElement->pName);

			if(Menu_IsNowListElement()){	/* Element typu lista - wyswietlamy liste */
				Menu_DrawList();
			}

		}

		if(MenuModule.pCurrentElement->loop_cb)		/* Wywolujemy petle elementu */
			MenuModule.pCurrentElement->loop_cb();

	}

}


/*
 * 				Inicjalizacja
 */
void Menu_Init(){

	Menu_InitElement(&MainMenu);

}


/*
 *
 * 					Obs³uga klawiszy
 *
 */
void OnMenuBtnClick(ButtonsData_t state){

	switch(state){

		case MENU_BTN_PREV:

			PlayBeep2();

			if(Menu_IsNowListElement()){
				if(!MenuModule.currMenuPos)
					MenuModule.currMenuPos = MenuModule.pCurrentElement->childreen_count;

				MenuModule.currMenuPos--;

				MenuModule.needsToRedraw = 1;
			}

			break;

		case MENU_BTN_NEXT:

			PlayBeep2();

			if(Menu_IsNowListElement()){
				MenuModule.currMenuPos++;
				if(MenuModule.currMenuPos >= MenuModule.pCurrentElement->childreen_count)
					MenuModule.currMenuPos = 0;

				MenuModule.needsToRedraw = 1;
			}

			break;

		case MENU_BTN_BACK:

			PlayBeep1();

			if(MenuModule.pCurrentElement->pParent){

				uint8_t pos = 0;

				if(MenuModule.pCurrentElement != &MainMenu){
					for(; pos<MenuModule.pCurrentElement->pParent->childreen_count; pos++){
						if(MenuModule.pCurrentElement->pParent->pChildreenList[pos] == MenuModule.pCurrentElement)
							break;
					}
				}

				Menu_InitElementWithPos(MenuModule.pCurrentElement->pParent, pos);

			}

			break;

		case MENU_BTN_SELECT:

			PlayBeep1();

			if(Menu_IsNowListElement()){
				Menu_InitElement(MenuModule.pCurrentElement->pChildreenList[MenuModule.currMenuPos]);
			}

			break;

	}

}





