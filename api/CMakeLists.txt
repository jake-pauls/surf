project(api)

set(SOURCE_FILES
    client/src/Bridge.c)

set(HEADER_FILES
    client/include/surf/Bridge.h
    client/include/surf/surf.h)

# Configure the API library with a suitable alias
add_library(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})
add_library(surf::api ALIAS ${PROJECT_NAME})

# Add the applicable include target
target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/client/include)