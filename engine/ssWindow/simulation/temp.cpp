#include <iostream>

#include <SDL.h>
#include <box2d.h>

#include "Box2DDebugRenderer.h"

#define WINDOW_WIDTH	(1280)
#define WINDOW_HEIGHT	(720)
#define DEG_TO_RAD(X)	((X) * ((float)M_PI / 180.0f))

SDL_Window*		pWindow		= nullptr;
SDL_Renderer*	pRenderer	= nullptr;

b2World*				pB2World			= nullptr;
CBox2DDebugRenderer*	pB2DebugRenderer	= nullptr;

double OldTime	 = 0.0;
double NewTime	 = 0.0;
double DeltaTime = 0.0;

float MetersPerPixelFactor = 32.0f;

bool Running = true;

// Some helper functions to convert from pixels to meters, and vice versa

float PixelToMeter(const float Value)
{
    return (Value * (1.0f / MetersPerPixelFactor));
}

b2Vec2 PixelToMeter(const b2Vec2& rVector)
{
    return b2Vec2(PixelToMeter(rVector.x), PixelToMeter(rVector.y));
}

float MeterToPixel(const float Value)
{
    return (Value * MetersPerPixelFactor);
}

b2Vec2 MeterToPixel(const b2Vec2& rVector)
{
    return b2Vec2(MeterToPixel(rVector.x), MeterToPixel(rVector.y));
}

bool Initialize()
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Error: SDL could not initialize! %s\n", SDL_GetError());
        return false;
    }

    pWindow = SDL_CreateWindow("SDL + Box2D", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if(!pWindow)
    {
        printf("Error: SDL window could not be created! %s\n", SDL_GetError());
        return false;
    }

    pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(!pRenderer)
    {
        printf("Error: SDL renderer could not be created! %s\n", SDL_GetError());
        return false;
    }

    SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);
    SDL_SetRenderDrawBlendMode(pRenderer, SDL_BLENDMODE_BLEND);

    SDL_RenderClear(pRenderer);
    SDL_RenderPresent(pRenderer);

    pB2World			= new b2World(b2Vec2(0.0f, 9.82f));
    pB2DebugRenderer	= new CBox2DDebugRenderer(pRenderer, MetersPerPixelFactor, b2Draw::e_shapeBit);

    pB2World->SetDebugDraw(pB2DebugRenderer);

    return true;
}

void Deinitialize()
{
    delete pB2DebugRenderer;
    pB2DebugRenderer = nullptr;

    pB2World->SetDebugDraw(nullptr);
    pB2World->ClearForces();
    delete pB2World;
    pB2World = nullptr;

    SDL_DestroyRenderer(pRenderer);
    pRenderer = nullptr;

    SDL_DestroyWindow(pWindow);
    pWindow = nullptr;

    SDL_Quit();
}

void InitializeAssets()
{
    // Ground body

    b2Vec2 Size		= b2Vec2(1200.0f, 30.0f);
    b2Vec2 SizeHalf	= b2Vec2(Size.x * 0.5f, Size.y * 0.5f);
    b2Vec2 Position	= b2Vec2(WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT - (Size.y * 0.5f));

    b2BodyDef BodyDefinition;
    BodyDefinition.type		= b2BodyType::b2_staticBody;
    BodyDefinition.angle	= 0.0f;

    // Box2D works in meters, so convert 'Position' from a position in pixels to a position in meters
    BodyDefinition.position	= PixelToMeter(Position);

    b2PolygonShape Shape;

    // Box2D works in meters so convert 'SizeHalf' from a size in pixels to a size in meters
    Shape.SetAsBox(PixelToMeter(SizeHalf.x), PixelToMeter(SizeHalf.y), PixelToMeter(b2Vec2(0.0f, 0.0f)), 0.0f);

    b2FixtureDef FixtureDefinition;
    FixtureDefinition.shape			= &Shape;
    FixtureDefinition.friction		= 0.2f;
    FixtureDefinition.restitution	= 0.6f;
    FixtureDefinition.density		= 1.0f;

    b2Body* pGroundBody = pB2World->CreateBody(&BodyDefinition);
    pGroundBody->CreateFixture(&FixtureDefinition);

    //////////////////////////////////////////////////////////////////////////

    // Left wall body

    Size		= b2Vec2(30.0f, 700.0f);
    SizeHalf	= b2Vec2(Size.x * 0.5f, Size.y * 0.5f);
    Position	= b2Vec2(SizeHalf.x, WINDOW_HEIGHT * 0.5f);

    BodyDefinition.type		= b2BodyType::b2_staticBody;
    BodyDefinition.position	= PixelToMeter(Position);
    BodyDefinition.angle	= 0.0f;

    Shape.SetAsBox(PixelToMeter(SizeHalf.x), PixelToMeter(SizeHalf.y), PixelToMeter(b2Vec2(0.0f, 0.0f)), 0.0f);

    FixtureDefinition.shape			= &Shape;
    FixtureDefinition.friction		= 0.2f;
    FixtureDefinition.restitution	= 0.6f;
    FixtureDefinition.density		= 1.0f;

    b2Body* pLeftWallBody = pB2World->CreateBody(&BodyDefinition);
    pLeftWallBody->CreateFixture(&FixtureDefinition);

    //////////////////////////////////////////////////////////////////////////

    // Right wall body

    Size		= b2Vec2(30.0f, 700.0f);
    SizeHalf	= b2Vec2(Size.x * 0.5f, Size.y * 0.5f);
    Position	= b2Vec2(WINDOW_WIDTH - SizeHalf.x, WINDOW_HEIGHT * 0.5f);

    BodyDefinition.type		= b2BodyType::b2_staticBody;
    BodyDefinition.position	= PixelToMeter(Position);
    BodyDefinition.angle	= 0.0f;

    Shape.SetAsBox(PixelToMeter(SizeHalf.x), PixelToMeter(SizeHalf.y), PixelToMeter(b2Vec2(0.0f, 0.0f)), 0.0f);

    FixtureDefinition.shape			= &Shape;
    FixtureDefinition.friction		= 0.2f;
    FixtureDefinition.restitution	= 0.6f;
    FixtureDefinition.density		= 1.0f;

    b2Body* pRightWallBody = pB2World->CreateBody(&BodyDefinition);
    pRightWallBody->CreateFixture(&FixtureDefinition);

    //////////////////////////////////////////////////////////////////////////

    // Left slope body

    Size		= b2Vec2(400.0f, 20.0f);
    SizeHalf	= b2Vec2(Size.x * 0.5f, Size.y * 0.5f);
    Position	= b2Vec2((WINDOW_WIDTH * 0.5f) - 300.0f, WINDOW_HEIGHT * 0.5f);

    BodyDefinition.type		= b2BodyType::b2_staticBody;
    BodyDefinition.position	= PixelToMeter(Position);
    BodyDefinition.angle	= DEG_TO_RAD(20.0f);

    Shape.SetAsBox(PixelToMeter(SizeHalf.x), PixelToMeter(SizeHalf.y), PixelToMeter(b2Vec2(0.0f, 0.0f)), 0.0f);

    FixtureDefinition.shape			= &Shape;
    FixtureDefinition.friction		= 0.2f;
    FixtureDefinition.restitution	= 0.6f;
    FixtureDefinition.density		= 1.0f;

    b2Body* pLeftSlopeBody = pB2World->CreateBody(&BodyDefinition);
    pLeftSlopeBody->CreateFixture(&FixtureDefinition);

    //////////////////////////////////////////////////////////////////////////

    // Right slope body

    Size		= b2Vec2(550.0f, 20.0f);
    SizeHalf	= b2Vec2(Size.x * 0.5f, Size.y * 0.5f);
    Position	= b2Vec2((WINDOW_WIDTH * 0.5f) + 300.0f, (WINDOW_HEIGHT * 0.5f) + 50.0f);

    BodyDefinition.type		= b2BodyType::b2_staticBody;
    BodyDefinition.position	= PixelToMeter(Position);
    BodyDefinition.angle	= DEG_TO_RAD(-30.0f);

    Shape.SetAsBox(PixelToMeter(SizeHalf.x), PixelToMeter(SizeHalf.y), PixelToMeter(b2Vec2(0.0f, 0.0f)), 0.0f);

    FixtureDefinition.shape			= &Shape;
    FixtureDefinition.friction		= 0.2f;
    FixtureDefinition.restitution	= 0.6f;
    FixtureDefinition.density		= 1.0f;

    b2Body* pRightSlopeBody = pB2World->CreateBody(&BodyDefinition);
    pRightSlopeBody->CreateFixture(&FixtureDefinition);

    //////////////////////////////////////////////////////////////////////////

    Size		= b2Vec2(20.0f, 20.0f);
    SizeHalf	= b2Vec2(Size.x * 0.5f, Size.y * 0.5f);

    BodyDefinition.type		= b2BodyType::b2_dynamicBody;
    BodyDefinition.angle	= 0.0f;

    Shape.SetAsBox(PixelToMeter(SizeHalf.x), PixelToMeter(SizeHalf.y), PixelToMeter(b2Vec2(0.0f, 0.0f)), 0.0f);

    FixtureDefinition.shape			= &Shape;
    FixtureDefinition.friction		= 0.2f;
    FixtureDefinition.restitution	= 0.6f;
    FixtureDefinition.density		= 0.5f;

    for(uint32_t i = 0; i < 50; ++i)
    {
        Position = b2Vec2(100.0f + ((Size.x * 1.1f) * i), -10.0f - ((Size.y * 0.7f) * i));

        BodyDefinition.position = PixelToMeter(Position);

        b2Body* pFallingBoxBody = pB2World->CreateBody(&BodyDefinition);
        pFallingBoxBody->CreateFixture(&FixtureDefinition);
    }
}

void HandleEvents()
{
    SDL_Event Event;
    while(SDL_PollEvent(&Event))
    {
        switch(Event.type)
        {
            case SDL_QUIT:
            {
                Running = false;
                break;
            }

            default:
                break;
        }
    }
}

void Update()
{
    NewTime = (double)SDL_GetTicks();
    DeltaTime = (NewTime - OldTime) * 0.001;
    OldTime = NewTime;

    pB2World->Step((float)DeltaTime, 8, 4);
}

void Render()
{
    SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
    SDL_RenderClear(pRenderer);

    pB2World->DebugDraw();

    SDL_RenderPresent(pRenderer);
}

int main(int argc, char** argv)
{
    // Initialize SDL, create the window and the renderer, initialize Box2D etc
    if(!Initialize())
        return -1;

    InitializeAssets();

    while(Running)
    {
        HandleEvents();
        Update();
        Render();
    }

    // Shutdown/destroy everything
    Deinitialize();

    return 0;
}