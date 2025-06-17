#include "World.h"

void RayEngine::World::LoadLevel(const Level &level) {
    level.LoadInto(*this);
}

void RayEngine::World::Spawn(const Entity &entity) {
    m_Entities.push_back(entity);
    Entity &newEntity = m_Entities.back();
    m_LastEntityID++;
    newEntity.SetID(m_LastEntityID);
    newEntity.Start();
}

void RayEngine::World::Clear() {
    for (Entity &entity : m_Entities) {
        entity.Destroy();
    }
    m_Entities.clear();
}

void RayEngine::World::Update(const UpdateContext &context) {
    for (Entity &entity : m_Entities) {
        if (!entity.MarkedForDestruction()) {
            entity.Update(context);
        }
    }
    for (int i = m_Entities.size() - 1; i >= 0; --i) {
        if (m_Entities[i].MarkedForDestruction()) {
            m_Entities.erase(m_Entities.begin() + i);
        }
    }
}

void RayEngine::World::Render(const RenderContext &context) const {
    for (const Entity &entity : m_Entities) {
        if (!entity.MarkedForDestruction()) {
            entity.Render(context);
        }
    }
}

void RayEngine::World::RenderUI(const RenderUIContext &context) const {
    for (const Entity &entity : m_Entities) {
        if (!entity.MarkedForDestruction()) {
            entity.RenderUI(context);
        }
    }
}