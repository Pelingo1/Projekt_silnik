#pragma once
#include <string>

namespace RayEngine {
    class Game;
    class LevelManager {
    public:
        explicit LevelManager(Game *game) : m_Game(game) {};
        void ChangeLevel(const std::string &name) const;

    private:
        Game *m_Game = nullptr;

    };
}