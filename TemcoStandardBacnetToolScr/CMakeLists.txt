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

# C# projects are supported only from 3.8.2.
cmake_minimum_required(VERSION 3.10.0 FATAL_ERROR)

################### Variables. ####################
# Change if you want modify path or other values. #
###################################################

PROJECT(BacnetExplore VERSION 1.0.0.0 LANGUAGES CSharp)
include(CSharpUtilities)

set(PROPERTIES_FOLDER ./Properties)
set(RESOURCES_FOLDER ./Resources)
set(SRC_FOLDER .)

set(SRC_FILES 
    ${SRC_FOLDER}/AlarmSummary.cs
    ${SRC_FOLDER}/AlarmSummary.Designer.cs
    ${SRC_FOLDER}/BACnetBase.cs
    ${SRC_FOLDER}/BACnetClient.cs
    ${SRC_FOLDER}/BACnetTransport.cs
    ${SRC_FOLDER}/BacnetTransportEthernet.cs
    ${SRC_FOLDER}/BACnetTransportIPv6.cs
    ${SRC_FOLDER}/BacnetTransportSerial.cs
    ${SRC_FOLDER}/BBMDEditor.cs
    ${SRC_FOLDER}/BBMDEditor.Designer.cs
    ${SRC_FOLDER}/CalendarEditor.cs
    ${SRC_FOLDER}/CalendarEditor.Designer.cs
    ${SRC_FOLDER}/ComUtilities.cs
    ${SRC_FOLDER}/CreateObject.cs
    ${SRC_FOLDER}/CreateObject.Designer.cs
    ${SRC_FOLDER}/DeviceCommunicationControlDialog.cs
    ${SRC_FOLDER}/DeviceCommunicationControlDialog.Designer.cs
    ${SRC_FOLDER}/DeviceStorage.cs
    ${SRC_FOLDER}/DynamicPropertyGridContainer.cs
    ${SRC_FOLDER}/FileTransfers.cs
    ${SRC_FOLDER}/ForeignRegistry.cs
    ${SRC_FOLDER}/ForeignRegistry.designer.cs
    ${SRC_FOLDER}/GenericInputBox.cs
    ${SRC_FOLDER}/MainDialog.cs
    ${SRC_FOLDER}/MainDialog.Designer.cs
    ${SRC_FOLDER}/NotificationEditor.cs
    ${SRC_FOLDER}/NotificationEditor.designer.cs
    ${SRC_FOLDER}/Program.cs
    ${SRC_FOLDER}/ProgressDialog.cs
    ${SRC_FOLDER}/ProgressDialog.Designer.cs
    ${SRC_FOLDER}/ScheduleDisplay.cs
    ${SRC_FOLDER}/ScheduleDisplay.Designer.cs
    ${SRC_FOLDER}/SearchDialog.cs
    ${SRC_FOLDER}/SearchDialog.Designer.cs
    ${SRC_FOLDER}/SettingsDialog.cs
    ${SRC_FOLDER}/SettingsDialog.Designer.cs
    ${SRC_FOLDER}/SourceAddressDialog.cs
    ${SRC_FOLDER}/SourceAddressDialog.Designer.cs
    ${SRC_FOLDER}/T3000BacnetTool.cs
    ${SRC_FOLDER}/T3000BacnetTool.Designer.cs
    ${SRC_FOLDER}/TemcoBacnetTestTool.cs
    ${SRC_FOLDER}/TemcoBacnetTestTool.Designer.cs
    ${SRC_FOLDER}/TreeView.cs
    ${SRC_FOLDER}/TrendLogDisplay.cs
    ${SRC_FOLDER}/TrendLogDisplay.Designer.cs
    ${SRC_FOLDER}/AlarmSummary.resx
    ${SRC_FOLDER}/BBMDEditor.resx
    ${SRC_FOLDER}/CalendarEditor.resx
    ${SRC_FOLDER}/CreateObject.resx
    ${SRC_FOLDER}/DeviceCommunicationControlDialog.resx
    ${SRC_FOLDER}/ForeignRegistry.resx
    ${SRC_FOLDER}/MainDialog.resx
    ${SRC_FOLDER}/NotificationEditor.resx
    ${SRC_FOLDER}/ProgressDialog.resx
    ${SRC_FOLDER}/ScheduleDisplay.resx
    ${SRC_FOLDER}/SearchDialog.resx
    ${SRC_FOLDER}/SettingsDialog.resx
    ${SRC_FOLDER}/SourceAddressDialog.resx
    ${SRC_FOLDER}/T3000BacnetTool.resx
    ${SRC_FOLDER}/TemcoBacnetTestTool.resx
    ${SRC_FOLDER}/TrendLogDisplay.resx
    ${SRC_FOLDER}/app.config
)

set(RESOURCE_FILES 
    ${RESOURCES_FOLDER}/add.png
    ${RESOURCES_FOLDER}/application_form.png
    ${RESOURCES_FOLDER}/bell.png
    ${RESOURCES_FOLDER}/book_open.png
    ${RESOURCES_FOLDER}/calendar.png
    ${RESOURCES_FOLDER}/calendar_view_week.png
    ${RESOURCES_FOLDER}/chart_line.png
    ${RESOURCES_FOLDER}/clock.png
    ${RESOURCES_FOLDER}/cross.png
    ${RESOURCES_FOLDER}/database.png
    ${RESOURCES_FOLDER}/database_delete.png
    ${RESOURCES_FOLDER}/database_edit.png
    ${RESOURCES_FOLDER}/database_go1.png
    ${RESOURCES_FOLDER}/date.png
    ${RESOURCES_FOLDER}/EmbeddedDll/CalendarView.dll
    ${RESOURCES_FOLDER}/EmbeddedDll/PacketDotNet.dll
    ${RESOURCES_FOLDER}/EmbeddedDll/SharpPcap.dll
    ${RESOURCES_FOLDER}/EmbeddedDll/ZedGraph.dll
    ${RESOURCES_FOLDER}/error.png
    ${RESOURCES_FOLDER}/eye.png
    ${RESOURCES_FOLDER}/find.png
    ${RESOURCES_FOLDER}/information.png
    ${RESOURCES_FOLDER}/magnifier.png
    ${RESOURCES_FOLDER}/monitor.png
    ${RESOURCES_FOLDER}/note.png
    ${RESOURCES_FOLDER}/pencil_add.png
    ${RESOURCES_FOLDER}/pencil_delete.png
    ${RESOURCES_FOLDER}/text_smallcaps.png
    ${RESOURCES_FOLDER}/time_go.png
    ${RESOURCES_FOLDER}/transmit.png
    ${RESOURCES_FOLDER}/world.png
)

set(PROPERTIES_FILES
  ${PROPERTIES_FOLDER}/AssemblyInfo.cs
  ${PROPERTIES_FOLDER}/Resources.Designer.cs
  ${PROPERTIES_FOLDER}/Resources.resx
  ${PROPERTIES_FOLDER}/Settings.Designer.cs
  ${PROPERTIES_FOLDER}/Settings.settings
)

add_executable(BacnetExplore WIN32 app.config ReadSinglePropDescr.xml ReadSinglePropDescrDefault.xml YabeDeviceDescriptor.xml ${SRC_FILES} ${PROPERTIES_FILES} ${RESOURCE_FILES})

csharp_set_windows_forms_properties(  
  ${SRC_FOLDER}/AlarmSummary.cs
  ${SRC_FOLDER}/AlarmSummary.Designer.cs
  ${SRC_FOLDER}/AlarmSummary.resx
  ${SRC_FOLDER}/BBMDEditor.cs
  ${SRC_FOLDER}/BBMDEditor.resx
  ${SRC_FOLDER}/BBMDEditor.Designer.cs
  ${SRC_FOLDER}/CalendarEditor.cs
  ${SRC_FOLDER}/CalendarEditor.resx
  ${SRC_FOLDER}/CalendarEditor.Designer.cs
  ${SRC_FOLDER}/CreateObject.cs
  ${SRC_FOLDER}/CreateObject.resx
  ${SRC_FOLDER}/CreateObject.Designer.cs
  ${SRC_FOLDER}/DeviceCommunicationControlDialog.cs
  ${SRC_FOLDER}/DeviceCommunicationControlDialog.resx
  ${SRC_FOLDER}/DeviceCommunicationControlDialog.Designer.cs
  ${SRC_FOLDER}/ForeignRegistry.cs
  ${SRC_FOLDER}/ForeignRegistry.resx
  ${SRC_FOLDER}/ForeignRegistry.Designer.cs
  ${SRC_FOLDER}/MainDialog.cs
  ${SRC_FOLDER}/MainDialog.resx
  ${SRC_FOLDER}/MainDialog.Designer.cs
  ${SRC_FOLDER}/NotificationEditor.cs
  ${SRC_FOLDER}/NotificationEditor.resx
  ${SRC_FOLDER}/NotificationEditor.Designer.cs
  ${SRC_FOLDER}/ProgressDialog.cs
  ${SRC_FOLDER}/ProgressDialog.resx
  ${SRC_FOLDER}/ProgressDialog.Designer.cs
  ${SRC_FOLDER}/ScheduleDisplay.cs
  ${SRC_FOLDER}/ScheduleDisplay.resx
  ${SRC_FOLDER}/ScheduleDisplay.Designer.cs
  ${SRC_FOLDER}/SearchDialog.cs
  ${SRC_FOLDER}/SearchDialog.resx
  ${SRC_FOLDER}/SearchDialog.Designer.cs
  ${SRC_FOLDER}/SettingsDialog.cs
  ${SRC_FOLDER}/SettingsDialog.resx
  ${SRC_FOLDER}/SettingsDialog.Designer.cs
  ${SRC_FOLDER}/SourceAddressDialog.cs
  ${SRC_FOLDER}/SourceAddressDialog.resx
  ${SRC_FOLDER}/SourceAddressDialog.Designer.cs
  ${SRC_FOLDER}/T3000BacnetTool.cs
  ${SRC_FOLDER}/T3000BacnetTool.resx
  ${SRC_FOLDER}/T3000BacnetTool.Designer.cs
  ${SRC_FOLDER}/TemcoBacnetTestTool.cs
  ${SRC_FOLDER}/TemcoBacnetTestTool.resx
  ${SRC_FOLDER}/TemcoBacnetTestTool.Designer.cs
  ${SRC_FOLDER}/TrendLogDisplay.cs
  ${SRC_FOLDER}/TrendLogDisplay.resx
  ${SRC_FOLDER}/TrendLogDisplay.Designer.cs
)

csharp_set_designer_cs_properties(
  ${PROPERTIES_FOLDER}/AssemblyInfo.cs
  ${PROPERTIES_FOLDER}/Resources.Designer.cs
  ${PROPERTIES_FOLDER}/Resources.resx
  ${PROPERTIES_FOLDER}/Settings.Designer.cs
  ${PROPERTIES_FOLDER}/Settings.settings
)

set_property(TARGET ${project} PROPERTY VS_DOTNET_TARGET_FRAMEWORK_VERSION "v4.0.0")
set_target_properties(${project} PROPERTIES VS_GLOBAL_TargetFrameworkProfile "Client" )
set_target_properties(${project} PROPERTIES VS_GLOBAL_ROOTNAMESPACE "Yabe")
set_target_properties(${project} PROPERTIES VS_GLOBAL_ApplicationIcon "${SRC_FOLDER}/Search.ico")
set_target_properties(${project} PROPERTIES WIN32_EXECUTABLE TRUE)
set(CMAKE_CSharp_FLAGS "/langversion:latest")

set(OUT_DIR ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE})

set_property(TARGET BacnetExplore PROPERTY VS_DOTNET_REFERENCES
    "Microsoft.CSharp"
    "System"
    "System.Core"
    "System.Data"
    "System.Windows.Forms"
    "System.Drawing"
    "System.Xml"
    "SharpPcap"
    "PacketDotNet"
    "CalendarView"
    "ZedGraph"
)


  