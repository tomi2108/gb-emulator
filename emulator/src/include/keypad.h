#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "common.h"
#include <SDL2/SDL.h>

bool keypad_key_is_pressed(u8 key);
u8 keypad_get_key(bool *res);

void keypad_press_key(u8 key);
void keypad_release_key(u8 key);

void keypad_handle_input();

void keypad_init();
void keypad_free();

#endif
