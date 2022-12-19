project(client)

##
##  Dependencies
##

add_subdirectory(vendor)

##
##  Client Executable
##

# Main Visual Studio solution name
set(CLIENT_EXECUTABLE_NAME surf)

set(SOURCE_FILES 
    src/Main.cpp)

add_executable(${CLIENT_EXECUTABLE_NAME} ${SOURCE_FILES})

# Link the surf DSL API via it's alias, see api/CMakeLists.txt for more details
target_link_libraries(${CLIENT_EXECUTABLE_NAME} surf::api)

##
##  Client Test Executable
##

enable_testing()

set(TEST_EXECUTABLE_NAME surf-tests)

set(TEST_SOURCE_FILES
    test/ExampleTest.cpp)

add_executable(${TEST_EXECUTABLE_NAME} 
    test/Main.cpp 
    ${TEST_SOURCE_FILES})

target_link_libraries(${TEST_EXECUTABLE_NAME} 
    surf::api
    Catch2)

# Tests for this project can be run via CTest 
# Run 'ctest' in the build/client folder
add_test(client_tests ${TEST_EXECUTABLE_NAME})