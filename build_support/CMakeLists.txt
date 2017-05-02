cmake_minimum_required(VERSION 2.8)
project(GLFWTraining)

set(INC_DIR .)
set(INC_DIR ../Include ${INC_DIR})
set(INC_DIR ../../assimp/include/ ${INC_DIR})
set(INC_DIR ../Include/freetype  ${INC_DIR}) 


include_directories(${INC_DIR})

set(LIBS libGL.so ${LIBS})
set(LIBS libGLEW.so ${LIBS})
set(LIBS libglfw.so.3 ${LIBS})

set(LIBS libSOIL.so ${LIBS}) 
set(LIBS libfreetype.so ${LIBS})
set(LIBS libassimp.so.3 ${LIBS})

set(LIBS_DIR /home/duvnjakk/workspace/GLFWTraining/Lib )
link_directories( ${LIBS_DIR} )

file(GLOB SOURCES "../Source/*.cpp")
file(GLOB SHADERS "./*.fs" "./*.vs")
#set(SOURCES "../Src/main.cpp")


set ( CMAKE_CXX_FLAGS "-std=c++11")

add_executable(glfwtraining ${SOURCES})

target_link_libraries(glfwtraining ${LIBS})


