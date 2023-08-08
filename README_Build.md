Using Visual Studio to compile T3000
============================

We have setup CI using MsBuild and github actions. This document explains compilation running locally on your machine.

Prerequisites
-------------
T3000 uses MFC, so make sure you have MFC installed within Visual Studio.

Specific instructions for Visual Studio 2019 on Windows 10 are:
* Download the Community edition of [Visual Studio 2019](https://visualstudio.microsoft.com/downloads/) installer (make sure you are covered by its licence terms).
* Run the installer and select:
	* Desktop development with C++
	* C++ MFC for latest 142 build tools (x86 & x64)
![T3000_VS2019_Installation](image/VS2019Installation.png "T3000 VS2019 Installation options")

Launching a Build
-----------------------------------------------------------
* Checkout T3000
* Sure, here's a step-by-step guide on how to compile a Visual Studio solution named "T3000 - VS2019.sln" using the Win32 platform and the Release configuration:

* **Open Visual Studio 2019:**
  
   Launch Visual Studio 2019 on your Windows machine.

* **Open Solution:**
  
   In the Visual Studio menu, click on "File" and then select "Open" > "Project/Solution." Navigate to the location where the "T3000 - VS2019.sln" solution file is located and select it.

* **Select Configuration and Platform:**
  
   In the toolbar at the top of the Visual Studio window, you'll see two dropdowns - one for the Solution Configuration and the other for the Solution Platform. Select "Release" from the Solution Configuration dropdown and "Win32" from the Solution Platform dropdown.

* **Build Solution:**
  
   Once you have set the appropriate configuration and platform, click on the "Build" menu in Visual Studio and then select "Build Solution" (or press Ctrl + Shift + B). This will initiate the compilation process.

* **Monitor Output:**
  
   The Output window in Visual Studio will display the progress of the compilation process. You'll see messages related to the compilation, linking, and any errors or warnings that might occur.

* **Check for Errors:**
  
   If there are any errors during the compilation process, the Output window will display information about the errors and their locations in the code. You will need to fix these errors before proceeding.

* **Successful Compilation:**
  
   If the compilation process completes without errors, you'll see a message indicating a successful build.

* **Locate Output Files:**
  
   Once the compilation is successful, the output files (executable, libraries, etc.) will be generated in the appropriate directory. By default, for the Release configuration, you can find the output files in the "Release" folder within your project directory.

* **Test the Application:**
  
   Before distributing or using the compiled application, it's important to test it thoroughly to ensure it behaves as expected. Run the executable generated during the compilation to verify its functionality.

Remember that these steps are general guidelines, and the actual process might vary based on the specifics of your project and any dependencies it might have. Additionally, make sure you have the required libraries, dependencies, and resources available to successfully compile the solution.

   
What to do if CI or local build fails with a build error
-----------------------------------------------------------
* If the above fails this is mostly due to:
   * Compilation errors in one or more CPP files
   * Developer _forgot_ to add new files to CMakeLists.txt



