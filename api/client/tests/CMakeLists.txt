enable_testing()

set(TEST_EXECUTABLE_NAME api-tests)

add_executable(${TEST_EXECUTABLE_NAME} Main.c)

# Place executable in the config directory on unix platforms 
if(UNIX)
    set_target_properties(${TEST_EXECUTABLE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/")
endif()

target_link_libraries(${TEST_EXECUTABLE_NAME} check)

# TODO: This kind of a hack 
target_include_directories(${TEST_EXECUTABLE_NAME} 
    PUBLIC ${SURF_OUTPUT_DIR}/api/vendor/check
    PUBLIC ${SURF_OUTPUT_DIR}/api/vendor/check/src)

# Tests for this project can be run via CTest 
# Run 'ctest' in the build/api folder
add_test(api_tests ${TEST_EXECUTABLE_NAME})