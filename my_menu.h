// my_menu.h

#ifndef _MY_MENU_h
#define _MY_MENU_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "U8glib/U8glib.h"

#define FUCK_MY_CLASS_TO_MAKE_UPDATE_TO_THREE_PARTS_SO_THAT_YOU_CAN_USE_MY_CLASS_MORE_HARD false

#define STACK_MAX_DEPTH 3
#define MAX_STRING_LENGTH 10
#define MAX_SELECTION_NUMBER 4

//point of four selections

#define S1Y 17
#define S2Y 32
#define S3Y 45
#define S4Y 62

#define SX 4

//type of Scene
#define SCENE_TEXT 0x01 //TODO
#define SCENE_SELECTIONS 0x02
#define SCENE_SELECTIONS_WITHOUT_TITLE 0x03 //TODO

//type of Selection
#define SELECTION_NEXT_SCENE 0x01
#define SELECTION_ON_OFF 0x02
#define SELECTION_FUNCTION 0x03
#define SELECTION_ON_OFF_WITH_FUNCTION 0x04

//type of keys
#define KEY_NONE 0x00
#define KEY_UP 0x01
#define KEY_DOWN 0x02
#define KEY_BACK 0x04
#define KEY_OK 0x08

/**
* Name: buildSel
* Type: Function
* Function: Build a Selection used like a switch and registed a function
* Parameter: 
** bool* sta         --The poniter of the selection state
** void (*fun)(bool) --The function which registed to the selection
**/
struct Selection buildSel(bool *sta, void(*fun)(bool));

/**
* Name: buildSel
* Type: Function
* Function: Build a Selection used like a switch
* Parameter:
** bool* sta --The poniter of the selection state
**/
struct Selection buildSel(bool *sta);

/**
* Name: buildSel
* Type: Function
* Function: Build a Selection linked to a new scene
* Parameter:
** char* text       --The text showing at the scene
** struct Scene* sc --The scene which the selection linked to
**/
struct Selection buildSel(char* text, struct Scene* sc);

/**
* Name: buildSel
* Type: Function
* Function: Build a Selection registed a function
* Parameter:
** char* text        --The text showing at the scene
** void (*fun)(bool) --The function which registed to the selection
**/
struct Selection buildSel(char* text, void(*fun)(void));


/**
* Name: buildSce
* Type: Function
* Function: Build a Scene
* Parameter:
** unsigned char len      --The length of Selection array
** struct Selection* sels --The Selection array the scene use
**/
struct Scene buildSce(unsigned char len, struct Selection* sels);


/**
* Name: Selection
* Type: structure
* Function: Record a selection
* Value:
** char* text              --The text showing at the scene
** unsigned char type      --The type of the selection
** union(anonymity) action --The action or state in the selection
**/
struct Selection;
/**
* Name: Scene
* Type: structure
* Function: Record a scene
* Value:
** uunsigned char selectionsNumber --The length of Selection array
** selections                      --The array of the selections
**/
struct Scene;
/**
* Name: Key
* Type: structure
* Function: Record a key
* Value:
** int pin            --The pin of the key
** bool pressed       --If the key is pressed
** unsigned char type --The type of the key
** bool pullTo        --The pull level of the key
**/
struct Key;

/**
* Name: Menu
* Type: class
* Function: Generate a menu via SSD1306--128x64
**/
class Menu {

public:
	Menu();
	void draw();

	void update();

	void FUCK_updateKey();
	void FUCK_update();
	void FUCK_draw();

	void registerScene(struct Scene scene);

	void setKeys(int up, int down, int back, int ok);

	U8GLIB_SSD1306_128X64 u8g;
private:
	bool __debug = true;

	void _debug(char* str) {
		if (__debug) {
			Serial.print("Debug:  ");
			Serial.println(str);
		}
	}

	bool _changed = true;

	unsigned char _selection = 0;
	unsigned char _startSelectionNumber = 0;

	bool settedKey = false;

	struct Key _key_up, _key_down, _key_back, _key_ok;
	unsigned char _keyState, _oldKeyState = KEY_NONE;

	bool _updateKeys();
	void _rollSelection();

	void _drawText(unsigned char num, char* str);
	void _drawSelection(struct Selection sel, unsigned char num);
	void _drawRoller();

	struct Scene _stack[STACK_MAX_DEPTH];
	unsigned char _pointer = 0;
	bool _push(struct Scene scene);
	struct Scene _top();
	bool _pop();
};


#endif

