#pragma once
#include "../Component.h"
#include "../Entity.h"

class SpinComponent : public RayEngine::Component {
public:
    static const std::string ID;
    SpinComponent(float rotateSpeed) : m_RotateSpeed(rotateSpeed) {}
    Component *Clone() const override { return new SpinComponent(m_RotateSpeed); }
    const std::string &GetID() const override { return ID; };

    void OnUpdate(const RayEngine::UpdateContext &context) override {
        RayEngine::Entity *owner = GetOwner();
        float rotation = owner->GetRotation();
        rotation += m_RotateSpeed * context.DeltaTime;
        owner->SetRotation(rotation);
    }

private:
    float m_RotateSpeed;

};

const std::string SpinComponent::ID = "SpinComponent";
