#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>

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

  static const size_t MAX_ROM_SIZE = Offsets::INTERNAL - Offsets::GAME_SPACE;

  void load_rom(const std::string &rom_file_path);

private:
  std::array<std::uint8_t, MEMORY_SIZE> memory{};
};
