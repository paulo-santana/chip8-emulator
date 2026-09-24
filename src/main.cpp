#include <iostream>

#include "chip8.hpp"

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;
  chip8 emulator;
  std::cout << "Hello, " << chip8::DISPLAY_WIDTH << "x" << chip8::DISPLAY_HEIGHT
            << " display Emulator!" << std::endl;
  return 0;
}
