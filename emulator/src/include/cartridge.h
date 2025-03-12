#ifndef CART_H_
#define CART_H_

#include "common.h"
#include "memory.h"
#include <commons/buffer.h>

typedef struct {
  u8 entry[4];
  u8 logo[0x30];
  u8 title[16];
  u16 new_licensee_code;
  u8 sgb_flag;
  u8 cartdrige_type;
  u8 rom_size;
  u8 ram_size;
  u8 destination_code;
  u8 old_licensee_code;
  u8 rom_version;
  u8 checksum;
  u16 global_checksum;
} cartdrige_header;

cartdrige_header *get_cartdrige_header();

void cartdrige_load(const char *filename);

#endif
