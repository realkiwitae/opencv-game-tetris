cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
project(tetris) #name of your project

include_directories(./)
include_directories(./headers)
add_executable(tetris main.cpp ./utils/cvutils.cpp)
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
target_compile_options(tetris PRIVATE -Wall -ggdb3)
target_link_libraries(tetris rt ${OpenCV_LIBS})