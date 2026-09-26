#include <iostream>

#include "chip8.hpp"

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

int main(int argc, char **argv) {
  check_args(argc, argv);

  chip8 emulator;
  emulator.initialize();

  std::cout << "Hello, " << chip8::DISPLAY_WIDTH << "x" << chip8::DISPLAY_HEIGHT
            << " display Emulator!" << std::endl;

  auto rom_file_path = std::string(argv[1]);

  std::cout << "Memory size: " << chip8::MEMORY_SIZE << " bytes" << std::endl;
  std::cout << "Arguments: " << argc << std::endl;

  for (int i = 0; i < argc; ++i) {
    std::cout << "Argument " << i << ": " << argv[i] << std::endl;
  }

  emulator.load_rom(rom_file_path);

  std::cin.get(); // Wait for user input before running

  emulator.run();

  return 0;
}
