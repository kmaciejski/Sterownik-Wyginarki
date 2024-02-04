/*
 * MenuStructure.c
 *
 *  Created on: 09.04.2019
 *      Author: Kajetan
 */

#include "Menu.h"
#include "Settings.h"
#include "MenuControls.h"
#include "i2c-lcd.h"


/*
 *
 * 					definicje petli w menu
 *
 */
void Default_Loop(void){}

extern void Running_Loop(void);

/* Obsluga trybow automatycznych */

void AB_Loop(void){
	MC_drawAutoModeSelector(0, 1, &Settings.AB);
}
void AC_Loop(void){
	MC_drawAutoModeSelector(0, 1, &Settings.AC);
}
void AS_Loop(void){
	MC_drawAutoModeSelector(0, 1, &Settings.AS);
}


/* Obsluga pamieci */

uint8_t GetMX(void){
	MenuItem_t *pCurrElement = Menu_GetCurrElement();
	uint8_t Mx = pCurrElement->pParent->pName[1] - '1';	// Pobieramy aktualnie wybrany slot Mx z nazwy elementu
	return Mx;
}

void Mx_F_Loop(){
	uint8_t Mx = GetMX();
	MC_drawAngleSelector(0, 1, &Settings.m[Mx].f_angle);
}
void Mx_R_Loop(){
	uint8_t Mx = GetMX();
	MC_drawAngleSelector(0, 1, &Settings.m[Mx].r_angle);
}
void Mx_LP_Loop(){
	uint8_t Mx = GetMX();
	MC_drawSwitchLP(0,1, &Settings.m[Mx].direction);
}

void Mx_Loop(){

	MenuItem_t *pCurrElement = Menu_GetCurrElement();
	uint8_t Mx = pCurrElement->pName[1] - '1';	// Pobieramy aktualnie wybrany slot Mx z nazwy elementu

	uint16_t f_angle = Settings.m[Mx].f_angle;
	uint16_t r_angle = Settings.m[Mx].r_angle;
	char dir = (Settings.m[Mx].direction) ? 'P' : 'L';

	lcd_xy(0, 0);
	lcd_printf("%4d  %4d  %c", f_angle, r_angle, dir);
}

/*
 *
 * 					Schemat drzewa menu (metadane)
 *
 */

MENU_LIST_ITEM_S(MainMenu, "Menu", MainMenu, Default_Loop,
	Ustawienia,
	Pamiec,
	Start
);

MENU_LIST_ITEM(Ustawienia, MainMenu, Default_Loop,
	AC,
	AB,
	AS
);

MENU_SINGLE_ITEM_S(AS, "Autostart", Ustawienia, AS_Loop);
MENU_SINGLE_ITEM_S(AB, "Autobazowanie", Ustawienia, AB_Loop);
MENU_SINGLE_ITEM_S(AC, "Autocofanie", Ustawienia, AC_Loop);


MENU_LIST_ITEM(Pamiec, MainMenu, Default_Loop,
	M1,
	M2,
	M3,
	M4,
	M5,
	M6,
	M7,
	M8
);

MENU_LIST_ITEM(M1, Pamiec, Mx_Loop,
	M1_F,
	M1_R,
	M1_LP
);

MENU_LIST_ITEM(M2, Pamiec, Mx_Loop,
	M2_F,
	M2_R,
	M2_LP
);

MENU_LIST_ITEM(M3, Pamiec, Mx_Loop,
	M3_F,
	M3_R,
	M3_LP
);

MENU_LIST_ITEM(M4, Pamiec, Mx_Loop,
	M4_F,
	M4_R,
	M4_LP
);

MENU_LIST_ITEM(M5, Pamiec, Mx_Loop,
	M5_F,
	M5_R,
	M5_LP
);

MENU_LIST_ITEM(M6, Pamiec, Mx_Loop,
	M6_F,
	M6_R,
	M6_LP
);

MENU_LIST_ITEM(M7, Pamiec, Mx_Loop,
	M7_F,
	M7_R,
	M7_LP
);

MENU_LIST_ITEM(M8, Pamiec, Mx_Loop,
	M8_F,
	M8_R,
	M8_LP
);

MENU_SINGLE_ITEM_S(M1_F, "Kat skretu",M1, Mx_F_Loop);
MENU_SINGLE_ITEM_S(M1_R, "Kat cofania",M1, Mx_R_Loop);
MENU_SINGLE_ITEM_S(M1_LP, "Kierunek",M1, Mx_LP_Loop);

MENU_SINGLE_ITEM_S(M2_F, "Kat skretu",M2, Mx_F_Loop);
MENU_SINGLE_ITEM_S(M2_R, "Kat cofania",M2, Mx_R_Loop);
MENU_SINGLE_ITEM_S(M2_LP, "Kierunek",M2, Mx_LP_Loop);

MENU_SINGLE_ITEM_S(M3_F, "Kat skretu",M3, Mx_F_Loop);
MENU_SINGLE_ITEM_S(M3_R, "Kat cofania",M3, Mx_R_Loop);
MENU_SINGLE_ITEM_S(M3_LP, "Kierunek",M3, Mx_LP_Loop);

MENU_SINGLE_ITEM_S(M4_F, "Kat skretu",M4, Mx_F_Loop);
MENU_SINGLE_ITEM_S(M4_R, "Kat cofania",M4, Mx_R_Loop);
MENU_SINGLE_ITEM_S(M4_LP, "Kierunek",M4, Mx_LP_Loop);

MENU_SINGLE_ITEM_S(M5_F, "Kat skretu",M5, Mx_F_Loop);
MENU_SINGLE_ITEM_S(M5_R, "Kat cofania",M5, Mx_R_Loop);
MENU_SINGLE_ITEM_S(M5_LP, "Kierunek",M5, Mx_LP_Loop);

MENU_SINGLE_ITEM_S(M6_F, "Kat skretu",M6, Mx_F_Loop);
MENU_SINGLE_ITEM_S(M6_R, "Kat cofania",M6, Mx_R_Loop);
MENU_SINGLE_ITEM_S(M6_LP, "Kierunek",M6, Mx_LP_Loop);

MENU_SINGLE_ITEM_S(M7_F, "Kat skretu",M7, Mx_F_Loop);
MENU_SINGLE_ITEM_S(M7_R, "Kat cofania",M7, Mx_R_Loop);
MENU_SINGLE_ITEM_S(M7_LP, "Kierunek",M7, Mx_LP_Loop);

MENU_SINGLE_ITEM_S(M8_F, "Kat skretu",M8, Mx_F_Loop);
MENU_SINGLE_ITEM_S(M8_R, "Kat cofania",M8, Mx_R_Loop);
MENU_SINGLE_ITEM_S(M8_LP, "Kierunek",M8, Mx_LP_Loop);


MENU_SINGLE_ITEM(Start, MainMenu, Running_Loop);







