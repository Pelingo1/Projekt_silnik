#pragma once
#include <box2d/box2d.h>

class PhysicsWorld {
public:
    static PhysicsWorld& Get() {
        static PhysicsWorld instance;
        return instance;
    }

    b2WorldId GetWorldId() const { return m_WorldId; }

    void Step(float deltaTime) {
        if (b2World_IsValid(m_WorldId)) {
            b2World_Step(m_WorldId, deltaTime, 4);
        }
    }

private:
    b2WorldId m_WorldId;

    PhysicsWorld() {
        b2WorldDef def = b2DefaultWorldDef();
        def.gravity = (b2Vec2){0.0f, -3.0f};
        m_WorldId = b2CreateWorld(&def);
    }

    PhysicsWorld(const PhysicsWorld&) = delete;
    PhysicsWorld& operator=(const PhysicsWorld&) = delete;
};
