# Shader Package
set(SHADER_FILES 
    glsl/Triangle.vert glsl/Triangle.frag
    glsl/TriangleMesh.vert glsl/TriangleMesh.frag
    hlsl/Triangle.vert hlsl/Triangle.frag
    hlsl/TriangleMesh.vert hlsl/TriangleMesh.frag)

# Add shaders to the source executable
list(TRANSFORM SHADER_FILES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
target_sources(${WAVE_EXECUTABLE_NAME} PRIVATE ${SHADER_FILES})

# Check for GLSLC
message(STATUS)
find_program(GLSLC glslc)
if (GLSLC)
    message(STATUS "[Shaders] Found GLSLC -> ${GLSLC}")
else()
    message(WARNING "[Shaders] Could not find GLSLC, GLSL will not be compiled to SPIR-V")
endif()

# Check for DXC (hlsl)
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
    set(CURRENT_SHADER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${SHADER_DIR}/${SHADER_NAME}")
    set(SPIRV_OUTPUT_DIR "${CMAKE_BINARY_DIR}/client/Wave/")

    # Set Shader output info, append shader dir (ie: glsl, hlsl, cg, etc.) to prevent duplicates
    set(SHADER_OUT_NAME "${SHADER_NAME}.${SHADER_DIR}.spv")
    message(STATUS "[Shaders] (SPIR-V) Adding command to convert ${SHADER_NAME} to ${SHADER_OUT_NAME}")

    # Add a custom command to compile shaders to SPIR-V at build time
    # By default, shaders will only be generated if a valid SPIR-V compiler is found on the system 
    if (SHADER_DIR STREQUAL "glsl" AND GLSLC)
        add_custom_command(OUTPUT "${SPIRV_OUTPUT_DIR}/$<CONFIG>/Shaders/${SHADER_OUT_NAME}"
            COMMAND ${CMAKE_COMMAND} -E make_directory "${SPIRV_OUTPUT_DIR}"
            COMMAND ${GLSLC} ${CURRENT_SHADER_PATH} -o "${SPIRV_OUTPUT_DIR}/$<CONFIG>/Shaders/${SHADER_OUT_NAME}"
            DEPENDS ${CURRENT_SHADER_PATH}
            VERBATIM)
    elseif (SHADER_DIR STREQUAL "hlsl" AND DXC)
        # Check the shader extension in advance for hlsl compilation
        if (SHADER_EXT STREQUAL ".vert")
            set(IS_VERT TRUE)
        elseif (SHADER_EXT STREQUAL ".frag")
            set(IS_VERT FALSE)
        endif()

        add_custom_command(OUTPUT "${SPIRV_OUTPUT_DIR}/$<CONFIG>/Shaders/${SHADER_OUT_NAME}"
            COMMAND ${CMAKE_COMMAND} -E make_directory "${SPIRV_OUTPUT_DIR}"
            COMMAND ${DXC} -spirv -T $<IF:$<BOOL:${IS_VERT}>,vs_6_0,ps_6_0>   # Ensure the proper target is used for either vertex or pixel shaders
                -E main ${CURRENT_SHADER_PATH} -Fo "${SPIRV_OUTPUT_DIR}/$<CONFIG>/Shaders/${SHADER_OUT_NAME}"
            DEPENDS ${CURRENT_SHADER_PATH}
            VERBATIM)
    endif()

    list(APPEND SPIRV_OUT_FILES "${SPIRV_OUTPUT_DIR}/$<CONFIG>/Shaders/${SHADER_OUT_NAME}")
endforeach()
message(STATUS)

# Add the generated SPIR-V to a custom target to depend on at build time
# This ensures that the custom commands are properly run
add_custom_target(SPIRV_GEN ALL DEPENDS ${SPIRV_OUT_FILES})
add_dependencies(${WAVE_EXECUTABLE_NAME} SPIRV_GEN)