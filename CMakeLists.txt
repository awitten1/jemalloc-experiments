cmake_minimum_required(VERSION 3.22)

project(jemalloc-experiments LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(gflags REQUIRED)
find_package(Threads)

set(JEMALLOC_INSTALL_DIR "" CACHE STRING "jemalloc installation directory")
if (NOT ${JEMALLOC_INSTALL_DIR} STREQUAL "")
    set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${JEMALLOC_INSTALL_DIR})
endif()

find_path(JEMALLOC_INCLUDE_DIR jemalloc REQUIRED)
find_library(JEMALLOC_LIBRARY jemalloc REQUIRED)
add_library(jemalloc INTERFACE)
target_link_libraries(jemalloc INTERFACE ${JEMALLOC_LIBRARY})
target_include_directories(jemalloc INTERFACE ${JEMALLOC_INCLUDE_DIR})

add_executable(memsetVsMadvise memsetVsMadvise.cpp)
set_property(TARGET memsetVsMadvise PROPERTY CXX_STANDARD 11)
target_link_libraries(memsetVsMadvise PRIVATE ${CMAKE_THREAD_LIBS_INIT} gflags)

add_executable(
    stress 
    stress_test/Main.cpp 
    stress_test/Producers.cpp 
    stress_test/Mixer.cpp 
    stress_test/ThreadObject.cpp 
    stress_test/Distribution.cpp 
    stress_test/Allocation.cpp
)

set_property(TARGET stress PROPERTY CXX_STANDARD 14)
target_link_libraries(stress PRIVATE gflags jemalloc m ${CMAKE_THREAD_LIBS_INIT})

add_executable(fillFlush fillFlush.cpp)
set_property(TARGET fillFlush PROPERTY CXX_STANDARD 11)
target_link_libraries(fillFlush PRIVATE gflags jemalloc ${CMAKE_THREAD_LIBS_INIT})
