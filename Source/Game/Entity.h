#pragma once
#include <raylib.h>
#include <string>
#include <vector>
#include "Component.h"
#include "Level.h"
#include "../Context.h"

namespace RayEngine {
    class Level;
    class Entity {
    public:
        explicit Entity(const std::string &name) : m_Name(name) {

        }
        Entity(const Entity &other);
        Entity &operator=(const Entity &other);
        ~Entity();

        int GetID() const {return m_ID;}
        void SetID(const int id) {m_ID = id;}
        bool MarkedForDestruction() const {return m_bDestroyed;}
        const Vector2 &GetPosition() const {return m_Position;}
        void SetPosition(const Vector2 &position) {m_Position = position;}
        const float &GetRotation() const {return m_Rotation;}
        void SetRotation(const float &rotation) {m_Rotation = rotation;}
        const Vector2 &GetScale() const {return m_Scale;}
        void SetScale(const Vector2 &scale) {m_Scale = scale;}

        Entity &WithPosition(const Vector2 &position);
        Entity &WithRotation(const float &rotation);
        Entity &WithScale(const Vector2 &scale);
        Entity &WithComponent(Component *component);

        void Start();
        void Update(const UpdateContext &context);
        void Render(const RenderContext &context) const;
        void RenderUI(const RenderUIContext &context) const;
        void Destroy();

        void AddComponent(Component *component);
        void RemoveComponent(const std::string &id);
        Component *GetComponent(const std::string &id) const;

        RayEngine::Level* GetScene() const {return m_Scene;}
        void SetScene(RayEngine::Level *scene) {m_Scene = scene;}
        RayEngine::Level* m_Scene = nullptr;

    private:
        int m_ID = 1;
        std::string m_Name;
        bool m_bDestroyed = false;
        Vector2 m_Position = Vector2{0.0f, 0.0f};
        float m_Rotation = 0.0f;
        Vector2 m_Scale = Vector2{1.0f, 1.0f};

        std::vector<Component *> m_Components;
        std::vector<std::string> m_ComponentIDsToRemove;
    };
}