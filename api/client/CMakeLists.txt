set(SOURCE_FILES
    src/Bridge.c
    src/Interp.c
    src/Cfg.c
    src/HashTable.c
    src/Argpack.c
    src/Vec.c
    src/Algorithm.c
    src/Utils.c)

set(HEADER_FILES
    include/surf/Bridge.h
    include/surf/Interp.h
    include/surf/Cfg.h
    include/surf/HashTable.h
    include/surf/Utils.h
    include/surf/Argpack.h
    include/surf/Vec.h
    include/surf/Algorithm.h
    include/surf/Define.h
    include/surf/surf.h)

# Configure the API library with a suitable alias
add_library(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})
add_library(surf::api ALIAS ${PROJECT_NAME})

# Link WinSock on Win32
if(WIN32)
    target_link_libraries(${PROJECT_NAME} Ws2_32 minitrace)
endif()

# Add the applicable include targets
# TODO: This is a complete hack
target_include_directories(${PROJECT_NAME} PUBLIC include)