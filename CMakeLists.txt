cmake_minimum_required(VERSION 3.6)
project(surf
    VERSION 0.0.1
    LANGUAGES CXX C)


# C17
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

# C++ 20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the 'wave' engine as the default startup project
set_directory_properties(PROPERTIES VS_STARTUP_PROJECT wave)

# Set a custom define for debug configurations
add_compile_options("$<$<CONFIG:DEBUG>:-D_WAVE_DEBUG>")

add_subdirectory(api)
add_subdirectory(client)