cmake_minimum_required(VERSION 2.8.11)
# 2.8.11 is required for how we use Qt5.

project(flut)

# To create a folder hierarchy within Visual Studio.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

find_path(FLUT_INCLUDE_DIR
    NAMES flut/system/types.hpp
	PATHS ${CMAKE_CURRENT_SOURCE_DIR}
	)

# Check required compiler versions
# ---------------
if (${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
	set(FLUT_REQUIRED_GCC_VERSION 4.9.1)
	if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS ${FLUT_REQUIRED_GCC_VERSION})
		message(FATAL_ERROR "GNU GCC/G++ version must be at least version: "
							"${FLUT_REQUIRED_GCC_VERSION}. GCC version is "
                            "${CMAKE_CXX_COMPILER_VERSION}.")
	endif()
endif()
	
# Compiler flags.
# ---------------
# Must compile with C++11 with gcc/clang.
if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR
   ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    if(APPLE)
        if(XCODE)
            set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11")
            set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
        else()
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
            if(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
                set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
            endif()
        endif()
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    endif()
endif()
	
# Place build products (libraries, executables) in root
# binary (build) directory. Otherwise, they get scattered around
# the build directory and so the dll's aren't next to the executables.
get_filename_component(CONFIG_NAME "${CMAKE_BINARY_DIR}" NAME)
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin/${CONFIG_NAME}")
set(LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin/${CONFIG_NAME}")

# Process source code.
add_subdirectory(flut)
add_subdirectory(test)

enable_testing()
add_test(flut_test "${CMAKE_SOURCE_DIR}/bin/${CONFIG_NAME}/flut_test")
