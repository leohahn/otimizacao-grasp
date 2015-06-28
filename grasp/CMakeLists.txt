cmake_minimum_required(VERSION 2.6)
project(grasp)

add_executable(grasp src/main.cpp)

install(TARGETS grasp RUNTIME DESTINATION bin)
