cmake_minimum_required(VERSION 3.12) # required for CONFIGURE_DEPENDS

# Add configure depends to make GLOB "safer" (https://cmake.org/cmake/help/v3.12/command/file.html#filesystem)
function(glob_cpp_source root arg)
file(GLOB_RECURSE ${arg} CONFIGURE_DEPENDS
  ${root}/*.cpp
  ${root}/*.c
  ${root}/*.cc
  ${root}/*.cxx
)
set(${arg} ${${arg}} PARENT_SCOPE)
endfunction(glob_cpp_source)

function(glob_cpp_header root arg)
file(GLOB_RECURSE ${arg} CONFIGURE_DEPENDS
  ${root}/*.h
  ${root}/*.hpp
)
set(${arg} ${${arg}} PARENT_SCOPE)
endfunction(glob_cpp_header)

function(glob_protobufs root arg)
file(GLOB_RECURSE ${arg} CONFIGURE_DEPENDS
  ${root}/*.proto
)
set(${arg} ${${arg}} PARENT_SCOPE)
endfunction(glob_protobufs)

set(TargetName cpp_algo)

set(CoreLibraryRoot ${CMAKE_CURRENT_SOURCE_DIR}/core)

project(${TargetName}
  LANGUAGES CXX
)

# Uses CMake's global variable to control static/dynamic shared lib builds.
# Option defaults to static libs if not otherwise specified.
# https://cmake.org/cmake/help/latest/variable/BUILD_SHARED_LIBS.html
OPTION(BUILD_SHARED_LIBS "Build a shared library" OFF)

set(CMAKE_POSITION_INDEPENDENT_CODE ON) # (e.g. fPIC)

find_library(TA_LIB_LIBRARY ta-lib REQUIRED) # Find the library

if (NOT TA_LIB_LIBRARY )
    message(FATAL_ERROR "TA-Lib not found. Please install TA-Lib.")
endif()

glob_cpp_source(${CoreLibraryRoot}/src cpp_algo_sources)
glob_cpp_header(${CoreLibraryRoot}/include cpp_algo_headers)

list(APPEND TargetSources
  ${cpp_algo_sources}
)

list(APPEND PublicHeaders
  ${cpp_algo_headers}
)

add_library(${TargetName} ${TargetSources})
target_link_libraries(${TargetName} ${TA_LIB_LIBRARY})


target_include_directories(${TargetName} PUBLIC
  $<BUILD_INTERFACE:${CoreLibraryRoot}/include/>
  $<INSTALL_INTERFACE:include/${TargetName}>
)

target_compile_features(${TargetName} PRIVATE cxx_std_11) # C++ 11
target_compile_options(${TargetName} PRIVATE -Wall -pedantic -Winit-self)


set_property(TARGET ${TargetName} PROPERTY VERSION ${${TargetName}_VERSION})
set_property(TARGET ${TargetName} PROPERTY SOVERSION ${${TargetName}_VERSION_MAJOR})

set(IncludeDest "include")
set(LibDest "lib")
set(ExeDest "bin")

set(Namespace "${TargetName}::")
set(APIDest "${IncludeDest}/${TargetName}")
set(CMakeDest "${LibDest}/cmake/${TargetName}")

set(CMakeTarget "${TargetName}Targets")

install(TARGETS ${TargetName} EXPORT ${CMakeTarget}
  LIBRARY DESTINATION ${LibDest}
  ARCHIVE DESTINATION ${LibDest}
  RUNTIME DESTINATION ${ExeDest}
  INCLUDES DESTINATION ${IncludeDest}
)

install(FILES ${PublicHeaders} DESTINATION ${APIDest})

install(
  FILES
    ${CMakeConfigFile}
  DESTINATION
    ${CMakeDest}
)

install(EXPORT ${CMakeTarget}
  FILE
    ${CMakeTarget}.cmake
  NAMESPACE
    ${Namespace}
  DESTINATION
    ${CMakeDest}
)