cmake_minimum_required(VERSION 3.0)
project(vendor)

# Vulkan
find_package(Vulkan REQUIRED)
if(${Vulkan_FOUND})
	message(STATUS "[Vendor] Found Vulkan include path -> '${Vulkan_INCLUDE_DIR}'")
	message(STATUS "[Vendor] Found Vulkan libraries ->  '${Vulkan_LIBRARY}'")
else()
	message(WARNING "[Vendor] Failed to find the Vulkan SDK on the system! Do you have it installed?")
endif()

# VMA
add_subdirectory(VulkanMemoryAllocator EXCLUDE_FROM_ALL)
message(STATUS "[Vendor] Recursed and collected libraries from VulkanMemoryAllocator")

# Catch2
add_library(Catch2 INTERFACE)
target_include_directories(Catch2 INTERFACE Catch2/single_include)
message(STATUS "[Vendor] Created library for submodule Catch2")

# spdlog
add_library(spdlog INTERFACE)
target_include_directories(spdlog INTERFACE spdlog/include)
message(STATUS "[Vendor] Created library for submodule spdlog")

# glm
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE glm)
message(STATUS "[Vendor] Created library for submodule glm")

# stb
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE stb)
message(STATUS "[Vendor] Created library for submodule stb")

# SDL
add_subdirectory(SDL EXCLUDE_FROM_ALL)
message(STATUS "[Vendor] Recursed and collected libraries from SDL3")

# tinyobjloader 
add_subdirectory(tinyobjloader EXCLUDE_FROM_ALL)
message(STATUS "[Vendor] Recursed and collected libraries from tinyobjloader")

# imgui
add_library(imgui STATIC)
target_include_directories(imgui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/imgui"
	"${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends")
target_sources(imgui PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_demo.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_draw.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_widgets.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_tables.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_vulkan.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_sdl3.cpp")
target_link_libraries(imgui PUBLIC ${Vulkan_LIBRARY} SDL3::SDL3)
target_include_directories(imgui PUBLIC ${Vulkan_INCLUDE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/SDL/include" stb)
message(STATUS "[Vendor] Collected libraries from imgui")

# Add to 'Vendor' directory for VS 
set_target_properties(SDL3-shared VulkanMemoryAllocator tinyobjloader imgui PROPERTIES FOLDER Vendor)