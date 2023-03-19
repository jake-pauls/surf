cmake_minimum_required(VERSION 3.0)
project(api)

# Get API dependencies
add_subdirectory(vendor)

# Add tests
add_subdirectory(client/tests)

# Build client API lib
add_subdirectory(client)