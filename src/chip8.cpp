#include "chip8.hpp"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <format>
#include <fstream>
#include <iostream>
#include <memory>
#include <ostream>
#include <sys/types.h>

#include "font.hpp"
#include "opcodes.hpp"
#include "window.hpp"

std::unique_ptr<Emulator> create_emulator() {
  return std::make_unique<Chip8>();
}

void Chip8::initialize() {
  this->memory.fill(0); // Clear memory before loading ROM
  load_font(this->memory.data() + Offsets::FONT);
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
  this->set_program_counter(pc + 2);
  // this->increase_program_counter();
  return opcode;
}

void Chip8::run() {
  std::cout << "Starting the emulator..." << std::endl;

  while (this->program_finished == false) {
    auto opcode = this->get_next_opcode();
    if (this->skip) {
      std::cerr << std::format("[debug] skipping opcode 0x{:04X}", opcode)
                << std::endl;
      this->skip = false;
    } else {
      // std::cerr << std::format("[debug] executing opcode 0x{:04X}", opcode)
      //           << std::endl;
      this->execute_opcode(opcode);
    }
    this->render();
    // std::cin.get();
  }
}

void Chip8::render() {
  const int32_t size = Window::FRAMEBUFFER_WIDTH * Window::FRAMEBUFFER_HEIGHT;
  std::array<uint32_t, size> buffer;

  for (int i = 0; i < size; i++) {
    auto line = i / Window::FRAMEBUFFER_WIDTH;
    auto column = i % Window::FRAMEBUFFER_WIDTH;
    auto dp_line = (line * Chip8::DISPLAY_HEIGHT) / Window::FRAMEBUFFER_HEIGHT;
    auto dp_column =
        (column * Chip8::DISPLAY_WIDTH) / Window::FRAMEBUFFER_WIDTH;
    auto pixel = dp_line * Chip8::DISPLAY_WIDTH + dp_column;
    auto byte = this->memory.at(Offsets::DISPLAY_BUFFER + pixel / 8);
    auto bit = (int)pixel % 8;
    auto bit_on = (byte >> (7 - bit)) & 1;
    buffer.at(i) = bit_on ? 0xFFFFFFFF : 0xFF000000;
  }

  this->window.render(buffer.data());

  if (!this->window.update()) {
    this->program_finished = true;
  }
}

uint16_t Chip8::get_program_counter() const { return get_word_at(Offsets::PC); }

void Chip8::execute_opcode(uint16_t opcode) {
  switch (opcode & 0xF000) {

  // TODO:
  // -0NNN - Calls RCA 1802 program at address NNN. Not necessary for most ROMs.
  // -5XY0 - Skips the next instruction if VX equals VY.
  // -8XY0 - Sets VX to the value of VY.
  // -8XY1 - Sets VX to VX or VY. (Bitwise OR)
  // -8XY2 - Sets VX to VX and VY. (Bitwise AND)
  // -8XY3 - Sets VX to VX xor VY.
  // -8XY4 - Adds VY to VX. VF is set to 1 if there's a carry, and to 0 if there
  // isn't.
  // -8XY5 - VY is subtracted from VX. VF is set to 0 if there's a
  // borrow, and 1 if there isn't.
  // -8XY6 - Stores the least significant bit of
  // VX in VF and then shifts VX to the right by 1.
  // -8XY7 - Sets VX to VY minus
  // VX. VF is set to 0 if there's a borrow, and 1 if there isn't.
  // -8XYE - Stores the most significant bit of VX in VF and then shifts VX to
  //  the left by 1.
  // -9XY0 - Skips the next instruction if VX doesn't equal VY.
  // -BNNN - Jumps to the address NNN plus V0.
  // -CXNN - Sets VX to a random number and NN.
  // -EX9E - Skips the next instruction if the key stored in VX is pressed.
  // -FX07 - Sets VX to the value of the delay timer.
  // -FX0A - A key press is awaited, and then stored in VX.
  // -FX15 - Sets the delay timer to VX.
  // -FX18 - Sets the sound timer to VX.
  // -FX1E - Adds VX to I. VF is not affected.
  // -FX29 - Sets I to the location of the sprite for the character in VX.
  // -FX33 - Stores the binary-coded decimal representation of VX at the
  //  addresses I, I plus 1, and I plus 2.
  // -FX55 - Stores V0 to VX in memory
  //  starting at address I.
  // -FX65 - Fills V0 to VX with values from memory starting at address I.
  case 0x0000:
    switch (opcode & 0x00FF) {
    case 0x00E0:
      opcode_00E0(*this, opcode);
      break;
    case 0x00EE:
      opcode_00EE(*this, opcode);
      break;
    default:
      std::cerr << std::format("[warn]: calling opcode_0NNN for: 0x{:04X}",
                               opcode)
                << std::endl;
      opcode_0NNN(*this, opcode);
      // exit(EXIT_FAILURE);
    }
    break;

  case 0x1000:
    opcode_1NNN(*this, opcode);
    break;
  case 0x2000:
    opcode_2NNN(*this, opcode);
    break;
  case 0x3000:
    opcode_3XNN(*this, opcode);
    break;
  case 0x4000:
    opcode_4XNN(*this, opcode);
    break;
  case 0x6000:
    opcode_6XNN(*this, opcode);
    break;
  case 0x7000:
    opcode_7XNN(*this, opcode);
    break;
  case 0xA000:
    opcode_ANNN(*this, opcode);
    break;
  case 0xC000:
    opcode_CXNN(*this, opcode);
    break;
  case 0xD000:
    opcode_DXYN(*this, opcode);
    break;
  case 0xE000:
    switch (opcode & 0x00FF) {
    case 0x00A1:
      opcode_EXA1(*this, opcode);
      break;
    }
    break;
  case 0xF000:
    switch (opcode & 0x00FF) {
    case 0x001E:
      opcode_FX1E(*this, opcode);
      break;
    case 0x0029:
      opcode_FX29(*this, opcode);
      break;
    default:
      std::cerr << std::format("[warn]: unimplemented 0xFXNN opcode: 0x{:04X}",
                               opcode)
                << std::endl;
    }
    break;
  default:
    std::cerr << std::format("Error: Unrecognized opcode: 0x{:04X}", opcode)
              << std::endl;
    exit(EXIT_FAILURE);
  }
}

void Chip8::set_program_counter(uint16_t new_pc) {
  // std::cerr << std::format("[debug] setting program counter to 0x{:04X}",
  //                          new_pc)
  //           << std::endl;
  this->set_word_at(Offsets::PC, new_pc);
}

void Chip8::set_address_i(uint16_t addr) {
  std::cerr << std::format("[debug] setting I to 0x{:04X}", addr & 0x0FFF)
            << std::endl;
  this->set_word_at(Offsets::I, addr & 0x0FFF);
}

uint16_t Chip8::get_address_i() {
  uint16_t addr = this->get_word_at(Offsets::I);
  std::cerr << std::format("[debug] returning address inside I 0x{:04X}", addr)
            << std::endl;
  return addr;
}

void Chip8::set_skip() {
  std::cerr << "[debug] enabling skip..." << std::endl;
  this->skip = true;
}

/**
set_BCD(Vx)
*(I+0) = BCD(3);
*(I+1) = BCD(2);
*(I+2) = BCD(1);
 */

// void Chip8::set_bcd(uint8_t value) {
//   uint16_t addr = this->get_address_i();
//   this->;
// }

void Chip8::set_register_value(uint8_t reg, uint8_t value) {
  std::cerr << std::format("[debug] setting register 0x{:02X} to 0x{:02X}", reg,
                           value)
            << std::endl;

  this->memory.at(Offsets::REGISTERS + reg) = value;
}

uint8_t Chip8::get_register_value(uint8_t reg) {
  auto value = this->memory.at(Offsets::REGISTERS + reg);
  std::cerr << std::format("[debug] getting register 0x{:02X} value 0x{:02X}",
                           reg, value)
            << std::endl;
  return value;
}

void Chip8::pop_stack() {
  uint8_t &stack_counter = this->memory.at(Offsets::STACK_COUNTER);

  this->get_word_at(Offsets::STACK + stack_counter);

  stack_counter -= 1;
  auto old_pc = this->get_word_at(Offsets::STACK + stack_counter * 2);
  std::cerr << std::format("[debug] retrieved 0x{:04X} back from stack #{}",
                           old_pc, stack_counter)
            << std::endl;
  this->set_program_counter(old_pc);
}

void Chip8::push_stack() {
  uint8_t &stack_counter = this->memory.at(Offsets::STACK_COUNTER);
  std::cerr << std::format("[debug] setting stack #{} to 0x{:04X}",
                           stack_counter, this->get_program_counter())
            << std::endl;

  if (stack_counter >= 12) {
    std::cerr << std::format("[error] ---- stack overflow ----\ntried to write "
                             "0x{:04X} to stack_counter #{}",
                             this->get_program_counter(), stack_counter)
              << std::endl;
    exit(EXIT_FAILURE);
  }

  this->set_word_at(Offsets::STACK + stack_counter * 2,
                    this->get_program_counter());
  stack_counter += 1;
}

void print_sprite(uint8_t *buffer, uint8_t height) {
  for (uint8_t i = 0; i < height; ++i) {
    std::cout << std::format("{:8b}", buffer[i]) << std::endl;
  }
}

void Chip8::clear_display() {
  memset(this->memory.data() + Offsets::DISPLAY_BUFFER, 0,
         Chip8::MEMORY_SIZE - Offsets::DISPLAY_BUFFER);
}

void Chip8::draw(uint8_t x, uint8_t y, uint8_t n) {
  auto sprite_addr = this->get_address_i();
  auto posx = this->get_register_value(x) % 64;
  auto posy = this->get_register_value(y) % 32;

  std::cerr << std::format("[debug] drawing sprite in 0x{:04X} at position "
                           "{}x{} with height {}",
                           sprite_addr, posx, posy, n)
            << std::endl;

  // print_sprite(&this->memory.at(sprite_addr), n);

  bool turned_off = false;

  for (uint8_t i = 0; i < n; ++i) {
    auto screen_byte = Offsets::DISPLAY_BUFFER + (posy + i) * 8 + posx / 8;
    auto next_screen_byte = screen_byte + 1;
    auto sprite_byte = sprite_addr + i;

    auto og = this->memory.at(screen_byte);
    auto mask = this->memory.at(sprite_byte);
    auto shift = (posx % 8);
    this->memory.at(screen_byte) = og ^ mask >> shift;

    if (og & mask >> shift) {
      turned_off = true;
    }

    if (shift != 0) {
      if (next_screen_byte >= 0x1000) {
        std::cerr << "Invalid next_screen_byte location: 0x" << std::hex
                  << next_screen_byte << std::endl;
        exit(EXIT_FAILURE);
      }
      auto nog = this->memory.at(next_screen_byte);
      this->memory.at(next_screen_byte) = nog ^ mask << (8 - shift);
      if (nog & mask << (8 - shift)) {
        turned_off = true;
      }
    }
  }

  if (turned_off) {
    this->set_register_value(0xF, 1);
  } else {
    this->set_register_value(0xF, 0);
  }
}

bool Chip8::is_key_pressed(int key) const {
  return this->window.is_key_pressed(static_cast<Chip8Key>(key));
}

uint16_t Chip8::get_character_address(uint8_t chr) const {
  auto addr = Offsets::FONT + chr * 5;
  std::cerr << std::format("[debug] returning character address for 0x{:02X} "
                           "as 0x{:04X}",
                           chr, addr)
            << std::endl;
  return addr;
}

void Chip8::print_display() const {
  const uint8_t *display_buffer = &this->memory.at(Offsets::DISPLAY_BUFFER);

  for (int y = 0; y < 32; ++y) {
    for (int x = 0; x < 64; ++x) {
      auto column = x / 8;
      auto line = y;
      auto byte = line * 8 + column;
      auto value = display_buffer[byte];
      auto bit = 7 - (x % 8);
      auto on = value & (1 << bit);
      if (on > 0)
        std::cout << "X";
      else
        std::cout << " ";
    }
    std::cout << std::endl;
  }
}
