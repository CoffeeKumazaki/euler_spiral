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

find_package(Boost REQUIRED)

## Define libraries
add_library(${PROJECT_NAME}_lib
  src/euler_spiral.cpp
  src/fresnel_integral.cpp
)
target_include_directories(${PROJECT_NAME}_lib PUBLIC 
  include
  ${Boost_INCLUDE_DIRS} 
)
