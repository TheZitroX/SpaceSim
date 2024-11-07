//
// Created by zitri on 07.11.2024.
//

#include "ssSWDebugDraw.h"

#include <vector>
#include <iostream>

ssSWDebugDraw::ssSWDebugDraw(SDL_Renderer *pRenderer, const float MetersPerPixelFactor)
        : b2DebugDraw(b2DefaultDebugDraw()), m_pRenderer(pRenderer), m_MetersPerPixelFactor(MetersPerPixelFactor) {
    this->drawShapes = true;
    this->drawJoints = true;
    this->drawAABBs = true;
}


SDL_FColor ToSDLFColor(const SDL_Color& color) {
    return {
            (float) color.r / 255,
            (float) color.g / 255,
            (float) color.b / 255,
            (float) color.a / 255
    };
}

void ssSWDebugDraw::DrawSolidPolygon(const b2Vec2 *pVertices, int VertexCount, const b2HexColor &rColor) {
    const SDL_Color LineColor = ToSDLColor(rColor);
    SDL_FColor TriangleColor = ToSDLFColor(ToSDLColor(rColor));
    TriangleColor.a = 128;

    std::vector<SDL_Vertex> vecVertices;
    for (int32_t i = 0; i < VertexCount; ++i) {
        if (i < (VertexCount - 2)) {
            const b2Vec2 TriangleVertex[3] =
                    {
                            MeterToPixel(pVertices[0 + 0]),
                            MeterToPixel(pVertices[i + 1]),
                            MeterToPixel(pVertices[i + 2]),
                    };

            for (auto j: TriangleVertex) {
                SDL_Vertex vertex = {{j.x, j.y}, TriangleColor, {0.0f, 0.0f}};
                vecVertices.push_back(vertex);
            }
        }
    }

    if (!SDL_RenderGeometry(m_pRenderer, nullptr, vecVertices.data(), (int32_t) vecVertices.size(), nullptr, 0)) {
        std::cerr << "SDL_RenderGeometry failed: " << SDL_GetError() << std::endl;
        SDL_ClearError();
    }

    if (!SDL_SetRenderDrawColor(m_pRenderer, LineColor.r, LineColor.g, LineColor.b, LineColor.a)) {
        std::cerr << "SDL_SetRenderDrawColor failed: " << SDL_GetError() << std::endl;
        SDL_ClearError();
    }

    for (int32_t i = 0; i < VertexCount; ++i) {
        const b2Vec2 LineStart = MeterToPixel(pVertices[i]);
        const b2Vec2 LineEnd = MeterToPixel(pVertices[((i < (VertexCount - 1)) ? (i + 1) : 0)]);

        if (!SDL_RenderLine(m_pRenderer, LineStart.x, LineStart.y, LineEnd.x, LineEnd.y)) {
            std::cerr << "SDL_RenderLine failed: " << SDL_GetError() << std::endl;
            SDL_ClearError();
        }
    }
}

float ssSWDebugDraw::MeterToPixel(const float Value) const {
    return (Value * m_MetersPerPixelFactor);
}

b2Vec2 ssSWDebugDraw::MeterToPixel(const b2Vec2 &rVector) {
    return b2Vec2(MeterToPixel(rVector.x), MeterToPixel(rVector.y));
}

SDL_Color ssSWDebugDraw::ToSDLColor(const b2HexColor& color) {
    SDL_Color sdlColor;
    sdlColor.r = (color >> 16) & 0xFF;
    sdlColor.g = (color >> 8) & 0xFF;
    sdlColor.b = color & 0xFF;
    sdlColor.a = 255;
    return sdlColor;
}
