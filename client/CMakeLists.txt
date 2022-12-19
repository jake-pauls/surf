project(client)

# Main Visual Studio solution name
set(EXECUTABLE_NAME surf)

set(SOURCE_FILES 
    src/Main.cpp)

add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})

# Link the surf DSL API via it's alias, see api/CMakeLists.txt for more details
target_link_libraries(${EXECUTABLE_NAME}
    surf::api)