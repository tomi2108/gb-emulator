#include "../include/emu.h"
#include <commons/log.h>
#include <pthread.h>
#include <unistd.h>

#define FRAMERATE 120

log_t emu_logger = {.file = "log.log",
                    .process = "EMU",
                    .level = LOG_LEVEL_DEBUG,
                    .is_active_console = 1};

void *input_handler() {
  keypad_init();
  while (1) {
    keypad_handle_input();
    usleep(1000 * 1000 * 1 / FRAMERATE);
  }
  keypad_free();
}

int main(const int argc, const char *argv[]) {
  emu_logger.logger =
      log_create(emu_logger.file, emu_logger.process,
                 emu_logger.is_active_console, emu_logger.level);

  if (argc < 2)
    exit_not_enough_arguments_error(emu_logger.logger);

  const char *rom_path = argv[1];

  memory_init();
  cartdrige_load(rom_path);

  cpu_init();
  screen_init();

  pthread_t input_handle_thread;
  pthread_create(&input_handle_thread, NULL, &input_handler, NULL);

  while (1) {
    cpu_exec();
    screen_draw();

    usleep(1000 * 1000 * 1 / FRAMERATE);
  }

  pthread_join(input_handle_thread, NULL);
  screen_free();
  cpu_free();
  memory_free();

  return EXIT_SUCCESS;
}
