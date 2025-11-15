# CMakeList.txt : CMake project for Joomba1, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.31)

# Enable Hot Reload for MSVC compilers if supported.
# if (POLICY CMP0141)
#   cmake_policy(SET CMP0141 NEW)
#   set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
# endif()

project(Joomba1)

find_package(fmt CONFIG REQUIRED)
# Add source to this project's executable.
add_executable(Joomba1 Joomba1.cpp main.cpp)
set_property(TARGET Joomba1 PROPERTY CXX_STANDARD 17)

target_link_libraries(Joomba1 PRIVATE fmt::fmt)
# TODO: Add tests and install targets if needed.
