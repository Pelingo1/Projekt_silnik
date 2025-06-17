#pragma once
#include <box2d/box2d.h>
#include "../Component.h"
#include "../PhysicsWorld.h"

class PhysicsBoxComponent : public RayEngine::Component {
public:
    static const std::string ID;
    b2BodyId bodyId;

    Component* Clone() const override {
        return new PhysicsBoxComponent();
    }

    const std::string& GetID() const override { return ID; }

    void OnStart() override {
        b2WorldId world = PhysicsWorld::Get().GetWorldId();

        b2BodyDef bodyDef = b2DefaultBodyDef();
        bodyDef.type = b2_dynamicBody;
        bodyDef.position = (b2Vec2){0.0f, -2.0f};
        bodyId = b2CreateBody(world, &bodyDef);

        b2Polygon box = b2MakeBox(4.0f, 0.2f);
        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.density = 1.0f;
        b2CreatePolygonShape(bodyId, &shapeDef, &box);

        b2Body_EnableHitEvents(bodyId, true);
    }

    void OnRender(const RayEngine::RenderContext&) const override {
        if (!b2Body_IsValid(bodyId)) return;

        b2Transform transform = b2Body_GetTransform(bodyId);
        b2Polygon polygon = b2MakeBox(4.0f, 0.2f);

        for (int i = 0; i < polygon.count; ++i) {
            b2Vec2 v1 = b2TransformPoint(transform, polygon.vertices[i]);
            b2Vec2 v2 = b2TransformPoint(transform, polygon.vertices[(i + 1) % polygon.count]);

            DrawLine(
                v1.x * 40.0f,
                -v1.y * 40.0f,
                v2.x * 40.0f,
                -v2.y * 40.0f,
                BLACK
            );
        }
    }

    void OnDestroy() override {
        b2DestroyBody(bodyId);
    }
};

const std::string PhysicsBoxComponent::ID = "PhysicsBoxComponent";
