#include <iostream>

// #include <defines.h>

#include <SDL3/SDL.h>

int main() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
    return 1;
  }

  SDL_Window *window = SDL_CreateWindow(
      "Hello World!",
      640,
      480,
      SDL_WINDOW_FULLSCREEN
  );

  if (window == nullptr) {
    std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return 1;
  }

  SDL_Delay(3000);

  SDL_Quit();

  return 0;
}
