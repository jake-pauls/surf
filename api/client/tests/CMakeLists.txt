enable_testing()

set(TEST_EXECUTABLE_NAME api-tests)

set(TEST_SOURCE_FILES Main.c 
    Unit/Suite_SymbolTable.c
    Unit/Suite_Interp.c
    Unit/Suite_Vec.c)

set(TEST_HEADER_FILES Unit/Suite_SymbolTable.h
    Unit/Suite_Interp.h
    Unit/Suite_Vec.h)

add_executable(${TEST_EXECUTABLE_NAME} ${TEST_SOURCE_FILES} ${TEST_HEADER_FILES})

# Place executable in the config directory on unix platforms 
if(UNIX)
    set_target_properties(${TEST_EXECUTABLE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/")
endif()

target_link_libraries(${TEST_EXECUTABLE_NAME} surf::api check)

# TODO: This kind of being hack 
target_include_directories(${TEST_EXECUTABLE_NAME} 
    PUBLIC ${CURRENT_PROJECT_DIR}/tests
    PUBLIC ${CURRENT_PROJECT_DIR}/client/include
    PRIVATE ${SURF_OUTPUT_DIR}/api/vendor/check
    PRIVATE ${SURF_OUTPUT_DIR}/api/vendor/check/src)

# Tests for this project can be run via CTest 
# Run 'ctest' in the build/api folder
add_test(api_tests ${TEST_EXECUTABLE_NAME})