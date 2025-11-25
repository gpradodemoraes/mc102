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

option(JOOMBA_1 "Enable Joomba 1" OFF)
option(JOOMBA_2 "Enable Joomba 2" OFF)

find_package(fmt CONFIG REQUIRED)
# Add source to this project's executable.
if(JOOMBA_1)
	add_executable(Joomba1 Joomba1.cpp predio.cpp main.cpp)
	add_compile_definitions(JOOMBA_1)
	set_property(TARGET Joomba1 PROPERTY CXX_STANDARD 17)
	target_link_libraries(Joomba1 PRIVATE fmt::fmt)
endif()
if(JOOMBA_2)
	add_executable(Joomba2 Joomba2.cpp predio.cpp permutations.c main.cpp)
	add_compile_definitions(JOOMBA_2)
	set_property(TARGET Joomba2 PROPERTY CXX_STANDARD 17)
	target_link_libraries(Joomba2 PRIVATE fmt::fmt)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# TODO: Add tests and install targets if needed.
