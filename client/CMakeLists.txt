project(client)

# Set output directories
set(VENDOR_OUTPUT_DIR ${SURF_OUTPUT_DIR}/client/Vendor)
set(WAVE_OUTPUT_DIR ${SURF_OUTPUT_DIR}/client/Wave/)
set(TESTS_OUTPUT_DIR ${SURF_OUTPUT_DIR}/client/Tests/)

add_subdirectory(Vendor)
add_subdirectory(Wave)
add_subdirectory(Tests)
