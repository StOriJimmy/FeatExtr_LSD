cmake_minimum_required (VERSION 3.0)
cmake_policy(VERSION 3.0)

project (FeatExtr_LSD C CXX)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

if(CMAKE_CL_64)  
    set(PLATFORM "x64")
else(CMAKE_CL_64)  
    set(PLATFORM "WIN32")
endif(CMAKE_CL_64)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${PLATFORM})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/${PLATFORM})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/${PLATFORM})

set(CMAKE_RELEASE_POSTFIX "")
set(CMAKE_DEBUG_POSTFIX "d")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED yes)

if(WIN32)
    add_definitions(-DNOMINMAX)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    if(MSVC)
      string(APPEND CMAKE_EXE_LINKER_FLAGS_DEBUG " /NODEFAULTLIB:LIBCMT /NODEFAULTLIB:MSVCRT")
    endif()
endif()

# The version number.
set(FeatExtr_VERSION_MAJOR 1)
set(FeatExtr_VERSION_MINOR 0)
set(FeatExtr_VERSION_PATCH 0)
set(FeatExtr_VERSION
  ${FeatExtr_VERSION_MAJOR}.${FeatExtr_VERSION_MINOR}.${FeatExtr_VERSION_PATCH})

option(OPTION_BUILD_MT "use mt build" OFF)

find_package(OpenCV REQUIRED COMPONENTS core highgui imgproc imgcodecs features2d calib3d xfeatures2d)
set(NEEDED_OPENCV_MODULE 
  opencv_core
  opencv_highgui
  opencv_features2d
  opencv_imgproc
  opencv_imgcodecs
  opencv_calib3d
  opencv_xfeatures2d)

# set MD to MT
if(OPTION_BUILD_MT)
set(CompilerFlags
    CMAKE_CXX_FLAGS
    CMAKE_CXX_FLAGS_DEBUG
    CMAKE_CXX_FLAGS_RELEASE
    CMAKE_C_FLAGS
    CMAKE_C_FLAGS_DEBUG
    CMAKE_C_FLAGS_RELEASE
  )
foreach(CompilerFlags ${CompilerFlags})
  string(REPLACE "/MD" "/MT" ${CompilerFlags} "${${CompilerFlags}}")
endforeach()
endif()

add_subdirectory(ExtraImageLine)