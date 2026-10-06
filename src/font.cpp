#include <cstdint>
#include <format>
#include <iostream>

void load_font(uint8_t *buffer) {
  // font data copied from
  // https://github.com/mattmikolay/chip-8/files/3365169/cosmacvipfont.txt

  // 0x0:
  buffer[0x0 * 5 + 0] = 0b11110000;
  buffer[0x0 * 5 + 1] = 0b10010000;
  buffer[0x0 * 5 + 2] = 0b10010000;
  buffer[0x0 * 5 + 3] = 0b10010000;
  buffer[0x0 * 5 + 4] = 0b11110000;
  // 0x1:
  buffer[0x1 * 5 + 0] = 0b01100000;
  buffer[0x1 * 5 + 1] = 0b00100000;
  buffer[0x1 * 5 + 2] = 0b00100000;
  buffer[0x1 * 5 + 3] = 0b00100000;
  buffer[0x1 * 5 + 4] = 0b01110000;
  // 0x2:
  buffer[0x2 * 5 + 0] = 0b11110000;
  buffer[0x2 * 5 + 1] = 0b00010000;
  buffer[0x2 * 5 + 2] = 0b11110000;
  buffer[0x2 * 5 + 3] = 0b10000000;
  buffer[0x2 * 5 + 4] = 0b11110000;
  // 0x3:
  buffer[0x3 * 5 + 0] = 0b11110000;
  buffer[0x3 * 5 + 1] = 0b00010000;
  buffer[0x3 * 5 + 2] = 0b11110000;
  buffer[0x3 * 5 + 3] = 0b00010000;
  buffer[0x3 * 5 + 4] = 0b11110000;
  // 0x4:
  buffer[0x4 * 5 + 0] = 0b10100000;
  buffer[0x4 * 5 + 1] = 0b10100000;
  buffer[0x4 * 5 + 2] = 0b11110000;
  buffer[0x4 * 5 + 3] = 0b00100000;
  buffer[0x4 * 5 + 4] = 0b00100000;
  // 0x5:
  buffer[0x5 * 5 + 0] = 0b11110000;
  buffer[0x5 * 5 + 1] = 0b10000000;
  buffer[0x5 * 5 + 2] = 0b11110000;
  buffer[0x5 * 5 + 3] = 0b00010000;
  buffer[0x5 * 5 + 4] = 0b11110000;
  // 0x6:
  buffer[0x6 * 5 + 0] = 0b11110000;
  buffer[0x6 * 5 + 1] = 0b10000000;
  buffer[0x6 * 5 + 2] = 0b11110000;
  buffer[0x6 * 5 + 3] = 0b10010000;
  buffer[0x6 * 5 + 4] = 0b11110000;
  // 0x7:
  buffer[0x7 * 5 + 0] = 0b11110000;
  buffer[0x7 * 5 + 1] = 0b00010000;
  buffer[0x7 * 5 + 2] = 0b00010000;
  buffer[0x7 * 5 + 3] = 0b00010000;
  buffer[0x7 * 5 + 4] = 0b00010000;
  // 0x8:
  buffer[0x8 * 5 + 0] = 0b11110000;
  buffer[0x8 * 5 + 1] = 0b10010000;
  buffer[0x8 * 5 + 2] = 0b11110000;
  buffer[0x8 * 5 + 3] = 0b10010000;
  buffer[0x8 * 5 + 4] = 0b11110000;
  // 0x9:
  buffer[0x9 * 5 + 0] = 0b11110000;
  buffer[0x9 * 5 + 1] = 0b10010000;
  buffer[0x9 * 5 + 2] = 0b11110000;
  buffer[0x9 * 5 + 3] = 0b00010000;
  buffer[0x9 * 5 + 4] = 0b11110000;
  // 0xA:
  buffer[0xA * 5 + 0] = 0b11110000;
  buffer[0xA * 5 + 1] = 0b10010000;
  buffer[0xA * 5 + 2] = 0b11110000;
  buffer[0xA * 5 + 3] = 0b10010000;
  buffer[0xA * 5 + 4] = 0b10010000;
  // 0xB:
  buffer[0xB * 5 + 0] = 0b11110000;
  buffer[0xB * 5 + 1] = 0b01010000;
  buffer[0xB * 5 + 2] = 0b01110000;
  buffer[0xB * 5 + 3] = 0b01010000;
  buffer[0xB * 5 + 4] = 0b11110000;
  // 0xC:
  buffer[0xC * 5 + 0] = 0b11110000;
  buffer[0xC * 5 + 1] = 0b10000000;
  buffer[0xC * 5 + 2] = 0b10000000;
  buffer[0xC * 5 + 3] = 0b10000000;
  buffer[0xC * 5 + 4] = 0b11110000;
  // 0xD:
  buffer[0xD * 5 + 0] = 0b11110000;
  buffer[0xD * 5 + 1] = 0b01010000;
  buffer[0xD * 5 + 2] = 0b01010000;
  buffer[0xD * 5 + 3] = 0b01010000;
  buffer[0xD * 5 + 4] = 0b11110000;
  // 0xE:
  buffer[0xE * 5 + 0] = 0b11110000;
  buffer[0xE * 5 + 1] = 0b10000000;
  buffer[0xE * 5 + 2] = 0b11110000;
  buffer[0xE * 5 + 3] = 0b10000000;
  buffer[0xE * 5 + 4] = 0b11110000;
  // 0xF:
  buffer[0xF * 5 + 0] = 0b11110000;
  buffer[0xF * 5 + 1] = 0b10000000;
  buffer[0xF * 5 + 2] = 0b11110000;
  buffer[0xF * 5 + 3] = 0b10000000;
  buffer[0xF * 5 + 4] = 0b10000000;

  std::cerr << std::format("[debug] font loaded into memory into addresses "
                           "0x{:04X} to 0x{:04X}",
                           0x0, 0xF * 5 + 4)
            << std::endl;
}
