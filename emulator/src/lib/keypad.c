#include "../include/keypad.h"
#include <commons/log.h>

u8 keys[0x10] = {0};

log_t keypad_logger = {.file = "log.log",
                       .process = "KEYPAD",
                       .level = LOG_LEVEL_DEBUG,
                       .is_active_console = 1};

u8 keycodes[0x10] = {
    SDL_SCANCODE_1, 
    SDL_SCANCODE_2, 
    SDL_SCANCODE_3, 
    SDL_SCANCODE_4, 
    SDL_SCANCODE_Q, 
    SDL_SCANCODE_W, 
    SDL_SCANCODE_E, 
    SDL_SCANCODE_R, 
    SDL_SCANCODE_A, 
    SDL_SCANCODE_S, 
    SDL_SCANCODE_D, 
    SDL_SCANCODE_F, 
    SDL_SCANCODE_Z, 
    SDL_SCANCODE_X, 
    SDL_SCANCODE_C, 
    SDL_SCANCODE_V, 
};

void keypad_press_key(u8 key) { keys[key] = 1; }
void keypad_release_key(u8 key) { keys[key] = 0; }

bool keypad_key_is_pressed(u8 key) { return keys[key]; }

u8 keypad_get_key(bool *res) {
  for (u8 i = 0; i < 0x10; i++) {
    if (keys[i])
      return i;
  }
  *res = 0;
  return 0;
}

u8 get_keycode(u8 target, bool *res) {
  for (int i = 0; i < 0x10; i++) {
    if (keycodes[i] == target)
      return i;
  }
  *res = 0;
  return 0;
}

void keypad_handle_input() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    bool res = 1;
    u8 key = get_keycode(event.key.keysym.scancode, &res);
    if (res == 0)
      continue;

    switch (event.type) {
    case SDL_KEYDOWN:
      keypad_press_key(key);
      break;

    case SDL_KEYUP:
      keypad_release_key(key);
      break;

    default:
      break;
    }
  }
}

void keypad_init() {
  keypad_logger.logger =
      log_create(keypad_logger.file, keypad_logger.process,
                 keypad_logger.is_active_console, keypad_logger.level);
}

void keypad_free() { log_destroy(keypad_logger.logger); }
