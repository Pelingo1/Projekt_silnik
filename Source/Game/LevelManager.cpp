#include "LevelManager.h"
#include "../Game.h"

void RayEngine::LevelManager::ChangeLevel(const std::string &name) const {
    if (!m_Game) {
        return;
    }
    m_Game->RequestLevelChange(name);
}