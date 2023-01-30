# Main Visual Studio solution name
set(WAVE_EXECUTABLE_NAME wave)

# Core Package
set(CORE_SOURCE_FILES
    Core/Log.cpp
    Core/Asserts.cpp
    Core/StringHelpers.cpp)

set(CORE_HEADER_FILES
    wpch.h
    Core/Define.h
    Core/Asserts.h
    Core/Log.h
    Core/Memory.h
    Core/StringHelpers.h)

# Engine Package
set(ENGINE_SOURCE_FILES
    Engine/Main.cpp
    Engine/Application.cpp
	Engine/Window.cpp)

set(ENGINE_HEADER_FILES
    Engine/Application.h
	Engine/Window.h)

# Renderer Package
set(RENDERER_SOURCE_FILES
    Renderer/IRenderer.cpp
    Renderer/IRendererContext.cpp)

set(RENDERER_HEADER_FILES
    Renderer/IRenderer.h
    Renderer/IRendererContext.h)

# Renderer/Vulkan Package
set(VKRENDERER_SOURCE_FILES
	Renderer/Vulkan/VkRenderer.cpp
    Renderer/Vulkan/VkRendererContext.cpp)

set(VKRENDERER_HEADER_FILES
	Renderer/Vulkan/VkRenderer.h
    Renderer/Vulkan/VkRendererContext.h)

# Codegen Package
file(GLOB_RECURSE CODEGEN_FILES Codegen/*.h)

# Executable definition
add_executable(${WAVE_EXECUTABLE_NAME}
    ${CORE_SOURCE_FILES} ${CORE_HEADER_FILES}
    ${ENGINE_SOURCE_FILES} ${ENGINE_HEADER_FILES}
    ${RENDERER_SOURCE_FILES} ${RENDERER_HEADER_FILES}
	${VKRENDERER_SOURCE_FILES} ${VKRENDERER_HEADER_FILES}
    ${CODEGEN_FILES})

# Groups for Visuals Studio filters
source_group("Core" FILES ${CORE_SOURCE_FILES} ${CORE_HEADER_FILES})
source_group("Engine" FILES ${ENGINE_SOURCE_FILES} ${ENGINE_HEADER_FILES})
source_group("Renderer" FILES ${RENDERER_SOURCE_FILES} ${RENDERER_HEADER_FILES})
source_group("Renderer/Vulkan" FILES ${VKRENDERER_SOURCE_FILES} ${VKRENDERER_HEADER_FILES})
source_group("Codegen" FILES ${CODEGEN_FILES})

target_precompile_headers(${WAVE_EXECUTABLE_NAME} PRIVATE wpch.h)

target_include_directories(${WAVE_EXECUTABLE_NAME}
    PRIVATE Core
    PRIVATE Engine
	PRIVATE Renderer
    PRIVATE Codegen
    # Vendor
    PRIVATE "${CMAKE_SOURCE_DIR}/client/Vendor/Vulkan/Include")

# Link the surf DSL API via it's alias, see api/CMakeLists.txt for more details
target_link_libraries(${WAVE_EXECUTABLE_NAME}
    surf::api
    SDL3::SDL3
    # Vendor
    spdlog
    "${CMAKE_SOURCE_DIR}/client/Vendor/Vulkan/Lib/vulkan-1.lib")

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
