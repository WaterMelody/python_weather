cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(protein_function)

include(cmake/AddFolderSubdirectories.cmake)
include(cmake/AddSourceGroups.cmake)

# This does not work on Win64
find_package(OpenMP REQUIRED)
if(OPENMP_FOUND)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=gnu++0x)
endif()

include_directories(${root_SOURCE_DIR} ./)
link_directories(/usr/local/lib)

add_subdirectory(data_class)
#add_subdirectory(external_library)
#add_subdirectory(learning)
#add_subdirectory(unit_tests)
