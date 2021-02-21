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

set(PROJECT_NAME T3000)

# Output Variables
set(OUTPUT_DEBUG Build/${CMAKE_BUILD_TYPE})
set(OUTPUT_REL Build/${CMAKE_BUILD_TYPE})

# Folders files
set(ISP_SOURCE_DIR ../ISP)
set(BADO_SOURCE_DIR ./BADO)
#The same ChartCtrl files in Modbus poll folder too!
#set(CHARTCTRL_SOURCE_DIR ../Modbus\ Poll/ChartCtrl)
set(AIRQUALITY_SOURCE_DIR ./AirQuality)
set(BACNET_SOURCE_DIR ./)
set(BACNET_SOURCE_CM5_DIR ./CM5)
set(CLASS_SOURCE_DIR ./Class)
set(CHARTCTRL_SOURCE_DIR ./ChartCtrl)
set(SINGLESLIDER_SOURCE_DIR ./singleslider)
set(SQLITE_DRIVER_DIR ../SQLiteDriver)
set(T3_SOURCE_DIR ./T3)
set(LIGHTINGCONTROLLER_SRC_DIR ./LightingController)
set(LIGHTINGCONTROLLER_HDR_DIR ./LightingController)
set(T3000_SOURCE_DIR ./)
set(T3000_HEADER_DIR ./)
set(AIRQUALITY_HEADER_DIR ./AirQuality)
set(BADO_HEADER_DIR ./BADO)
set(BACNET_HEADER_DIR ./)
set(CLASS_SOURCE_FILES ./Class)
set(CLASSCTRL_HEADER_DIR ./Class)
set(SINGLESLIDER_HEADER_DIR ./singleslider)
set(SQLITE_HEADER_DIR ../SQLiteDriver)
set(T3_HEADER_DIR ./T3)
set(HEADER_DIR_CM5 ./CM5)
set(HEADER_DIR_LIGHTINGCTRL ./LightingController)
set(T3_RESOURCE_DIR ./Res)
set(BACNET_DLL_VC_INCLUDE ../BacNetDllforVc/Include)
set(BACNET_DLL_OBJECT_INCLUDE ../BacNetDllforVc/Object)
set(BACNET_DLL_HANDLER_INCLUDE ../BacNetDllforVc/Handler)
set(BACNET_MAIN_INCLUDE ../BacNetDllforVc/MainCpp)
set(HTTP_API ../BravocontrolAPI/BravocontrolAPI/HttpAPI)
set(CUSTOM_SOURCE_DIR ./CustomSource)

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
   set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /W3 /MTd /EHsc /wd4190 /wd4091 /wd4018")
   set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT /W3 /EHsc /wd4190 /wd4091 /wd4018")
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

set(ISP_SOURCE_FILES
  ${ISP_SOURCE_DIR}/MyPing.cpp
)

# I don't know why Splash.cpp is there in both ISP and here. Refactor?
set(CUSTOM_SOURCE_FILES
  ${CUSTOM_SOURCE_DIR}/Splash.cpp
  ${CUSTOM_SOURCE_DIR}/JPGLoader.cpp
)

set(AIRQUALITY_FILES 
  ${AIRQUALITY_SOURCE_DIR}/AirQuality.cpp
  ${AIRQUALITY_HEADER_DIR}/AirQuality.h
)

set(BACNET_FILES
  ${BACNET_SOURCE_DIR}/Bacnetaddintodb.cpp
  ${BACNET_SOURCE_DIR}/BacnetAddLabel.cpp
  ${BACNET_SOURCE_DIR}/BacnetAddRemoteDevice.cpp
  ${BACNET_SOURCE_DIR}/BacnetAddVirtualDevice.cpp
  ${BACNET_SOURCE_DIR}/BacnetAlarmLog.cpp
  ${BACNET_SOURCE_DIR}/BacnetAlarmWindow.cpp
  ${BACNET_SOURCE_DIR}/BacnetAnalogCusRang.cpp
  ${BACNET_SOURCE_DIR}/BacnetAnnualRoutine.cpp
  ${BACNET_SOURCE_DIR}/BacnetATCommand.cpp
  ${BACNET_SOURCE_DIR}/BacnetController.cpp
  ${BACNET_SOURCE_DIR}/BacnetCustomerDigitalRange.cpp
  ${BACNET_SOURCE_DIR}/BacnetEditLabel.cpp
  ${BACNET_SOURCE_DIR}/BacnetGraphic.cpp
  ${BACNET_SOURCE_DIR}/BacnetGraphicSetting.cpp
  ${BACNET_SOURCE_DIR}/BacnetInput.cpp
  ${BACNET_SOURCE_DIR}/BacnetIOConfig.cpp
  ${BACNET_SOURCE_DIR}/BacnetMessageInput.cpp
  ${BACNET_SOURCE_DIR}/BacnetMonitor.cpp
  ${BACNET_SOURCE_DIR}/BacnetOutput.cpp
  ${BACNET_SOURCE_DIR}/BacnetProgram.cpp
  ${BACNET_SOURCE_DIR}/BacnetProgramDebug.cpp
  ${BACNET_SOURCE_DIR}/BacnetProgramEdit.cpp
  ${BACNET_SOURCE_DIR}/BacnetProgramSetting.cpp
  ${BACNET_SOURCE_DIR}/BacnetRange.cpp
  ${BACNET_SOURCE_DIR}/BacnetRegisterListView.cpp
  ${BACNET_SOURCE_DIR}/BacnetRemotePoint.cpp
  ${BACNET_SOURCE_DIR}/BacnetRemotePortWarning.cpp
  ${BACNET_SOURCE_DIR}/BacnetScheduleTime.cpp
  ${BACNET_SOURCE_DIR}/BacnetScreen.cpp
  ${BACNET_SOURCE_DIR}/BacnetScreenEdit.cpp
  ${BACNET_SOURCE_DIR}/BacnetSetting.cpp
  ${BACNET_SOURCE_DIR}/BacnetSettingHealth.cpp
  ${BACNET_SOURCE_DIR}/BacnetTool.cpp
  ${BACNET_SOURCE_DIR}/BacnetToolReadProperty.cpp
  ${BACNET_SOURCE_DIR}/BacnetTstat.cpp
  ${BACNET_SOURCE_DIR}/BacnetTstatSchedule.cpp
  ${BACNET_SOURCE_DIR}/BacnetUserConfig.cpp
  ${BACNET_SOURCE_DIR}/BacnetUserlogin.cpp
  ${BACNET_SOURCE_DIR}/BacnetVarCusRang.cpp
  ${BACNET_SOURCE_DIR}/BacnetVariable.cpp
  ${BACNET_SOURCE_DIR}/BacnetView.cpp
  ${BACNET_SOURCE_DIR}/BacnetWait.cpp
  ${BACNET_SOURCE_DIR}/BacnetWeeklyRoutine.cpp
  ${BACNET_SOURCE_DIR}/Bacnet_Range_Msv.cpp
  ${BACNET_SOURCE_CM5_DIR}/BacNetProgram_transplant.cpp
  ${BACNET_SOURCE_CM5_DIR}/ListCtrlCellEx.cpp
  ${BACNET_SOURCE_CM5_DIR}/ListCtrlEx.cpp
  ${BACNET_SOURCE_CM5_DIR}/MyOwnListCtrl.cpp
  ${BACNET_SOURCE_DIR}/ConnectRemoteServer.cpp
  ${BACNET_SOURCE_DIR}/DebugWindow.cpp
  ${BACNET_SOURCE_DIR}/DownloadSocket.cpp
  ${BACNET_SOURCE_DIR}/DuplicateIdDetected.cpp
  ${BACNET_SOURCE_DIR}/ISPModeSlove.cpp    
  ${BACNET_SOURCE_CM5_DIR}/PointItem.cpp
  ${BACNET_SOURCE_DIR}/ShowMessageDlg.cpp
  ${BACNET_SOURCE_CM5_DIR}/SliderBoth.cpp
  ${BACNET_SOURCE_CM5_DIR}/SliderBothBtn.cpp
  ${BACNET_SOURCE_CM5_DIR}/CStatic/staticex.cpp
  ${BACNET_SOURCE_DIR}/StatusbarCtrl.cpp
  ${BACNET_SOURCE_DIR}/BacnetWeb.cpp
  ${BACNET_SOURCE_DIR}/BacnetSettingParamter.cpp
  ${BACNET_SOURCE_DIR}/CWebBrowser2.cpp
)

set(BACNET_CM5_H_FILES
  ${BACNET_SOURCE_CM5_DIR}/CStatic/staticex.h
  ${BACNET_SOURCE_CM5_DIR}/PointItem.h
  ${BACNET_SOURCE_CM5_DIR}/DialogCM5.h
  ${BACNET_SOURCE_CM5_DIR}/ListCtrlCellEx.h
  ${BACNET_SOURCE_CM5_DIR}/ListCtrlEx.h
  ${BACNET_SOURCE_CM5_DIR}/mygdiplus.h
  ${BACNET_SOURCE_CM5_DIR}/MyOwnListCtrl.h
  ${BACNET_SOURCE_CM5_DIR}/PARSER.H
  ${BACNET_SOURCE_CM5_DIR}/PerfTimer.h
  ${BACNET_SOURCE_CM5_DIR}/ROUTER.H
  ${BACNET_SOURCE_CM5_DIR}/SliderBoth.h
  ${BACNET_SOURCE_CM5_DIR}/SliderBothBtn.h
  ${BACNET_SOURCE_CM5_DIR}/T3000DEF.H
  ${BACNET_SOURCE_CM5_DIR}/ud_str.h
  ${BACNET_SOURCE_CM5_DIR}/user_data.h
)

set(BADO_SOURCE_FILES
  ${BADO_SOURCE_DIR}/BADO.cpp
  ${BADO_SOURCE_DIR}/BADO.h
)

set(CHARTCTRL_SOURCE_FILES
  ${CHARTCTRL_SOURCE_DIR}/ChartAxis.cpp
  ${CHARTCTRL_SOURCE_DIR}/ChartAxis.h
  ${CHARTCTRL_SOURCE_DIR}/ChartAxisLabel.cpp
  ${CHARTCTRL_SOURCE_DIR}/ChartAxisLabel.h
  ${CHARTCTRL_SOURCE_DIR}/ChartBalloonLabel.h
  ${CHARTCTRL_SOURCE_DIR}/ChartBalloonLabel.inl
  ${CHARTCTRL_SOURCE_DIR}/ChartBarSerie.cpp
  ${CHARTCTRL_SOURCE_DIR}/ChartBarSerie.h
  ${CHARTCTRL_SOURCE_DIR}/ChartCandlestickSerie.cpp
  ${CHARTCTRL_SOURCE_DIR}/ChartCandlestickSerie.h
  ${CHARTCTRL_SOURCE_DIR}/ChartCrossHairCursor.cpp
  ${CHARTCTRL_SOURCE_DIR}/ChartCrossHairCursor.h
  ${CHARTCTRL_SOURCE_DIR}/ChartCtrl.cpp
  ${CHARTCTRL_SOURCE_DIR}/ChartCtrl.h
  ${CHARTCTRL_SOURCE_DIR}/ChartCursor.cpp
  ${CHARTCTRL_SOURCE_DIR}/ChartCursor.h
  ${CHARTCTRL_SOURCE_DIR}/ChartDateTimeAxis.cpp
  ${CHARTCTRL_SOURCE_DIR}/ChartDateTimeAxis.h
  ${CHARTCTRL_SOURCE_DIR}/ChartDragLineCursor.cpp
  ${CHARTCTRL_SOURCE_DIR}/ChartDragLineCursor.h
  ${CHARTCTRL_SOURCE_DIR}/ChartFont.cpp
  ${CHARTCTRL_SOURCE_DIR}/ChartFont.h
  ${CHARTCTRL_SOURCE_DIR}/ChartGanttSerie.cpp
  ${CHARTCTRL_SOURCE_DIR}/ChartGanttSerie.h
  ${CHARTCTRL_SOURCE_DIR}/ChartGradient.cpp
  ${CHARTCTRL_SOURCE_DIR}/ChartGradient.h
  ${CHARTCTRL_SOURCE_DIR}/ChartGrid.cpp
  ${CHARTCTRL_SOURCE_DIR}/ChartGrid.h
  ${CHARTCTRL_SOURCE_DIR}/ChartLabel.h
  ${CHARTCTRL_SOURCE_DIR}/ChartLabel.inl
  ${CHARTCTRL_SOURCE_DIR}/ChartLegend.cpp
  ${CHARTCTRL_SOURCE_DIR}/ChartLegend.h
  ${CHARTCTRL_SOURCE_DIR}/ChartLineSerie.cpp
  ${CHARTCTRL_SOURCE_DIR}/ChartLineSerie.h
  ${CHARTCTRL_SOURCE_DIR}/ChartLogarithmicAxis.cpp
  ${CHARTCTRL_SOURCE_DIR}/ChartLogarithmicAxis.h
  ${CHARTCTRL_SOURCE_DIR}/ChartMouseListener.h
  ${CHARTCTRL_SOURCE_DIR}/ChartPointsArray.h
  ${CHARTCTRL_SOURCE_DIR}/ChartPointsArray.inl
  ${CHARTCTRL_SOURCE_DIR}/ChartPointsSerie.cpp
  ${CHARTCTRL_SOURCE_DIR}/ChartPointsSerie.h
  ${CHARTCTRL_SOURCE_DIR}/ChartScrollBar.cpp
  ${CHARTCTRL_SOURCE_DIR}/ChartScrollBar.h
  ${CHARTCTRL_SOURCE_DIR}/ChartSerie.cpp
  ${CHARTCTRL_SOURCE_DIR}/ChartSerie.h
  ${CHARTCTRL_SOURCE_DIR}/ChartSerieBase.h
  ${CHARTCTRL_SOURCE_DIR}/ChartSerieBase.inl
  ${CHARTCTRL_SOURCE_DIR}/ChartSeriesMouseListener.h
  ${CHARTCTRL_SOURCE_DIR}/ChartStandardAxis.cpp
  ${CHARTCTRL_SOURCE_DIR}/ChartStandardAxis.h
  ${CHARTCTRL_SOURCE_DIR}/ChartString.h
  ${CHARTCTRL_SOURCE_DIR}/ChartSurfaceSerie.cpp
  ${CHARTCTRL_SOURCE_DIR}/ChartSurfaceSerie.h
  ${CHARTCTRL_SOURCE_DIR}/ChartTitle.cpp
  ${CHARTCTRL_SOURCE_DIR}/ChartTitle.h
  ${CHARTCTRL_SOURCE_DIR}/ChartXYSerie.cpp
  ${CHARTCTRL_SOURCE_DIR}/ChartXYSerie.h
  ${CHARTCTRL_SOURCE_DIR}/PointsOrdering.h
)


set(CLASS_SOURCE_FILES
  ${CLASS_SOURCE_DIR}/md5.cpp
  ${CLASS_SOURCE_DIR}/md5.h
  ${CLASS_SOURCE_DIR}/MulitithreadSocket.cpp
  ${CLASS_SOURCE_DIR}/MulitithreadSocket.h
)

set(BACNET_HEADER_FILES
  ${BACNET_HEADER_DIR}/AddBuilding.h
  ${BACNET_HEADER_DIR}/AfxMessageDialog.h
  ${BACNET_HEADER_DIR}/AirflowSettingDlg.h
  ${BACNET_HEADER_DIR}/AllNodesDiaolg.h
  ${BACNET_HEADER_DIR}/AnnualRout_InsertDia.h
  ${BACNET_HEADER_DIR}/Annual_Routines.h
  ${BACNET_HEADER_DIR}/ApplyGraphicLabelsDlg.h
  ${BACNET_HEADER_DIR}/ARDDlg.h
  ${BACNET_HEADER_DIR}/AutoRichEditCtrl.h
  ${BACNET_HEADER_DIR}/Bacnetaddintodb.h
  ${BACNET_HEADER_DIR}/BacnetAddLabel.h
  ${BACNET_HEADER_DIR}/BacnetAddRemoteDevice.h
  ${BACNET_HEADER_DIR}/BacnetAddVirtualDevice.h
  ${BACNET_HEADER_DIR}/BacnetAlarmLog.h
  ${BACNET_HEADER_DIR}/BacnetAlarmWindow.h
  ${BACNET_HEADER_DIR}/BacnetAnalogCusRang.h
  ${BACNET_HEADER_DIR}/BacnetAnnualRoutine.h
  ${BACNET_HEADER_DIR}/BacnetATCommand.h
  ${BACNET_HEADER_DIR}/BacnetController.h
  ${BACNET_HEADER_DIR}/BacnetCustomerDigitalRange.h
  ${BACNET_HEADER_DIR}/BacnetEditLabel.h
  ${BACNET_HEADER_DIR}/BacnetGraphic.h
  ${BACNET_HEADER_DIR}/BacnetGraphicSetting.h
  ${BACNET_HEADER_DIR}/BacnetInput.h
  ${BACNET_HEADER_DIR}/BacnetIOConfig.h
  ${BACNET_HEADER_DIR}/BacnetMessageInput.h
  ${BACNET_HEADER_DIR}/BacnetMonitor.h
  ${BACNET_HEADER_DIR}/BacnetOutput.h
  ${BACNET_HEADER_DIR}/BacnetProgram.h
  ${BACNET_HEADER_DIR}/BacnetProgramDebug.h
  ${BACNET_HEADER_DIR}/BacnetProgramEdit.h
  ${BACNET_HEADER_DIR}/BacnetProgramSetting.h
  ${BACNET_HEADER_DIR}/BacnetRange.h
  ${BACNET_HEADER_DIR}/BacnetRegisterListView.h
  ${BACNET_HEADER_DIR}/BacnetRemotePoint.h
  ${BACNET_HEADER_DIR}/BacnetRemotePortWarning.h
  ${BACNET_HEADER_DIR}/BacnetScheduleTime.h
  ${BACNET_HEADER_DIR}/BacnetScreen.h
  ${BACNET_HEADER_DIR}/BacnetScreenEdit.h
  ${BACNET_HEADER_DIR}/BacnetSetting.h
  ${BACNET_HEADER_DIR}/BacnetSettingHealth.h
  ${BACNET_HEADER_DIR}/BacnetTool.h
  ${BACNET_HEADER_DIR}/BacnetToolReadProperty.h
  ${BACNET_HEADER_DIR}/BacnetTstat.h
  ${BACNET_HEADER_DIR}/BacnetTstatSchedule.h
  ${BACNET_HEADER_DIR}/BacnetUserConfig.h
  ${BACNET_HEADER_DIR}/BacnetUserlogin.h
  ${BACNET_HEADER_DIR}/BacnetVarCusRang.h
  ${BACNET_HEADER_DIR}/BacnetVariable.h
  ${BACNET_HEADER_DIR}/BacnetWait.h
  ${BACNET_HEADER_DIR}/BacnetWeeklyRoutine.h
  ${BACNET_HEADER_DIR}/Bacnet_Include.h
  ${BACNET_HEADER_DIR}/Bacnet_Range_Msv.h
  ${BACNET_HEADER_DIR}/BatchSNDlg.h
  ${BACNET_HEADER_DIR}/BinFileParser.h
  ${BACNET_HEADER_DIR}/BoatMonitorViewer.h
  ${BACNET_HEADER_DIR}/BootFinderDlg.h
  ${BACNET_HEADER_DIR}/BTUMeterDlg.h
  ${BACNET_HEADER_DIR}/BuildingConfigEditDlg.h
  ${BACNET_HEADER_DIR}/BuildingConfigration.h
  ${BACNET_HEADER_DIR}/BuildTable1.h
  ${BACNET_HEADER_DIR}/CControlBasicEditorCOM.h
  ${BACNET_HEADER_DIR}/ChangePasswordDlg.h
  ${BACNET_HEADER_DIR}/CO2NetView.h
  ${BACNET_HEADER_DIR}/CO2_NodeView.h
  ${BACNET_HEADER_DIR}/CO2_View.h
  ${BACNET_HEADER_DIR}/compat-T3000.h
  ${BACNET_HEADER_DIR}/ConfigFileHandler.h
  ${BACNET_HEADER_DIR}/Config_Routines.h
  ${BACNET_HEADER_DIR}/ConnectRemoteServer.h
  ${BACNET_HEADER_DIR}/ControlBasicEditorView.h
  ${BACNET_HEADER_DIR}/cslidercontrol0.h
  ${BACNET_HEADER_DIR}/CustomSTable.h
  ${BACNET_HEADER_DIR}/DebugWindow.h
  ${BACNET_HEADER_DIR}/define.h
  ${BACNET_HEADER_DIR}/DetectOnlineDlg.h
  ${BACNET_HEADER_DIR}/devintf.h
  ${BACNET_HEADER_DIR}/DialogCM5_BacNet.h
  ${BACNET_HEADER_DIR}/DialogInfo.h
  ${BACNET_HEADER_DIR}/Dialog_Progess.h
  ${BACNET_HEADER_DIR}/DisplayConfig.h
  ${BACNET_HEADER_DIR}/DllFunction.h
  ${BACNET_HEADER_DIR}/Dowmloadfile.h
  ${BACNET_HEADER_DIR}/DownloadSocket.h
  ${BACNET_HEADER_DIR}/DuplicateIdDetected.h
  ${BACNET_HEADER_DIR}/EnterValueDlg.h
  ${BACNET_HEADER_DIR}/Enter_Value_BinaryDlg.h
  ${BACNET_HEADER_DIR}/EreaseDlg.h
  ${BACNET_HEADER_DIR}/excel9.h
  ${BACNET_HEADER_DIR}/FileOperations.h
  ${BACNET_HEADER_DIR}/fileRW.h
  ${BACNET_HEADER_DIR}/Flash_Multy.h
  ${BACNET_HEADER_DIR}/FreeCoolDlg.h
  ${BACNET_HEADER_DIR}/global_define.h
  ${BACNET_HEADER_DIR}/global_function.h
  ${BACNET_HEADER_DIR}/global_struct.h
  ${BACNET_HEADER_DIR}/global_variable.h
  ${BACNET_HEADER_DIR}/global_variable_extern.h
  ${BACNET_HEADER_DIR}/GraphicMode.h
  ${BACNET_HEADER_DIR}/GraphicView.h
  ${BACNET_HEADER_DIR}/GridButton.h
  ${BACNET_HEADER_DIR}/GridLoad.h
  ${BACNET_HEADER_DIR}/GroupScheduleDlg.h
  ${BACNET_HEADER_DIR}/hangeIDDlg.h
  ${BACNET_HEADER_DIR}/HexFileParser.h
  ${BACNET_HEADER_DIR}/ImageTreeCtrl.h
  ${BACNET_HEADER_DIR}/IniFile.h
  ${BACNET_HEADER_DIR}/InputSetDlg.h
  ${BACNET_HEADER_DIR}/IONameConfig.h
  ${BACNET_HEADER_DIR}/ISPModeSlove.h
  ${BACNET_HEADER_DIR}/LabelEditDlg.h
  ${BACNET_HEADER_DIR}/LanguageLocale.h
  ${BACNET_HEADER_DIR}/LCOutNameConfig.h
  ${BACNET_HEADER_DIR}/LedsDialog.h
  ${BACNET_HEADER_DIR}/LoginDlg.h
  ${BACNET_HEADER_DIR}/MainFrm.h
  ${BACNET_HEADER_DIR}/ManageAccountDlg.h
  ${BACNET_HEADER_DIR}/modbus.h
  ${BACNET_HEADER_DIR}/ModbusToBacnetRouterDlg.h
  ${BACNET_HEADER_DIR}/monthview1.h
  ${BACNET_HEADER_DIR}/msflexgrid1.h
  ${BACNET_HEADER_DIR}/MySocket.h
  ${BACNET_HEADER_DIR}/NetworkControllView.h
  ${BACNET_HEADER_DIR}/NewTstatSchedulesDlg.h
  ${BACNET_HEADER_DIR}/OfficeStPtDlg.h
  ${BACNET_HEADER_DIR}/OutPutDlg.h
  ${BACNET_HEADER_DIR}/OutputSetDlg.h
  ${BACNET_HEADER_DIR}/OutsidDailog.h
  ${BACNET_HEADER_DIR}/OXBitmapButton.h
  ${BACNET_HEADER_DIR}/OXDllExt.h
  ${BACNET_HEADER_DIR}/OXMainRes.h
  ${BACNET_HEADER_DIR}/OXToolTipCtrl.h
  ${BACNET_HEADER_DIR}/ParameterDlg.h
  ${BACNET_HEADER_DIR}/ParameterExtDlg.h
  ${BACNET_HEADER_DIR}/ping.h
  ${BACNET_HEADER_DIR}/Powermeter.h
  ${BACNET_HEADER_DIR}/PowerMeterList.h
  ${BACNET_HEADER_DIR}/PressureSensorForm.h
  ${BACNET_HEADER_DIR}/ProductModel.h
  ${BACNET_HEADER_DIR}/ProductRegisterListView.h
  ${BACNET_HEADER_DIR}/RefreshTreeThread.h
  ${BACNET_HEADER_DIR}/RegisterWriteValueDlg.h
  ${BACNET_HEADER_DIR}/RelayLabel.h
  ${BACNET_HEADER_DIR}/RemotePtpLogin.h
  ${BACNET_HEADER_DIR}/resource.h
  ${BACNET_HEADER_DIR}/ScanDbWaitDlg.h
  ${BACNET_HEADER_DIR}/ScanDlg.h
  ${BACNET_HEADER_DIR}/Scanwaydlg.h
  ${BACNET_HEADER_DIR}/schedule.h
  ${BACNET_HEADER_DIR}/Schedule_grid.h
  ${BACNET_HEADER_DIR}/SetPtDayDlg.h
  ${BACNET_HEADER_DIR}/SetptGraphicBar.h
  ${BACNET_HEADER_DIR}/SetTimeDlg.h
  ${BACNET_HEADER_DIR}/ShowMessageDlg.h
  ${BACNET_HEADER_DIR}/StatusbarCtrl.h
  ${BACNET_HEADER_DIR}/stdafx.h
  ${BACNET_HEADER_DIR}/T3000.h
  ${BACNET_HEADER_DIR}/T3000DefaultView.h
  ${BACNET_HEADER_DIR}/T3000Doc.h
  ${BACNET_HEADER_DIR}/T3000LogWindow.h
  ${BACNET_HEADER_DIR}/T3000Option.h
  ${BACNET_HEADER_DIR}/T3000RegAddress.h
  ${BACNET_HEADER_DIR}/T3000UpdateDlg.h
  ${BACNET_HEADER_DIR}/T3000View.h
  ${BACNET_HEADER_DIR}/T3000_Default_MainView.h
  ${BACNET_HEADER_DIR}/T3000_Help_Map.h
  ${BACNET_HEADER_DIR}/T328IN.h
  ${BACNET_HEADER_DIR}/T332AI.h
  ${BACNET_HEADER_DIR}/T36CT.h
  ${BACNET_HEADER_DIR}/T38AI16O.h
  ${BACNET_HEADER_DIR}/T38AI8AO.h
  ${BACNET_HEADER_DIR}/T38I13O.h
  ${BACNET_HEADER_DIR}/T3ModulesOutputDlg.h
  ${BACNET_HEADER_DIR}/T3ModulesView.h
  ${BACNET_HEADER_DIR}/T3RangeDlg.h
  ${BACNET_HEADER_DIR}/T3RTDView.h
  ${BACNET_HEADER_DIR}/targetver.h
  ${BACNET_HEADER_DIR}/TCP_Server.h
  ${BACNET_HEADER_DIR}/TemcoDataGrid.h
  ${BACNET_HEADER_DIR}/TemcoStandardBacnetToolDlg.h
  ${BACNET_HEADER_DIR}/TempHumSensorForm.h
  ${BACNET_HEADER_DIR}/Timer_Calibrate.h
  ${BACNET_HEADER_DIR}/TrendLogView.h
  ${BACNET_HEADER_DIR}/TrendLogWnd.h
  ${BACNET_HEADER_DIR}/TroubleShootDlg.h
  ${BACNET_HEADER_DIR}/TstatAQ.h
  ${BACNET_HEADER_DIR}/TStatBase.h
  ${BACNET_HEADER_DIR}/TstatIconSetting.h
  ${BACNET_HEADER_DIR}/TStatInputView.h
  ${BACNET_HEADER_DIR}/TStatOutputView.h
  ${BACNET_HEADER_DIR}/TstatRangeDlg.h
  ${BACNET_HEADER_DIR}/TStatScanner.h
  ${BACNET_HEADER_DIR}/TStatScheduleDlg.h
  ${BACNET_HEADER_DIR}/TstatZigbeeLogic.h
  ${BACNET_HEADER_DIR}/TStat_Dev.h
  ${BACNET_HEADER_DIR}/Tstat_HelpDoc.h
  ${BACNET_HEADER_DIR}/TStat_Net.h
  ${BACNET_HEADER_DIR}/ULRBindCallback.h
  ${BACNET_HEADER_DIR}/UserAcessSetDlg.h
  ${BACNET_HEADER_DIR}/UTB64Bit.h
  ${BACNET_HEADER_DIR}/vector_declare.h
  ${BACNET_HEADER_DIR}/WeeklyRout_InsertDia.h
  ${BACNET_HEADER_DIR}/Weekly_Routines.h
  ${BACNET_HEADER_DIR}/WifiConfigDlg.h
  ${BACNET_HEADER_DIR}/WorkspaceBar.h
  ${BACNET_HEADER_DIR}/WriteRegistersValueDlg.h
  ${BACNET_HEADER_DIR}/WriteSingleRegDlg.h
  ${BACNET_HEADER_DIR}/ZigbeeInformationDlg.h
  ${BACNET_HEADER_DIR}/ZigbeeRepeater.h
  ${BACNET_HEADER_DIR}/BacnetProperty.h
  ${BACNET_HEADER_DIR}/BacnetSettingBasicInfo.h
  ${BACNET_HEADER_DIR}/BacnetSettingTcpip.h
  ${BACNET_HEADER_DIR}/BacnetSettingTime.h
  ${BACNET_HEADER_DIR}/BacnetSettingDyndns.h
  ${BACNET_HEADER_DIR}/BacnetEmailAlarm.h
  ${BACNET_HEADER_DIR}/CO2_AUTO_CALIBRATION.h
  ${BACNET_HEADER_DIR}/BacnetThirdPartyMain.h
  ${BACNET_HEADER_DIR}/TstatSetpointDetail.h
  ${BACNET_HEADER_DIR}/TstatAQI_Detail.h
)

set(SINGLESLIDER_SOURCE_FILES
  ${SINGLESLIDER_SOURCE_DIR}/MacSliderCtrl.cpp
  ${SINGLESLIDER_SOURCE_DIR}/MacSliderCtrl.h
)

set(T3000_SOURCE_FILES
  ${T3000_SOURCE_DIR}/AddBuilding.cpp
  ${T3000_SOURCE_DIR}/AfxMessageDialog.cpp
  ${T3000_SOURCE_DIR}/AirflowSettingDlg.cpp
  ${T3000_SOURCE_DIR}/AllNodesDiaolg.cpp
  ${T3000_SOURCE_DIR}/AnnualRout_InsertDia.cpp
  ${T3000_SOURCE_DIR}/Annual_Routines.cpp
  ${T3000_SOURCE_DIR}/ApplyGraphicLabelsDlg.cpp
  ${T3000_SOURCE_DIR}/ARDDlg.cpp
  ${T3000_SOURCE_DIR}/AutoRichEditCtrl.cpp
  ${T3000_SOURCE_DIR}/Bacnetaddintodb.cpp
  ${T3000_SOURCE_DIR}/BacnetAddLabel.cpp
  ${T3000_SOURCE_DIR}/BacnetAddRemoteDevice.cpp
  ${T3000_SOURCE_DIR}/BacnetAddVirtualDevice.cpp
  ${T3000_SOURCE_DIR}/BacnetAlarmLog.cpp
  ${T3000_SOURCE_DIR}/BacnetAlarmWindow.cpp
  ${T3000_SOURCE_DIR}/BacnetAnalogCusRang.cpp
  ${T3000_SOURCE_DIR}/BacnetAnnualRoutine.cpp
  ${T3000_SOURCE_DIR}/BacnetATCommand.cpp
  ${T3000_SOURCE_DIR}/BacnetController.cpp
  ${T3000_SOURCE_DIR}/BacnetCustomerDigitalRange.cpp
  ${T3000_SOURCE_DIR}/BacnetEditLabel.cpp
  ${T3000_SOURCE_DIR}/BacnetGraphic.cpp
  ${T3000_SOURCE_DIR}/BacnetGraphicSetting.cpp
  ${T3000_SOURCE_DIR}/BacnetInput.cpp
  ${T3000_SOURCE_DIR}/BacnetIOConfig.cpp
  ${T3000_SOURCE_DIR}/BacnetMessageInput.cpp
  ${T3000_SOURCE_DIR}/BacnetMonitor.cpp
  ${T3000_SOURCE_DIR}/BacnetOutput.cpp
  ${T3000_SOURCE_DIR}/BacnetProgram.cpp
  ${T3000_SOURCE_DIR}/BacnetProgramDebug.cpp
  ${T3000_SOURCE_DIR}/BacnetProgramEdit.cpp
  ${T3000_SOURCE_DIR}/BacnetProgramSetting.cpp
  ${T3000_SOURCE_DIR}/BacnetRange.cpp
  ${T3000_SOURCE_DIR}/BacnetRegisterListView.cpp
  ${T3000_SOURCE_DIR}/BacnetRemotePoint.cpp
  ${T3000_SOURCE_DIR}/BacnetRemotePortWarning.cpp
  ${T3000_SOURCE_DIR}/BacnetScheduleTime.cpp
  ${T3000_SOURCE_DIR}/BacnetScreen.cpp
  ${T3000_SOURCE_DIR}/BacnetScreenEdit.cpp
  ${T3000_SOURCE_DIR}/BacnetSetting.cpp
  ${T3000_SOURCE_DIR}/BacnetSettingHealth.cpp
  ${T3000_SOURCE_DIR}/BacnetTool.cpp
  ${T3000_SOURCE_DIR}/BacnetToolReadProperty.cpp
  ${T3000_SOURCE_DIR}/BacnetTstat.cpp
  ${T3000_SOURCE_DIR}/BacnetTstatSchedule.cpp
  ${T3000_SOURCE_DIR}/BacnetUserConfig.cpp
  ${T3000_SOURCE_DIR}/BacnetUserlogin.cpp
  ${T3000_SOURCE_DIR}/BacnetVarCusRang.cpp
  ${T3000_SOURCE_DIR}/BacnetVariable.cpp
  ${T3000_SOURCE_DIR}/BacnetView.cpp
  ${T3000_SOURCE_DIR}/BacnetWait.cpp
  ${T3000_SOURCE_DIR}/BacnetWeeklyRoutine.cpp
  ${T3000_SOURCE_DIR}/Bacnet_Range_Msv.cpp
  ${T3000_SOURCE_DIR}/BatchSNDlg.cpp
  ${T3000_SOURCE_DIR}/BinFileParser.cpp
  ${T3000_SOURCE_DIR}/BoatMonitorViewer.cpp
  ${T3000_SOURCE_DIR}/BootFinderDlg.cpp
  ${T3000_SOURCE_DIR}/BTUMeterDlg.cpp
  ${T3000_SOURCE_DIR}/BuildingConfigEditDlg.cpp
  ${T3000_SOURCE_DIR}/BuildingConfigration.cpp
  ${T3000_SOURCE_DIR}/BuildTable1.cpp
  ${T3000_SOURCE_DIR}/ChangePasswordDlg.cpp
  ${T3000_SOURCE_DIR}/CO2NetView.cpp
  ${T3000_SOURCE_DIR}/CO2_NodeView.cpp
  ${T3000_SOURCE_DIR}/CO2_View.cpp
  ${T3000_SOURCE_DIR}/ConfigFileHandler.cpp
  ${T3000_SOURCE_DIR}/Config_Routines.cpp
  ${T3000_SOURCE_DIR}/ConnectRemoteServer.cpp
  ${T3000_SOURCE_DIR}/ControlBasicEditorView.cpp
  ${T3000_SOURCE_DIR}/CustomSTable.cpp
  ${T3000_SOURCE_DIR}/DebugWindow.cpp
  ${T3000_SOURCE_DIR}/DetectOnlineDlg.cpp
  ${T3000_SOURCE_DIR}/DialogInfo.cpp
  ${T3000_SOURCE_DIR}/Dialog_Progess.cpp
  ${T3000_SOURCE_DIR}/DisplayConfig.cpp
  ${T3000_SOURCE_DIR}/DllFunction.cpp
  ${T3000_SOURCE_DIR}/Dowmloadfile.cpp
  ${T3000_SOURCE_DIR}/DownloadSocket.cpp
  ${T3000_SOURCE_DIR}/DuplicateIdDetected.cpp
  ${T3000_SOURCE_DIR}/EnterValueDlg.cpp
  ${T3000_SOURCE_DIR}/Enter_Value_BinaryDlg.cpp
  ${T3000_SOURCE_DIR}/EreaseDlg.cpp
  ${T3000_SOURCE_DIR}/excel9.cpp
  ${T3000_SOURCE_DIR}/FileOperations.cpp
  ${T3000_SOURCE_DIR}/fileRW.cpp
  ${T3000_SOURCE_DIR}/Flash_Multy.cpp
  ${T3000_SOURCE_DIR}/FreeCoolDlg.cpp
  ${T3000_SOURCE_DIR}/global_function.cpp
  ${T3000_SOURCE_DIR}/GraphicMode.cpp
  ${T3000_SOURCE_DIR}/GraphicView.cpp
  ${T3000_SOURCE_DIR}/GridButton.cpp
  ${T3000_SOURCE_DIR}/GridLoad.cpp
  ${T3000_SOURCE_DIR}/GroupScheduleDlg.cpp
  ${T3000_SOURCE_DIR}/hangeIDDlg.cpp
  ${T3000_SOURCE_DIR}/HexFileParser.cpp
  ${T3000_SOURCE_DIR}/ImageTreeCtrl.cpp
  ${T3000_SOURCE_DIR}/IniFile.cpp
  ${T3000_SOURCE_DIR}/InputSetDlg.cpp
  ${T3000_SOURCE_DIR}/IONameConfig.cpp
  ${T3000_SOURCE_DIR}/ISPModeSlove.cpp
  ${T3000_SOURCE_DIR}/LabelEditDlg.cpp
  ${T3000_SOURCE_DIR}/LanguageLocale.cpp
  ${T3000_SOURCE_DIR}/LCOutNameConfig.cpp
  ${T3000_SOURCE_DIR}/LedsDialog.cpp
  ${T3000_SOURCE_DIR}/LoginDlg.cpp
  ${T3000_SOURCE_DIR}/MainFrm.cpp
  ${T3000_SOURCE_DIR}/ManageAccountDlg.cpp
  ${T3000_SOURCE_DIR}/ModbusToBacnetRouterDlg.cpp
  ${T3000_SOURCE_DIR}/monthview1.cpp
  ${T3000_SOURCE_DIR}/msflexgrid1.cpp
  ${T3000_SOURCE_DIR}/MySocket.cpp
  ${T3000_SOURCE_DIR}/NetworkControllView.cpp
  ${T3000_SOURCE_DIR}/NewTstatSchedulesDlg.cpp
  ${T3000_SOURCE_DIR}/OfficeStPtDlg.cpp
  ${T3000_SOURCE_DIR}/OutPutDlg.cpp
  ${T3000_SOURCE_DIR}/OutputSetDlg.cpp
  ${T3000_SOURCE_DIR}/OutsidDailog.cpp
  ${T3000_SOURCE_DIR}/OXBitmapButton.cpp
  ${T3000_SOURCE_DIR}/OXToolTipCtrl.cpp
  ${T3000_SOURCE_DIR}/ParameterDlg.cpp
  ${T3000_SOURCE_DIR}/ParameterExtDlg.cpp
  ${T3000_SOURCE_DIR}/ping.cpp
  ${T3000_SOURCE_DIR}/Powermeter.cpp
  ${T3000_SOURCE_DIR}/PowerMeterList.cpp
  ${T3000_SOURCE_DIR}/PressureSensorForm.cpp
  ${T3000_SOURCE_DIR}/ProductRegisterListView.cpp
  ${T3000_SOURCE_DIR}/RefreshTreeThread.cpp
  ${T3000_SOURCE_DIR}/RegisterWriteValueDlg.cpp
  ${T3000_SOURCE_DIR}/RelayLabel.cpp
  ${T3000_SOURCE_DIR}/RemotePtpLogin.cpp
  ${T3000_SOURCE_DIR}/ScanDbWaitDlg.cpp
  ${T3000_SOURCE_DIR}/ScanDlg.cpp
  ${T3000_SOURCE_DIR}/Scanwaydlg.cpp
  ${T3000_SOURCE_DIR}/SetPtDayDlg.cpp
  ${T3000_SOURCE_DIR}/SetptGraphicBar.cpp
  ${T3000_SOURCE_DIR}/SetTimeDlg.cpp
  ${T3000_SOURCE_DIR}/ShowMessageDlg.cpp
  ${T3000_SOURCE_DIR}/StatusbarCtrl.cpp
  ${T3000_SOURCE_DIR}/stdafx.cpp
  ${T3000_SOURCE_DIR}/T3000.cpp
  ${T3000_SOURCE_DIR}/T3000DefaultView.cpp
  ${T3000_SOURCE_DIR}/T3000Doc.cpp
  ${T3000_SOURCE_DIR}/T3000LogWindow.cpp
  ${T3000_SOURCE_DIR}/T3000Option.cpp
  ${T3000_SOURCE_DIR}/T3000RegAddress.cpp
  ${T3000_SOURCE_DIR}/T3000UpdateDlg.cpp
  ${T3000_SOURCE_DIR}/T3000View.cpp
  ${T3000_SOURCE_DIR}/T3000_Default_MainView.cpp
  ${T3000_SOURCE_DIR}/T328IN.cpp
  ${T3000_SOURCE_DIR}/T332AI.cpp
  ${T3000_SOURCE_DIR}/T36CT.cpp
  ${T3000_SOURCE_DIR}/T38AI16O.cpp
  ${T3000_SOURCE_DIR}/T38AI8AO.cpp
  ${T3000_SOURCE_DIR}/T38I13O.cpp
  ${T3000_SOURCE_DIR}/T3ModulesOutputDlg.cpp
  ${T3000_SOURCE_DIR}/T3ModulesView.cpp
  ${T3000_SOURCE_DIR}/T3RangeDlg.cpp
  ${T3000_SOURCE_DIR}/T3RTDView.cpp
  ${T3000_SOURCE_DIR}/TCP_Server.cpp
  ${T3000_SOURCE_DIR}/TemcoDataGrid.cpp
  ${T3000_SOURCE_DIR}/TemcoStandardBacnetToolDlg.cpp
  ${T3000_SOURCE_DIR}/TempHumSensorForm.cpp
  ${T3000_SOURCE_DIR}/Timer_Calibrate.cpp
  ${T3000_SOURCE_DIR}/TrendLogView.cpp
  ${T3000_SOURCE_DIR}/TrendLogWnd.cpp
  ${T3000_SOURCE_DIR}/TroubleShootDlg.cpp
  ${T3000_SOURCE_DIR}/TstatAQ.cpp
  ${T3000_SOURCE_DIR}/TStatBase.cpp
  ${T3000_SOURCE_DIR}/TstatIconSetting.cpp
  ${T3000_SOURCE_DIR}/TStatInputView.cpp
  ${T3000_SOURCE_DIR}/TStatOutputView.cpp
  ${T3000_SOURCE_DIR}/TstatRangeDlg.cpp
  ${T3000_SOURCE_DIR}/TStatScanner.cpp
  ${T3000_SOURCE_DIR}/TStatScheduleDlg.cpp
  ${T3000_SOURCE_DIR}/TstatZigbeeLogic.cpp
  ${T3000_SOURCE_DIR}/TStat_Dev.cpp
  ${T3000_SOURCE_DIR}/TStat_Net.cpp
  ${T3000_SOURCE_DIR}/UserAcessSetDlg.cpp
  ${T3000_SOURCE_DIR}/WeeklyRout_InsertDia.cpp
  ${T3000_SOURCE_DIR}/Weekly_Routines.cpp
  ${T3000_SOURCE_DIR}/WifiConfigDlg.cpp
  ${T3000_SOURCE_DIR}/WorkspaceBar.cpp
  ${T3000_SOURCE_DIR}/WriteRegistersValueDlg.cpp
  ${T3000_SOURCE_DIR}/WriteSingleRegDlg.cpp
  ${T3000_SOURCE_DIR}/ZigbeeInformationDlg.cpp
  ${T3000_SOURCE_DIR}/ZigbeeRepeater.cpp
  ${T3000_SOURCE_DIR}/BacnetProperty.cpp
  ${T3000_SOURCE_DIR}/BacnetSettingBasicInfo.cpp
  ${T3000_SOURCE_DIR}/BacnetSettingTcpip.cpp
  ${T3000_SOURCE_DIR}/BacnetSettingTime.cpp
  ${T3000_SOURCE_DIR}/BacnetSettingDyndns.cpp
  ${T3000_SOURCE_DIR}/BacnetEmailAlarm.cpp
  ${T3000_SOURCE_DIR}/CO2_AUTO_CALIBRATION.cpp
  ${T3000_SOURCE_DIR}/BacnetThirdPartyMain.cpp
  ${T3000_SOURCE_DIR}/TstatSetpointDetail.cpp
  ${T3000_SOURCE_DIR}/TstatAQI_Detail.cpp
)

set(LIGHTINGCONTROLLER_SRC_FILES
  ${LIGHTINGCONTROLLER_SRC_DIR}/configure.cpp
  ${LIGHTINGCONTROLLER_SRC_DIR}/configure.h
  ${LIGHTINGCONTROLLER_SRC_DIR}/LightingController.cpp
  ${LIGHTINGCONTROLLER_SRC_DIR}/LightingController.h
  ${LIGHTINGCONTROLLER_SRC_DIR}/LightingSet.cpp
  ${LIGHTINGCONTROLLER_SRC_DIR}/LightingSet.h
)  

set(SQLITE_FILES
  ${SQLITE_HEADER_DIR}/CppSQLite3.cpp
  ${SQLITE_HEADER_DIR}/CppSQLite3.h
  ${SQLITE_HEADER_DIR}/sqlite3.h
)

set(T3_DIALOG_FILES
  ${T3_HEADER_DIR}/DialogT3.cpp
  ${T3_HEADER_DIR}/DialogT3.h
)

include_directories(${T3_HEADER_DIR} ${SQLITE_HEADER_DIR} ${T3000_SOURCE_DIR} ${SINGLESLIDER_SOURCE_DIR}
  ${BACNET_HEADER_DIR} ${CLASSCTRL_SOURCE_DIR} ${CHARTCTRL_SOURCE_DIR} ${BADO_SOURCE_DIR} ${BACNET_SOURCE_CM5_DIR}
  ${BACNET_SOURCE_CM5_DIR} ${AIRQUALITY_HEADER_DIR} ${BACNET_DLL_VC_INCLUDE} ${BACNET_DLL_OBJECT_INCLUDE}
  ${BACNET_DLL_HANDLER_INCLUDE} ${BACNET_MAIN_INCLUDE} ${LIGHTINGCONTROLLER_SRC_DIR} ${HTTP_API})

set(RESOURCE_FILES
  ${T3000_SOURCE_DIR}/T3000.rc
)

set_source_files_properties(${T3000_SOURCE_DIR}/T3000.rc   PROPERTIES LANGUAGE RC)
set(CMAKE_RC_FLAGS "${CMAKE_RC_FLAGS} -i ../")

# Add executable to build.
add_executable(${PROJECT_NAME} WIN32
   ${AIRQUALITY_FILES} ${BACNET_FILES} ${BACNET_CM5_H_FILES} ${BADO_SOURCE_FILES} ${CLASS_SOURCE_FILES} ${ISP_SOURCE_FILES} 
   ${CHARTCTRL_SOURCE_FILES} ${BACNET_HEADER_FILES} ${CLASSCTRL_SOURCE_FILES} ${BACNET_HEADER_FILES}
   ${SINGLESLIDER_SOURCE_FILES} ${T3000_SOURCE_FILES} ${SQLITE_FILES} ${T3_DIALOG_FILES} ${LIGHTINGCONTROLLER_SRC_FILES}
   ${CUSTOM_SOURCE_FILES} ${RESOURCE_FILES}
)

if(MSVC)
   #link_directories(${SQLITE_HEADER_DIR})
   target_link_libraries(${PROJECT_NAME} setupapi.lib 
   ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE}/ModbusDllforVc.lib 
   ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE}/FlexSlideBar.lib
   ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE}/BACnet_Stack_Library.lib
   ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE}/MultipleMonthCal32.lib
   ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE}/HttpAPI.lib
   )
   target_link_libraries(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/../SQLiteDriver/sqlite3.lib)
   add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different 
   ${CMAKE_CURRENT_SOURCE_DIR}/../SQLiteDriver/sqlite3.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE})
   SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/level='requireAdministrator' /uiAccess='false'")
endif(MSVC)

if(MSVC)
     SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "/Yustdafx.h")
     set_source_files_properties(stdafx.cpp PROPERTIES COMPILE_FLAGS "/Ycstdafx.h")	 
endif(MSVC)