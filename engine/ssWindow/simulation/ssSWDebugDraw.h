//
// Created by zitri on 07.11.2024.
//

#pragma once

#include <box2d/box2d.h>
#include <sdl3/sdl.h>


class ssSWDebugDraw : public b2DebugDraw
{
public:
    ssSWDebugDraw()
        : b2DebugDraw(b2DefaultDebugDraw()) {};

    ssSWDebugDraw(SDL_Renderer* pRenderer, float MetersPerPixelFactor);

    ~ssSWDebugDraw() = default;

    void DrawSolidPolygon(const b2Vec2* pVertices, int VertexCount, const b2HexColor& rColor);

private:

    float MeterToPixel(float Value) const;

    b2Vec2 MeterToPixel(const b2Vec2& rVector);

    static SDL_Color ToSDLColor(const b2HexColor& rColor);

private:

    SDL_Renderer* m_pRenderer{};

    float m_MetersPerPixelFactor{};


};
