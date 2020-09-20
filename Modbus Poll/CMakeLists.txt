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

set(PROJECT_NAME ModbusPoll)

# Output Variables
set(OUTPUT_DEBUG Build/${CMAKE_BUILD_TYPE})
set(OUTPUT_REL Build/${CMAKE_BUILD_TYPE})

# Folders files
set(CHARTCTRL_SOURCE_DIR ./ChartCtrl)
set(CHARTCTRL_HEADER_DIR ./ChartCtrl)
set(GRIDCTRL_SOURCE_DIR ./GridCtrl_src)
set(GRIDCTRL_HEADER_DIR ./GridCtrl_src)
set(LISTCTRL_SOURCE_DIR ./ListControlEx)
set(LISTCTRL_HEADER_DIR ./ListControlEx)
set(NEWCELLTYPES_SOURCE_DIR ./NewCellTypes)
set(NEWCELLTYPES_HEADER_DIR ./NewCellTypes)
set(MODBUSPOLL_SOURCE_DIR ./)
set(MODBUSPOLL_HEADER_DIR ./)
set(CURRENT_DIR ./)
set(RES_DIRECTORY ./Res)
set(EXTERNAL_DIR_SQLITEDRIVER ../SQLiteDriver)

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

################ Files ################
#   --   Add files to project.   --   #
#######################################

set(CHARTCTRL_SOURCES 
  ${CHARTCTRL_SOURCE_DIR}/ChartAxis.cpp
  ${CHARTCTRL_SOURCE_DIR}/ChartAxisLabel.cpp
  ${CHARTCTRL_SOURCE_DIR}/ChartBarSerie.cpp
  ${CHARTCTRL_SOURCE_DIR}/ChartCandlestickSerie.cpp
  ${CHARTCTRL_SOURCE_DIR}/ChartCrossHairCursor.cpp
  ${CHARTCTRL_SOURCE_DIR}/ChartCtrl.cpp
  ${CHARTCTRL_SOURCE_DIR}/ChartCursor.cpp
  ${CHARTCTRL_SOURCE_DIR}/ChartDateTimeAxis.cpp
  ${CHARTCTRL_SOURCE_DIR}/ChartDragLineCursor.cpp
  ${CHARTCTRL_SOURCE_DIR}/ChartFont.cpp
  ${CHARTCTRL_SOURCE_DIR}/ChartGanttSerie.cpp
  ${CHARTCTRL_SOURCE_DIR}/ChartGradient.cpp
  ${CHARTCTRL_SOURCE_DIR}/ChartGrid.cpp
  ${CHARTCTRL_SOURCE_DIR}/ChartLegend.cpp
  ${CHARTCTRL_SOURCE_DIR}/ChartLineSerie.cpp
  ${CHARTCTRL_SOURCE_DIR}/ChartLogarithmicAxis.cpp
  ${CHARTCTRL_SOURCE_DIR}/ChartPointsSerie.cpp
  ${CHARTCTRL_SOURCE_DIR}/ChartScrollBar.cpp
  ${CHARTCTRL_SOURCE_DIR}/ChartSerie.cpp
  ${CHARTCTRL_SOURCE_DIR}/ChartStandardAxis.cpp
  ${CHARTCTRL_SOURCE_DIR}/ChartSurfaceSerie.cpp
  ${CHARTCTRL_SOURCE_DIR}/ChartTitle.cpp
  ${CHARTCTRL_SOURCE_DIR}/ChartXYSerie.cpp
)

set(CHARTCTRL_HEADERS
  ${CHARTCTRL_HEADER_DIR}/ChartAxis.h
  ${CHARTCTRL_HEADER_DIR}/ChartAxisLabel.h
  ${CHARTCTRL_HEADER_DIR}/ChartBalloonLabel.h
  ${CHARTCTRL_HEADER_DIR}/ChartBarSerie.h
  ${CHARTCTRL_HEADER_DIR}/ChartCandlestickSerie.h
  ${CHARTCTRL_HEADER_DIR}/ChartCrossHairCursor.h
  ${CHARTCTRL_HEADER_DIR}/ChartCtrl.h
  ${CHARTCTRL_HEADER_DIR}/ChartCursor.h
  ${CHARTCTRL_HEADER_DIR}/ChartDateTimeAxis.h
  ${CHARTCTRL_HEADER_DIR}/ChartDragLineCursor.h
  ${CHARTCTRL_HEADER_DIR}/ChartFont.h
  ${CHARTCTRL_HEADER_DIR}/ChartGanttSerie.h
  ${CHARTCTRL_HEADER_DIR}/ChartGradient.h
  ${CHARTCTRL_HEADER_DIR}/ChartGrid.h
  ${CHARTCTRL_HEADER_DIR}/ChartLabel.h
  ${CHARTCTRL_HEADER_DIR}/ChartLegend.h
  ${CHARTCTRL_HEADER_DIR}/ChartLineSerie.h
  ${CHARTCTRL_HEADER_DIR}/ChartLogarithmicAxis.h
  ${CHARTCTRL_HEADER_DIR}/ChartMouseListener.h
  ${CHARTCTRL_HEADER_DIR}/ChartPointsArray.h
  ${CHARTCTRL_HEADER_DIR}/ChartPointsSerie.h
  ${CHARTCTRL_HEADER_DIR}/ChartScrollBar.h
  ${CHARTCTRL_HEADER_DIR}/ChartSerie.h
  ${CHARTCTRL_HEADER_DIR}/ChartSerieBase.h
  ${CHARTCTRL_HEADER_DIR}/ChartSeriesMouseListener.h
  ${CHARTCTRL_HEADER_DIR}/ChartStandardAxis.h
  ${CHARTCTRL_HEADER_DIR}/ChartString.h
  ${CHARTCTRL_HEADER_DIR}/ChartSurfaceSerie.h
  ${CHARTCTRL_HEADER_DIR}/ChartTitle.h
  ${CHARTCTRL_HEADER_DIR}/ChartXYSerie.h
  ${CHARTCTRL_HEADER_DIR}/PointsOrdering.h
)

set(GRIDCTRL_SOURCES
  ${GRIDCTRL_SOURCE_DIR}/GridCell.cpp
  ${GRIDCTRL_SOURCE_DIR}/GridCellBase.cpp
  ${GRIDCTRL_SOURCE_DIR}/GridCtrl.cpp
  ${GRIDCTRL_SOURCE_DIR}/GridDropTarget.cpp
  ${GRIDCTRL_SOURCE_DIR}/InPlaceEdit.cpp
  ${GRIDCTRL_SOURCE_DIR}/TitleTip.cpp
)

set(GRIDCTRL_HEADERS
  ${GRIDCTRL_SOURCE_DIR}/GridCell.cpp
  ${GRIDCTRL_SOURCE_DIR}/GridCellBase.cpp
  ${GRIDCTRL_SOURCE_DIR}/GridCtrl.cpp
  ${GRIDCTRL_SOURCE_DIR}/GridDropTarget.cpp
  ${GRIDCTRL_SOURCE_DIR}/InPlaceEdit.cpp
  ${GRIDCTRL_SOURCE_DIR}/TitleTip.cpp
)

set(LISTCTRL_SOURCES
  ${LISTCTRL_SOURCE_DIR}/EditListCtrl.cpp
)

set(LISTCTRL_HEADERS
  ${LISTCTRL_SOURCE_DIR}/EditListCtrl.h
)

set(NEWCELLTYPES_SOURCES
  ${NEWCELLTYPES_SOURCE_DIR}/GridCellCheck.cpp
  ${NEWCELLTYPES_SOURCE_DIR}/GridCellCombo.cpp
  ${NEWCELLTYPES_SOURCE_DIR}/GridCellDateTime.cpp
  ${NEWCELLTYPES_SOURCE_DIR}/GridCellNumeric.cpp
  ${NEWCELLTYPES_SOURCE_DIR}/GridURLCell.cpp
)

set(NEWCELLTYPES_HEADERS
  ${NEWCELLTYPES_HEADER_DIR}/GridCellCheck.h
  ${NEWCELLTYPES_HEADER_DIR}/GridCellCombo.h
  ${NEWCELLTYPES_HEADER_DIR}/GridCellDateTime.h
  ${NEWCELLTYPES_HEADER_DIR}/GridCellNumeric.h
  ${NEWCELLTYPES_HEADER_DIR}/GridURLCell.h
)

set(MODBUSPOLL_SOURCES
  ${MODBUSPOLL_SOURCE_DIR}/AddDeviceDlg.cpp
  ${MODBUSPOLL_SOURCE_DIR}/AutoRichEditCtrl.cpp
  ${MODBUSPOLL_SOURCE_DIR}/CDMSChart.cpp
  ${MODBUSPOLL_SOURCE_DIR}/ChildFrm.cpp
  ${MODBUSPOLL_SOURCE_DIR}/CommunicationTrafficDlg.cpp
  ${MODBUSPOLL_SOURCE_DIR}/ConnectionSetup.cpp
  ${MODBUSPOLL_SOURCE_DIR}/CVBDataObject.cpp
  ${MODBUSPOLL_SOURCE_DIR}/CVBDataObjectFiles.cpp
  ${MODBUSPOLL_SOURCE_DIR}/CVcAxis.cpp
  ${MODBUSPOLL_SOURCE_DIR}/CVcAxisGrid.cpp
  ${MODBUSPOLL_SOURCE_DIR}/CVcAxisScale.cpp
  ${MODBUSPOLL_SOURCE_DIR}/CVcAxisTitle.cpp
  ${MODBUSPOLL_SOURCE_DIR}/CVcBackdrop.cpp
  ${MODBUSPOLL_SOURCE_DIR}/CVcBrush.cpp
  ${MODBUSPOLL_SOURCE_DIR}/CVcCategoryScale.cpp
  ${MODBUSPOLL_SOURCE_DIR}/CVcColor.cpp
  ${MODBUSPOLL_SOURCE_DIR}/CVcCoor.cpp
  ${MODBUSPOLL_SOURCE_DIR}/CVcDataGrid.cpp
  ${MODBUSPOLL_SOURCE_DIR}/CVcDataPoint.cpp
  ${MODBUSPOLL_SOURCE_DIR}/CVcDataPointLabel.cpp
  ${MODBUSPOLL_SOURCE_DIR}/CVcDataPoints.cpp
  ${MODBUSPOLL_SOURCE_DIR}/CVcFill.cpp
  ${MODBUSPOLL_SOURCE_DIR}/CVcFont.cpp
  ${MODBUSPOLL_SOURCE_DIR}/CVcFootnote.cpp
  ${MODBUSPOLL_SOURCE_DIR}/CVcFrame.cpp
  ${MODBUSPOLL_SOURCE_DIR}/CVcIntersection.cpp
  ${MODBUSPOLL_SOURCE_DIR}/CVcLabel.cpp
  ${MODBUSPOLL_SOURCE_DIR}/CVcLabels.cpp
  ${MODBUSPOLL_SOURCE_DIR}/CVcLCoor.cpp
  ${MODBUSPOLL_SOURCE_DIR}/CVcLegend.cpp
  ${MODBUSPOLL_SOURCE_DIR}/CVcLight.cpp
  ${MODBUSPOLL_SOURCE_DIR}/CVcLightSource.cpp
  ${MODBUSPOLL_SOURCE_DIR}/CVcLightSources.cpp
  ${MODBUSPOLL_SOURCE_DIR}/CVcLocation.cpp
  ${MODBUSPOLL_SOURCE_DIR}/CVcMarker.cpp
  ${MODBUSPOLL_SOURCE_DIR}/CVcPen.cpp
  ${MODBUSPOLL_SOURCE_DIR}/CVcPlot.cpp
  ${MODBUSPOLL_SOURCE_DIR}/CVcPlotBase.cpp
  ${MODBUSPOLL_SOURCE_DIR}/CVcRect.cpp
  ${MODBUSPOLL_SOURCE_DIR}/CVcSeries.cpp
  ${MODBUSPOLL_SOURCE_DIR}/CVcSeriesCollection.cpp
  ${MODBUSPOLL_SOURCE_DIR}/CVcSeriesMarker.cpp
  ${MODBUSPOLL_SOURCE_DIR}/CVcSeriesPosition.cpp
  ${MODBUSPOLL_SOURCE_DIR}/CVcShadow.cpp
  ${MODBUSPOLL_SOURCE_DIR}/CVcStatLine.cpp
  ${MODBUSPOLL_SOURCE_DIR}/CVcTextLayout.cpp
  ${MODBUSPOLL_SOURCE_DIR}/CVcTick.cpp
  ${MODBUSPOLL_SOURCE_DIR}/CVcTitle.cpp
  ${MODBUSPOLL_SOURCE_DIR}/CVcValueScale.cpp
  ${MODBUSPOLL_SOURCE_DIR}/CVcView3d.cpp
  ${MODBUSPOLL_SOURCE_DIR}/CVcWall.cpp
  ${MODBUSPOLL_SOURCE_DIR}/CVcWeighting.cpp
  ${MODBUSPOLL_SOURCE_DIR}/DataMonitorDlg.cpp
  ${MODBUSPOLL_SOURCE_DIR}/DeviceTestDlg.cpp
  ${MODBUSPOLL_SOURCE_DIR}/DeviecTesterConfigDlg.cpp
  ${MODBUSPOLL_SOURCE_DIR}/EnterValueDlg.cpp
  ${MODBUSPOLL_SOURCE_DIR}/Enter_Value_BinaryDlg.cpp
  ${MODBUSPOLL_SOURCE_DIR}/EnumSerial.cpp
  ${MODBUSPOLL_SOURCE_DIR}/excel9.cpp
  ${MODBUSPOLL_SOURCE_DIR}/FileView.cpp
  ${MODBUSPOLL_SOURCE_DIR}/global_function.cpp
  ${MODBUSPOLL_SOURCE_DIR}/MainFrm.cpp
  ${MODBUSPOLL_SOURCE_DIR}/Modbus\ Poll.cpp
  ${MODBUSPOLL_SOURCE_DIR}/Modbus\ PollDoc.cpp
  ${MODBUSPOLL_SOURCE_DIR}/Modbus\ PollView.cpp
  ${MODBUSPOLL_SOURCE_DIR}/msflexgrid1.cpp
  ${MODBUSPOLL_SOURCE_DIR}/ReadWriteDefinitionDlg.cpp
  ${MODBUSPOLL_SOURCE_DIR}/ReadWrite_Multiple_RegDlg.cpp
  ${MODBUSPOLL_SOURCE_DIR}/RegisterValueAnalyzerDlg.cpp
  ${MODBUSPOLL_SOURCE_DIR}/stdafx.cpp
  ${MODBUSPOLL_SOURCE_DIR}/TestCenter.cpp
  ${MODBUSPOLL_SOURCE_DIR}/ViewTree.cpp
  ${MODBUSPOLL_SOURCE_DIR}/WriteFunctionDlg.cpp
  ${MODBUSPOLL_SOURCE_DIR}/WriteMultipleCoilsDlg.cpp
  ${MODBUSPOLL_SOURCE_DIR}/WriteMultiRegistersDlg.cpp
  ${MODBUSPOLL_SOURCE_DIR}/WriteSingleCoilDlg.cpp
  ${MODBUSPOLL_SOURCE_DIR}/WriteSingleRegisterDlg.cpp
  ${MODBUSPOLL_SOURCE_DIR}/WriteSingle_BinaryDlg.cpp
  ${EXTERNAL_DIR_SQLITEDRIVER}/CppSQLite3.cpp
)

set(MODBUSPOLL_HEADERS
  ${MODBUSPOLL_HEADER_DIR}/AddDeviceDlg.h
  ${MODBUSPOLL_HEADER_DIR}/AutoRichEditCtrl.h
  ${MODBUSPOLL_HEADER_DIR}/CApplication.h
  ${MODBUSPOLL_HEADER_DIR}/CDMSChart.h
  ${MODBUSPOLL_HEADER_DIR}/ChildFrm.h
  ${MODBUSPOLL_HEADER_DIR}/CommunicationTrafficDlg.h
  ${MODBUSPOLL_HEADER_DIR}/ConnectionSetup.h
  ${MODBUSPOLL_HEADER_DIR}/CRange.h
  ${MODBUSPOLL_HEADER_DIR}/CRanges.h
  ${MODBUSPOLL_HEADER_DIR}/CVBDataObject.h
  ${MODBUSPOLL_HEADER_DIR}/CVBDataObjectFiles.h
  ${MODBUSPOLL_HEADER_DIR}/CVcAxis.h
  ${MODBUSPOLL_HEADER_DIR}/CVcAxisGrid.h
  ${MODBUSPOLL_HEADER_DIR}/CVcAxisScale.h
  ${MODBUSPOLL_HEADER_DIR}/CVcAxisTitle.h
  ${MODBUSPOLL_HEADER_DIR}/CVcBackdrop.h
  ${MODBUSPOLL_HEADER_DIR}/CVcBrush.h
  ${MODBUSPOLL_HEADER_DIR}/CVcCategoryScale.h
  ${MODBUSPOLL_HEADER_DIR}/CVcColor.h
  ${MODBUSPOLL_HEADER_DIR}/CVcCoor.h
  ${MODBUSPOLL_HEADER_DIR}/CVcDataGrid.h
  ${MODBUSPOLL_HEADER_DIR}/CVcDataPoint.h
  ${MODBUSPOLL_HEADER_DIR}/CVcDataPointLabel.h
  ${MODBUSPOLL_HEADER_DIR}/CVcDataPoints.h
  ${MODBUSPOLL_HEADER_DIR}/CVcFill.h
  ${MODBUSPOLL_HEADER_DIR}/CVcFont.h
  ${MODBUSPOLL_HEADER_DIR}/CVcFootnote.h
  ${MODBUSPOLL_HEADER_DIR}/CVcFrame.h
  ${MODBUSPOLL_HEADER_DIR}/CVcIntersection.h
  ${MODBUSPOLL_HEADER_DIR}/CVcLabel.h
  ${MODBUSPOLL_HEADER_DIR}/CVcLabels.h
  ${MODBUSPOLL_HEADER_DIR}/CVcLCoor.h
  ${MODBUSPOLL_HEADER_DIR}/CVcLegend.h
  ${MODBUSPOLL_HEADER_DIR}/CVcLight.h
  ${MODBUSPOLL_HEADER_DIR}/CVcLightSource.h
  ${MODBUSPOLL_HEADER_DIR}/CVcLightSources.h
  ${MODBUSPOLL_HEADER_DIR}/CVcLocation.h
  ${MODBUSPOLL_HEADER_DIR}/CVcMarker.h
  ${MODBUSPOLL_HEADER_DIR}/CVcPen.h
  ${MODBUSPOLL_HEADER_DIR}/CVcPlot.h
  ${MODBUSPOLL_HEADER_DIR}/CVcPlotBase.h
  ${MODBUSPOLL_HEADER_DIR}/CVcRect.h
  ${MODBUSPOLL_HEADER_DIR}/CVcSeries.h
  ${MODBUSPOLL_HEADER_DIR}/CVcSeriesCollection.h
  ${MODBUSPOLL_HEADER_DIR}/CVcSeriesMarker.h
  ${MODBUSPOLL_HEADER_DIR}/CVcSeriesPosition.h
  ${MODBUSPOLL_HEADER_DIR}/CVcShadow.h
  ${MODBUSPOLL_HEADER_DIR}/CVcStatLine.h
  ${MODBUSPOLL_HEADER_DIR}/CVcTextLayout.h
  ${MODBUSPOLL_HEADER_DIR}/CVcTick.h
  ${MODBUSPOLL_HEADER_DIR}/CVcTitle.h
  ${MODBUSPOLL_HEADER_DIR}/CVcValueScale.h
  ${MODBUSPOLL_HEADER_DIR}/CVcView3d.h
  ${MODBUSPOLL_HEADER_DIR}/CVcWall.h
  ${MODBUSPOLL_HEADER_DIR}/CVcWeighting.h
  ${MODBUSPOLL_HEADER_DIR}/CWorkbook.h
  ${MODBUSPOLL_HEADER_DIR}/CWorkbooks.h
  ${MODBUSPOLL_HEADER_DIR}/CWorksheet.h
  ${MODBUSPOLL_HEADER_DIR}/CWorksheets.h
  ${MODBUSPOLL_HEADER_DIR}/DataMonitorDlg.h
  ${MODBUSPOLL_HEADER_DIR}/DeviceTestDlg.h
  ${MODBUSPOLL_HEADER_DIR}/DeviecTesterConfigDlg.h
  ${MODBUSPOLL_HEADER_DIR}/EnterValueDlg.h
  ${MODBUSPOLL_HEADER_DIR}/Enter_Value_BinaryDlg.h
  ${MODBUSPOLL_HEADER_DIR}/EnumSerial.h
  ${MODBUSPOLL_HEADER_DIR}/excel9.h
  ${MODBUSPOLL_HEADER_DIR}/FileView.h
  ${MODBUSPOLL_HEADER_DIR}/gloab_define.h
  ${MODBUSPOLL_HEADER_DIR}/global_Function.h
  ${MODBUSPOLL_HEADER_DIR}/global_struct.h
  ${MODBUSPOLL_HEADER_DIR}/global_variable.h
  ${MODBUSPOLL_HEADER_DIR}/global_variable_extern.h
  ${MODBUSPOLL_HEADER_DIR}/MainFrm.h
  ${MODBUSPOLL_HEADER_DIR}/Modbus\ Poll.h
  ${MODBUSPOLL_HEADER_DIR}/Modbus\ PollDoc.h
  ${MODBUSPOLL_HEADER_DIR}/Modbus\ PollView.h
  ${MODBUSPOLL_HEADER_DIR}/ModbusDllForVC.h
  ${MODBUSPOLL_HEADER_DIR}/msflexgrid1.h
  ${MODBUSPOLL_HEADER_DIR}/ReadWriteDefinitionDlg.h
  ${MODBUSPOLL_HEADER_DIR}/ReadWrite_Multiple_RegDlg.h
  ${MODBUSPOLL_HEADER_DIR}/RegisterValueAnalyzerDlg.h
  ${MODBUSPOLL_HEADER_DIR}/resource.h
  ${MODBUSPOLL_HEADER_DIR}/stdafx.h
  ${MODBUSPOLL_HEADER_DIR}/targetver.h
  ${MODBUSPOLL_HEADER_DIR}/TestCenter.h
  ${MODBUSPOLL_HEADER_DIR}/ViewTree.h
  ${MODBUSPOLL_HEADER_DIR}/WriteFunctionDlg.h
  ${MODBUSPOLL_HEADER_DIR}/WriteMultipleCoilsDlg.h
  ${MODBUSPOLL_HEADER_DIR}/WriteMultiRegistersDlg.h
  ${MODBUSPOLL_HEADER_DIR}/WriteSingleCoilDlg.h
  ${MODBUSPOLL_HEADER_DIR}/WriteSingleRegisterDlg.h
  ${MODBUSPOLL_HEADER_DIR}/WriteSingle_BinaryDlg.h
  ${EXTERNAL_DIR_SQLITEDRIVER}/CppSQLite3.h
)

set(RESOURCE_FILES
  ${MODBUSPOLL_SOURCE_DIR}/Modbus\ Poll.rc
  ${RES_DIRECTORY}/ModbusPoll.rc2
)

set_source_files_properties(${MODBUSPOLL_SOURCE_DIR}/Modbus\ Poll.rc  PROPERTIES LANGUAGE RC)
set_source_files_properties(${RES_DIRECTORY}/ModbusPoll.rc2   PROPERTIES LANGUAGE RC)
set(CMAKE_RC_FLAGS "${CMAKE_RC_FLAGS} -i ../")

include_directories(${CURRENT_DIR})

# Add executable to build.
add_executable(${PROJECT_NAME} 
   ${CHARTCTRL_SOURCES} ${CHARTCTRL_HEADERS} ${GRIDCTRL_SOURCES} ${GRIDCTRL_HEADERS} 
   ${LISTCTRL_SOURCES} ${LISTCTRL_HEADERS} ${NEWCELLTYPES_SOURCES} ${NEWCELLTYPES_HEADERS}
   ${MODBUSPOLL_SOURCES} ${MODBUSPOLL_HEADERS} ${RESOURCE_FILES} 
)

add_dependencies(${PROJECT_NAME} ModbusDllforVc)
target_link_libraries(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/../SQLiteDriver/sqlite3.lib)
target_link_libraries(${PROJECT_NAME} setupapi.lib ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE}/ModbusDllforVc.lib)
