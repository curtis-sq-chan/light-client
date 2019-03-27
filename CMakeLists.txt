cmake_minimum_required (VERSION 3.5)

project (LightClient)

find_package(CURL REQUIRED)
find_package(Threads REQUIRED)

file(GLOB SOURCES "*.cpp" "*.h")
add_library(LightClient ${SOURCES})
target_link_libraries(LightClient curl ${CMAKE_THREAD_LIBS_INIT})
add_executable(tester "main.cpp")
target_link_libraries(tester LightClient)