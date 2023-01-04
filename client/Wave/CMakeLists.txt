# Main Visual Studio solution name
set(WAVE_EXECUTABLE_NAME wave)

# Core
set(CORE_SOURCE_FILES
    Core/Define.h)

# Engine
set(ENGINE_SOURCE_FILES 
    Engine/Main.cpp)

# Executable definition
add_executable(${WAVE_EXECUTABLE_NAME} 
    ${CORE_SOURCE_FILES} 
    ${ENGINE_SOURCE_FILES})

# Groups for Visuals Studio filters
source_group("Core" FILES ${CORE_SOURCE_FILES})
source_group("Engine" FILES ${ENGINE_SOURCE_FILES})

# Link the surf DSL API via it's alias, see api/CMakeLists.txt for more details
target_link_libraries(${WAVE_EXECUTABLE_NAME} 
    surf::api
    SDL3::SDL3)
