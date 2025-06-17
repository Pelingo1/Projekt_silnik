# Define sources
file(GLOB COMPONENTS_SOURCE
        "${SOURCE_DIR}/Game/Components/*.h"
        "${SOURCE_DIR}/Game/Components/*.cpp"
        "${SOURCE_DIR}/Game/Components/*.hpp"
)


target_sources(${PROJECT_NAME} PRIVATE
        ${SOURCE_DIR}/Context.h
        ${CMAKE_SOURCE_DIR}/Source/Game/Component.h

        ${COMPONENTS_SOURCE}
        ${SOURCE_DIR}/UserGame.hpp

        ${SOURCE_DIR}/Game/Entity.cpp
        ${SOURCE_DIR}/Game/Entity.h
        ${SOURCE_DIR}/Game/Level.cpp
        ${SOURCE_DIR}/Game/Level.h
        ${SOURCE_DIR}/Game/LevelManager.cpp
        ${SOURCE_DIR}/Game/LevelManager.h
        ${SOURCE_DIR}/Game/World.cpp
        ${SOURCE_DIR}/Game/World.h
)
