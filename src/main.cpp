#include <iostream>

#include "emulator.hpp"

void check_args(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <rom_file>" << std::endl;
    exit(EXIT_FAILURE);
  }

  if (std::string(argv[1]).empty()) {
    std::cerr << "Error: ROM file path is empty." << std::endl;
    exit(EXIT_FAILURE);
  }
}

void run(Emulator &emulator, const std::string &rom_path) {
  emulator.initialize();

  emulator.load_rom(rom_path);

  emulator.run();
}

int main(int argc, char **argv) {
  check_args(argc, argv);

  auto rom_file_path = std::string(argv[1]);

  auto emulator = create_emulator();

  run(*emulator, rom_file_path);

  return 0;
}
