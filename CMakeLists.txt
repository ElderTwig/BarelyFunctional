#---------------------------CMake Project Config-------------------------------
cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0091 NEW)

project(BarelyFunctional LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

if(NOT CMAKE_BUILD_TYPE)
    message("Setting build configuration to Release.")
    message("To change, configure with -DCMAKE_BUILD_TYPE=Release/Debug/etc...")

    set(CMAKE_BUILD_TYPE Release CACHE 
        STRING "Sets the configuration to build (Release, Debug, etc...)")
endif()

option(CLANG_TIDY "Run clang-tidy on tests" OFF)
option(BARELY_BUILD_TESTING OFF)

#---------------------------Compilation database generation--------------------
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(TOUCH ${CMAKE_BINARY_DIR}/compile_commands.json)
file(REMOVE ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json)

configure_file(${CMAKE_BINARY_DIR}/compile_commands.json 
    ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
    COPYONLY)

#---------------------------git submodule setup--------------------------------
option(BARELY_UPDATE_SUBMODULES "Update git submodules before configuration" ON)

if(BARELY_UPDATE_SUBMODULES)
  include(UpdateSubmodules)
endif()

#---------------------------CCache setup---------------------------------------
find_program(CCACHE_PROGRAM ccache)

if(CCACHE_PROGRAM)
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
endif()

#---------------------------Catch2---------------------------------------------
if(BARELY_BUILD_TESTING)
    find_package(Catch2 QUIET)

    if(NOT Catch2_FOUND)
      add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/extern/Catch2)
    endif()
endif()

#---------------------------Barely::Functional--------------------------------------
add_library(BarelyFunctional INTERFACE)

target_include_directories(BarelyFunctional
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(Barely::Functional ALIAS BarelyFunctional)

#---------------------------tests----------------------------------------------
if(BARELY_BUILD_TESTING)
    add_subdirectory(tests)
endif()
