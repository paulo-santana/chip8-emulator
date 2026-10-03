#pragma once

#include "chip8key.hpp"
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_video.h>
#include <array>
#include <cstdint>
#include <map>

class Window {
public:
  constexpr static const char *WINDOW_TITLE = "Krieg's Chip8 Emulator";
  static const int FRAMEBUFFER_WIDTH = 256;
  static const int FRAMEBUFFER_HEIGHT = 128;

  Window();

  ~Window();

  /// needs to be INITIAL_WIDTH * INITIAL_HEIGHT in size
  void render(uint32_t *buffer);

  bool update();

  bool is_key_pressed(Chip8Key key) const;

private:
  SDL_Window *sdlWindow;
  SDL_Renderer *sdlRenderer;
  SDL_Texture *sdlTexture;

  const bool *keyboardState;
  int keyboardKeys;

  std::map<Chip8Key, SDL_Scancode> keymap{
      {KEY_0, SDL_SCANCODE_0}, {KEY_1, SDL_SCANCODE_1}, {KEY_2, SDL_SCANCODE_2},
      {KEY_3, SDL_SCANCODE_3}, {KEY_4, SDL_SCANCODE_4}, {KEY_5, SDL_SCANCODE_5},
      {KEY_6, SDL_SCANCODE_6}, {KEY_7, SDL_SCANCODE_7}, {KEY_8, SDL_SCANCODE_8},
      {KEY_9, SDL_SCANCODE_9}, {KEY_A, SDL_SCANCODE_A}, {KEY_B, SDL_SCANCODE_B},
      {KEY_C, SDL_SCANCODE_C}, {KEY_D, SDL_SCANCODE_D}, {KEY_E, SDL_SCANCODE_E},
      {KEY_F, SDL_SCANCODE_F},
  };

  std::array<int, FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT> frameBuffer;
};
