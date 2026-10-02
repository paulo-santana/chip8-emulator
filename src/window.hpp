#pragma once

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <array>
#include <cstdint>

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

private:
  SDL_Window *sdlWindow;
  SDL_Renderer *sdlRenderer;
  SDL_Texture *sdlTexture;

  std::array<int, FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT> frameBuffer;
};
