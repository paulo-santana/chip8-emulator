#pragma once

#include <memory>
#include <string>

class Emulator {
public:
  virtual void initialize() = 0;

  virtual void load_rom(const std::string &rom_file_path) = 0;

  virtual void run() = 0;

  virtual void print_display() const = 0;
};

std::unique_ptr<Emulator> create_emulator();
