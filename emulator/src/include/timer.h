#ifndef TIMER_H_
#define TIMER_H_

#include "common.h"

void timer_init();

u8 d_timer_get();
u8 s_timer_get();

void d_timer_set(u8 n);
void s_timer_set(u8 n);

void s_timer_tick();
void d_timer_tick();

#endif
