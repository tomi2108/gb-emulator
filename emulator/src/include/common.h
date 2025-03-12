#ifndef COMMON_H_
#define COMMON_H_

#include <commons/log.h>
#include <commons/sys/exit.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef u_int8_t u8;
typedef u_int16_t u16;
typedef u_int32_t u32;

typedef struct {
  t_log *logger;
  bool is_active_console;
  char *file;
  char *process;
  t_log_level level;
} log_t;

void *safe_malloc(t_log *logger, size_t size);

FILE *safe_fopen(t_log *logger, const char *filename, const char *mode);

#endif
