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

set(PROJECT_NAME ISP)

# Output Variables
set(OUTPUT_DEBUG Build/${CMAKE_BUILD_TYPE})
set(OUTPUT_RELEASE Build/${CMAKE_BUILD_TYPE})

# Folders files
set(CPP_SOURCE_DIR ./)
set(CPP_HEADER_DIR ./)
set(EXTERNAL_HEADER_DIR_BACNET_MAIN ../BacNetDllforVc/MainCpp)
set(EXTERNAL_HEADER_DIR_BACNET_INCLUDE ../BacNetDllforVc/Include)
set(EXTERNAL_HEADER_DIR_BACNET_OBJECT ../BacNetDllforVc/Object)
set(EXTERNAL_HEADER_DIR_BACNET_HANDLER ../BacNetDllforVc/Handler)
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
   set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /W3 /EHsc")
   set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /W3 /EHsc")
   set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
   set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
   set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
endif(MSVC)
if(NOT MSVC)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
   if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
       set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
   endif()
endif(NOT MSVC)

# Include directories 
include_directories(${CPP_HEADER_DIR})

################ Files ################
#   --   Add files to project.   --   #
#######################################

set(SOURCE_FILES
  ${CPP_SOURCE_DIR}/BacnetMstp.cpp
  ${CPP_SOURCE_DIR}/BinFileParser.cpp
  ${CPP_SOURCE_DIR}/ComWriter.cpp
  ${CPP_SOURCE_DIR}/ConfigFileHandler.cpp
  ${CPP_SOURCE_DIR}/FlashBase.cpp
  ${CPP_SOURCE_DIR}/FlashSN.cpp
  ${CPP_SOURCE_DIR}/global_function.cpp
  ${CPP_SOURCE_DIR}/HexFileParser.cpp
  ${CPP_SOURCE_DIR}/ISP.cpp
  ${CPP_SOURCE_DIR}/ISPDlg.cpp
  ${CPP_SOURCE_DIR}/ISPSetting.cpp
  ${CPP_SOURCE_DIR}/JPGLoader.cpp
  ${CPP_SOURCE_DIR}/MyPing.cpp
  ${CPP_SOURCE_DIR}/MySocket.cpp
  ${CPP_SOURCE_DIR}/Splash.cpp
  ${CPP_SOURCE_DIR}/stdafx.cpp
  ${CPP_SOURCE_DIR}/TCPFlasher.cpp
  ${CPP_SOURCE_DIR}/TFTPServer.cpp
  ${CPP_SOURCE_DIR}/TstatFlashDlg.cpp
  ${CPP_SOURCE_DIR}/BADO/BADO.cpp
  ${CPP_SOURCE_DIR}/StaticEx/staticex.cpp
)

set(HEADER_FILES
  ${CPP_HEADER_DIR}/BacnetMstp.h
  ${CPP_HEADER_DIR}/bacnet_include.h
  ${CPP_HEADER_DIR}/BinFileParser.h
  ${CPP_HEADER_DIR}/ComWriter.h
  ${CPP_HEADER_DIR}/ConfigFileHandler.h
  ${CPP_HEADER_DIR}/FlashBase.h
  ${CPP_HEADER_DIR}/FlashSN.h
  ${CPP_HEADER_DIR}/global_function.h
  ${CPP_HEADER_DIR}/Global_Struct.h
  ${CPP_HEADER_DIR}/HexFileParser.h
  ${CPP_HEADER_DIR}/ISP.h
  ${CPP_HEADER_DIR}/ISPDlg.h
  ${CPP_HEADER_DIR}/ISPSetting.h
  ${CPP_HEADER_DIR}/JPGLoader.h
  ${CPP_HEADER_DIR}/MyPing.h
  ${CPP_HEADER_DIR}/MySocket.h
  ${CPP_HEADER_DIR}/Resource.h
  ${CPP_HEADER_DIR}/Splash.h
  ${CPP_HEADER_DIR}/stdafx.h
  ${CPP_HEADER_DIR}/targetver.h
  ${CPP_HEADER_DIR}/TCPFlasher.h
  ${CPP_HEADER_DIR}/TFTP\ Struct.h
  ${CPP_HEADER_DIR}/TFTPServer.h
  ${CPP_HEADER_DIR}/TstatFlashDlg.h
  ${CPP_HEADER_DIR}/BADO/BADO.h
  ${CPP_HEADER_DIR}/StaticEx/staticex.h
  ${CPP_HEADER_DIR}/StaticEx/logfont.h
  ${EXTERNAL_HEADER_DIR_BACNET_MAIN}/rs485.h
  ${EXTERNAL_HEADER_DIR_BACNET_INCLUDE}/mstp.h
  ${EXTERNAL_HEADER_DIR_BACNET_OBJECT}/trendlog.h
  ${EXTERNAL_HEADER_DIR_BACNET_HANDLER}/h_pt_a.h
)

include_directories(${EXTERNAL_HEADER_DIR_BACNET_MAIN})
include_directories(${EXTERNAL_HEADER_DIR_BACNET_INCLUDE})
include_directories(${EXTERNAL_HEADER_DIR_BACNET_OBJECT})
include_directories(${EXTERNAL_HEADER_DIR_BACNET_HANDLER})

set(RESOURCE_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/isp.rc
  ${RES_DIRECTORY}/ISP.rc2
  ${RES_DIRECTORY}/ISP.ico
)

set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/isp.rc PROPERTIES LANGUAGE RC)
set_source_files_properties(${RES_DIRECTORY}/ISP.rc2 PROPERTIES LANGUAGE RC)
set_source_files_properties(${RES_DIRECTORY}/ISP.ico PROPERTIES LANGUAGE RC)
set(CMAKE_RC_FLAGS "${CMAKE_RC_FLAGS} -i ../")
set(CMAKE_RC_FLAGS "${CMAKE_RC_FLAGS} -i ./")

# Since we link to BACnet_Stack_Library through pragma directive, we need to specify the 
# link directory path.
link_directories(${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE})

# Add executable to build.
add_executable(${PROJECT_NAME} 
   ${SOURCE_FILES} ${HEADER_FILES} ${RESOURCE_FILES}
)

# This program requires admin previleges to run.
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/level='requireAdministrator' /uiAccess='false'")


add_dependencies(${PROJECT_NAME} BACnet_Stack_Library ModbusDllforVc)
# Link with other dependencies. Linking will happen using pragma directive.
#target_link_libraries(${PROJECT_NAME} ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE}/BACnetStackLibrary.lib
#  ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE}/ModbusDllforVc.lib)
  