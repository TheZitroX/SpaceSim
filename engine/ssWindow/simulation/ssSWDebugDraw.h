//
// Created by zitri on 07.11.2024.
//

#pragma once

#include <vector>
#include <sdl3/sdl.h>
#include <box2d/box2d.h>

class ssSWDebugDraw : public b2DebugDraw
{
public:
    ssSWDebugDraw()
        : b2DebugDraw(b2DefaultDebugDraw()) {};

    ssSWDebugDraw(SDL_Renderer* pRenderer, const float MetersPerPixelFactor)
        : b2DebugDraw(b2DefaultDebugDraw())
        , m_pRenderer(pRenderer)
        , m_MetersPerPixelFactor(MetersPerPixelFactor)
    {
        b2DebugDraw::drawShapes = true;
        b2DebugDraw::DrawSolidPolygon = ssSWDebugDraw::DrawSolidPolygon;

        b2DebugDraw::drawingBounds = {0, 0, 0, 0};
        b2DebugDraw::context = this;
    }

    ~ssSWDebugDraw() = default;

    static void DrawSolidPolygon(b2Transform transform, const b2Vec2* pVertices, int VertexCount,
                                 float radius,
                                 b2HexColor rColor, void* context);

    [[nodiscard]] SDL_Renderer* getRenderer() const;
    [[nodiscard]] float getMeterPerPixelFactor() const;

    //static void b2DrawPolygon(const b2Vec2* vertices, int vertexCount, b2HexColor color, void* context){};
    //
    //static void b2DrawCircle(b2Vec2 center, float radius, b2HexColor color, void* context){};
    //
    //static void b2DrawSolidCircle(b2Transform transform, float radius, b2HexColor color, void* context){};
    //
    //static void b2DrawSolidCapsule(b2Vec2 p1, b2Vec2 p2, float radius, b2HexColor color, void* context){};
    //
    //static void b2DrawSegment(b2Vec2 p1, b2Vec2 p2, b2HexColor color, void* context){};
    //
    //static void b2DrawTransform(b2Transform transform, void* context){};
    //
    //static void b2DrawPoint(b2Vec2 p, float size, b2HexColor color, void* context){};
    //
    //static void b2DrawString(b2Vec2 p, const char* s, void* context){};

private:

    SDL_Renderer* m_pRenderer{};

    float m_MetersPerPixelFactor{};
};
