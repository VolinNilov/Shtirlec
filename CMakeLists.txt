# Minimum CMake version.
cmake_minimum_required(VERSION 3.7.2)

# Create project.
project(Shtirlec)

# Headers directory
include_directories(include)

# Adding source files.
# set(SOURCES source/main.cpp) # - Manually.
file(GLOB_RECURSE CPP_SOURCES "source/*.cpp") # - Automatically.
file(GLOB_RECURSE C_SOURCES "source/*.c") # - Automatically.

add_executable(Shtirlec ${CPP_SOURCES} ${C_SOURCES}) # Using variable SOURCES.

# Flags for builds
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wpedantic -Wextra -fexceptions -O0 -g3 -ggdb --std=c++17")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Wextra -O3 --std=c++17")

# Libraries
#target_link_libraries(Shtirlec serial)
target_link_libraries(Shtirlec pthread)
target_link_libraries(Shtirlec pqxx)
