#include "../include/memory.h"
#include <commons/log.h>
#include <string.h>

log_t memory_logger = {.file = "log.log",
                       .process = "MEMORY",
                       .level = LOG_LEVEL_DEBUG,
                       .is_active_console = 1};

// TODO: use 0xFFFF when MBCs are implemented
u8 ram[0xFFFFF] = {0};

void ram_write(const u8 *stream, u32 length, u16 offset) {
  memcpy(&ram[offset], stream, length);
}

void ram_write8(u16 addr, u8 val) { ram_write(&val, 1, addr); }

void ram_write16(u16 addr, u16 val) {
  ram_write8(addr, val & 0xFF);     // low byte
  ram_write8(addr + 1, (val >> 8)); // high byte
}

u8 ram_read8(u16 offset) { return ram[offset]; }

u16 ram_read16(u16 addr) {
  u8 low = ram_read8(addr);
  u8 high = ram_read8(addr + 1);
  return ((u16)high << 8) | low;
}

void memory_init() {
  memory_logger.logger =
      log_create(memory_logger.file, memory_logger.process,
                 memory_logger.is_active_console, memory_logger.level);
}

void memory_free() {
  log_destroy(memory_logger.logger);
  memset(ram, 0, 0xFFFFF);
}
