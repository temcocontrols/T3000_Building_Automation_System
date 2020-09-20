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

set(PROJECT_NAME ModbusDllforVc)

# Output Variables
set(OUTPUT_DEBUG Build/${CMAKE_BUILD_TYPE})
set(OUTPUT_REL Build/${CMAKE_BUILD_TYPE})

# Folders files
set(MODBUSDLLFORVC_SOURCE ./)
set(MODBUSDLLFORVC_HEADER ./)
set(RES_DIRECTORY ./Res)

############## CMake Project ################
#        The main options of project        #
#############################################

project(${PROJECT_NAME} CXX)

# Define Release by default.
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE $(Configuration))
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
   set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
   set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
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
  ${MODBUSDLLFORVC_SOURCE}/common.cpp
  ${MODBUSDLLFORVC_SOURCE}/crc.cpp
  ${MODBUSDLLFORVC_SOURCE}/DllFunction.cpp
  ${MODBUSDLLFORVC_SOURCE}/extern_variable.cpp
  ${MODBUSDLLFORVC_SOURCE}/ModbusDllforVc.cpp
  ${MODBUSDLLFORVC_SOURCE}/SerialPort.cpp
  ${MODBUSDLLFORVC_SOURCE}/stdafx.cpp
)

set(HEADER_FILES
  ${MODBUSDLLFORVC_HEADER}/common.h
  ${MODBUSDLLFORVC_HEADER}/crc.h
  ${MODBUSDLLFORVC_HEADER}/define.h
  ${MODBUSDLLFORVC_HEADER}/devintf.h
  ${MODBUSDLLFORVC_HEADER}/DllFunction.h
  ${MODBUSDLLFORVC_HEADER}/ModbusDllforVc.h
  ${MODBUSDLLFORVC_HEADER}/Resource.h
  ${MODBUSDLLFORVC_HEADER}/SerialPort.h
  ${MODBUSDLLFORVC_HEADER}/stdafx.h
  ${MODBUSDLLFORVC_HEADER}/targetver.h
)

set(RESOURCE_FILES
  ${MODBUSDLLFORVC_SOURCE}/ModbusDllforVc.rc
  ${RES_DIRECTORY}/ModbusDllforVc.rc2
)

set_source_files_properties(${MODBUSDLLFORVC_SOURCE}/ModbusDllforVc.rc  PROPERTIES LANGUAGE RC)
set_source_files_properties(${RES_DIRECTORY}/ModbusDllforVc.rc2 PROPERTIES LANGUAGE RC)
set(CMAKE_RC_FLAGS "${CMAKE_RC_FLAGS} -i ../")

include_directories(${MODBUSDLLFORVC_HEADER})

# Add library to build.
add_library(${PROJECT_NAME} SHARED
   ${SOURCE_FILES} ${HEADER_FILES} ${RESOURCE_FILES}
)
