#include "Game.h"
#include <raymath.h>
#include <cmath>
#include <iostream>
#include <external/glad.h>
#include <GL/gl.h>
#include "Input/Input.h"
#include "Game/PhysicsWorld.h"

RayEngine::Game::Game() : m_LevelManager(this) {}

void RayEngine::Game::LoadLevel(const std::string &levelName) {
    m_World.Clear();
    const Level *levelToLoad = nullptr;
    for (Level &level : m_Levels) {
        if (level.GetName() == levelName) {
            levelToLoad = &level;
            break;
        }
    }
    if (!levelToLoad) {
        std::cout<<"Failed to load level "<<levelName<<std::endl;
        return;
    }
    m_World.LoadLevel(*levelToLoad);
}

void RayEngine::Game::RequestLevelChange(const std::string &levelName) {
    m_LevelToLoad = levelName;
    m_bLevelChangeRequested = true;
}

void RayEngine::Game::Init(const GameSpec &gameSpec) {
    m_WindowSize = gameSpec.WindowSize;
    InitWindow(
        m_WindowSize.x,
        m_WindowSize.y,
        gameSpec.WindowTitle.c_str()
        );

    InitAudioDevice();

    m_ClickSound = LoadSound("../Data/Sounds/Cursor.mp3");
    if (!IsSoundValid(m_ClickSound)) {
        std::cout<<"Nie mozna zaladowac pliku kilkniecia"<<std::endl;
    }
    SetSoundVolume(m_ClickSound, 0.2f);
    SetSoundPitch(m_ClickSound, 0.7f);

    m_BackgroundMusic = LoadMusicStream("../Data/Sounds/Background.mp3");
    if (!IsMusicValid(m_BackgroundMusic)) {
        std::cout<<"Nie mozna zaladowac pliku muzyki"<<std::endl;
    }
    PlayMusicStream(m_BackgroundMusic);

    const Vector2 screenHalfSize = m_WindowSize * 0.5;
    m_RenderCamera = {
        .offset = screenHalfSize,
        .target = {0.0f, 0.0f },
        .rotation = 0.0f,
        .zoom = 1.0f
    };
    m_bDebug = false;
    RegisterLevels(m_Levels);
    if (!m_Levels.empty()) {
        LoadLevel(m_Levels[0].GetName());
    }
}

void RayEngine::Game::Shutdown() {
    UnloadSound(m_ClickSound);
    UnloadMusicStream(m_BackgroundMusic);
    CloseAudioDevice();
    CloseWindow();
}

void RayEngine::Game::Run() {
    m_Running = true;
    double deltaTime = 1.0 / 30.0;
    SetTargetFPS(60);
    while (m_Running && !WindowShouldClose()) {
        if (m_Input.GetKey(KeyCode::F10, InputState::Pressed)) {
            m_bDebug = !m_bDebug;
        }

        const double frameStartTime = GetTime();
        m_Input.Handle(m_WindowSize);
        if (m_Input.GetKey(KeyCode::LeftAlt, InputState::Held) &&
            m_Input.GetKey(KeyCode::F4, InputState::Pressed))
        {
            m_Running = false;
        }
        UpdateContext updateContext = {
            .bDebug = m_bDebug,
            .DeltaTime = deltaTime,
            .Input = &m_Input,
            .LevelManager = &m_LevelManager
        };
        Update(updateContext);
        BeginDrawing();
        ClearBackground(GRAY);
        const Vector2 screenHalfSize = m_WindowSize * 0.5f;
        m_RenderCamera.offset = screenHalfSize;
        BeginMode2D(m_RenderCamera);
        RenderContext renderContext = {
            .bDebug = m_bDebug,
            .Camera = &m_RenderCamera
        };
        Render(renderContext);
        EndMode2D();
        RenderUIContext renderUIContext = {
            .bDebug = m_bDebug,
            .ScreenSize = m_WindowSize,
            .Camera = &m_RenderCamera
        };
        RenderUI(renderUIContext);
        EndDrawing();
        const double frameEndTime = GetTime();
        deltaTime = frameEndTime - frameStartTime;
        deltaTime = std::min(deltaTime, 1.0 / 30.0);
        if (deltaTime > 0.0) {
            m_FPS = 1.0 / deltaTime;
        }
        if (m_bLevelChangeRequested) {
            m_bLevelChangeRequested = false;
            LoadLevel(m_LevelToLoad);
        }
    }
}

void RayEngine::Game::Update(const UpdateContext &context) {
    m_World.Update(context);

    PhysicsWorld::Get().Step((float)context.DeltaTime);

    if (IsMusicStreamPlaying(m_BackgroundMusic)) {
        UpdateMusicStream(m_BackgroundMusic);
    }
    if (context.Input->GetKey(KeyCode::Space, InputState::Pressed)) {
        if (IsMusicStreamPlaying(m_BackgroundMusic)) {
            StopMusicStream(m_BackgroundMusic);
        }
        else {
            PlayMusicStream(m_BackgroundMusic);
        }
    }

    if (context.Input->GetKey(KeyCode::Up, InputState::Pressed)) {
        m_MusicVolume += 0.1f;
        SetMusicVolume(m_BackgroundMusic, m_MusicVolume);
    }
    if (context.Input->GetKey(KeyCode::Down, InputState::Pressed)) {
        m_MusicVolume -= 0.1f;
        SetMusicVolume(m_BackgroundMusic, m_MusicVolume);
    }
    if (context.Input->GetKey(KeyCode::Right, InputState::Pressed)) {
        m_MusicPitch += 0.1f;
        SetMusicPitch(m_BackgroundMusic, m_MusicPitch);
    }
    if (context.Input->GetKey(KeyCode::Left, InputState::Pressed)) {
        m_MusicPitch -= 0.1f;
        SetMusicPitch(m_BackgroundMusic, m_MusicPitch);
    }


    m_CursorPosition = context.Input->GetCursorPosition();
    m_cursorWorldPosition = context.Input->GetCursorWorldPosition(m_RenderCamera);
    if (context.Input->GetMouseButton(MouseButton::Left, InputState::Pressed)) {
        m_MouseClickTimer = 0.3f;
        PlaySound(m_ClickSound);
    }
    if (m_MouseClickTimer > 0.0f) {
        m_MouseClickTimer -= context.DeltaTime;
    }
}

void RayEngine::Game::Render(const RenderContext &context) const {
    if (context.bDebug) {
        DrawLine(-1000.0f,0.0f,1000.0f,0.0f,ColorAlpha(RED,0.5f));
        DrawLine(0.0f,-1000.0f,0.0f,1000.0f,ColorAlpha(GREEN,0.5f));
    }
    m_World.Render(context);

    DrawCircle(m_cursorWorldPosition.x, m_cursorWorldPosition.y, 20.0f, ColorAlpha(BLUE,0.5f));
}

void RayEngine::Game::RenderUI(const RenderUIContext & context) const {
    m_World.RenderUI(context);

    char fpsText[32];
    sprintf(fpsText, "FPS: %.0f", m_FPS);

    const int fontSize_fps = 20;
    const int margin = 10;
    Vector2 textSize_fps = MeasureTextEx(GetFontDefault(), fpsText, fontSize_fps, 1.0f);

    DrawText(
        fpsText,
        context.ScreenSize.x - textSize_fps.x - margin,
        margin,
        fontSize_fps,
        BLACK
    );

    const Vector2 screenHalfSize = m_WindowSize * 0.5;
    float circleRadius = 20.0f;
    if (m_MouseClickTimer > 0.0f) {
        circleRadius += 15.0f * std::sin(3.14f * (1.0f - (m_MouseClickTimer / m_MouseClickDuration)));
    }
    Vector2 cursorPosition = m_Input.GetCursorPosition();
    float pixelX = cursorPosition.x * screenHalfSize.x;
    float pixelY = cursorPosition.y * screenHalfSize.y;
    DrawCircle(pixelX, pixelY, circleRadius, ColorAlpha(GREEN,0.3f));

    char musicVolumeText[32];
    sprintf(musicVolumeText, "Volume: %2.1f", m_MusicVolume);
    const int musicVolumeFontSize = 24;
    const Vector2 musicVolumeTextSize = MeasureTextEx(
        GetFontDefault(), musicVolumeText, musicVolumeFontSize, 2.0f
        );
    DrawText(
        musicVolumeText,
        screenHalfSize.x * 2.0f - musicVolumeTextSize.x - 10.0f,
        20.0f + musicVolumeTextSize.y * 0.5f,
        musicVolumeFontSize, BLUE
        );

    char musicPitchText[32];
    sprintf(musicPitchText, "Pitch: %2.1f", m_MusicPitch);
    const int musicPitchFontSize = 24;
    const Vector2 musicPitchTextSize = MeasureTextEx(
        GetFontDefault(), musicPitchText, musicPitchFontSize, 2.0f
        );
    DrawText(
        musicPitchText,
        screenHalfSize.x * 2.0f - musicPitchTextSize.x - 10.0f,
        25.0f + musicPitchTextSize.y * 1.5f,
        musicPitchFontSize, GREEN
        );
}
