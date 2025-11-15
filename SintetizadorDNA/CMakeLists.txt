cmake_minimum_required(VERSION 4.0)
project(mc102_tarefa14)

set(CMAKE_CXX_STANDARD 17)
find_package(fmt CONFIG REQUIRED)

add_executable(mc102_tarefa14
        main.cpp
        sintetizadorDNA.cpp)

target_link_libraries(mc102_tarefa14 PRIVATE fmt::fmt)