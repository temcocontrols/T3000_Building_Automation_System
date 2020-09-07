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

PROJECT(T3000Controls VERSION 1.0.0.0 LANGUAGES CSharp)
include(CSharpUtilities)

set(CONTROLS_FOLDER ./Controls)
set(EXTENSIONS_FOLDER ./Extensions)
set(FORMS_FOLDER ./Forms)
set(PROPERTIES_FOLDER ./Properties)
set(UTILITIES_FOLDER ./Utilities)

set(CONTROLS_FILES 
  ${CONTROLS_FOLDER}/BackgroundControl.cs
  ${CONTROLS_FOLDER}/BackgroundControl.Designer.cs
  ${CONTROLS_FOLDER}/BackgroundControl.resx
  ${CONTROLS_FOLDER}/HandleControl.cs
  ${CONTROLS_FOLDER}/HandleControl.Designer.cs
  ${CONTROLS_FOLDER}/HandleControl.resx
  ${CONTROLS_FOLDER}/IndicatorControl.cs
  ${CONTROLS_FOLDER}/IndicatorControl.Designer.cs
  ${CONTROLS_FOLDER}/IndicatorControl.resx
  ${CONTROLS_FOLDER}/Interfaces/ICheck.cs
  ${CONTROLS_FOLDER}/Interfaces/ICheckEvents.cs
  ${CONTROLS_FOLDER}/Interfaces/ISetPointsControl.cs
  ${CONTROLS_FOLDER}/Interfaces/ISliderControl.cs
  ${CONTROLS_FOLDER}/Interfaces/ISliderControlEvents.cs
  ${CONTROLS_FOLDER}/Interfaces/ITLabel.cs
  ${CONTROLS_FOLDER}/SetPointsControl.cs
  ${CONTROLS_FOLDER}/SetPointsControl.Designer.cs
  ${CONTROLS_FOLDER}/SetPointsControl.resx
  ${CONTROLS_FOLDER}/SliderControl.cs
  ${CONTROLS_FOLDER}/SliderControl.Designer.cs
  ${CONTROLS_FOLDER}/SliderControl.resx
  ${CONTROLS_FOLDER}/TCheckBox.cs
  ${CONTROLS_FOLDER}/TCheckBox.Designer.cs
  ${CONTROLS_FOLDER}/TLabel.cs
  ${CONTROLS_FOLDER}/TLabel.Designer.cs
  ${CONTROLS_FOLDER}/TLabel.resx
  ${CONTROLS_FOLDER}/TransparentLabel.cs
  ${CONTROLS_FOLDER}/TransparentLabel.resx
)

set(EXTENSION_FILES 
  ${EXTENSIONS_FOLDER}/IntegerExtensions.cs
)

set(FORMS_FILES
  ${FORMS_FOLDER}/Form1.cs
  ${FORMS_FOLDER}/Form1.Designer.cs
  ${FORMS_FOLDER}/Form1.resx
  ${FORMS_FOLDER}/SetPointsForm.cs
  ${FORMS_FOLDER}/SetPointsForm.Designer.cs
  ${FORMS_FOLDER}/SetPointsForm.resx
)

set(PROPERTIES_FILES
  ${PROPERTIES_FOLDER}/AssemblyInfo.cs
  ${PROPERTIES_FOLDER}/Resources.Designer.cs
  ${PROPERTIES_FOLDER}/Resources.resx
)

set(UTILITIES_FILES
  ${UTILITIES_FOLDER}/ComUtilities.cs
  ${UTILITIES_FOLDER}/GraphicsUtilities.cs
  ${UTILITIES_FOLDER}/MouseMover.cs
  ${UTILITIES_FOLDER}/SliderUtilities.cs
)

add_library(T3000Controls SHARED ${CONTROLS_FILES} ${EXTENSION_FILES} ${FORMS_FILES} ${PROPERTIES_FILES} ${UTILITIES_FILES})

csharp_set_designer_cs_properties(
  ${FORMS_FOLDER}/Form1.Designer.cs
  ${FORMS_FOLDER}/Form1.resx
  ${FORMS_FOLDER}/SetPointsForm.Designer.cs
  ${FORMS_FOLDER}/SetPointsForm.resx
  ${PROPERTIES_FOLDER}/Resources.Designer.cs
  ${PROPERTIES_FOLDER}/Resources.resx
  ${CONTROLS_FOLDER}/TLabel.Designer.cs
  ${CONTROLS_FOLDER}/TLabel.resx
  ${CONTROLS_FOLDER}/SliderControl.Designer.cs
  ${CONTROLS_FOLDER}/SliderControl.resx
  ${CONTROLS_FOLDER}/SetPointsControl.Designer.cs
  ${CONTROLS_FOLDER}/SetPointsControl.resx
  ${CONTROLS_FOLDER}/IndicatorControl.Designer.cs
  ${CONTROLS_FOLDER}/IndicatorControl.resx
  ${CONTROLS_FOLDER}/HandleControl.Designer.cs
  ${CONTROLS_FOLDER}/HandleControl.resx
  ${CONTROLS_FOLDER}/BackgroundControl.Designer.cs
  ${CONTROLS_FOLDER}/BackgroundControl.resx
)

set_property(TARGET ${project} PROPERTY VS_DOTNET_TARGET_FRAMEWORK_VERSION "v4.0.0")
set_property(TARGET ${project} PROPERTY WIN32_EXECUTABLE TRUE)
set(CMAKE_CSharp_FLAGS "/langversion:latest")

set(OUT_DIR ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE})

set_property(TARGET T3000Controls PROPERTY VS_DOTNET_REFERENCES
    "Microsoft.CSharp"
    "System"
    "System.Core"
    "System.Data"
    "System.Windows.Forms"
    "System.Drawing"
    "System.Xml"
)


  