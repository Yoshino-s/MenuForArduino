#include "my_menu.h"

/**
* Name: state1
* Type: Boolan
* Function: record two states
**/
bool sta1 = false;
/**
* Name: state2
* Type: Boolan
* Function: record two states
**/
bool sta2 = false;

void timee() {};
void temp() {};
void humi() {};
void yes() {};
void check() {};
void internet() {};

/**
* Name: sandData
* Type: Function
* Function: Send data via nrf24l01p
* Parameter: 
** bool sta --The state of the selection which execute this function
**/
void sendData(bool sta) {};

Menu menu;

struct Selection first[4];
struct Selection second[4];
struct Selection dev1[4];
struct Selection dev2[4];
struct Scene firstpage;
struct Scene secondpage;
struct Scene DEV1page;
struct Scene DEV2page;


void setup() {
	Serial.begin(9600);
	menu.u8g.setFont(u8g_font_chikitar);

	first[0] = buildSel("TIME", timee);
	first[1] = buildSel("TEMP", temp);
	first[2] = buildSel("HUMI", humi);
	first[3] = buildSel("NEXT", &secondpage);

	firstpage = buildSce(4, first);

	second[0] = buildSel("DEV1", &DEV1page);
	second[1] = buildSel("DEV2", &DEV2page);
	second[2] = buildSel("CHECK", check);
	second[3] = buildSel("INTERNET", internet);

	secondpage = buildSce(4, second);

	dev1[0] = buildSel(&sta1, sendData);
	dev1[1] = buildSel("TEMP", temp);
	dev1[2] = buildSel("HUMI", humi);
	dev1[3] = buildSel("YES", yes);

	DEV1page = buildSce(4, dev1);

	dev2[0] = buildSel(&sta2);
	dev2[1] = buildSel("TEMP", temp);
	dev2[2] = buildSel("HUMI", humi);
	dev2[3] = buildSel("YES", yes);

	DEV2page = buildSce(4, dev2);

	menu.setKeys(3, 4, 5, 6);//int up, int down, int back, int ok
	menu.registerScene(firstpage);
}

void loop()
{
	//FUCK_MY_CLASS_TO_MAKE_UPDATE_TO_THREE_PARTS_SO_THAT_YOU_CAN_USE_MY_CLASS_MORE_HARD
	//menu.FUCK_updateKey();
	//menu.FUCK_update();
	//menu.FUCK_.draw();
	//NORMAL
	menu.update();

	/* add main program code here */

}