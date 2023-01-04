# Main Visual Studio solution name
set(WAVE_EXECUTABLE_NAME wave)

# Core Package
set(CORE_SOURCE_FILES
    Core/Log.cpp)

set(CORE_HEADER_FILES
    wpch.h
    Core/Define.h
    Core/Assert.h
    Core/Log.h)

# Engine Package
set(ENGINE_SOURCE_FILES 
    Engine/Main.cpp)

# Executable definition
add_executable(${WAVE_EXECUTABLE_NAME} 
    ${CORE_SOURCE_FILES} ${CORE_HEADER_FILES}
    ${ENGINE_SOURCE_FILES})

# Groups for Visuals Studio filters
source_group("Core" FILES ${CORE_SOURCE_FILES})
source_group("Engine" FILES ${ENGINE_SOURCE_FILES})

target_precompile_headers(${WAVE_EXECUTABLE_NAME} PRIVATE wpch.h)

target_include_directories(${WAVE_EXECUTABLE_NAME} 
    PRIVATE Core
    PRIVATE Engine)

# Link the surf DSL API via it's alias, see api/CMakeLists.txt for more details
target_link_libraries(${WAVE_EXECUTABLE_NAME} 
    surf::api
    SDL3::SDL3
    spdlog)
