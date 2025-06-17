# Create executable
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
add_executable(${PROJECT_NAME} ${SOURCE_DIR}/Main.cpp
        Game.cpp
        Game.h
        Visuals/AnimatedSprite.cpp
        Visuals/AnimatedSprite.h
        Visuals/CLAMP.h
        Input/Input.cpp
        Input/Input.h
        Input/InputCodes.h
        Game/Entity.cpp
        Game/Entity.h
        Game/Level.cpp
        Game/Level.h
        Game/LevelManager.cpp
        Game/LevelManager.h
        Game/World.cpp
        Game/World.h
        Context.h
        UserGame.hpp
        Game/Component.h
        Game/Components/SpinComponent.hpp
        Game/Components/SpriteComponent.hpp
        Game/Components/SwimmerComponent.hpp
        Game/Components/PhysicsBoxComponent.hpp
        Game/PhysicsWorld.h
        Game/Components/PhysicsCircleComponent.hpp

)

include(${CMAKE_CURRENT_SOURCE_DIR}/Sources.cmake)

# Add libraries include directories
target_include_directories(${PROJECT_NAME} PRIVATE ${lua_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${sol2_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/Libs/Box2D/include)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
        raylib
        Lua_static
        box2d
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Data
        ${CMAKE_BINARY_DIR}/Data
)