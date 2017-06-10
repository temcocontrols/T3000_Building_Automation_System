@echo off

xcopy T3000Controls.dll  C:\Windows\Microsoft.NET\Framework\v4.0.30319\T3000Controls.dll        /y /f
cd C:\Windows\Microsoft.NET\Framework\v4.0.30319
RegAsm  .\T3000Controls.dll
pause
rem ::E:\COM.DELL为组件和存放全路径
rem ::C:\Windows\Microsoft.NET\Framework\v2.0.50727\;为REGASM.EXE所在文件夹路径。