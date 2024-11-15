//
// Created by zitri on 15.11.2024.
//

#include <SDL3/SDL.h>
#include <iostream>

bool ssSDLDrawCircle(SDL_Renderer* renderer, int32_t centreX, int32_t centreY, int32_t radius)
{
    const int32_t diameter = (radius * 2);

    int32_t x     = (radius - 1);
    int32_t y     = 0;
    int32_t tx    = 1;
    int32_t ty    = 1;
    int32_t error = (tx - diameter);

    while (x >= y)
    {
        bool success = true;

        //  Each of the following renders an octant of the circle
        success &= SDL_RenderPoint(renderer, centreX + x, centreY - y);
        success &= SDL_RenderPoint(renderer, centreX + x, centreY + y);
        success &= SDL_RenderPoint(renderer, centreX - x, centreY - y);
        success &= SDL_RenderPoint(renderer, centreX - x, centreY + y);
        success &= SDL_RenderPoint(renderer, centreX + y, centreY - x);
        success &= SDL_RenderPoint(renderer, centreX + y, centreY + x);
        success &= SDL_RenderPoint(renderer, centreX - y, centreY - x);
        success &= SDL_RenderPoint(renderer, centreX - y, centreY + x);

        if (!success)
        {
            std::cerr << "SDL_RenderPoint failed: " << SDL_GetError() << std::endl;
            SDL_ClearError();
            return false;
        }

        if (error <= 0)
        {
            ++y;
            error += ty;
            ty += 2;
        }

        if (error > 0)
        {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }

    return true;
}