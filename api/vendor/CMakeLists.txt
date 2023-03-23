cmake_minimum_required(VERSION 3.0)
project(api-vendor)

add_subdirectory(check EXCLUDE_FROM_ALL)
message(STATUS "[api-vendor] Loaded check as a dependency")

add_subdirectory(minitrace EXCLUDE_FROM_ALL)
message(STATUS "[api-vendor] Loaded minitrace as a dependency")

# Add to 'Vendor' directory for VS 
set_target_properties(check minitrace PROPERTIES FOLDER Vendor)