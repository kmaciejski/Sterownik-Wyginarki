/*
 * Menu.h
 *
 *  Created on: 07.03.2019
 *      Author: Kajetan
 */

#ifndef MODULES_MENU_H_
#define MODULES_MENU_H_

#include "stm32f0xx_hal.h"
#include "Buttons.h"
#include "MacroFramework.h"

/*
 *		Przyciski steruj¹ce menu
 */
#define MENU_BTN_PREV		BTN_D
#define MENU_BTN_NEXT		BTN_U
#define MENU_BTN_BACK		BTN_L
#define MENU_BTN_SELECT		BTN_R
#define MENU_BUTTONS MENU_BTN_PREV | MENU_BTN_NEXT | MENU_BTN_BACK | MENU_BTN_SELECT


/*
 *		Struktury i callbacki
 */
typedef void (* MenuLoopCb_t) (void);


typedef struct tagMenuItem_t{

	char 			*pName;
	uint8_t 		childreen_count;
	MenuLoopCb_t 	loop_cb;

	struct tagMenuItem_t *pParent;
	struct tagMenuItem_t **pChildreenList;

}MenuItem_t;

typedef struct{

	uint8_t needsToRedraw;

	uint8_t currMenuPos;			/* aktualna pozycja listy menu */

	MenuItem_t *pCurrentElement;

}MenuModule_t;


/*
 *		Makrodefinicje definiuj¹ce strukturê menu (metadane)
 */

/* Pojedynczy element drzewa */
#define MENU_SINGLE_ITEM(name,parent,cb_loop)		\
	MenuItem_t name = {								\
		#name,										\
		0,											\
		cb_loop,									\
		&parent,									\
		0											\
	}

/* Pojedynczy element drzewa (podajemy sami ciag znakow opisujacy nazwe elementu) */
#define MENU_SINGLE_ITEM_S(name,str,parent,cb_loop)		\
	MenuItem_t name = {								\
		str,										\
		0,											\
		cb_loop,									\
		&parent,									\
		0											\
	}

/* Element typu lista */
#define MENU_LIST_ITEM(name,parent,cb_loop,...)											\
extern MenuItem_t __VA_ARGS__;															\
MenuItem_t* name##_childreen[MF_NARGS(__VA_ARGS__)] = {REF_ARGS(__VA_ARGS__)};			\
MenuItem_t name = {																		\
		#name,																			\
		MF_NARGS(__VA_ARGS__),															\
		cb_loop,																		\
		&parent,																		\
		name##_childreen																\
	}

/* Element typu lista (podajemy sami ciag znakow opisujacy nazwe elementu) */
#define MENU_LIST_ITEM_S(name,str,parent,cb_loop,...)									\
extern MenuItem_t __VA_ARGS__;															\
MenuItem_t* name##_childreen[MF_NARGS(__VA_ARGS__)] = {REF_ARGS(__VA_ARGS__)};			\
MenuItem_t name = {																		\
		str,																			\
		MF_NARGS(__VA_ARGS__),															\
		cb_loop,																		\
		&parent,																		\
		name##_childreen																\
	}

/*
 *		Procedury
 */
void Menu_Init();
void Menu_Process(void);
MenuItem_t * Menu_GetCurrElement(void);

#endif /* MODULES_MENU_H_ */
