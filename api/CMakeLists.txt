cmake_minimum_required(VERSION 3.0)
project(api)

# Add compile flag for debug
add_compile_options("$<$<CONFIG:DEBUG>:-D_SURF_DEBUG>")

# Allow profiling in 'Debug'
add_compile_options("$<$<CONFIG:DEBUG>:-DMTR_ENABLED>")

# Get API dependencies
add_subdirectory(vendor)

# Add tests
add_subdirectory(client/tests)

# Build client API lib
add_subdirectory(client)