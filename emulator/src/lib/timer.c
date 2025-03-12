#include "../include/timer.h"

u8 delay_timer = 0;
u8 sound_timer = 0;

void timer_init() {
  delay_timer = 0;
  sound_timer = 0;
}

u8 d_timer_get() { return delay_timer; }
u8 s_timer_get() { return sound_timer; }

void d_timer_set(u8 n) { delay_timer = n; }
void s_timer_set(u8 n) { sound_timer = n; }

void s_timer_tick() {
  if (sound_timer > 0)
    sound_timer--;
}
void d_timer_tick() {
  if (delay_timer > 0)
    delay_timer--;
}
