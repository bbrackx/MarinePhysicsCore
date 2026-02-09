cmake_minimum_required(VERSION 3.20)
project(MarinePhysicsCore LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find raylib installed via Homebrew
find_package(raylib REQUIRED)

# Collect all source files
file(GLOB SOURCES src/*.cpp)

# Build executable
add_executable(marine_sim ${SOURCES})

# Link raylib
target_link_libraries(marine_sim PRIVATE raylib)