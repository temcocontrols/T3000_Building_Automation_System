1. Run Visual Studio in administator mode(Right click -> Run as administator)
2. Build SliderControlLibrary(ir is registered dll with regasm)
3. Open MFC Conrol Designer ![](3.png)
4. Right click on dialog and select Insert ActiveX Control ![](4.png)
5. Select need control ![](5.png)
6. Control added ![](6-1.png). Properties and events are available: ![](6-2.png)![](6-3.png)
7. For set property from code need right click and select Add Variable ![](7.png)
8. Input variable name and click Finish ![](8.png)
9. If class not created, need next actions![](9.png):
10. Right click to project, Add -> Class.. ![](10.png)
11. Select MFC -> MFC Class From TypeLib ![](11.png)
12. Select SliderControlLibrary from dropdown menu, select need interface and click to button ">". Click finish ![](12.png)
13. Include header ![](13-1.png), rename class![](13-2.png), replace this![](13-3.png) as this![](13-4.png). Add include in stdafx![](13-5.png). Create file csharptypes.h with next data![](13-6.png)
```vc
#pragma once
#include <utility>

typedef CPoint Point;
typedef CSize Size;
typedef std::pair<float, float> PointF;
typedef std::pair<float, float> SizeF;

//#undef VTS_NONE
//#define VTS_NONE 

#define Rectangle CRect
```
14. Enter need properties ![](14-1.png) and get results![](14-2.png).
15. Thanks!

#Alternative:
1. Run Visual Studio Native Tools Command Promt in administator mode 
2. Go to SliderControlLibrary dll path
3. regasm SliderControlLibrary.dll /codebase /tlb SliderControlLibrary.tlb
4. Go to 3-15 first method.

#For internal usage in the resource file:

SliderControl guid: "ABA068FC-6B49-3031-B74A-1C51A3C8833A"

SetPointsControl guid: "A7CFC298-FE4E-4819-B868-2C014BA244DE"

#Other

Please, add post build events(for release and debug): 
```
copy /Y "..\SQLiteDriver\sqlite3.dll" "..\T3000 Output\release\"
copy /Y "..\SQLiteDriver\sqlite3.dll" "..\T3000 Output\debug\"
```
You can also add dependencies (Solution -> Properties -> Project Dependencies) to exclude situations where, after the build, the old version of the lib file of the dependent projects is used

Still, it is desirable to add a standard .gitignore .gitattribute files and Visual Studio.
https://github.com/HavenDV/ImprovedControls/blob/master/.gitattributes
https://github.com/HavenDV/ImprovedControls/blob/master/.gitignore

Before closing, need delete control with next code:
```
m_slider.Dispose();
```

In control .h file use next #import directive
```
#import "progid:T3000Controls.SliderControl" no_namespace
```