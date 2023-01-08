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
    Engine/Main.cpp
    Engine/Application.cpp
	Engine/Window.cpp)

set(ENGINE_HEADER_FILES
    Engine/Application.h
	Engine/Window.h)

# Executable definition
add_executable(${WAVE_EXECUTABLE_NAME}
    ${CORE_SOURCE_FILES} ${CORE_HEADER_FILES}
    ${ENGINE_SOURCE_FILES} ${ENGINE_HEADER_FILES})

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

# Platform specific compiler flags
if(MSVC)
	target_compile_options(${WAVE_EXECUTABLE_NAME} PRIVATE /WX)
else()
	target_compile_options(${WAVE_EXECUTABLE_NAME} PRIVATE -pthread)
endif()

# Post-build: copy the SDL dll into the build folder
if (WIN32)
    add_custom_command(TARGET ${WAVE_EXECUTABLE_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${VENDOR_OUTPUT_DIR}/SDL/$<CONFIGURATION>/SDL3.dll"
        $<TARGET_FILE_DIR:${WAVE_EXECUTABLE_NAME}>)
endif()