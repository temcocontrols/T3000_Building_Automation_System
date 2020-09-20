# Copyright 2020 Temco Controls 

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the 
# rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
# copies of the Software, and to permit persons to whom the Software is furnished to do so, 
# subject to the following conditions:

# The above copyright notice and this permission notice shall be included
# in all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS 
# OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER 
# IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR 
# IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

# We support CMake starting at version 3.0.0
cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

################### Variables. ####################
# Change if you want modify path or other values. #
###################################################

set(PROJECT_NAME FlexSlideBar)

# Output Variables
set(OUTPUT_DEBUG Build/${CMAKE_BUILD_TYPE})
set(OUTPUT_RELEASE Build/${CMAKE_BUILD_TYPE})

# Folders files
set(CPP_SOURCE_DIR ./)
set(CPP_HEADER_DIR ./)
set(RES_DIRECTORY ./Res)

############## CMake Project ################
#        The main options of project        #
#############################################

project(${PROJECT_NAME} CXX)

# Define Release by default.
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE  $(Configuration))
  message(STATUS "Build type not specified: Use Release by default.")
endif(NOT CMAKE_BUILD_TYPE)

# Definition of Macros
add_definitions(
   -DWIN32
   -D_WINDOWS
   -DUNICODE
   -D_UNICODE
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_definitions(-D_DEBUG)
else()
  add_definitions(-D_NDEBUG)
endif()

############## Artefacts Output #################
# Defines outputs , depending Debug or Release. #
#################################################
# Already defined in the main CMakeLists.txt at root.

################# Flags ################
# Defines Flags for Windows and Linux. #
########################################

if(MSVC)
   set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /W3 /EHsc")
   set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /W3 /EHsc")
endif(MSVC)
if(NOT MSVC)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
   if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
       set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
   endif()
endif(NOT MSVC)

################ Files ################
#   --   Add files to project.   --   #
#######################################

set(SOURCE_FILES
 ${CPP_SOURCE_DIR}/CalcTics.cpp
 ${CPP_SOURCE_DIR}/dllmain.cpp
 ${CPP_SOURCE_DIR}/DoubleLinkageOpt.cpp
 ${CPP_SOURCE_DIR}/DoubleThumbOpt.cpp
 ${CPP_SOURCE_DIR}/FlexSlideBar.cpp
 ${CPP_SOURCE_DIR}/FlexSlideWnd.cpp
 ${CPP_SOURCE_DIR}/FSBChannel.cpp
 ${CPP_SOURCE_DIR}/FSBContainer.cpp
 ${CPP_SOURCE_DIR}/FSBThumb.cpp
 ${CPP_SOURCE_DIR}/MultiThumbOpt.cpp
 ${CPP_SOURCE_DIR}/SingleThumbOpt.cpp
 ${CPP_SOURCE_DIR}/stdafx.cpp
 ${CPP_SOURCE_DIR}/ThumbOperator.cpp
 ${CPP_SOURCE_DIR}/TripleLinkageOpt.cpp
 ${CPP_SOURCE_DIR}/TripleThumbOpt.cpp
)

set(HEADER_FILES
 ${CPP_SOURCE_DIR}/CalcTics.h
 ${CPP_SOURCE_DIR}/DllDefine.h
 ${CPP_SOURCE_DIR}/DoubleLinkageOpt.h
 ${CPP_SOURCE_DIR}/DoubleThumbOpt.h
 ${CPP_SOURCE_DIR}/FlexSlideBarDef.h
 ${CPP_SOURCE_DIR}/FlexSlideWnd.h
 ${CPP_SOURCE_DIR}/FSBChannel.h
 ${CPP_SOURCE_DIR}/FSBContainer.h
 ${CPP_SOURCE_DIR}/FSBThumb.h
 ${CPP_SOURCE_DIR}/MultiThumbOpt.h
 ${CPP_SOURCE_DIR}/Resource.h
 ${CPP_SOURCE_DIR}/SingleThumbOpt.h
 ${CPP_SOURCE_DIR}/stdafx.h
 ${CPP_SOURCE_DIR}/targetver.h
 ${CPP_SOURCE_DIR}/ThumbManager.h
 ${CPP_SOURCE_DIR}/ThumbOperator.h
 ${CPP_SOURCE_DIR}/TripleLinkageOpt.h
 ${CPP_SOURCE_DIR}/TripleThumbOpt.h
)

set(RESOURCE_FILES
  ${CPP_SOURCE_DIR}/FlexSlideBar.rc
  ${RES_DIRECTORY}/FlexSlideBar.rc2
)

set_source_files_properties(${CPP_SOURCE_DIR}/FlexSlideBar.rc PROPERTIES LANGUAGE RC)
set_source_files_properties(${RES_DIRECTORY}/FlexSlideBar.rc2 PROPERTIES LANGUAGE RC)
set(CMAKE_RC_FLAGS "${CMAKE_RC_FLAGS} -i ../")

include_directories(${CPP_HEADER_DIR})

# Add library to build.
add_library(${PROJECT_NAME} SHARED
   ${SOURCE_FILES} ${HEADER_FILES} ${RESOURCE_FILES}
)
