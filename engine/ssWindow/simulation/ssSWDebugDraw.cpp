//
// Created by zitri on 07.11.2024.
//

#include "ssSWDebugDraw.h"

#include <iostream>
#include <vector>
#include <cassert>


static SDL_Color ToSDLColor(const b2HexColor& color);

static b2Vec2 MeterToPixel(b2Vec2 rVector, const ssSWDebugDraw* self);

SDL_FColor ToSDLFColor(const SDL_Color& color)
{
    return {
        (float) color.r / 255,
        (float) color.g / 255,
        (float) color.b / 255,
        (float) color.a / 255
    };
}

void ssSWDebugDraw::DrawSolidPolygon(b2Transform transform, const b2Vec2* pVertices, int VertexCount,
                                                      float radius,
                                                      b2HexColor rColor, void* context)
{
    auto* self = static_cast<ssSWDebugDraw*>(context);
    assert(self != nullptr);

    const SDL_Color LineColor     = ToSDLColor(rColor);
    SDL_FColor      TriangleColor = ToSDLFColor(ToSDLColor(rColor));
    TriangleColor.a = 128;

    std::vector<SDL_Vertex> vecVertices;
    for (int32_t            i = 0; i < VertexCount; ++i)
    {
        if (i < (VertexCount - 2))
        {
            const b2Vec2 TriangleVertex[3] = {
                MeterToPixel(b2Add(pVertices[0 + 0], transform.p), self),
                MeterToPixel(b2Add(pVertices[i + 1], transform.p), self),
                MeterToPixel(b2Add(pVertices[i + 2], transform.p), self),
            };

            for (auto j: TriangleVertex)
            {
                SDL_Vertex vertex = {{j.x, j.y}, TriangleColor, {0.0f, 0.0f}};
                vecVertices.push_back(vertex);
            }
        }
    }

    auto renderer = self->getRenderer();
    assert(renderer != nullptr);


    if (!SDL_RenderGeometry(renderer, nullptr, vecVertices.data(), (int32_t) vecVertices.size(), nullptr, 0))
    {
        std::cerr << "SDL_RenderGeometry failed: " << SDL_GetError() << std::endl;
        SDL_ClearError();
    }

    if (!SDL_SetRenderDrawColor(renderer, LineColor.r, LineColor.g, LineColor.b, LineColor.a))
    {
        std::cerr << "SDL_SetRenderDrawColor failed: " << SDL_GetError() << std::endl;
        SDL_ClearError();
    }

    for (int32_t i = 0; i < VertexCount; ++i)
    {
        const b2Vec2 LineStart = MeterToPixel(b2Add(pVertices[i], transform.p), self);
        const b2Vec2 LineEnd   = MeterToPixel(b2Add(pVertices[(i < (VertexCount - 1)) ? (i + 1) : 0], transform.p), self);

        if (!SDL_RenderLine(renderer, LineStart.x, LineStart.y, LineEnd.x, LineEnd.y))
        {
            std::cerr << "SDL_RenderLine failed: " << SDL_GetError() << std::endl;
            SDL_ClearError();
        }
    }
}

static SDL_Color ToSDLColor(const b2HexColor& color)
{
    SDL_Color sdlColor;
    sdlColor.r = (color >> 16) & 0xFF;
    sdlColor.g = (color >> 8) & 0xFF;
    sdlColor.b = color & 0xFF;
    sdlColor.a = 255;
    return sdlColor;
}

static float MeterToPixel(const float Value, const ssSWDebugDraw* self)
{
    return (Value * self->getMeterPerPixelFactor());
}

static b2Vec2 MeterToPixel(const b2Vec2 rVector, const ssSWDebugDraw* self)
{
    return b2Vec2(MeterToPixel(rVector.x, self), MeterToPixel(rVector.y, self));
}

SDL_Renderer* ssSWDebugDraw::getRenderer() const
{
    return m_pRenderer;
}

float ssSWDebugDraw::getMeterPerPixelFactor() const
{
    return m_MetersPerPixelFactor;
}
