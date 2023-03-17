project(api)

set(SOURCE_FILES
    client/src/Bridge.c
    client/src/Interp.c
    client/src/SymbolTable.c
    client/src/Utils.c)

set(HEADER_FILES
    client/include/surf/Bridge.h
    client/include/surf/Interp.h
    client/include/surf/SymbolTable.h
    client/include/surf/Utils.h
    client/include/surf/Algorithm.h
    client/include/surf/Define.h
    client/include/surf/surf.h)

# Configure the API library with a suitable alias
add_library(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})
add_library(surf::api ALIAS ${PROJECT_NAME})

# Link WinSock on Win32
if(WIN32)
    target_link_libraries(${PROJECT_NAME} Ws2_32)
endif()

# Add the applicable include target
target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/client/include)