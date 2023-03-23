cmake_minimum_required(VERSION 3.0)
project(api)

# Add compile flag for debug
add_compile_options("$<$<CONFIG:DEBUG>:-D_SURF_DEBUG>")

# Check if profiling is enabled
if(${ENABLE_PROFILING})
    message(STATUS "[api] Enabled profiling")
    add_compile_options("$<$<CONFIG:DEBUG>:-D_PROFILE_ENABLED>")
endif()

# Get API dependencies
add_subdirectory(vendor)

# Add tests
add_subdirectory(client/tests)

# Build client API lib
add_subdirectory(client)