# Main Visual Studio solution name
set(WAVE_EXECUTABLE_NAME wave)

# Core Package
set(CORE_SOURCE_FILES
    Log.cpp
    Asserts.cpp
    StringHelpers.cpp
    FileHelpers.cpp)

set(CORE_HEADER_FILES
    Define.h
    Asserts.h
    Log.h
    Memory.h
    StringHelpers.h
    FileHelpers.h
    FileSystem.h)

list(TRANSFORM CORE_SOURCE_FILES PREPEND "Core/")
list(TRANSFORM CORE_HEADER_FILES PREPEND "Core/")

# Engine Package
set(ENGINE_SOURCE_FILES
    Main.cpp
    Application.cpp
	Window.cpp)

set(ENGINE_HEADER_FILES
    Application.h
	Window.h)

list(TRANSFORM ENGINE_SOURCE_FILES PREPEND "Engine/")
list(TRANSFORM ENGINE_HEADER_FILES PREPEND "Engine/")

# Renderer Package
set(RENDERER_SOURCE_FILES
    Renderer.cpp
    RendererContext.cpp)

set(RENDERER_HEADER_FILES
    Renderer.h
    RendererContext.h
    Shader.h)

list(TRANSFORM RENDERER_SOURCE_FILES PREPEND "Renderer/")
list(TRANSFORM RENDERER_HEADER_FILES PREPEND "Renderer/")

# Renderer/Vulkan Package
set(VKRENDERER_SOURCE_FILES
    VkInitializers.cpp
	VkSwapChain.cpp
	VkHardware.cpp
	VkRenderer.cpp
    VkRendererContext.cpp
    VkShaderPipeline.cpp
    VkPass.cpp
    VkMesh.cpp)

set(VKRENDERER_HEADER_FILES
    VkInitializers.h
	VkSwapChain.h
	VkHardware.h
	VkRenderer.h
    VkRendererContext.h
    VkShaderPipeline.h
    VkPass.h
    VkMesh.h
    VkTypes.h)

list(TRANSFORM VKRENDERER_SOURCE_FILES PREPEND "Renderer/Vulkan/")
list(TRANSFORM VKRENDERER_HEADER_FILES PREPEND "Renderer/Vulkan/")

# Executable definition
add_executable(${WAVE_EXECUTABLE_NAME} wpch.h
    ${CORE_SOURCE_FILES} ${CORE_HEADER_FILES}
    ${ENGINE_SOURCE_FILES} ${ENGINE_HEADER_FILES}
    ${RENDERER_SOURCE_FILES} ${RENDERER_HEADER_FILES}
	${VKRENDERER_SOURCE_FILES} ${VKRENDERER_HEADER_FILES})

# Compile and add shader files
add_subdirectory(Renderer/Shaders)

# Groups for Visuals Studio filters
source_group("Core" FILES ${CORE_SOURCE_FILES} ${CORE_HEADER_FILES})
source_group("Engine" FILES ${ENGINE_SOURCE_FILES} ${ENGINE_HEADER_FILES})
source_group("Renderer" FILES ${RENDERER_SOURCE_FILES} ${RENDERER_HEADER_FILES})
source_group("Renderer/Vulkan" FILES ${VKRENDERER_SOURCE_FILES} ${VKRENDERER_HEADER_FILES})

# Source groups can only be added relative to the directory the target is created in? Nice CMake.
source_group("Renderer/Shaders" REGULAR_EXPRESSION ".*\.vert|.*\.frag")

target_precompile_headers(${WAVE_EXECUTABLE_NAME} PRIVATE wpch.h)

target_include_directories(${WAVE_EXECUTABLE_NAME}
    PRIVATE Core
    PRIVATE Engine
	PRIVATE Renderer
    # Vendor
	PRIVATE ${Vulkan_INCLUDE_DIR})

# Link the surf DSL API via it's alias, see api/CMakeLists.txt for more details
target_link_libraries(${WAVE_EXECUTABLE_NAME}
    surf::api
    # Vendor
    SDL3::SDL3
	${Vulkan_LIBRARY}
    VulkanMemoryAllocator
    glm
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

# Post-build: Create 'Vendor' directory for VS 
set_target_properties(SDL3 VulkanMemoryAllocator PROPERTIES FOLDER Vendor)