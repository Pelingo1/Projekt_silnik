#pragma once
#include "Entity.h"
#include "Level.h"

namespace RayEngine {
    class World {
    public:
        void LoadLevel(const Level &level);
        void Spawn(const Entity &entity);
        void Clear();

        void Update(const UpdateContext &context);
        void Render(const RenderContext &context) const;
        void RenderUI(const RenderUIContext &context) const;

    private:
        int m_LastEntityID = 0;
        std::vector<Entity> m_Entities;

    };

}