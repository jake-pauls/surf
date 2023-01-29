cmake_minimum_required(VERSION 3.0)
project(vendor)

# Vulkan
find_package(Vulkan REQUIRED COMPONENTS dxc)
message(STATUS "")
if (${Vulkan_FOUND})
    message(STATUS "[Vendor] Vulkan Information")
    message(STATUS "[Vendor] Vulkan include dir: ${Vulkan_INCLUDE_DIR}")
    message(STATUS "[Vendor] Vulkan library path: ${Vulkan_LIBRARIES}")

    if ((${Vulkan_dxc_lib_FOUND}) OR (${Vulkan_dxc_exe_FOUND}))
        message(STATUS "[Vendor] DXC Information")
        message(STATUS "  This is some information for DXC, for some reason Vulkan checks for a DXC debug library that is never configured in FindVulkan.")
        message(STATUS "  If it hasn't been fixed/changed you might still see a warning above this.")
        message(STATUS "  Note #1: This DXC library is not enabled to work with DirectX by default, visa versa the Visual Studio DXC is not enabled to work with Vulkan.")
        message(STATUS "  Why can't they be enabled for both APIs? Well. Capitalism probably. Read this: https://vulkan.lunarg.com/doc/view/latest/windows/DXC.html")
        message(STATUS "  Note #2: Compile and link DXC manually for both APIs to be compatable with it.")
        message(STATUS "[Vendor] DXC lib path: ${Vulkan_dxc_LIBRARY}")
        message(STATUS "[Vendor] DXC executable path: ${Vulkan_dxc_EXECUTABLE}")
    endif()
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
message(STATUS "")

# SDL
add_subdirectory(SDL EXCLUDE_FROM_ALL)
message(STATUS "[Vendor] Recursed and collected libraries from SDL3")