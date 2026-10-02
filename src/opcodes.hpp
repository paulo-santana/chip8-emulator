#pragma once

#include "chip8.hpp"
#include <cstdint>

void opcode_00E0(Chip8 &chip8, uint16_t opcode);
void opcode_00EE(Chip8 &chip8, uint16_t opcode);
void opcode_0NNN(Chip8 &chip8, uint16_t opcode);

void opcode_1NNN(Chip8 &chip8, uint16_t opcode);
void opcode_2NNN(Chip8 &chip8, uint16_t opcode);
void opcode_3XNN(Chip8 &chip8, uint16_t opcode);
void opcode_6XNN(Chip8 &chip8, uint16_t opcode);
void opcode_7XNN(Chip8 &chip8, uint16_t opcode);

void opcode_ANNN(Chip8 &chip8, uint16_t opcode);
void opcode_DXYN(Chip8 &chip8, uint16_t opcode);
