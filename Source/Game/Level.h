#pragma once
#include <string>
#include <vector>
#include "Entity.h"

namespace RayEngine {
    class World;
    class Level {
      protected:
        explicit Level(const std::string &name) : m_Name(name) {};

    private:
        std::string m_Name;
        std::vector<Entity> m_Entities;

    public:
        const std::string &GetName() const { return m_Name; }
        static Level Create(const std::string &name);
        Level &WithEntity(const Entity &entity);
        void LoadInto(World &world) const;
        const std::vector<Entity>& GetEntities() const { return m_Entities; }

    };
}