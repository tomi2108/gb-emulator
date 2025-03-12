#include "../include/common.h"

void *safe_malloc(t_log *logger, size_t size) {
  void *p = malloc(size);
  if (p == NULL)
    exit_out_of_memory(logger);
  memset(p, 0, size);
  return p;
}

FILE *safe_fopen(t_log *logger, const char *filename, const char *mode) {
  FILE *file = fopen(filename, mode);
  if (file == NULL)
    exit_out_of_memory(NULL);

  return file;
}
