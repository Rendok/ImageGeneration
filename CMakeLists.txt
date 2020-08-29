cmake_minimum_required(VERSION 3.17)
project(ImageGeneration)

set(CMAKE_CXX_STANDARD 20)

add_executable(ImageGeneration main.cpp RandomGenerator.cpp RandomGenerator.h)

find_package(OpenCV REQUIRED)
target_link_libraries(ImageGeneration PUBLIC ${OpenCV_LIBS})

find_package(OpenMP REQUIRED)
target_link_libraries(ImageGeneration PUBLIC OpenMP::OpenMP_CXX)
