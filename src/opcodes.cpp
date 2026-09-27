#include "chip8.hpp"
#include <iostream>

void opcode_00E0(Chip8 &chip8, uint16_t opcode) {
  (void)chip8;
  (void)opcode;
  std::cerr << "clear display not implemented yet" << std::endl;
}

void opcode_00EE(Chip8 &chip8, uint16_t opcode) {
  (void)chip8;
  (void)opcode;
  std::cerr << "return not implemented yet" << std::endl;
}

void opcode_1NNN(Chip8 &chip8, uint16_t opcode) {
  chip8.set_program_counter(opcode & 0x0FFF);
}

void opcode_6XNN(Chip8 &chip8, uint16_t opcode) {
  auto reg = (opcode & 0x0F00) >> 8;
  auto value = opcode & 0xFF;
  chip8.set_register_value(reg, value);
}

void opcode_7XNN(Chip8 &chip8, uint16_t opcode) {
  auto reg = (opcode & 0x0F00) >> 8;
  auto value = opcode & 0xFF;
  auto curr = chip8.get_register_value(reg);
  chip8.set_register_value(reg, value + curr);
}
