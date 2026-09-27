#include "chip8.hpp"
#include <cstdint>
#include <cstdlib>
#include <format>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <ostream>
#include <sys/types.h>

#include "opcodes.hpp"

std::unique_ptr<Emulator> create_emulator() {
  return std::make_unique<Chip8>();
}

void Chip8::initialize() {
  this->memory.fill(0); // Clear memory before loading ROM
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
  std::cout << "step mode: press a key to continue" << std::endl;

  while (true) {
    // while (std::cin.get()) {
    auto pc = this->get_program_counter();

    auto opcode = this->get_next_opcode();
    std::cerr << std::format("[debug] PC: 0x{:04x} - got opcode 0x{:04X}", pc,
                             opcode)
              << std::endl;
    this->execute_opcode(opcode);
    this->print_display();
  }
}

uint16_t Chip8::get_program_counter() const { return get_word_at(Offsets::PC); }

void Chip8::execute_opcode(uint16_t opcode) {
  switch (opcode & 0xF000) {

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
  case 0x6000:
    opcode_6XNN(*this, opcode);
    break;
  case 0x7000:
    opcode_7XNN(*this, opcode);
    break;
  case 0xA000:
    opcode_ANNN(*this, opcode);
    break;
  case 0xD000:
    opcode_DXYN(*this, opcode);
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
  std::cerr << std::format("[debug] setting I to 0x{:04X}", addr) << std::endl;
  this->memory.at(Offsets::I) = addr >> 8; // & 0xF;
  this->memory.at(Offsets::I + 1) = addr & 0xFF;
}

uint16_t Chip8::get_address_i() {
  uint16_t addr = ((uint16_t)this->memory.at(Offsets::I) << 8) | // & 0xF;
                  ((uint16_t)this->memory.at(Offsets::I + 1));
  std::cout << std::format("[debug] returning address inside I 0x{:04X}", addr)
            << std::endl;
  return addr;
}
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

void print_sprite(uint8_t *buffer, uint8_t height) {
  for (uint8_t i = 0; i < height; ++i) {
    std::cout << std::format("{:8b}", buffer[i]) << std::endl;
  }
}

void Chip8::draw(uint8_t x, uint8_t y, uint8_t n) {
  auto sprite_addr = this->get_address_i();
  auto posx = this->get_register_value(x);
  auto posy = this->get_register_value(y);

  std::cerr << std::format("[debug] drawing sprite in 0x{:04X} at position "
                           "{}x{} with height {}",
                           sprite_addr, posx, posy, n)
            << std::endl;

  // print_sprite(&this->memory.at(sprite_addr), n);

  for (uint8_t i = 0; i < n; ++i) {
    auto screen_byte = Offsets::DISPLAY_REFRESH + (posy + i) * 8 + posx / 8;
    auto next_screen_byte = screen_byte + 1;
    auto sprite_byte = sprite_addr + i;

    // TODO: set VF to 1 if pixels are turned off and to 0 otherwise
    this->memory.at(screen_byte) ^= this->memory.at(sprite_byte) >> (posx % 8);
    if (posx % 8 != 0)
      if (next_screen_byte >= 0x1000) {
        std::cerr << "Invalid next_screen_byte location: 0x" << std::hex
                  << next_screen_byte << std::endl;
        exit(EXIT_FAILURE);
      }
    this->memory.at(next_screen_byte) ^= this->memory.at(sprite_byte)
                                         << (8 - (posx % 8));
  }
  // this->print_display();
}

void Chip8::print_display() const {
  const uint8_t *display_buffer = &this->memory.at(Offsets::DISPLAY_REFRESH);

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
