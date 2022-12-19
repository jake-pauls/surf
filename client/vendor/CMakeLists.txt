cmake_minimum_required(VERSION 3.0)

project(vendor)

# Catch2
add_library(Catch2 INTERFACE)
target_include_directories(Catch2 INTERFACE Catch2/single_include)