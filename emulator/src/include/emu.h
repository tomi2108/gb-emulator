#ifndef EMU_H_
#define EMU_H_

#include "cartridge.h"
#include "ppu.h"
#include "memory.h"
#include "timer.h"
#include "cpu.h"
#include <commons/buffer.h>
#include <commons/collections/list.h>
#include <commons/config.h>
#include <commons/log.h>
#include <commons/sys/exit.h>
#include <commons/sys/file.h>
#include <stdint.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

typedef struct {
  bool running;
  int ticks;
} state;

#endif
