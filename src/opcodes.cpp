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
  chip8.pop_stack();
}

void opcode_0NNN(Chip8 &chip8, uint16_t opcode) {
  (void)chip8;
  (void)opcode;
  std::cerr << "[opcode] 0NNN machine code" << std::endl;
}

void opcode_1NNN(Chip8 &chip8, uint16_t opcode) {
  chip8.set_program_counter(opcode & 0x0FFF);
}

void opcode_2NNN(Chip8 &chip8, uint16_t opcode) {
  chip8.push_stack();
  chip8.set_program_counter(opcode & 0x0FFF);
}

void opcode_3XNN(Chip8 &chip8, uint16_t opcode) {
  uint8_t reg = (opcode & 0x0F00) >> 8;
  uint8_t value = opcode & 0xFF;

  std::cerr << "[opcode] running opcode_3XNN" << std::endl;

  if (chip8.get_register_value(reg) == value) {
    chip8.set_skip();
  }
}

void opcode_4XNN(Chip8 &chip8, uint16_t opcode) {
  uint8_t reg = (opcode & 0x0F00) >> 8;
  uint8_t value = opcode & 0xFF;

  std::cerr << "[opcode] running opcode_4XNN" << std::endl;

  if (chip8.get_register_value(reg) != value) {
    chip8.set_skip();
  }
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

void opcode_EXA1(Chip8 &chip8, uint16_t opcode) {
  auto reg = (opcode & 0x0F00) >> 8;

  auto key = chip8.get_register_value(reg) & 0x0F;

  if (!chip8.is_key_pressed(key)) {
    chip8.set_skip();
  }
}

void opcode_FX1E(Chip8 &chip8, uint16_t opcode) {
  auto reg = (opcode & 0x0F00) >> 8;

  auto value = chip8.get_register_value(reg) & 0x0F;

  auto addr = chip8.get_address_i();
  chip8.set_address_i(addr + value);
}

// void opcode_FX33(Chip8 &chip8, uint16_t opcode) {
//   uint8_t reg = opcode & 0x0F00 >> 8;
//   uint8_t value = chip8.get_register_value(reg);
//   chip8.set_bcd(value);
// }
