#include "../include/memory.h"
#include <commons/log.h>
#include <string.h>

t_list *stack;
log_t memory_logger = {.file = "log.log",
                       .process = "MEMORY",
                       .level = LOG_LEVEL_DEBUG,
                       .is_active_console = 1};

// TODO: use 0xFFFF when MBCs are implemented
u8 ram[0xFFFFF] = {0};

void ram_write(const u8 *stream, u32 length, u16 offset) {
  memcpy(&ram[offset], stream, length);
}

u8* ram_read(u16 offset) { 
  return &ram[offset]; 
}

void stack_push(u16 bytes) {
  u16 *entry = safe_malloc(NULL, sizeof(u16));
  *entry = bytes;
  list_add_in_index(stack, list_size(stack) - 1, entry);
}

u16 *stack_pop() { return list_get(stack, list_size(stack) - 1); }

void memory_init() {
  memory_logger.logger =
      log_create(memory_logger.file, memory_logger.process,
                 memory_logger.is_active_console, memory_logger.level);
  stack = list_create();
}

void memory_free() {
  log_destroy(memory_logger.logger);
  memset(ram, 0, 0xFFFFF);
  list_destroy(stack);
}
