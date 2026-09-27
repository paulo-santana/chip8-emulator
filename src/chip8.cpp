#include "chip8.hpp"
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <ostream>
#include <sys/types.h>

#include "opcodes.hpp"

std::unique_ptr<Emulator> create_emulator() {
  return std::make_unique<Chip8>();
}

void Chip8::initialize() {
  this->memory.at(Offsets::PC) = Offsets::GAME_SPACE >> 8;
  this->memory.at(Offsets::PC + 1) = Offsets::GAME_SPACE & 0xFF;
  this->initialized = 1;
}

void Chip8::load_rom(const std::string &rom_file_path) {
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

  if (static_cast<uint16_t>(size) > Chip8::MAX_ROM_SIZE) {
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

uint16_t Chip8::get_word_at(uint16_t pos) const {
  if (static_cast<size_t>(pos) + 1 >= this->memory.size()) {
    std::cerr << "Error: Attempt to read at invalid memory position: 0x"
              << std::hex << pos << std::dec << std::endl;
    exit(EXIT_FAILURE);
  }
  return this->memory.at(pos) << 8 | this->memory.at(pos + 1);
}

void Chip8::set_word_at(uint16_t pos, uint16_t word) {
  if (static_cast<size_t>(pos) + 1 >= this->memory.size()) {
    std::cerr << "Error: Attempt to write at invalid memory position: 0x"
              << std::hex << pos << std::dec << std::endl;
    exit(EXIT_FAILURE);
  }
  this->memory.at(pos) = (word >> 8) & 0xFF;
  this->memory.at(pos + 1) = word & 0xFF;
}

void Chip8::increase_program_counter() {
  this->memory.at(Offsets::PC + 1) += 2;
}

uint16_t Chip8::get_next_opcode() {
  auto pc = this->get_program_counter();
  auto opcode = this->get_word_at(pc);
  this->increase_program_counter();
  return opcode;
}

void Chip8::run() {
  std::cout << "Starting the emulator..." << std::endl;
  std::cout << "Emulator is running." << std::endl;

  auto opcode = this->get_next_opcode();
  this->execute_opcode(opcode);
}

uint16_t Chip8::get_program_counter() const { return get_word_at(Offsets::PC); }

void Chip8::execute_opcode(uint16_t opcode) {
  switch (opcode & 0xF000) {

  case 0x0000:
    switch (opcode & 0x000F) {
    case 0x0000:
      opcode_00E0(*this, opcode);
      break;
    case 0x000E:
      opcode_00EE(*this, opcode);
      break;
    }
    break;

  case 0x1000:
    opcode_1NNN(*this, opcode);
    break;
  case 0x6000:
    opcode_6XNN(*this, opcode);
    break;
  case 0x7000:
    opcode_7XNN(*this, opcode);
    break;
  default:
    std::cerr << "Error: Unrecognized opcode: 0x" << std::hex << opcode
              << std::dec << std::endl;
    exit(EXIT_FAILURE);
  }
}

void Chip8::set_program_counter(uint16_t new_pc) {
  std::cerr << "[debug] setting program counter to 0x" << std::hex << new_pc
            << std::endl;
  this->set_word_at(Offsets::PC, new_pc);
}

void Chip8::set_register_value(uint8_t reg, uint8_t value) {
  std::cerr << "[debug] setting register 0x0" << std::hex << (int)reg
            << " to 0x" << std::hex << (int)value << std::endl;
  this->memory.at(Offsets::REGISTERS + reg) = value;
}

uint8_t Chip8::get_register_value(uint8_t reg) {
  std::cerr << "[debug] getting register 0x0" << (int)reg << " value"
            << std::endl;
  return this->memory.at(Offsets::REGISTERS + reg);
}
