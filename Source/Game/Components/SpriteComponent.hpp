#pragma once
#include <iostream>
#include <ostream>
#include <raylib.h>
#include "../Component.h"
#include "../Entity.h"

class SpriteComponent : public RayEngine::Component {
public:
    static const std::string ID;
    SpriteComponent(const std::string &texturePath, Color tint = WHITE) : m_Tint(tint) {
        m_TexturePath = texturePath;
        m_SpriteTexture = LoadTexture(m_TexturePath.c_str());
        if (!IsTextureValid(m_SpriteTexture)) {
            std::cout << "Failed to load a texture " << texturePath << std::endl;
        }
    }

    Component *Clone() const override {
        return new SpriteComponent(m_TexturePath, m_Tint);
    }
    const std::string &GetID() const override {return ID;}

    void OnRender(const RayEngine::RenderContext &context) const override {
        RayEngine::Entity *owner = GetOwner();

        float rotation = owner->GetRotation();
        Vector2 position = owner->GetPosition();
        Vector2 scale = owner->GetScale();
        const float textureWidth = (float) m_SpriteTexture.width;
        const float textureHeight = (float) m_SpriteTexture.height;
        const Rectangle sourceRect = {0.0f, 0.0f, textureWidth, textureHeight};
        const Vector2 spriteSize = {1.0f * scale.x, 1.0f * scale.y};
        const Rectangle destRect = {position.x, position.y, spriteSize.x, spriteSize.y};
        const Vector2 origin = {spriteSize.x * 0.5f, spriteSize.y * 0.5f};
        DrawTexturePro(m_SpriteTexture, sourceRect, destRect, origin, rotation, m_Tint);

    }

private:
    std::string m_TexturePath;
    Texture2D m_SpriteTexture;
    Color m_Tint;

};

const std::string SpriteComponent::ID = "SpriteComponent";