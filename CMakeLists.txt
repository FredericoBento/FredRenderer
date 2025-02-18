cmake_minimum_required(VERSION 3.10)

project(Renderer VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_EXPORT_ADD_DEFINITIONS ON)
set(CMAKE_CXX_FLAGS "-pg")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")

include_directories(include)

add_executable(Renderer
  src/main.cpp
  src/tgaimage.cpp
)

target_include_directories(Renderer PUBLIC
    include/
)
