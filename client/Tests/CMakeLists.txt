enable_testing()

set(TEST_EXECUTABLE_NAME wave-tests)

set(UNIT_SOURCE_FILES
    Unit/ExampleTest.cpp)

add_executable(${TEST_EXECUTABLE_NAME} 
    Main.cpp 
    ${UNIT_SOURCE_FILES})

# Place executable in the config directory on unix platforms 
if (UNIX)
    set_target_properties(${TEST_EXECUTABLE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/")
endif()

target_link_libraries(${TEST_EXECUTABLE_NAME} 
    surf::api
    Catch2)

# Tests for this project can be run via CTest 
# Run 'ctest' in the build/client folder
add_test(client_tests ${TEST_EXECUTABLE_NAME})