

#include <SDL3/SDL.h>
#include <iostream>


int main() {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
    return 1;
  }

  bool isFullscreen = true;
  auto window = SDL_CreateWindow("Hello, SDL3!", 800, 600,
                            SDL_WINDOW_FULLSCREEN | SDL_WINDOW_RESIZABLE);

  if (!window) {
    std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
    return 1;
  }

  auto renderer = SDL_CreateRenderer(window, "Hello");

  auto running = true;
  while (running) {
    // vsync
    auto start = SDL_GetTicks();
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
        auto end = SDL_GetTicks();
        auto elapsed = end - start;
        if (elapsed < 16) {
          SDL_Delay(16 - elapsed);
        }

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        running = false;
      } else if (event.key.scancode == SDL_SCANCODE_F11) {
        if (event.key.down) {
          isFullscreen = !isFullscreen;
          SDL_SetWindowFullscreen(window, isFullscreen);
        }
      }
    }
  }



  return 0;
}
