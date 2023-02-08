# Philipp Neufeld, 2023

cmake_minimum_required (VERSION 3.17)

# don't allow build in the source directory
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
   message(FATAL_ERROR "You don't want to configure in the source directory! \
   Please use cmake [<options>] -S <path-to-source> -B <path-to-build>")
endif()

# define project
set(QPT_Project "QuantumPhysicsToolbox")
project (QPT_Project VERSION 1.0)

# include directories
add_executable("Test" "main.cpp" )

# require C++17 for library and all children using it
target_compile_features("Test" PUBLIC cxx_std_17)

# set include directory as interface
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}")
target_include_directories("Test" INTERFACE "${INCLUDE_DIR}")

# find Eigen3
find_package(Eigen3 REQUIRED)
target_link_libraries("Test" PUBLIC Eigen3::Eigen)

find_package(HDF5 REQUIRED)
target_include_directories("Test" PUBLIC "${HDF5_INCLUDE_DIRS}")
target_link_libraries("Test" PUBLIC "${HDF5_LIBRARIES}")

find_package(HighFive REQUIRED)
target_link_libraries("Test" PUBLIC HighFive)
target_compile_definitions("Test" PUBLIC "-DH5_USE_EIGEN 1")

