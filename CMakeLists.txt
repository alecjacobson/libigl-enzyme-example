cmake_minimum_required(VERSION 3.16)
project(example)

list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Libigl
include(libigl)
# Enable the target igl::glfw
igl_include(glfw)

if(NOT DEFINED LLVM_DIR AND APPLE)
  # Use CMake script to find brew llvm's prefix
  execute_process(COMMAND brew --prefix llvm OUTPUT_VARIABLE BREW_LLVM_PREFIX OUTPUT_STRIP_TRAILING_WHITESPACE)
  # Assuming LLVM is installed via brew, construct the likely path
  set(Possible_LLVM_DIR "${BREW_LLVM_PREFIX}/lib/cmake/llvm")

  # Check if the constructed path exists and set it as LLVM_DIR
  if(EXISTS "${Possible_LLVM_DIR}")
    set(LLVM_DIR "${Possible_LLVM_DIR}")
  else()
    message(WARNING "LLVM_DIR not set and default brew path does not exist. Please set LLVM_DIR manually.")
  endif()
endif()

# find llvm
find_package(LLVM REQUIRED CONFIG)

# use llvm's clang as the compiler
set(CMAKE_CXX_COMPILER "${LLVM_INSTALL_PREFIX}/bin/clang++")
set(CMAKE_C_COMPILER "${LLVM_INSTALL_PREFIX}/bin/clang")

if(NOT DEFINED ENZYME_DIR AND APPLE)
  # Use CMake script to find brew enzyme's prefix
  execute_process(COMMAND brew --prefix enzyme OUTPUT_VARIABLE BREW_ENZYME_PREFIX OUTPUT_STRIP_TRAILING_WHITESPACE)
  # Assuming ENZYME is installed via brew, construct the likely path
  set(Possible_ENZYME_DIR "${BREW_ENZYME_PREFIX}/lib/cmake/Enzyme")

  # Check if the constructed path exists and set it as ENZYME_DIR
  if(EXISTS "${Possible_ENZYME_DIR}")
    set(ENZYME_DIR "${Possible_ENZYME_DIR}")
  else()
    message(WARNING "ENZYME_DIR not set and default brew path does not exist. Please set ENZYME_DIR manually.")
  endif()
endif()

# Is this find_package helping?
find_package(Enzyme REQUIRED CONFIG)
# I'm not sure why I need to construct the flags manually
set(ClangEnzyme_SHARED_LIB "${ENZYME_DIR}/../../ClangEnzyme-${Enzyme_LLVM_VERSION_MAJOR}${CMAKE_SHARED_LIBRARY_SUFFIX}")
set(ENZYME_PLUGIN_FLAG "-fplugin=${ClangEnzyme_SHARED_LIB}")
message(STATUS "ENZYME_PLUGIN_FLAG: ${ENZYME_PLUGIN_FLAG}")
# add to CMAKE_CXX_FLAGS
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ENZYME_PLUGIN_FLAG}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${ENZYME_PLUGIN_FLAG}")



# add the executable
add_executable(${PROJECT_NAME} example.cpp)
# Link igl (and the glfw module) to your project
target_link_libraries(${PROJECT_NAME} PUBLIC igl::glfw)
