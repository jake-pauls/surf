cmake_minimum_required(VERSION 3.0)
project(api-vendor)

add_subdirectory(check EXCLUDE_FROM_ALL)
message(STATUS "[API-Vendor] Loaded Check as a dependency")

# Add to 'Vendor' directory for VS 
set_target_properties(check PROPERTIES FOLDER Vendor)