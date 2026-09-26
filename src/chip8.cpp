#include "chip8.hpp"
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ostream>

void chip8::initialize() {
  this->memory.at(Offsets::PC) = Offsets::GAME_SPACE >> 8;
  this->memory.at(Offsets::PC + 1) = Offsets::GAME_SPACE & 0xFF;
  this->initialized = 1;
}

void chip8::load_rom(const std::string &rom_file_path) {
  if (this->initialized == 0) {
    std::cerr << "Emulator is not initialized" << std::endl;
    exit(EXIT_FAILURE);
  }
  std::cerr << "Emulator is initialized :)" << std::endl;

  std::cout << "Loading ROM from: " << rom_file_path << std::endl;

  // file size
  std::ifstream file(rom_file_path, std::ios::binary | std::ios::ate);
  if (!file.is_open()) {
    std::cerr << "Error: Could not open ROM file: " << rom_file_path
              << std::endl;
    exit(EXIT_FAILURE);
  }

  std::streamsize size = file.tellg();

  if (static_cast<uint16_t>(size) > chip8::MAX_ROM_SIZE) {
    std::cerr << "Error: ROM file is too large to fit in memory." << std::endl;
    exit(EXIT_FAILURE);
  }

  std::cout << "ROM file size: " << size << " bytes" << std::endl;

  file.seekg(0, std::ios::beg);
  // this->memory.fill(0); // Clear memory before loading ROM
  file.read(reinterpret_cast<char *>(this->memory.data() + Offsets::GAME_SPACE),
            size);

  std::cout << "ROM loaded successfully into memory." << std::endl;
  file.close();
}

uint16_t chip8::get_word_at(uint16_t pos) const {
  if (pos + 1 >= this->memory.size()) {
    std::cerr << "Error: Attempt to read at invalid memory position: 0x"
              << std::hex << pos << std::dec << std::endl;
    exit(EXIT_FAILURE);
  }
  return this->memory.at(pos) << 8 | this->memory.at(pos + 1);
}

uint16_t chip8::get_program_counter() const { return get_word_at(Offsets::PC); }
void chip8::increase_program_counter() {
  this->memory.at(Offsets::PC) += 2 >> 8;
}

uint16_t chip8::get_next_opcode() const {
  auto pc = this->get_program_counter();
  auto opcode = this->get_word_at(pc);
  return opcode;
}

void chip8::run() {
  std::cout << "Starting the emulator..." << std::endl;
  std::cout << "Emulator is running." << std::endl;

  auto opcode = this->get_next_opcode(); // Fetch the first opcode (placeholder)
  std::cout << "Fetched firs opcode ever: 0x" << std::hex << opcode << std::dec
            << std::endl;
}
