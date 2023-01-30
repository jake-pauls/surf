cmake_minimum_required(VERSION 3.0)
project(vendor)

# Vulkan
add_library(Vulkan STATIC IMPORTED)
set_target_properties(Vulkan PROPERTIES
    IMPORTED_LOCATION "Vulkan/Lib/vulkan-1.lib"
    INTERFACE_INCLUDE_DIRECTORIES "Vulkan/Include")
message(STATUS "[Vendor] Created library for local include Vulkan")

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