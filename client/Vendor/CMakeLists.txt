cmake_minimum_required(VERSION 3.0)
project(vendor)

find_package(Vulkan REQUIRED)

# Catch2
add_library(Catch2 INTERFACE)
target_include_directories(Catch2 INTERFACE Catch2/single_include)

# SDL
add_subdirectory(SDL EXCLUDE_FROM_ALL)

# spdlog
add_library(spdlog INTERFACE)
target_include_directories(spdlog INTERFACE spdlog/include)

# Vulkan
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE glm/glm)
