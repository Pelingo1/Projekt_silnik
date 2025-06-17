#pragma once
#include <box2d/box2d.h>
#include "Game.h"
#include "Game/Components/SpriteComponent.hpp"
#include "Game/Components/SpinComponent.hpp"
#include "Game/Components/SwimmerComponent.hpp"
#include "Game/Components/PhysicsBoxComponent.hpp"
#include "Game/Components/PhysicsCircleComponent.hpp"

class UserGame : public RayEngine::Game {
protected:

    void RegisterLevels(std::vector<RayEngine::Level> &outLevels) const override {

        auto level1 = RayEngine::Level::Create("Level 1")
        .WithEntity(
            RayEngine::Entity("WMII_RED")
            .WithPosition(Vector2{400.0f, 250.0f})
            .WithScale(Vector2{100.0f, 100.0f})
            .WithComponent(new SpriteComponent("../Data/Sprites/wmii.png",RED))
            .WithComponent(new SpinComponent(70.0f))
            )
        .WithEntity(
            RayEngine::Entity("Kotek")
            .WithRotation(0.0f)
            .WithScale(Vector2{150.0f, 150.0f})
            .WithComponent(new SpriteComponent("../Data/Sprites/cat.jpg"))
            )
        .WithEntity(
            RayEngine::Entity("WMII")
            .WithPosition(Vector2{-200.0f, -250.0f})
            .WithRotation(-30.0f)
            .WithScale(Vector2{75.0f, 75.0f})
            .WithComponent(new SpriteComponent("../Data/Sprites/wmii.png")
            )
            );





        auto& physicsWorld = PhysicsWorld::Get();
        b2WorldId world = physicsWorld.GetWorldId();

        b2BodyDef groundDef = b2DefaultBodyDef();
        groundDef.position = {0.0f, -12.0f};
        b2BodyId groundId = b2CreateBody(world, &groundDef);

        b2Polygon groundShape = b2MakeBox(50.0f, 7.5f);
        b2ShapeDef shapeDef = b2DefaultShapeDef();
        b2CreatePolygonShape(groundId, &shapeDef, &groundShape);

        b2Body_EnableHitEvents(groundId, true);

        auto groundEntity = RayEngine::Entity("GroundDebug");

        auto boxEntity = RayEngine::Entity("Box")
            .WithComponent(new PhysicsBoxComponent());

        auto* circleComponent = new PhysicsCircleComponent(0.5f, 1.0f);
        auto circleEntity = RayEngine::Entity("Ball")
            .WithPosition({0.0f, -200.0f})
            .WithComponent(circleComponent);

        auto level2 = RayEngine::Level::Create("Level 2")
            .WithEntity(groundEntity)
            .WithEntity(boxEntity)
            .WithEntity(circleEntity);







        Texture2D swimmerTexture = LoadTexture("../Data/Sprites/swimmer.png");
        auto level3 = RayEngine::Level::Create("Level 3")
        .WithEntity(
            RayEngine::Entity("Swimmer")
            .WithPosition({400.0f, 300.0f})
            .WithRotation(0.0f)
            .WithScale(Vector2{100.0f, 100.0f})
            .WithComponent(new SwimmerComponent(swimmerTexture))
            );

        outLevels.emplace_back(level1);
        outLevels.emplace_back(level2);
        outLevels.emplace_back(level3);
    }
};

inline RayEngine::Game * User_CreateGame() {
    const RayEngine::GameSpec gameSpec {
        .WindowTitle = "RayEngine Game",
        .WindowSize = {1280, 720}
    };
    RayEngine::Game *game = new UserGame();
    game->Init(gameSpec);
    return game;
}