#pragma once
#include <raylib.h>
#include "../Component.h"
#include "../PhysicsWorld.h"

class PhysicsCircleComponent : public RayEngine::Component {
public:
    static const std::string ID;

    PhysicsCircleComponent(float radius = 0.5f, float restitution = 1.3f)
        : m_Radius(radius), m_Restitution(restitution) {}

    Component *Clone() const override {
        return new PhysicsCircleComponent(m_Radius, m_Restitution);
    }

    const std::string &GetID() const override { return ID; }

    void OnStart() override {
        auto worldId = PhysicsWorld::Get().GetWorldId();
        if (!b2World_IsValid(worldId)) return;

        RayEngine::Entity *owner = GetOwner();
        Vector2 pos = owner->GetPosition();
        b2BodyDef def = b2DefaultBodyDef();
        def.type = b2_dynamicBody;
        def.position = (b2Vec2){ pos.x / 40.0f, -pos.y / 40.0f };
        m_BodyId = b2CreateBody(worldId, &def);

        b2Circle circle = {
            .center = {0.0f, 0.0f},
            .radius = m_Radius
        };

        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.density = 1.0f;
        shapeDef.material.friction = 0.3f;

        m_ShapeId = b2CreateCircleShape(m_BodyId, &shapeDef, &circle);
        b2Body_EnableHitEvents(m_BodyId, true);
        b2Shape_SetRestitution(m_ShapeId, m_Restitution);

        m_BounceSound = LoadSound("../Data/Sounds/vine-boom.mp3");
        SetSoundVolume(m_BounceSound, 0.4f);
    }

    void OnRender(const RayEngine::RenderContext&) const override {
        if (!b2Body_IsValid(m_BodyId)) return;

        b2Vec2 center = b2Body_GetPosition(m_BodyId);
        DrawCircle((int)(center.x * 40.0f), (int)(-center.y * 40.0f), m_Radius * 40.0f, BLUE);
    }

    void OnUpdate(const RayEngine::UpdateContext &context) override {


        b2WorldId worldId = PhysicsWorld::Get().GetWorldId();
        b2ContactEvents contactEvents = b2World_GetContactEvents(worldId);

        if (contactEvents.hitCount > 0) {
            PlaySound(m_BounceSound);
            //TraceLog(LOG_INFO, "Bounce");
        }
    }

    void OnDestroy() override {
        if (b2Body_IsValid(m_BodyId)) {
            b2DestroyBody(m_BodyId);
            m_BodyId = {0};
        }

            UnloadSound(m_BounceSound);

    }

private:
    float m_Radius;
    float m_Restitution;
    b2BodyId m_BodyId = {0};
    b2ShapeId m_ShapeId = {0};

    Sound m_BounceSound{};

};

const std::string PhysicsCircleComponent::ID = "PhysicsCircleComponent";
