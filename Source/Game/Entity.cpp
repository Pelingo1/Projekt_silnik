#include <iostream>
#include <ostream>
#include <cmath>
#include "Entity.h"

RayEngine::Entity::Entity(const Entity &other) {
    *this = other;
}

RayEngine::Entity &RayEngine::Entity::operator=(const Entity &other) {
    m_ID = other.m_ID;
    m_Name = other.m_Name;
    m_Position = other.m_Position;
    m_Rotation = other.m_Rotation;
    m_Scale = other.m_Scale;
    //m_bDestroyed = other.m_bDestroyed;
    m_bDestroyed = false;
    m_Components.clear();
    for (Component *component : other.m_Components) {
        AddComponent(component->Clone());
    }
    return *this;
}

RayEngine::Entity::~Entity() {
    for (const Component *component : m_Components) {
        delete component;
    }
}


RayEngine::Entity &RayEngine::Entity::WithPosition(const Vector2 &position) {
    m_Position = position;
    return *this;
}

RayEngine::Entity &RayEngine::Entity::WithRotation(const float &rotation) {
    m_Rotation = rotation;
    return *this;
}

RayEngine::Entity &RayEngine::Entity::WithScale(const Vector2 &scale) {
    m_Scale = scale;
    return *this;
}

RayEngine::Entity &RayEngine::Entity::WithComponent(Component *component) {
    AddComponent(component);
    return *this;
}


void RayEngine::Entity::Start() {
    std::cout << "Starting Entity :: " << m_Name <<std::endl;
    for (Component *component : m_Components) {
        component->OnStart();
    }
}

void RayEngine::Entity::Update(const UpdateContext &context) {
    for (Component *component : m_Components) {
        component->OnUpdate(context);
    }
    for (const std::string &id: m_ComponentIDsToRemove) {
        for (int i=0; i<m_Components.size(); i++) {
            if (m_Components[i]->GetID() == id) {
                m_Components[i]->OnDestroy();
                m_Components.erase(m_Components.begin() + i);
                break;
            }
        }
    }
    m_ComponentIDsToRemove.clear();

    if (context.Input->GetKey(KeyCode::F1, InputState::Pressed)) {
        context.LevelManager->ChangeLevel("Level 1");
    }
    if (context.Input->GetKey(KeyCode::F2, InputState::Pressed)) {
        context.LevelManager->ChangeLevel("Level 2");
    }
    if (context.Input->GetKey(KeyCode::F3, InputState::Pressed)) {
        context.LevelManager->ChangeLevel("Level 3");
    }
}

void RayEngine::Entity::Render(const RenderContext &context) const {
    for (Component *component : m_Components) {
        component->OnRender(context);
    }
}

void RayEngine::Entity::RenderUI(const RenderUIContext &context) const{
    if (context.bDebug) {
        const Vector2 screenPos = GetWorldToScreen2D(Vector2{m_Position.x, m_Position.y}, *context.Camera);
        DrawText(m_Name.c_str(), screenPos.x + 15.0f, screenPos.y - 50.0f, 16, ORANGE);
    }
    for (Component *component : m_Components) {
        component->OnRenderUI(context);
    }
}

void RayEngine::Entity::Destroy() {
    for (Component *component : m_Components) {
        component->OnDestroy();
    }
    m_bDestroyed = true;
}

void RayEngine::Entity::AddComponent(Component *component) {
    const std::string &id = component->GetID();
    for (const Component *existingComponent : m_Components) {
        if (existingComponent->GetID() == id) {
            std::cout << "Entity " << m_Name << " already has component with ID = " << id << std::endl;
        }
    }
    component->Init(this);
    m_Components.emplace_back(component);
}

void RayEngine::Entity::RemoveComponent(const std::string &id) {
    m_ComponentIDsToRemove.push_back(id);
}

RayEngine::Component *RayEngine::Entity::GetComponent(const std::string &id) const {
    for (Component *component : m_Components) {
        if (component->GetID() == id) {
            return component;
        }
    }
    return nullptr;
}


