#pragma once

#include "Context.h"
#include "Game/Level.h"
#include "Game/World.h"
#include <string>
#include "raylib.h"
#include "Input/Input.h"
#include <memory>
#include <vector>

namespace RayEngine {
    struct GameSpec {
        std::string WindowTitle;
        Vector2 WindowSize;

    };

    class Game {
    public:
        Game();
        void Init(const GameSpec& gameSpec);
        void Run();
        void Shutdown();

        void LoadLevel(const std::string &levelName);
        void RequestLevelChange(const std::string &levelName);

        Music m_BackgroundMusic;
        float m_MusicVolume = 1.0f;
        float m_MusicPitch = 1.0f;

        Sound m_ClickSound;

        double m_FPS = 0.0f;
    private:
        void Update(const UpdateContext &context);
        void Render(const RenderContext &context) const;
        void RenderUI(const RenderUIContext &context) const;

        bool m_Running;
        Vector2 m_WindowSize;
        Camera2D m_RenderCamera;
        Input m_Input;
        bool m_bDebug;
        World m_World;
        std::vector<Level> m_Levels;
        LevelManager m_LevelManager;
        bool m_bLevelChangeRequested;
        std::string m_LevelToLoad;

        Vector2 m_cursorWorldPosition;
        Vector2 m_CursorPosition;
        double m_MouseClickTimer = 0.0f;
        double m_MouseClickDuration = 0.3f;

    protected:
        virtual void RegisterLevels(std::vector<Level> &outLevels) const = 0;
    };
}

RayEngine::Game *User_CreateGame();
