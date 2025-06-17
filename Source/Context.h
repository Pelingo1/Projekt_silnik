#pragma once

#include <raylib.h>
#include "Game/LevelManager.h"
#include "Input/Input.h"

namespace RayEngine {
    class Game;
    struct UpdateContext {
        bool bDebug;
        double DeltaTime;
        Input *Input;
        LevelManager *LevelManager;
    };
    struct RenderContext {
        bool bDebug;
        Camera2D *Camera;
    };
    struct RenderUIContext {
        bool bDebug;
        Vector2 ScreenSize;
        Camera2D *Camera;
    };
}

