#include "my_menu.h"

#define KEY_READ_STATE(key,sta) {key.pressed = digitalRead(key.pin); if(key.pressed) {sta = key.type;}}

struct Selection {
	char *text;
	unsigned char type;
	union {
		struct Scene* nextScene;
		bool* state;
		void(*func_state)(bool);
		void(*func) (void);
	} action;
};

struct Scene {
	unsigned char selectionsNumber = 0;
	struct Selection selections[MAX_SELECTION_NUMBER];
};

struct Key {
	int pin;
	bool pressed = false;
	unsigned char type;
	bool pullTo = HIGH;
};

struct Selection buildSel(bool *sta, void(*fun)(bool)) {
	struct Selection sel;
	sel.type = SELECTION_ON_OFF_WITH_FUNCTION;
	sel.action.state = sta;
	sel.action.func_state = fun;
	return sel;
}
struct Selection buildSel(bool *sta) {
	struct Selection sel;
	sel.type = SELECTION_ON_OFF;
	sel.action.state = sta;
	return sel;
}
struct Selection buildSel(char* text, struct Scene* sc) {
	struct Selection sel;
	sel.text = text;
	sel.type = SELECTION_NEXT_SCENE;
	sel.action.nextScene = sc;
	return sel;
}
struct Selection buildSel(char* text, void(*fun)(void)) {
	struct Selection sel;
	sel.text = text;
	sel.type = SELECTION_FUNCTION;
	sel.action.func = fun;
	return sel;
}

struct Scene buildSce(unsigned char len, struct Selection* sels) {
	struct Scene sce;
	sce.selectionsNumber = len;
	for (unsigned char i = 0; i < len; i++)
		sce.selections[i] = sels[i];
	return sce;
}


/**
* Name: Menu
* Type: Constructor Function
* Function: construct a Menu class
* Parameter:
** No parameter
**/
Menu::Menu() {
	u8g = U8GLIB_SSD1306_128X64(U8G_I2C_OPT_NONE);
	_key_up.type = KEY_UP;
	_key_down.type = KEY_DOWN;
	_key_back.type = KEY_BACK;
	_key_ok.type = KEY_OK;
}

/**
* Name: draw
* Type: Class Function
* Function: draw if changed
* Parameter:
** No parameter
**/
void Menu::draw() {
	if (_changed) {
		u8g.firstPage();
		do {
			for (unsigned char i = 0; i < 4; i++) {
				if (_startSelectionNumber + i >= _top().selectionsNumber) {
					break;
				}
				_drawSelection(_top().selections[_startSelectionNumber + i], i);
			}
		} while (u8g.nextPage());
	}
}

/**
* Name: draw
* Type: Class Function
* Function: update Selections if key pressed
* Parameter:
** No parameter
**/
void Menu::update() {
	if (_updateKeys()) {
		_changed = true;
	}
	if (_changed) {
		switch (_keyState) {
		case KEY_UP:
			_selection++;
			_rollSelection();
			break;
		case KEY_DOWN:
			_selection--;
			_rollSelection();
			break;
		case KEY_BACK:
			_selection = 0;
			_startSelectionNumber = 0;
			_pop();
			break;
		case KEY_OK:
			struct Selection sel = (_top().selections)[_selection];
			switch (sel.type) {
			case SELECTION_NEXT_SCENE:
				_push(*sel.action.nextScene);
				break;
			case SELECTION_ON_OFF_WITH_FUNCTION:
				sel.action.func_state(!(*sel.action.state));
			case SELECTION_ON_OFF:
				*sel.action.state = !*sel.action.state;
				break;
			case SELECTION_FUNCTION:
				sel.action.func();
				break;
			}
			break;
		}
	}
	draw();
	_changed = false;
}

//FUCK_MY_CLASS_TO_MAKE_UPDATE_TO_THREE_PARTS_SO_THAT_YOU_CAN_USE_MY_CLASS_MORE_HARD
/**
* Name: FUCK_updateKey
* Type: Class Function
* Function: a FUCKed Function to update keys
* Parameter:
** No parameter
**/
void Menu::FUCK_updateKey() {
	if (FUCK_MY_CLASS_TO_MAKE_UPDATE_TO_THREE_PARTS_SO_THAT_YOU_CAN_USE_MY_CLASS_MORE_HARD) {
		if (_updateKeys()) {
			_changed = true;
		}
	}
	else {
		while (true) {
			Serial.println("FUCK YOU!");
		}
	}

}

/**
* Name: FUCK_update
* Type: Class Function
* Function: a FUCKed Function to update if key pressed
* Parameter:
** No parameter
**/
void Menu::FUCK_update() {
	if (FUCK_MY_CLASS_TO_MAKE_UPDATE_TO_THREE_PARTS_SO_THAT_YOU_CAN_USE_MY_CLASS_MORE_HARD) {
		if (_changed) {
			switch (_keyState) {
			case KEY_UP:
				_selection++;
				_rollSelection();
				break;
			case KEY_DOWN:
				_selection--;
				_rollSelection();
				break;
			case KEY_BACK:
				_selection = 0;
				_startSelectionNumber = 0;
				_pop();
				break;
			case KEY_OK:
				struct Selection sel = (_top().selections)[_selection];
				switch (sel.type) {
				case SELECTION_NEXT_SCENE:
					_push(*sel.action.nextScene);
					break;
				case SELECTION_ON_OFF_WITH_FUNCTION:
					sel.action.func_state(!(*sel.action.state));
				case SELECTION_ON_OFF:
					*sel.action.state = !*sel.action.state;
					break;
				case SELECTION_FUNCTION:
					sel.action.func();
					break;
				}
				break;
			}
		}
	}
	else {
		while (true) {
			Serial.println("FUCK YOU!");
		}
	}
}

/**
* Name: FUCK_draw
* Type: Class Function
* Function: a FUCKed Function to draw if changed
* Parameter:
** No parameter
**/
void Menu::FUCK_draw() {
	if (FUCK_MY_CLASS_TO_MAKE_UPDATE_TO_THREE_PARTS_SO_THAT_YOU_CAN_USE_MY_CLASS_MORE_HARD) {
		draw();
		_changed = false;
	}
	else {
		while (true) {
			Serial.println("FUCK YOU!");
		}
	}
}

/**
* Name: setKeys
* Type: Class Function
* Function: regist a pin to keys
* Parameter:
** int up   --pin of UP key
** int down --pin of DOWN key
** int back --pin of BACK key
** int ok   --pin of OK key
**/
void Menu::setKeys(int up, int down, int back, int ok) {
	settedKey = true;
	_key_up.pin = up;
	_key_down.pin = down;
	_key_back.pin = back;
	_key_ok.pin = ok;
}

/**
* Name: registerScene
* Type: Class Function
* Function: regist a scene to start
* Parameter:
** struct Scene scene --scene registed
**/
void Menu::registerScene(struct Scene scene) {
	_push(scene);
}

bool Menu::_push(struct Scene scene) {

	if (_pointer + 1 >= STACK_MAX_DEPTH) {
		return false;
	}
	_stack[++_pointer] = scene;
	return true;
}

struct Scene Menu::_top() {
	return _stack[_pointer];
}

bool Menu::_pop() {
	if (_pointer <= 1) {
		return false;
	}
	--_pointer;
	return true;
}

void Menu::_rollSelection() {
	if (_selection < 0 || _selection == 255) {
		_selection = _top().selectionsNumber - 1;
	}
	else if (_selection >= _top().selectionsNumber) {
		_selection = 0;
	}
	if (_selection < _startSelectionNumber) {
		_startSelectionNumber = _selection;
	}
	else if (_selection > _startSelectionNumber + 3) {
		_startSelectionNumber = _selection - 3;
	}
}

bool Menu::_updateKeys() {
	_oldKeyState = _keyState;
	if (settedKey) {
		KEY_READ_STATE(_key_up, _keyState);
		KEY_READ_STATE(_key_down, _keyState);
		KEY_READ_STATE(_key_back, _keyState);
		KEY_READ_STATE(_key_ok, _keyState);
		if (!(_key_up.pressed || _key_down.pressed || _key_back.pressed || _key_ok.pressed)) {
			_keyState = KEY_NONE;
		}
		return (_oldKeyState != _keyState);
	}
	else {
		return false;
	}
}

void Menu::_drawRoller() {
	unsigned char start = 64 / _top().selectionsNumber * _startSelectionNumber;
	unsigned char end = 64 / _top().selectionsNumber * (_top().selectionsNumber - _startSelectionNumber - 4);
	u8g.drawBox(126, start, 2, end - start);
}

void Menu::_drawSelection(struct Selection sel, unsigned char num) {
	switch (sel.type) {
	case SELECTION_FUNCTION:
	case SELECTION_NEXT_SCENE:
		_drawText(num, sel.text);
		break;
	case SELECTION_ON_OFF:
		_drawText(num, sel.action.state ? "ON" : "OFF");
		break;
	}
}

void Menu::_drawText(unsigned char num, char* str) {
	bool sel = false;
	if (_startSelectionNumber + num == _selection) {
		sel = true;
	}
	int x = SX;
	int y;
	switch (num) {
	case 0:
		y = S1Y;
		break;
	case 1:
		y = S2Y;
		break;
	case 2:
		y = S3Y;
		break;
	case 3:
		y = S4Y;
		break;
	}
	u8g.setDefaultForegroundColor();
	if (sel) {
		u8g.drawBox(2, y - 15, 124, 15);
		u8g.setDefaultBackgroundColor();
	}
	u8g.setPrintPos(x + 2, y);
	u8g.print(str);
}


