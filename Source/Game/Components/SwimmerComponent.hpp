#pragma once
#include <raylib.h>
#include "../Component.h"
#include "../Entity.h"
#include "../../Visuals/AnimatedSprite.h"
#include "../../Input/Input.h"

class SwimmerComponent : public RayEngine::Component {
public:
    static const std::string ID;

    SwimmerComponent(Texture2D texture)
        : m_SwimmerTexture(texture)
    {
        m_SwimmerIdleAnimation = RayEngine::AnimatedSprite(
            m_SwimmerTexture,
            {0.0f, 0.0f},
            {166.0f, 78.0f},
            {0.5f, 1.0f},
            6,
            1.0f / 4.0f,
            4,
            0,
            true
        );
        m_SwimmerIdleAnimation.Play();

        m_SwimmerWalkAnimation = RayEngine::AnimatedSprite(
            m_SwimmerTexture,
            {0.0f, 78.0f},
            {166.0f, 78.0f},
            {0.5f, 1.0f},
            6,
            1.0f / 6.0f,
            6,
            0,
            true
        );
        m_SwimmerWalkAnimation.Play();

        m_AnimatedSprite = &m_SwimmerIdleAnimation;
    }

    const std::string &GetID() const override {
        return ID;
    }

    RayEngine::Component *Clone() const override {
        return new SwimmerComponent(m_SwimmerTexture);
    }

    void OnStart() override {
        m_SwimmerColorShader = LoadShader(
    "../Data/Shaders/SwimmerColor.vs",
    "../Data/Shaders/SwimmerColor.fs"
        );
    }

    void OnUpdate(const RayEngine::UpdateContext &context) override {
        if (m_AnimatedSprite)
            m_AnimatedSprite->Update(context.DeltaTime);

        auto owner = GetOwner();
        if (!owner) return;

        const RayEngine::Input &input = *context.Input;

        const float speed = 200.0f;

        Vector2 pos = owner->GetPosition();

        bool moving = false;

        if (input.GetKey(RayEngine::KeyCode::A, RayEngine::InputState::Held)){
            pos.x -= speed * (float)context.DeltaTime;
            moving = true;
            m_Flip = true;
        }

        if (input.GetKey(RayEngine::KeyCode::D, RayEngine::InputState::Held)){
            pos.x += speed * (float)context.DeltaTime;
            moving = true;
            m_Flip = false;
        }

        owner->SetPosition(pos);

        if (moving) {
            if (m_AnimatedSprite != &m_SwimmerWalkAnimation) {
                m_SwimmerWalkAnimation.Reset();
                m_SwimmerWalkAnimation.Play();
                m_AnimatedSprite = &m_SwimmerWalkAnimation;
            }
        } else {
            if (m_AnimatedSprite != &m_SwimmerIdleAnimation) {
                m_SwimmerIdleAnimation.Reset();
                m_SwimmerIdleAnimation.Play();
                m_AnimatedSprite = &m_SwimmerIdleAnimation;
            }
        }

        if (m_AnimatedSprite) {
            m_AnimatedSprite->Update(context.DeltaTime);
        }
    }

    void OnRender(const RayEngine::RenderContext &context) const override {
        BeginShaderMode(m_SwimmerColorShader);
        auto owner = GetOwner();
        if (owner && m_AnimatedSprite) {
            m_AnimatedSprite->Render(
                owner->GetPosition(),
                0.0f,
                {2.0f, 2.0f},
                m_Flip,
                false
            );
        }
        EndShaderMode();
    }

    void OnDestroy() override {
        UnloadTexture(m_SwimmerTexture);
        UnloadShader(m_SwimmerColorShader);
    }

private:
    Texture2D m_SwimmerTexture;
    RayEngine::AnimatedSprite m_SwimmerIdleAnimation;
    RayEngine::AnimatedSprite m_SwimmerWalkAnimation;

    RayEngine::AnimatedSprite* m_AnimatedSprite = nullptr;

    bool m_Flip = false;

    Shader m_SwimmerColorShader;
};

const std::string SwimmerComponent::ID = "Swimmer";
