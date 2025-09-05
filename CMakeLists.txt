cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if ("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS" OR IOS)
    set(CMAKE_OSX_ARCHITECTURES "arm64")
else()
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
endif()
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

project(BatteryInfo VERSION 1.0.0)

file(GLOB_RECURSE CPP_SOURCES CONFIGURE_DEPENDS src/*.cpp)

# Darwin/iOS specific files
if(APPLE OR CMAKE_SYSTEM_NAME MATCHES "Darwin" OR CMAKE_SYSTEM_NAME MATCHES "iOS")
    file(GLOB_RECURSE MM_SOURCES CONFIGURE_DEPENDS src/*.mm)
    set(CMAKE_XCODE_ATTRIBUTE_GCC_ENABLE_OBJC_EXCEPTIONS "YES")
    set(CMAKE_PCH_EXTENSION "pch")
    set_source_files_properties(${MM_SOURCES} PROPERTIES SKIP_PRECOMPILE_HEADERS TRUE)
    
    # Add proper definitions for platform-specific code
    if("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS" OR IOS)
        add_definitions(-DTARGET_OS_IPHONE=1)
    else()
        add_definitions(-DTARGET_OS_IPHONE=0)
    endif()
    add_definitions(-DTARGET_IPHONE_SIMULATOR=0) # Assume not simulator by default
else()
    set(MM_SOURCES "")
endif()

add_library(${PROJECT_NAME} SHARED ${CPP_SOURCES} ${MM_SOURCES})

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

setup_geode_mod(${PROJECT_NAME})

# Add platform-specific frameworks
if(APPLE)
    if("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS" OR IOS)
        target_link_libraries(${PROJECT_NAME} "-framework UIKit")
    else()
        target_link_libraries(${PROJECT_NAME} "-framework Foundation" "-framework IOKit")
    endif()
endif()
