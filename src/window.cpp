#include "window.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>
#include <cstdint>
#include <cstring>
#include <exception>
#include <format>
#include <iostream>

Window::Window() {
  this->sdlWindow =
      SDL_CreateWindow(WINDOW_TITLE, Window::FRAMEBUFFER_WIDTH,
                       Window::FRAMEBUFFER_HEIGHT, SDL_WINDOW_RESIZABLE);

  if (this->sdlWindow == nullptr) {
    std::cerr << "Failed to create SDL window: " << SDL_GetError() << std::endl;
    return;
  }

  this->sdlRenderer = SDL_CreateRenderer(this->sdlWindow, NULL);
  if (this->sdlRenderer == nullptr) {
    std::cerr << "Failed to create SDL Renderer: " << SDL_GetError()
              << std::endl;
    return;
  }

  this->sdlTexture = SDL_CreateTexture(
      this->sdlRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
      Window::FRAMEBUFFER_WIDTH, Window::FRAMEBUFFER_HEIGHT);

  if (this->sdlTexture == nullptr) {
    std::cerr << "Failed to create SDL Texture: " << SDL_GetError()
              << std::endl;
    return;
  }

  this->frameBuffer.fill(0);

  this->keyboardState = SDL_GetKeyboardState(&this->keyboardKeys);

  std::cerr << "window initialized" << std::endl;
}

Window::~Window() {
  SDL_DestroyTexture(this->sdlTexture);
  SDL_DestroyRenderer(this->sdlRenderer);
  SDL_DestroyWindow(this->sdlWindow);
  this->sdlWindow = nullptr;
}

void Window::render(uint32_t *buffer) {
  for (int i = 0, c = 0; i < FRAMEBUFFER_HEIGHT; ++i) {
    for (int j = 0; j < FRAMEBUFFER_WIDTH; ++j, ++c) {
      this->frameBuffer[c] = buffer[i * FRAMEBUFFER_WIDTH + j] | 0xFF000000;
    }
  }
}

bool Window::update() {

  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_EVENT_QUIT) {
      return false;
    }

    if (e.type == SDL_EVENT_KEY_UP && e.key.key == SDLK_ESCAPE) {
      return false;
    }
  }

  char *pix;
  int pitch;

  if (!SDL_LockTexture(this->sdlTexture, NULL, (void **)&pix, &pitch)) {
    std::cerr << "Failed to lock texture: " << SDL_GetError() << std::endl;
    return false;
  }

  for (int i = 0, sp = 0, dp = 0; i < FRAMEBUFFER_HEIGHT;
       i++, sp += pitch, dp += FRAMEBUFFER_WIDTH) {
    memcpy(pix + sp, this->frameBuffer.data() + dp, FRAMEBUFFER_WIDTH * 4);
  }

  SDL_UnlockTexture(this->sdlTexture);
  SDL_RenderTexture(this->sdlRenderer, this->sdlTexture, NULL, NULL);
  SDL_RenderPresent(this->sdlRenderer);
  SDL_Delay(1);

  return true;
}

bool Window::is_key_pressed(Chip8Key key) const {
  try {

    auto scancode = this->keymap.at(key);
    return this->keyboardState[scancode];
  } catch (std::exception &e) {
    std::cerr << std::format("Failed verifying key {} press: ", (int)key)
              << e.what() << std::endl;
  }

  return false;
}
