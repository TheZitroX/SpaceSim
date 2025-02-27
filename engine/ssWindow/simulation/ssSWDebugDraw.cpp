//
// Created by zitri on 07.11.2024.
//

#include <iostream>
#include <vector>
#include <cassert>

#include "ssSWDebugDraw.h"
#include "rendering/ssSDLUtils.cpp"


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
    TriangleColor.a = 0.5f;

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

    // rotate around the center
    // calc center of SDL_Vertices
    b2Vec2 center = {0, 0};
    for (auto& vertex: vecVertices)
    {
        center.x += vertex.position.x;
        center.y += vertex.position.y;
    }
    center.x /= vecVertices.size();
    center.y /= vecVertices.size();

    const auto angle = transform.q;
    for (auto& vertex: vecVertices)
    {
        const float x = vertex.position.x - center.x;
        const float y = vertex.position.y - center.y;
        vertex.position.x = center.x + x * angle.c - y * angle.s;
        vertex.position.y = center.y + x * angle.s + y * angle.c;
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

    // todo fix angle
    //for (int32_t i = 0; i < VertexCount; ++i)
    //{
    //    const b2Vec2 LineStart = MeterToPixel(b2Add(pVertices[i], transform.p), self);
    //    const b2Vec2 LineEnd   = MeterToPixel(b2Add(pVertices[(i < (VertexCount - 1)) ? (i + 1) : 0], transform.p),
    //                                          self);
    //
    //    if (!SDL_RenderLine(renderer, LineStart.x, LineStart.y, LineEnd.x, LineEnd.y))
    //    {
    //        std::cerr << "SDL_RenderLine failed: " << SDL_GetError() << std::endl;
    //        SDL_ClearError();
    //    }
    //}
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

void ssSWDebugDraw::DrawSolidCircle(b2Transform transform, float radius, b2HexColor color, void* context)
{
    auto* self = static_cast<ssSWDebugDraw*>(context);
    assert(self != nullptr);

    const SDL_Color sdlColor = ToSDLColor(color);

    const b2Vec2 pixelCenter = MeterToPixel(transform.p, self);
    const auto   pixelRadius = static_cast<int32_t>(std::round(MeterToPixel(radius, self)));

    auto renderer = self->getRenderer();
    assert(renderer != nullptr);

    if (!SDL_SetRenderDrawColor(renderer, sdlColor.r, sdlColor.g, sdlColor.b, sdlColor.a))
    {
        std::cerr << "SDL_SetRenderDrawColor failed: " << SDL_GetError() << std::endl;
        SDL_ClearError();
    }

    if (!ssSDLDrawCircle(renderer, static_cast<int32_t>(std::round(pixelCenter.x)),
                         static_cast<int32_t>(std::round(pixelCenter.y)), pixelRadius))
    {
        std::cerr << "ssSDLDrawCircle failed: " << std::endl;
        return;
    }
}

void ssSWDebugDraw::DrawCircle(b2Vec2 center, float radius, b2HexColor color, void* context)
{
    auto* self = static_cast<ssSWDebugDraw*>(context);
    assert(self != nullptr);

    const SDL_Color sdlColor = ToSDLColor(color);

    const b2Vec2 pixelCenter = MeterToPixel(center, self);
    const auto   pixelRadius = static_cast<int32_t>(std::round(MeterToPixel(radius, self)));

    auto renderer = self->getRenderer();
    assert(renderer != nullptr);

    if (!SDL_SetRenderDrawColor(renderer, sdlColor.r, sdlColor.g, sdlColor.b, sdlColor.a))
    {
        std::cerr << "SDL_SetRenderDrawColor failed: " << SDL_GetError() << std::endl;
        SDL_ClearError();
    }

    if (!ssSDLDrawCircle(renderer, static_cast<int32_t>(std::round(pixelCenter.x)),
                         static_cast<int32_t>(std::round(pixelCenter.y)), pixelRadius))
    {
        std::cerr << "ssSDLDrawCircle failed: " << std::endl;
        return;
    }
}

void ssSWDebugDraw::b2DrawSegment(b2Vec2 p1, b2Vec2 p2, b2HexColor color, void* context)
{
    auto* self = static_cast<ssSWDebugDraw*>(context);
    assert(self != nullptr);

    const SDL_Color sdlColor = ToSDLColor(color);

    const b2Vec2 pixelP1 = MeterToPixel(p1, self);
    const b2Vec2 pixelP2 = MeterToPixel(p2, self);

    auto renderer = self->getRenderer();
    assert(renderer != nullptr);

    if (!SDL_SetRenderDrawColor(renderer, sdlColor.r, sdlColor.g, sdlColor.b, sdlColor.a))
    {
        std::cerr << "SDL_SetRenderDrawColor failed: " << SDL_GetError() << std::endl;
        SDL_ClearError();
    }

    if (!SDL_RenderLine(renderer, pixelP1.x, pixelP1.y, pixelP2.x, pixelP2.y))
    {
        std::cerr << "ssSDLDrawLine failed: " << std::endl;
        return;
    }
}

void ssSWDebugDraw::b2DrawPoint(b2Vec2 p, float size, b2HexColor color, void* context)
{
    auto* self = static_cast<ssSWDebugDraw*>(context);
    assert(self != nullptr);

    const SDL_Color sdlColor = ToSDLColor(color);

    const b2Vec2 pixelP = MeterToPixel(p, self);

    auto renderer = self->getRenderer();
    assert(renderer != nullptr);

    if (!SDL_SetRenderDrawColor(renderer, sdlColor.r, sdlColor.g, sdlColor.b, sdlColor.a))
    {
        std::cerr << "SDL_SetRenderDrawColor failed: " << SDL_GetError() << std::endl;
        SDL_ClearError();
    }

    if (!ssSDLDrawCircle(renderer, static_cast<int32_t>(std::round(pixelP.x)),
                         static_cast<int32_t>(std::round(pixelP.y)), static_cast<int>(std::round(size))))
    {
        std::cerr << "ssSDLDrawPoint failed: " << std::endl;
        return;
    }
}
