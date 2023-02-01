cmake_minimum_required(VERSION 3.0)
project(vendor)

# Vulkan
find_package(Vulkan REQUIRED)
if (${Vulkan_FOUND})
	message(STATUS "[Vendor] Found Vulkan include path -> '${Vulkan_INCLUDE_DIR}'")
	message(STATUS "[Vendor] Found Vulkan libraries ->  '${Vulkan_LIBRARY}'")
else()
	message(WARNING "[Vendor] Failed to find the Vulkan SDK on the system! Do you have it installed?")
endif()

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
target_include_directories(glm INTERFACE glm/glm)
message(STATUS "[Vendor] Created library for submodule glm")

# SDL
add_subdirectory(SDL EXCLUDE_FROM_ALL)
message(STATUS "[Vendor] Recursed and collected libraries from SDL3")
