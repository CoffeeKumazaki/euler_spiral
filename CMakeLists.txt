## Use the variable PROJECT_NAME for changing the target name
get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
set( PROJECT_NAME ${ProjectId} )

cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
project(${PROJECT_NAME})

# include_directories(include)

find_package(OpenGL REQUIRED)
find_package(boost REQUIRED)
add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GLEW)
include_directories(/usr/local/opt/glew/include)
include_directories(/opt/homebrew/opt/glew/include)
include_directories(/usr/local/opt/glfw/include)
include_directories(/opt/homebrew/opt/glfw/include)
link_directories(/usr/local/lib)
link_directories(/opt/homebrew/lib)

## Define libraries
add_library(${PROJECT_NAME}_lib
  src/euler_spiral.cpp
  src/fresnel_integral.cpp
)
target_include_directories(${PROJECT_NAME}_lib PUBLIC 
  include
  ${Boost_INCLUDE_DIRS} 
)