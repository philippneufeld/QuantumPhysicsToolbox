# Philipp Neufeld, 2023

cmake_minimum_required (VERSION 3.17)

# don't allow build in the source directory
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
   message(FATAL_ERROR "You don't want to configure in the source directory! \
   Please use cmake [<options>] -S <path-to-source> -B <path-to-build>")
endif()

# define project
set(QPT_PROJECT "QuantumPhysicsToolbox")
project (QPT_PROJECT VERSION 1.0)

set(QPT_SOURCE_DIR "${CMAKE_SOURCE_DIR}/QPT")
set(QPT_SOURCES 
   "${QPT_SOURCE_DIR}/HDF5/H5Object.cpp"
   "${QPT_SOURCE_DIR}/HDF5/H5Group.cpp"
   "${QPT_SOURCE_DIR}/HDF5/H5Dataset.cpp"
   "${QPT_SOURCE_DIR}/HDF5/H5File.cpp"
   )
set(QPT_LIB_TARGET "QPT")
add_library("${QPT_LIB_TARGET}" STATIC "${QPT_SOURCES}")

# set library properties
target_compile_features("${QPT_LIB_TARGET}" PUBLIC cxx_std_17)
target_include_directories("${QPT_LIB_TARGET}" INTERFACE "${CMAKE_SOURCE_DIR}")

# add Eigen3 dependency
find_package(Eigen3 REQUIRED)
target_link_libraries("${QPT_LIB_TARGET}" PUBLIC Eigen3::Eigen)

# add HDF5 dependency
find_package(HDF5 REQUIRED)
target_include_directories("${QPT_LIB_TARGET}" PUBLIC "${HDF5_INCLUDE_DIRECTORIES}")
target_link_libraries("${QPT_LIB_TARGET}" PUBLIC "${HDF5_LIBRARIES}")

# put the built library file into the <build>/lib/ folder
set(BUILD_LIB_DIR "${CMAKE_BINARY_DIR}/lib")
set_target_properties("${QPT_LIB_TARGET}" 
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${BUILD_LIB_DIR}"
    LIBRARY_OUTPUT_DIRECTORY "${BUILD_LIB_DIR}"
    RUNTIME_OUTPUT_DIRECTORY "${BUILD_LIB_DIR}"
)

add_subdirectory("Apps")
