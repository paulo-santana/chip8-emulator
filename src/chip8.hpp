#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>

#include "chip8key.hpp"
#include "emulator.hpp"
#include "window.hpp"

namespace Offsets {
static const std::uint16_t SYSTEM = 0x000;
static const std::uint16_t GAME_SPACE = 0x200;
static const std::uint16_t INTERNAL = 0xEA0;
static const std::uint16_t PC = INTERNAL;
static const std::uint16_t REGISTERS = PC + 2;
static const std::uint16_t I = REGISTERS + 16;
static const std::uint16_t STACK_COUNTER = I + 2;
static const std::uint16_t STACK = STACK_COUNTER + 1;
static const std::uint16_t DISPLAY_BUFFER = 0xF00;
static const std::uint16_t STACK_SIZE = DISPLAY_BUFFER - STACK;
} // namespace Offsets

class Chip8 : public Emulator {
public:
  static const size_t DISPLAY_WIDTH = 64;
  static const size_t DISPLAY_HEIGHT = 32;

  static const size_t MEMORY_SIZE = 0x1000;

  static const size_t MAX_ROM_SIZE = Offsets::INTERNAL - Offsets::GAME_SPACE;

  virtual void initialize() override;

  virtual void load_rom(const std::string &rom_file_path) override;

  virtual void run() override;

  virtual void print_display() const override;

  void set_program_counter(uint16_t new_pc);

  uint8_t get_register_value(uint8_t reg);
  void set_register_value(uint8_t reg, uint8_t value);

  void set_address_i(uint16_t addr);
  uint16_t get_address_i();

  void set_skip();

  void set_bcd(uint8_t value);

  void push_stack();
  void pop_stack();

  void clear_display();
  void draw(uint8_t x, uint8_t y, uint8_t n);

  bool is_key_pressed(int key) const;

private:
  Window window;

  void render();
  bool program_finished = false;

  std::array<std::uint8_t, MEMORY_SIZE> memory{};

  std::uint8_t initialized = 0;

  bool skip = false;

  uint16_t get_word_at(std::uint16_t pos) const;
  void set_word_at(std::uint16_t pos, std::uint16_t word);

  uint16_t get_next_opcode();
  void increase_program_counter();

  uint16_t get_program_counter() const;

  void execute_opcode(uint16_t opcode);
};
