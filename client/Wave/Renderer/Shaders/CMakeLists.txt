# Shader Package
set(SHADER_FILES 
    glsl/Triangle.vert glsl/Triangle.frag)

# Add shaders to the source executable
list(TRANSFORM SHADER_FILES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
target_sources(${WAVE_EXECUTABLE_NAME} PRIVATE ${SHADER_FILES})

# Find shader compilers
message(STATUS)
find_program(GLSLC glslc)
if (GLSLC)
    message(STATUS "[Shaders] Found GLSLC -> ${GLSLC}")
else()
    message(WARNING "[Shaders] Could not find GLSLC, GLSL will not be compiled to SPIR-V")
endif()

find_program(DXC dxc)
if (DXC)
    message(STATUS "[Shaders] Found DXC -> ${DXC}")
else()
    message(WARNING "[Shaders] Could not find DXC, HLSL will not be compiled to SPIR-V")
endif()

# Fail build if a valid shader compiler was not found
if (NOT GLSLC AND NOT DXC)
    message(FATAL_ERROR "[Shaders] Couldn't find a valid shader compiler")
endif()

# Compile shaders to SPIR-V
foreach (SHADER_FILE ${SHADER_FILES}) 
    # Retrieve shader file metadata 
    get_filename_component(SHADER_NAME ${SHADER_FILE} NAME)
    get_filename_component(SHADER_EXT ${SHADER_FILE} LAST_EXT)

    # Retrieve the parent directory of the shader file (ie: 'glsl', 'hlsl', etc.)
    get_filename_component(SHADER_DIR ${SHADER_FILE} DIRECTORY)
    cmake_path(GET SHADER_DIR FILENAME SHADER_DIR)

    # Set SPIR-V output info
    # TODO: Change SPIR-V output directory?
    set(CURRENT_SHADER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${SHADER_DIR}/${SHADER_NAME}")
    set(SPIRV_OUTPUT_DIR "${CMAKE_BINARY_DIR}/client/Wave/Shaders")

    # Set Shader output info, append shader dir (ie: glsl, hlsl, cg, etc.) to prevent duplicates
    set(SHADER_OUT_NAME "${SHADER_NAME}.spv")
    set(SHADER_OUTPUT "${SPIRV_OUTPUT_DIR}/${SHADER_OUT_NAME}")
    message(STATUS "[Shaders] (SPIR-V) Adding command to convert ${SHADER_NAME} to ${SHADER_OUT_NAME}")

    # Add a custom command to compile shaders to SPIR-V at build time
    # By default, shaders will only be generated if they a valid shader to SPIR-V compiler is found on the system 
    if (SHADER_DIR STREQUAL "glsl" AND GLSLC)
        add_custom_command(OUTPUT ${SHADER_OUTPUT}
            COMMAND ${CMAKE_COMMAND} -E make_directory "${SPIRV_OUTPUT_DIR}"
            COMMAND ${GLSLC} ${CURRENT_SHADER_PATH} -o ${SHADER_OUTPUT}
            DEPENDS ${CURRENT_SHADER_PATH}
            VERBATIM)
    elseif (SHADER_DIR STREQUAL "hlsl" AND DXC)
        set(SHADER_COMPILER ${DXC})
    endif()

    list(APPEND SPIRV_OUT_FILES ${SHADER_OUTPUT})
endforeach()
message(STATUS)

# Add the generated SPIR-V to a custom target to depend on at build time
# This ensures that the custom commands are properly run
add_custom_target(SPIRV_GEN ALL DEPENDS ${SPIRV_OUT_FILES})
add_dependencies(${WAVE_EXECUTABLE_NAME} SPIRV_GEN)