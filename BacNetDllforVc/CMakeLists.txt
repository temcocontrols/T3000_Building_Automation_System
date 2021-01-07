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

set(PROJECT_NAME BACnet_Stack_Library)

# Output Variables
set(OUTPUT_DEBUG Build/${CMAKE_BUILD_TYPE})
set(OUTPUT_REL Build/${CMAKE_BUILD_TYPE})

# Folders files
set(C_DIR_HANDLER ./Handler)
set(C_DIR_MAIN ./MainCpp)
set(C_DIR_OBJECT ./Object)
set(C_DIR_SOURCE ./Src)
set(C_DIR_CURRENT ./)
set(HEADER_DIR_INCLUDE ./include)
set(HEADER_DIR_OBJECT ./Object)

############## CMake Project ################
#        The main options of project        #
#############################################

project(${PROJECT_NAME} C CXX)

# Define Release by default.
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE $(Configuration))
  message(STATUS "Build type not specified: Uses Release by default.")
endif(NOT CMAKE_BUILD_TYPE)

# Definition of Macros
add_definitions(
   -DWIN32 
   -D_WINDOWS  
   -DPRINT_ENABLED=1 
   -D_LIB 
   -D_USRDLL 
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

# Include directories 
include_directories(./)
include_directories(./MainCpp/)
include_directories(./include/)
include_directories(./object/)
include_directories(./handler/)
include_directories(%(AdditionalIncludeDirectories))

################# Flags ################
# Defines Flags for Windows and Linux. #
########################################

if(MSVC)
   set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /W3 /EHsc")
   set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /W3 /EHsc")
endif(MSVC)
if(NOT MSVC)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
   if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
       set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
   endif()
endif(NOT MSVC)

################ Files ################
#   --   Add files to project.   --   #
#######################################

set(HANDLER_FILES
  ${C_DIR_HANDLER}/h_alarm_ack.c
  ${C_DIR_HANDLER}/h_arf.c
  ${C_DIR_HANDLER}/h_arf_a.c
  ${C_DIR_HANDLER}/h_awf.c
  ${C_DIR_HANDLER}/h_ccov.c
  ${C_DIR_HANDLER}/h_cov.c
  ${C_DIR_HANDLER}/h_dcc.c
  ${C_DIR_HANDLER}/h_gas_a.c
  ${C_DIR_HANDLER}/h_getevent.c
  ${C_DIR_HANDLER}/h_get_alarm_sum.c
  ${C_DIR_HANDLER}/h_iam.c
  ${C_DIR_HANDLER}/h_ihave.c
  ${C_DIR_HANDLER}/h_lso.c
  ${C_DIR_HANDLER}/h_npdu.c
  ${C_DIR_HANDLER}/h_pt.c
  ${C_DIR_HANDLER}/h_pt_a.c
  ${C_DIR_HANDLER}/h_rd.c
  ${C_DIR_HANDLER}/h_rp.c
  ${C_DIR_HANDLER}/h_rpm.c
  ${C_DIR_HANDLER}/h_rpm_a.c
  ${C_DIR_HANDLER}/h_rp_a.c
  ${C_DIR_HANDLER}/h_rr.c
  ${C_DIR_HANDLER}/h_rr_a.c
  ${C_DIR_HANDLER}/h_ts.c
  ${C_DIR_HANDLER}/h_ucov.c
  ${C_DIR_HANDLER}/h_upt.c
  ${C_DIR_HANDLER}/h_whohas.c
  ${C_DIR_HANDLER}/h_whois.c
  ${C_DIR_HANDLER}/h_wp.c
  ${C_DIR_HANDLER}/h_wpm.c
  ${C_DIR_HANDLER}/noserv.c
  ${C_DIR_HANDLER}/objects.c
  ${C_DIR_HANDLER}/s_ack_alarm.c
  ${C_DIR_HANDLER}/s_arfs.c
  ${C_DIR_HANDLER}/s_awfs.c
  ${C_DIR_HANDLER}/s_cevent.c
  ${C_DIR_HANDLER}/s_cov.c
  ${C_DIR_HANDLER}/s_dcc.c
  ${C_DIR_HANDLER}/s_iam.c
  ${C_DIR_HANDLER}/s_ihave.c
  ${C_DIR_HANDLER}/s_lso.c
  ${C_DIR_HANDLER}/s_ptransfer.c
  ${C_DIR_HANDLER}/s_rd.c
  ${C_DIR_HANDLER}/s_readrange.c
  ${C_DIR_HANDLER}/s_router.c
  ${C_DIR_HANDLER}/s_rp.c
  ${C_DIR_HANDLER}/s_rpm.c
  ${C_DIR_HANDLER}/s_ts.c
  ${C_DIR_HANDLER}/s_uevent.c
  ${C_DIR_HANDLER}/s_upt.c
  ${C_DIR_HANDLER}/s_whohas.c
  ${C_DIR_HANDLER}/s_whois.c
  ${C_DIR_HANDLER}/s_wp.c
  ${C_DIR_HANDLER}/txbuf.c
)

set(MAIN_FILES  
  ${C_DIR_MAIN}/dlmstp.c
  ${C_DIR_MAIN}/rs485.c
  ${C_DIR_MAIN}/timer.c
)

set(MAIN_HEADER_FILES
  ${C_DIR_MAIN}/net.h
  ${C_DIR_MAIN}/ptp.h
  ${C_DIR_MAIN}/rs485.h
  ${C_DIR_MAIN}/stdbool.h
  ${C_DIR_MAIN}/stdint.h
  ${C_DIR_MAIN}/timer.h
)

set(OBJECT_FILES
  ${C_DIR_OBJECT}/ai.c
  ${C_DIR_OBJECT}/ao.c
  ${C_DIR_OBJECT}/av.c
  ${C_DIR_OBJECT}/bacfile.c
  ${C_DIR_OBJECT}/bi.c
  ${C_DIR_OBJECT}/bo.c
  ${C_DIR_OBJECT}/bv.c
  ${C_DIR_OBJECT}/csv.c
  ${C_DIR_OBJECT}/device.c  
  ${C_DIR_OBJECT}/lc.c
  ${C_DIR_OBJECT}/lo.c
  ${C_DIR_OBJECT}/lsp.c
  ${C_DIR_OBJECT}/ms-input.c
  ${C_DIR_OBJECT}/mso.c
  ${C_DIR_OBJECT}/msv.c
  ${C_DIR_OBJECT}/nc.c
  ${C_DIR_OBJECT}/trendlog.c
)

set(OBJECT_FILE_HEADERS
  ${HEADER_DIR_OBJECT}/ai.h
  ${HEADER_DIR_OBJECT}/ao.h
  ${HEADER_DIR_OBJECT}/av.h
  ${HEADER_DIR_OBJECT}/bacfile.h
  ${HEADER_DIR_OBJECT}/bi.h
  ${HEADER_DIR_OBJECT}/bo.h
  ${HEADER_DIR_OBJECT}/bv.h
  ${HEADER_DIR_OBJECT}/csv.h
  ${HEADER_DIR_OBJECT}/device.h
  ${HEADER_DIR_OBJECT}/lc.h
  ${HEADER_DIR_OBJECT}/lo.h
  ${HEADER_DIR_OBJECT}/lsp.h
  ${HEADER_DIR_OBJECT}/ms-input.h
  ${HEADER_DIR_OBJECT}/mso.h
  ${HEADER_DIR_OBJECT}/msv.h
  ${HEADER_DIR_OBJECT}/nc.h
  ${HEADER_DIR_OBJECT}/stdbool.h
  ${HEADER_DIR_OBJECT}/trendlog.h
)

set(SOURCE_DIR_FILES
  ${C_DIR_SOURCE}/abort.c
  ${C_DIR_SOURCE}/address.c
  ${C_DIR_SOURCE}/alarm_ack.c
  ${C_DIR_SOURCE}/apdu.c
  ${C_DIR_SOURCE}/arf.c
  ${C_DIR_SOURCE}/awf.c
  ${C_DIR_SOURCE}/bacaddr.c
  ${C_DIR_SOURCE}/bacapp.c
  ${C_DIR_SOURCE}/bacdcode.c
  ${C_DIR_SOURCE}/bacdevobjpropref.c
  ${C_DIR_SOURCE}/bacerror.c
  ${C_DIR_SOURCE}/bacint.c
  ${C_DIR_SOURCE}/bacprop.c
  ${C_DIR_SOURCE}/bacpropstates.c
  ${C_DIR_SOURCE}/bacreal.c
  ${C_DIR_SOURCE}/bacstr.c
  ${C_DIR_SOURCE}/bactext.c
  ${C_DIR_SOURCE}/bigend.c
  ${C_DIR_SOURCE}/bip.c
  ${C_DIR_SOURCE}/bvlc.c
  ${C_DIR_SOURCE}/cov.c
  ${C_DIR_SOURCE}/crc.c
  ${C_DIR_SOURCE}/datalink.c
  ${C_DIR_SOURCE}/datetime.c
  ${C_DIR_SOURCE}/dcc.c
  ${C_DIR_SOURCE}/debug.c
  ${C_DIR_SOURCE}/event.c
  ${C_DIR_SOURCE}/fifo.c
  ${C_DIR_SOURCE}/filename.c
  ${C_DIR_SOURCE}/getevent.c
  ${C_DIR_SOURCE}/get_alarm_sum.c
  ${C_DIR_SOURCE}/iam.c
  ${C_DIR_SOURCE}/ihave.c
  ${C_DIR_SOURCE}/indtext.c
  ${C_DIR_SOURCE}/key.c
  ${C_DIR_SOURCE}/keylist.c
  ${C_DIR_SOURCE}/lso.c
  ${C_DIR_SOURCE}/memcopy.c
  ${C_DIR_SOURCE}/mstp.c
  ${C_DIR_SOURCE}/mstptext.c
  ${C_DIR_SOURCE}/npdu.c
  ${C_DIR_SOURCE}/proplist.c
  ${C_DIR_SOURCE}/ptransfer.c
  ${C_DIR_SOURCE}/rd.c
  ${C_DIR_SOURCE}/readrange.c
  ${C_DIR_SOURCE}/reject.c
  ${C_DIR_SOURCE}/ringbuf.c
  ${C_DIR_SOURCE}/rp.c
  ${C_DIR_SOURCE}/rpm.c
  ${C_DIR_SOURCE}/sbuf.c
  ${C_DIR_SOURCE}/timestamp.c
  ${C_DIR_SOURCE}/timesync.c
  ${C_DIR_SOURCE}/tsm.c
  ${C_DIR_SOURCE}/version.c
  ${C_DIR_SOURCE}/whohas.c
  ${C_DIR_SOURCE}/whois.c
  ${C_DIR_SOURCE}/wp.c
  ${C_DIR_SOURCE}/wpm.c
)

set(HEADER_FILES
  ${HEADER_DIR_INCLUDE}/abort.h
  ${HEADER_DIR_INCLUDE}/address.h
  ${HEADER_DIR_INCLUDE}/alarm_ack.h
  ${HEADER_DIR_INCLUDE}/apdu.h
  ${HEADER_DIR_INCLUDE}/arcnet.h
  ${HEADER_DIR_INCLUDE}/arf.h
  ${HEADER_DIR_INCLUDE}/awf.h
  ${HEADER_DIR_INCLUDE}/bacaddr.h
  ${HEADER_DIR_INCLUDE}/bacapp.h
  ${HEADER_DIR_INCLUDE}/bacdcode.h
  ${HEADER_DIR_INCLUDE}/bacdef.h
  ${HEADER_DIR_INCLUDE}/bacdevobjpropref.h
  ${HEADER_DIR_INCLUDE}/bacenum.h
  ${HEADER_DIR_INCLUDE}/bacerror.h
  ${HEADER_DIR_INCLUDE}/bacint.h
  ${HEADER_DIR_INCLUDE}/bacnet.h
  ${HEADER_DIR_INCLUDE}/bacprop.h
  ${HEADER_DIR_INCLUDE}/bacpropstates.h
  ${HEADER_DIR_INCLUDE}/bacreal.h
  ${HEADER_DIR_INCLUDE}/bacstr.h
  ${HEADER_DIR_INCLUDE}/bactext.h
  ${HEADER_DIR_INCLUDE}/bigend.h
  ${HEADER_DIR_INCLUDE}/bip.h
  ${HEADER_DIR_INCLUDE}/bits.h
  ${HEADER_DIR_INCLUDE}/bvlc.h
  ${HEADER_DIR_INCLUDE}/bytes.h
  ${HEADER_DIR_INCLUDE}/client.h
  ${HEADER_DIR_INCLUDE}/config.h
  ${HEADER_DIR_INCLUDE}/cov.h
  ${HEADER_DIR_INCLUDE}/crc.h
  ${HEADER_DIR_INCLUDE}/datalink.h
  ${HEADER_DIR_INCLUDE}/datetime.h
  ${HEADER_DIR_INCLUDE}/dcc.h
  ${HEADER_DIR_INCLUDE}/debug.h
  ${HEADER_DIR_INCLUDE}/dlenv.h
  ${HEADER_DIR_INCLUDE}/dlmstp.h
  ${HEADER_DIR_INCLUDE}/ethernet.h
  ${HEADER_DIR_INCLUDE}/event.h
  ${HEADER_DIR_INCLUDE}/fifo.h
  ${HEADER_DIR_INCLUDE}/filename.h
  ${HEADER_DIR_INCLUDE}/getevent.h
  ${HEADER_DIR_INCLUDE}/get_alarm_sum.h
  ${HEADER_DIR_INCLUDE}/handlers.h
  ${HEADER_DIR_INCLUDE}/iam.h
  ${HEADER_DIR_INCLUDE}/ihave.h
  ${HEADER_DIR_INCLUDE}/indtext.h
  ${HEADER_DIR_INCLUDE}/key.h
  ${HEADER_DIR_INCLUDE}/keylist.h
  ${HEADER_DIR_INCLUDE}/lso.h
  ${HEADER_DIR_INCLUDE}/memcopy.h
  ${HEADER_DIR_INCLUDE}/mstp.h
  ${HEADER_DIR_INCLUDE}/mstpdef.h
  ${HEADER_DIR_INCLUDE}/mstptext.h
  ${HEADER_DIR_INCLUDE}/mydata.h
  ${HEADER_DIR_INCLUDE}/npdu.h
  ${HEADER_DIR_INCLUDE}/objects.h
  ${HEADER_DIR_INCLUDE}/proplist.h
  ${HEADER_DIR_INCLUDE}/ptransfer.h
  ${HEADER_DIR_INCLUDE}/rd.h
  ${HEADER_DIR_INCLUDE}/readrange.h
  ${HEADER_DIR_INCLUDE}/reject.h
  ${HEADER_DIR_INCLUDE}/ringbuf.h
  ${HEADER_DIR_INCLUDE}/rp.h
  ${HEADER_DIR_INCLUDE}/rpm.h
  ${HEADER_DIR_INCLUDE}/sbuf.h
  ${HEADER_DIR_INCLUDE}/timestamp.h
  ${HEADER_DIR_INCLUDE}/timesync.h
  ${HEADER_DIR_INCLUDE}/tsm.h
  ${HEADER_DIR_INCLUDE}/txbuf.h
  ${HEADER_DIR_INCLUDE}/ud_str.h
  ${HEADER_DIR_INCLUDE}/version.h
  ${HEADER_DIR_INCLUDE}/whohas.h
  ${HEADER_DIR_INCLUDE}/whois.h
  ${HEADER_DIR_INCLUDE}/wp.h
  ${HEADER_DIR_INCLUDE}/wpm.h
)

# Add library to build.
add_library(${PROJECT_NAME} SHARED
   ${HANDLER_FILES} ${MAIN_FILES} ${MAIN_HEADER_FILES} 
   ${OBJECT_FILES} ${OBJECT_FILE_HEADERS} ${SOURCE_DIR_FILES}
   ${HEADER_FILES}
)

if(MSVC)
   target_link_libraries(${PROJECT_NAME} ws2_32.lib iphlpapi.lib )
endif(MSVC)
