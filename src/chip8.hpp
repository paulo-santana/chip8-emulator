#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace Offsets {
static const std::uint16_t SYSTEM = 0x000;
static const std::uint16_t GAME_SPACE = 0x200;
static const std::uint16_t INTERNAL = 0xEA0;
static const std::uint16_t DISPLAY_REFRESH = 0xF00;
} // namespace Offsets

class chip8 {
public:
  static const size_t DISPLAY_WIDTH = 64;
  static const size_t DISPLAY_HEIGHT = 32;

  static const size_t MEMORY_SIZE = 0x1000;

private:
  std::array<std::uint8_t, MEMORY_SIZE> memory{};
  std::array<std::uint8_t, 16> registers{};

  std::uint16_t address_register{};
  std::uint16_t program_counter = Offsets::GAME_SPACE;

  std::array<std::uint8_t, DISPLAY_WIDTH * DISPLAY_HEIGHT> display{};
};
