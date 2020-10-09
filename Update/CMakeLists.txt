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

set(PROJECT_NAME Update)

# Output Variables
set(OUTPUT_DEBUG Build/${CMAKE_BUILD_TYPE})
set(OUTPUT_REL Build/${CMAKE_BUILD_TYPE})

# Folders files
set(UPDATE_SOURCE_DIR ./)
set(UPDATE_HEADER_DIR ./)
set(UPDATE_RESOURCE_DIR ./Res)
set(STATIC_SOURCE_DIR ../ISP/StaticEx)

############## CMake Project ################
#        The main options of project        #
#############################################

project(${PROJECT_NAME} CXX)

# Define Release by default.
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
  message(STATUS "Build type not specified: Use Release by default.")
endif(NOT CMAKE_BUILD_TYPE)

# Definition of Macros
add_definitions(
   -DWIN32 
   -D_WINDOWS
   -DUNICODE
   -D_UNICODE
)

# Link to MFC. 1 for static, 2 for shared.
set(CMAKE_MFC_FLAG 1)

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
   set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /W3 /MTd /EHsc /wd4190 /wd4091 /wd4018")
   set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT /W3 /EHsc /wd4190 /wd4091 /wd4018")
   set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /LIBPATH:${CMAKE_CURRENT_SOURCE_DIR}")
   #set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}   /LIBPATH:../Update")
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

set(UPDATE_SOURCE_FILES
  ${UPDATE_SOURCE_DIR}/stdafx.cpp
  ${UPDATE_SOURCE_DIR}/Update.cpp
  ${UPDATE_SOURCE_DIR}/UpdateDlg.cpp
  ${STATIC_SOURCE_DIR}/staticex.cpp
)

include_directories(${UPDATE_HEADER_DIR})

set(RESOURCE_FILES
   #${UPDATE_RESOURCE_DIR}/Update.rc2
   ${UPDATE_RESOURCE_DIR}/Step.ico
   ${UPDATE_RESOURCE_DIR}/temcologo.bmp
   ${UPDATE_SOURCE_DIR}/Update.rc
)

set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/Update.rc2 PROPERTIES LANGUAGE RC)
#set_source_files_properties(${UPDATE_RESOURCE_DIR}/Update.rc2 PROPERTIES LANGUAGE RC)

# Add executable to build.
add_executable(${PROJECT_NAME} WIN32 ${UPDATE_SOURCE_FILES} ${RESOURCE_FILES} 
)

#configure_file(MFC16API.lib ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/Debug/MFC16API.lib COPYONLY)
#set(MFC16API ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/Debug/MFC16API.lib)
#SET_SOURCE_FILES_PROPERTIES(${MFC16API} PROPERTIES EXTERNAL_OBJECT true GENERATED true)
#set_property(TARGET MFC16API PROPERTY IMPORTED_LOCATION ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/Debug/MFC16API.lib)
#target_link_libraries(${PROJECT_NAME} MFC16API.lib)
