cmake_minimum_required(VERSION 2.6)
project(grasp)

# Set flags for compilation
file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Set flags for compilation
set(CMAKE_CXX_FLAGS "-Wall -std=gnu++11 -g")


# Add files in SRC_FILES in the executable
add_executable(grasp ${SRC_FILES})

install(TARGETS grasp RUNTIME DESTINATION bin)
