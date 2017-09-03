#include "input.h"

#include <libndls.h>

#define KEY(_name) (isKeyPressed(KEY_NSPIRE_##_name))

bool input::exit() {
	return KEY(ESC);
}

bool input::up() {
	return KEY(LEFTUP) || KEY(UP) || KEY(UPRIGHT);
}

bool input::down() {
	return KEY(DOWNLEFT) || KEY(DOWN) || KEY(RIGHTDOWN);
}

bool input::left() {
	return KEY(LEFTUP) || KEY(LEFT) || KEY(DOWNLEFT);
}

bool input::right() {
	return KEY(UPRIGHT) || KEY(RIGHT) || KEY(RIGHTDOWN);
}
