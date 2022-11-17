## Use the variable PROJECT_NAME for changing the target name
get_filename_component(DIR_ONE_ABOVE ../ ABSOLUTE)
get_filename_component(ProjectId ${DIR_ONE_ABOVE} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
set( PROJECT_NAME ${ProjectId} )

cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
project(${PROJECT_NAME})

# include_directories(include)

add_subdirectory(.. ${PROJECT_NAME}_lib)

## add imgui as a library
add_library(imgui STATIC
  src/imgui/imgui.cpp
  src/imgui/imgui_demo.cpp
  src/imgui/imgui_draw.cpp
  src/imgui/imgui_widgets.cpp
  src/imgui/imgui_impl_glfw.cpp
  src/imgui/imgui_impl_opengl3.cpp
)

## Define the executable
set(SRCS 
  src/main.cpp
)
add_executable(${PROJECT_NAME} ${SRCS})
target_include_directories(${PROJECT_NAME} PUBLIC
  ../include
)
target_link_libraries(${PROJECT_NAME} 
  imgui
  GLEW
  glfw 
  ${OPENGL_gl_LIBRARY} 
  ${PROJECT_NAME}_lib
)

add_executable(${PROJECT_NAME}_test 
  src/euler_spiral_test.cpp
)
target_include_directories(${PROJECT_NAME}_test PUBLIC
  ../include
)
target_link_libraries(${PROJECT_NAME}_test 
  ${PROJECT_NAME}_lib
)

add_executable(biarc_test 
src/biarc_test.cpp
)
target_include_directories(biarc_test PUBLIC
  ../include
)
target_link_libraries(biarc_test
  ${PROJECT_NAME}_lib
)

add_executable(demo 
  ../demo/demo_parameter_estimation.cpp
)
target_include_directories(demo PUBLIC
  ../include
)
target_link_libraries(demo
  ${PROJECT_NAME}_lib
)

add_executable(demo_anime
  ../demo/demo_parameter_estimation_anime.cpp
)
target_include_directories(demo_anime PUBLIC
  ../include
)
target_link_libraries(demo_anime
  ${PROJECT_NAME}_lib
)
