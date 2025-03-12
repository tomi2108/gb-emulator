#ifndef PPU_H_
#define PPU_H_

#include "common.h"
#include <SDL2/SDL.h>

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 144
#define SCALE 10

void screen_init();
void screen_free();

void screen_set(u8 x, u8 y, u8 to[3]);
u8 *screen_get(u8 x, u8 y);

void screen_draw();
void screen_clear();

#endif
