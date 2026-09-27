#include "chip8.hpp"
#include <cstdint>
#include <iostream>

void opcode_00E0(Chip8 &chip8, uint16_t opcode) {
  (void)chip8;
  (void)opcode;
  std::cerr << "[opcode] display cleared" << std::endl;
}

void opcode_00EE(Chip8 &chip8, uint16_t opcode) {
  (void)chip8;
  (void)opcode;
  std::cerr << "[opcode] return not implemented yet" << std::endl;
}

void opcode_0NNN(Chip8 &chip8, uint16_t opcode) {
  (void)chip8;
  (void)opcode;
  std::cerr << "[opcode] 0NNN machine code" << std::endl;
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

void opcode_ANNN(Chip8 &chip8, uint16_t opcode) {
  auto addr = opcode & 0x0FFF;
  chip8.set_address_i(addr);
}

void opcode_DXYN(Chip8 &chip8, uint16_t opcode) {
  auto x = (opcode & 0x0F00) >> 8;
  auto y = (opcode & 0xF0) >> 4;
  auto n = (opcode & 0xF);

  chip8.draw(x, y, n);
}
