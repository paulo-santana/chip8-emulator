#include "chip8.hpp"
#include <fstream>
#include <iostream>
#include <ostream>

void chip8::load_rom(const std::string &rom_file_path) {
  std::cout << "Loading ROM from: " << rom_file_path << std::endl;

  // file size
  std::ifstream file(rom_file_path, std::ios::binary | std::ios::ate);
  if (!file.is_open()) {
    std::cerr << "Error: Could not open ROM file: " << rom_file_path
              << std::endl;
    exit(EXIT_FAILURE);
  }

  std::streamsize size = file.tellg();

  if (size > chip8::MAX_ROM_SIZE) {
    std::cerr << "Error: ROM file is too large to fit in memory." << std::endl;
    exit(EXIT_FAILURE);
  }

  std::cout << "ROM file size: " << size << " bytes" << std::endl;

  file.seekg(0, std::ios::beg);
  this->memory.fill(0); // Clear memory before loading ROM
  file.read(reinterpret_cast<char *>(this->memory.data() + Offsets::GAME_SPACE),
            size);

  std::cout << "ROM loaded successfully into memory." << std::endl;
  file.close();
}
