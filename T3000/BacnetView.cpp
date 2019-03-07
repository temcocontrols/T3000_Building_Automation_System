// DialogCM5_BacNet.cpp : implementation file
// DialogCM5 Bacnet programming by Fance 2013 05 01
/*
//使用VS2010 编译需删除 c:\Program Files\Microsoft Visual Studio 10.0\VC\bin\cvtres.exe 来确保用更高版本的 来转换资源文件

2019 03 04
1. 解决 program 读出来 存入prg文件 只存了1600个字节的问题;

2019 03 01
1. 解决 program 中  save  load  某些 program decode error的问题;
2. ProgramEditDebug 中可以 显示 并更改 SCH ，HOL.
3. 更新Update 引擎， 并自动在T3000中替换原有Update.exe 文件.


2018 12 21
1. ScreenEdit 中 Sch 以及Hol 可以显示 ON OFF 值
2. AR1 AR2 动态加载至  SCH的 combo 中
3. 新增 BTU 界面  创建 resourcefile 的文件夹 以后资源文件都放在这个文件夹下更新
4. Zigbee Pan ID   for TSTAT8 

2018 12 11
1. 修复CO2 NET 无法显示 MODBUS_DEW_PT  的值
2. 修复CO2  Output2  value为负值的
3. Graphic view 下面的时间刻度 整数化, 左右移动改25%.

2018 11 27
1. Setting 界面 Done 不在修改IP
2. Setting Show LCD 只有BB 有，其他的需要隐藏;
3. Monitor 处新增 SD卡状态显示;

2018 09 03
1.TSTAT8 输入根据 寄存器来判断 是否显示HUM CO2 和光强;

2018 08 27
1.加载或保存 TSTAT8的配置时 新增更多寄存器
2.不在显示 初次加载TB 之类的对话框.


1. Fix the program bug "10 28AO5 = 3" when open again it show "10  = 3"

2018 05 29 
1.T3系列 扩展IO支持 T3-PT12 
2.支持更改名字  HUM CO2 PRESSURE .
3.mstp 通讯参数修改.
4.Bacnet Tool 改为非模态窗口, 支持多个窗口同时工作;
5.解决在切换panel时 ， 哪一个program是选中状态需要重新获取.否则会读到其他不相关的 program
6.修复network point 中 subpanel 为0 时 的显示问题.
7.优化扫描，若设备没有Zigbee ，不再会进行此项扫描
8.修复Screen 中 output 和variable  由 Auto 改为manual时  点击无法更改 digital 值的问题.

1. Support using Temco Bacnet tool and T3000 at the same time
2. Modify the Bacnet MSTP parameter to improve the communication.
3. Fix when the subpanel is 0 , the network point can't show in the UI.
4. Scanning -> If device don't have Zigbee module , don't scan this device using Zigbee.
5. Fix the screen edit interface , change the output and variable digital value from auto to manual ,the value can't change .


2018 04 27
1. 没有Zigbee的设备 Com1 不允许修改 ，以免误导客户.


2018 04 24 
1.修复 若保存Prg失败 却将以前的文件删除的问题.
2.从时间服务器更新时间返回信息提醒.
3.修复时间同步问题，包括改时区，夏令时 从470版本以后 用新的命令读写更新时间;


1. ISP tool (Prompt message optimization)
2. Change the basic ModbusDll
3. Support RS485 and Network  multy scan.
4. Fix the group window , the status of the  second ICON path not  correct.
5. Fix the bug when changing the range of T3TB,  the custom digital range window unexpected popup.
6. optimize the custom experience when changing T3 controller's IP address.

2018-0409 Update by Fance
1.支持同时扫描
2.开启T3000 默认进入的设备 由上次成功的连接决定.
3.修复 在设置 custom  range是 digitalrange 意外弹出 的问题;
4.在T3BB点击和电脑同步的时候 时区也一并同步 .

2018-04-02  Update by Fance
1. 支持 T3-6CTA 通过MODBUS 485 正常访问
2. 串口  多线程同时扫描  ， 修改底层 动态库 。（改动较大）


1.  The subpanel can be zero , ex:  1.0.VAR3
2.  Tool bar and menu and new icon , it will show the subpanel.Function not finished ,but ICON already added.
3.  Optimize the UI , when changing the IP address , if it is occupied by other PC or device , it will mention user can't use this IP.
4.  T3000 can auto find the device has the same IP address , and will pop up message to guide the user change one of the IP Address.
5. T3 controller can change LCD mode on the setting UI.
6. T3000 will release the UDP port 47808 when user want to use it in other software.
7. Fix the controll logic of  TSTAT8 setpoint in the parameter UI.
8. Fix the T3 controller , some of the input and output can't show the detail information when user in program edit interface ,Fix bugs  press 'Insert' ,nothing happen.


2018-0312 Update by Fance
1.修复 部分 input output debug 模式无法正常弹出的问题
2.修复 TSTAT8  setpoint ，现在修改值能够正常反应实际状态了.
3. T3000的把车net 端口不在占用 标准的 47808端口了
4. T3controller 增加 修改 LCD 显示 模式

2018-0130 Update by Fance
1.在加载prg文件的时候，变更 修改 panel 的方式.
2.支持subpanel 为0 ，以前跟main 一样.
3.toorbar和menu 菜单 新增network 和panel 的图标.
4.minipanel 优化修改 IP地址 ，优化等候方式及界面;有冲突的IP 不允许设置;
5.当广播发送 收到 两个设备的IP地址冲突时，回弹出更改视窗，修改IP地址;

2017-12-13 Update by Fance
1.串口二  支持  MSTP_MASTER.
2.修复list 在利用鼠标移动时 有些list 显示的背景色杂乱无章的问题
3.修复设备状态显示不正常的问题。
4.Tstat Schedual 控制增加DaySetpoint NightSetpoint SleepSetpoint AwakeSetpoint

2017-12-08 Update by Fance
1. 新增Minipanel界面下 tstat的schedule界面
2. 修复扫描时 对重复数据的处理
3.T3000的bacnet不在应答whois
4.解决 在22I的input界面往TSTAT8的input界面切换时始终无法切的问题。
5.新增Minipanel ARM 版本的 com0口支持全波特率。

2017 - 03 - 13 Update by Fance
1. 优化了在delete 树 产品时，重新加载 数据库  的问题.
2. 解决掉sort by floor 代码bug.

2017 - 02 - 23 Update by Fance
1. 针对minipanel 菜单上的保存上次浏览的记录.
2. 针对minipanel 重复panel的问题也修复了.

2017 - 02 - 20 Update by Fance
1. 在monitor input 中 改变一项时，颜色闪烁
2. custom table 限制不规则 表格

2017 - 02 - 10Update by Fance
1. 毛列了接近40条的 to do list ， 吐槽下 登山包。

2017 - 02 - 07 Update by Fance
1. Add feature  extension I/O .

2017 - 01 - 18 Update by Fance
1. Merge code finished . 
2. 修复 range edit填入数字引起的问题.

2017 - 01 - 16 Update by Fance
1. Fix 编程代码超过2000的时候会出问题...
2. 进一步优化Trend log 里面 digital 个数 显示的位置.

2017 - 01 - 16 Update by Fance
1. 增加 Var custmer units 客户自定义的 单位.
2. PRG 存成第六版 。存入这个单位.
3. 修复screen edit 中 操作数据库崩溃的问题.

2017 - 01 - 12 Update by Fance
1. 修复在input 界面跳转至其他设备住界面时  界面覆盖的问题;

2017 - 01 - 10 Update by Fance
1. 修复 bacview 中whois 反复发送的问题
2. 修复 bac setting 时间控件焦点问题
3. 改善Bac 操作UI ，支持所有窗口 双击标题栏 放大 缩小.
4. PID 增加时间 积分参数.

2017 - 01 - 7 Update by Fance
2. 在trendlog 界面加入Loading 图标.
3. PT12 Range支持 DI 输入.

2016 - 12 - 19 Update by Fance
1. 修复一个program里面 变量超过100个 就编译无法通过的问题;

2016 - 12 - 15 Update by Fance
1. 解决在program 时 TIME-ON 或者 TIME-OFF  个数超过30个以上后 TIME-ON( ) 里面的的值显示异常的问题;

2016 - 12 - 14 Update by Fance
1. 修改program 里面的时间  12：30  传值为1250.000;

2016 - 12 - 12 Update by Fance
1. 修复bug Setting将 

2016 - 12 - 02 Update by Fance
1. ISP slove 界面恢复初始.

2016 - 12 - 01 Update by Fance
1. 解决ISP tool 通过网络可能将A 的固件烧入B 的问题;
2. 在Trendlog 里面 的digital 不在叠加在一起，错开显示,并在最前面显示Label.
3. 修复 在Alarm 界面 由于接收到的时间错误引起的  弹窗  参数错误的问题.
4. 加入自动关闭 ISP_SLOVE 窗口 和 ID重复窗口.

2016 - 11 - 22 Update by Fance
2.  修改input 的扩展属性的显示
3. Download file 中修改  自动烧写 解决设备一直在isp中的问题.
4. 修改TSTAT Scan , 需要选择是否扫描Minipanel 下面的所有口的 各个波特率;
5. Update.exe 中加入 默认更新完成后打开T3000.

2016 - 11 - 04 Update by Fance
1. Setting界面点击update 3秒后自动刷新 setting 界面
2. 新增解决id冲突的显示界面.

2016 - 10 - 10 Update by Fance
1.修复针对T3的，如果minipanel界面在output 切换到T3时界面也要在output ，同理input.
2016 - 09 - 09 Update by Fance
1. Bacnet view Setting UI , 哪些下拉菜单  只有下拉功能，去掉编辑功能;
2. Downloadfile 里面 传文件的 包大小  改为3.5K ，大大加快传输效率;


2016 - 09 - 07 Update by Fance
1. 在其他设备的input 界面时,如果点击TSTAT的设备，需要跳转至TSTAT的input 界面.
2. 允许删除screen 里面的picture.
3. 支持T3 在多个设备之间切换INPUT , output.
4. Trend Log Y轴 取整.

2016 - 07 - 05 Update by Fance
1. 修复minipanel 时区与电脑时区不一致 引起的 trendlog 无法正常显示的问题;
2. 在点击扫描的时候 ， 发送 FF 55 FF 55 命令，让Minipanel 立刻去扫描下面的 设备;
3. 在选择range的时候 打开界面默认要选中字符串，以供客户修改;

2016 - 06 -23 Update by Fance
1. 树形结构里面支持鼠标移动，修复以前那种选中的设备无法正常选中的问题;
2. 修复扫描到的设备如果设备名称包含 "'" 这种数据库关键字  就崩溃的问题;

2016 - 06 -14 Update by Fance
1. Setting里面也存入prg文件 ，Setting 界面微调 位置;
2. IP冲突的界面微调位置和显示的label.
3. 不回广播的T3-BB 在线状态 不受 广播的影响;
4. Building 里面输入序列号后，保存起来;
5. ScreenEdit 里面 客户满意手动选择icon的时候用 default icon.
6. Analog custom tabel 在input 界面显示其单位.
7. 修复一个线程里面删数据库 可能由于vector 引起的 崩溃问题;

2016 - 06 - 03 Update by Fance
1. 修改Trend log graphic 宽度, 在绝大部分显示器上能显示完全;

2016 - 06 - 01 Update by Fance
1. program edit 界面增加右键 goto definition ,可以查看 当前的value
2. edit 的 debug 界面查看当前值 加入自动刷新 仅限 out in var;
3. 优化扫描网络设备时 数据库有的之前会先删掉, 不合适;
4. 修复 数据库更新时引起的  产品列表丢失;
5. 修复 Graphic Zoom in /Zoom out 不连续的问题;
6. Graphic  加入prg1 后  在锁定的情况下，点击能进入编程界面;
2016 - 05 - 31 Update by Fance
1. Graphic 右键在绘图区域点击 会显示当前的值;

2016 - 05 - 30  Update by Fance
1. Output PWM 周期 在选择digital的时候 禁止改动 wpm 周期这一列;
2. 删除一些不用的屏蔽代码;

2016 - 05 - 27 Update by Fance
1. Variable range  time 由 00:00 -> 00:00:00

2016 - 05 -26 Update by Fance
1.不同网段修改IP 成功后，立即更新数据库;
2.Setting里面Ip修改完毕后夜立即修复数据库;
3.Building 里面 区别处理remote device; remote 和本地的 不同时显示;
4.Graphic 修复digital 部分如果在 选定时间内没有点就不划线，现在改为 划此前点的 值;
5.Graphic 最上端增加显示 X轴的刻度; 修复X轴 时间 按10秒取整;

2016 - 05 - 23 Update by Fance
1. Cus Analog range 增加 单位显示和修改;

2016 - 05 - 19 Update by Fance
1. 修改Status bar 置顶的时候产生的 覆盖新窗口的问题;
2. 增加 远程连接temco 服务器的功能 ，未完全测试通过;

2016 - 05 - 11 Update by Fance
1.T3 -22i 1-11的input 增加 high speed count 功能.

2016 - 05 - 10 Update by Fance
1.Bacnet Setting  中 加入time update 最后更新的时间.
2.修复 input output 点击menu时 重新加载缓存文件 引起的 表格数据 异常.

2016 - 05 - 04 Update by Fance
1.支持更改 object instance;改为4个字节
2.重写 后台扫描函数;
3.后台扫描自动删除重复序列号的 设备;

2016 - 04 - 29 Update by Fance
1. 解决 Output table 宽度 显示 太长;
2. Range 里面 ON/OFF 和 OFF/ON 用两个 radio button 来表示; 并且 修复 客户自定义的 在第一次的时候自动读取;

2016 - 04 - 28 Update by Fance
1. 修改 Program 中 WAIT 函数 ;
2. 修复ISP  误烧写 .

2016 - 04 - 25 Update by Fance
1. 修复ISP 烧写的时候提示 不匹配的bug . mark 住了 最早以前的  ISP 的 部分;

2016 - 04 - 22 Update by Fance
1.支持旧版的T3 ，判断版本号 ，旧版就进入旧版的界面.
2.解决扫描的时候 会判断读multy的线程 引起 后台不扫描的bug

2016 - 04 - 21 Update by Fance
1. 支持在minipanel 界面显示
2. 调整graphic 界面的 大小, 将14个 input label 位置调整至左边，支持更小分辨率的客户.

2016 - 04 - 19 Update by Fance
1. 修复在program debug 界面下 按insert键后  新串口的 列错位的问题;
2. 修复label 的第一个字符如果是数字 引起的 program里面 解析 认为是 远程的点的问题，导致 Var1 显示为1.0.var1
3. 在debug 界面加入 bacnet 的 数据调试信息;


2016 - 04 -14 Update by Fance
1. 修复alarmlog 私有的数据 赋值时 的bug.

2016 - 04 -11 Update by Fance
1. Controller 的刷新时间改为4s 一次.

2016 - 04 - 07 Update by Fance
1. Input 和Output 加入panel 两列 如果是扩展的就要写n-m.
2. 小改动，trend log 右移后提示.

2016 - 04 - 06 Update by Fance
1. Graphic 按照老毛的 要求  把下面14个改成双行的.
2. 修复 unit 的问题，获取不到unit 的时候就从 其他data 那获取;
3. 在load prg 之后 
4. Output 扩展的最高位 区分是DO 还是AO
5. HAND_AM 如果不是Auto output 就全部显示红色.

2016 - 03 -30 Update by Fance
1. Monitor 删掉  时间限制 ，只要On 就开始记录.
2. Range 在output 里添加一个 模拟的PWM range .
3. Graphic 显示label 和单位 在左边的 树那里.
4. Setting 里面略微调整界面.
5. 所有协议只有Auto ，不管客户选什么协议都能搜到除了远程的和MSTP的设备.

2016 - 03 - 19 Update by Fance
1. 修复treeview 产品 状态显示的问题;

2016 - 03 - 18 Update by Fance
1. 在bacnet setting界面 加入 modbus ID.

2016 - 03 - 17 Update by Fance
//Fix the bugs which make the program crash when start up.
//原因是由 自动进入上一次的界面引起的;

2016 - 03 - 16  Update by Fance
//Merge Code.

//minipanel 寄存器表
//  9800	-	9999    200个寄存器   setting
//  10000	-   11471   1472		  OUT
//  11472   -   12943   1472		  IN
//	12943   -   15502	2560		  VAR					sizeof(Str_variable_point)= 39
//	15503   -  	15806	16*19=304	  PRG	                sizeof(Str_program_point) = 37
//  15807   -   15974	21*8=336	  SCH			sizeof(Str_weekly_routine_point) = 42
//	15975   -	16043		17*4=68		  HOL				sizeof(Str_annual_routine_point) = 33
//  


//  32712   - 32753		 14*16 =224								sizeof(Str_controller_point)	= 28						
1.在下载界面加入 固件文件夹。
2.Download hex or bin  file name change to 40 bytes.

2016 - 03 - 04 Update by Fance
1.ISP 修改显示 ISP 的 对话框 引起的 烧写网络 速度过快引起的 烧写中断的问题.


2016 - 03 -01 Update by Fance
1.自动连接服务器下载Hex代码.

2016 - 02 - 24 Update by Fance
1.修复当点击产品时，产品树 状态显示不正常的问题;

2016 - 1 -11 Update by Fance
1. T3需要加Modbus 寄存器 还要和minipanel 界面一样,要求先 加Minipanel 的 寄存器 ，建立一个数据库和T3对应起来.
2. 改改改 ，艹 ， 困的要死.

2016 - 1 - 8 Update by Fance
1. Setting 里面增加 health ，新的窗口显示各个TX RX的 value.
2. 增加一个 结构，和monitor 一样 ，用于在grp 里面存储 图片;

2016 - 1 - 5 Update by Fance
1. Input external show T3 加入两个新的T3 模块的产品号.

2016 - 1 - 5 Update by Fance
1.  Big  input27 - 32  small 11 - 16  tiny 6 - 11 range 里面有高速脉冲.  其他的 设置此range 都是低速脉冲;
	Add Analog input range -> Low speed count;

2016 - 1 - 4 Update by Fance
1. 吐槽下一月份 每个周六都要上班 ，操蛋.

2015 - 12 - 29 Update by Fance
1. 修复Input output 中关于 T3 扩展 无法正常显示的Bug.

2015 - 12 - 21 Update by Fance
1. 修复不同网卡下面 无法正常访问的问题.
2. 修复刷新树形产品时，先显示默认label，在显示真实label的问题;
3. 开启时检测防火墙是否打开，若打开了，提示客户关闭防火墙.
4.  Fix Bacnet Setting 中IP Auto 写入问题; 

2015 - 12 - 10 Update by Fance
1. Bacnet Screen  ,上下左右移动.
2. Screen 选择图片的时候 由单机改为双击.

2015 - 12 - 08  Update by Fance
1. Program IDE 大写小强制控制键盘输入
2. 更改 Program IDE color 的 default value.
3. Add new toolbar .Home.
4. Program 超过 2000个字节时不在溢出.给出超出提示;
5. Trend log 在有大部分数据要读时，优先只读最近的数据。然后在后台读取剩余的;
6. Trend log 在后台加范围 ， 用于过滤不合理的值;


2015 - 11 - 17 Update by Fance
1. 修复 program IDE debug window , 因 切换时间控件的焦点引起的 值不正确的BUG;
2. 在graphic window 最小化时 再次点击show graphic 时 ，会自动最大化;


2015 - 11 - 16 Update by Fance
1. 修复 在graphic 中 不选中部分input 的时候 ，在切换X轴的时间 不响应此动作的bug.

2015 - 11 - 12
1. Fix trend log analog value graphic line mess.

2015 - 11 - 11
1. Program IDE show error message ,if input some error label.
2. Program IDE warning color finished.

2015 - 11 - 09
1. Fix bugs Screen edit write one label to another device.

2015 - 11 - 04
Merge with Alex .

2015 - 11 - 04
1.Monitor no sd disk , save the data as temp data.

2015 - 10 - 28
1. Fix the second monitor doesn't work.

2015 - 10 - 26
1. Add a feature ,when saving the program file , also save the "txt" file into the temp folder 

2015 - 10 - 22
1. Fix graphic trend log ,value  .

2015 - 10 - 21
1. Improve Graphic Y axis value . automatic change the float value and position .
2. Add Analog input range -> High speed count;

2015 - 10 - 20
1. Add custom analog range when choose the input range.
2. Custom can delete trend log data.
3. Read monitor data can refresh data.
4. Schedules add a button for copy Monday-Friday data.

2015 - 09 - 14
1. Fix DI Package and AI

2015 - 09 - 10
1. Fix NewHumChamber can't write mini panel reg normally.
2. Show AI package and DI package number in monitor list.
3. Delete the mode "Graphic / Text" in screen list.
4. Add lock icon in screen edit window.When lock enabled , we can't move the label.
   Also double click label can edit the label.

2015 - 09 -09
Update by Fance
1. PC only run one T3000 .

2015 - 09 - 07
Update by Fance
1. Fix save config ,recover file not work.
2. In monitor UI , user can delete the monitor database .
3. In program UI , show the panel number and description in the window title.


2015 - 09 - 06 
Update by Fance
1. In the programming editor, if you save to  file.txt over top of an existing file.txt it will append to the end of the file. 
2. Descriptors were weird. We edited some inputs several times before they would 'take' properly. 
3. Icons
They don't work if the range is unused. Should work either way, configured or not. 
4. Getting out of the graphics screen is cumbersome. When I am viewing a graphic screen and hit INPUTS on the menu bar, I want to jump right direct to the inputs, not have to hit escape first.
5. Input status alarms. 
6. Read data succeed . change it to say "Read data: OK"
IF the range is unused, no alarms shall be generated. 
7 PID Titles. 
Change titles as shown. 
The grid spacing can be shrunk down , Action column for example. Most cols can be half this width. 



2015 - 08 - 31
1. Screen window hot key no longer occupied other application .
2. Input add a jumper setting feature.

2015 - 08 - 28
Update by Fance
1. 1-1-VAR2  ->  VAR2 
2. Fix the program color

2015 - 07 - 30
Update by Fance
1.Fix the bug when minimize the window ,can't restore the window.
2.Monitor unit in graphic now can normally show .

2015 - 07 - 29
Update by Fance
1.No Image information "No image has been config for current device, please config one first."  only show in the first 5 seconds.

2015 - 07 - 24
Update by Fance
1. Trend logs if data interval larger than the value we sampling don't connect the two point.
2. Config UI, don't let the user change the COM1 baud rate to 9600. And COM1 only support GSM and MB Master.

2015 - 07 - 16
Update by Fance
1.Update the keyword
Input			IN  ->   IN
Output			OUT ->   OUT
Variable		VAR ->   VAR
Program			PRG ->   PRG
PID				CON ->   PID  *Change*
SCHEDULE		WR  ->   SCH  *Change*
HOLIDAY			AR  ->   HOL  *Change*
GROUP			GRP ->   GRP

2015 - 07 - 15
Update by Fance
1. Bacnet PID Controller ,input support variables.

2015 - 07 - 07
Update by Fance
1. Auto update data in graphic view.
2. Fix the bug when sort the products , the status of the products is incorrect;

2015 - 07 - 03
Update by Fance
1.Add more information in bacnet configuration user interface.About "Network number" "MAC address".
2.In bacnet list ,In item zero press Upper key ,it will jump to the next panel.
3.Sort product view list by right click , and choose the menu .

2015 - 06 - 26
Update by Fance
1>>When click bacnet products , it connect faster than before .
2>>Bacnet program IDE. Add operate '&' and "|"  feature. eg: 10  VAR1 = 17 & 14.Custom wants program to controls the TSTAT register by bit.

2015 - 06 - 25
Update by Fance
1>>Graphic labels don't need read all 240. If some item is empty , don't read the left part.

2015 - 06 - 19
Update by Fance
1>>Change the status bar ,redraw the status.

2015 - 06 - 17
Update by Fance
1>>Change building products view,can view by floor or link structure.
2>>Icons changed, hotkey changed.


2015 - 06 - 09
Update by Fance
1>> Bacnet BIP_To_MSTP  input ,output var prg  items limit.Don't need read so many items.

2015 - 06 - 02
Update by Fance
1>> All bacnet list , colume header set character left side. 

2015 - 05 - 28
Update by Fance
1>> Add progress when read the BIP_TO_MSTP produtcs;

2015 - 05 - 20
Update by Fance
1>> Limit  input output variable numbers of items.
2>> Don't refresh data when the protocol is BIP_TO_MSTP;

2015 - 05 - 18
1>> Bacnet IP to MSTP ,need change.Need to read the data by blocking.
2015 - 04 - 27
1>> Fix the scan function can't show the device status correctly.

2015 - 04 - 20
Update by Fance
1>> Program IDE ,fix the bug when the error message length larger than MAX_PATH , it will crash.
2>> When decoding the program , fix some of the problem. Before "IN1 = IN1 - 0.05" can't decode.
3>> Support tiny minipanel .
2015 - 04 - 13
Update by Fance
1>> Screen edit window ,optimize the label position and operation of the label.
2>> Variable counts change to 64.

2015 - 04 - 10
Update by Fance
1>>In Screen edit window , when user using the full screen mode ,the label's position is not correctly show.
2>>Setting window add serial number.
3>>Variable label auto manual not show correctly.

2015 - 04 - 09
Update by Fance
1>> Load T3 serial register description . It can show in Minipanel remote point normally.
2>> All label can not have a same name . inputs label outputs label ...

2015 - 04 -08
Update by Fance
1>> Fix Screen Edit Label window , ICON name is not show correctly; GRP graphic not show correctly.


2015 - 04 - 03
Update by Fance
1>> Fix Monitor inputs can't show normally.
2>> Change the point type of ScreenEdit window.
3>> Screen window now can use left double click to full screen or cancel it.
4>> Program full label disappear when write then last item's full label.

2015 - 04 - 02
Update by Fance
1>>  Fixed the label in screen window can't show normally in the first few seconds. 
2>>  When add or edit label in screen window ,it will write data into minipanel right now.Before we write into device when exit the window. 
3>>  Fixed the  monitor input list crash when user change focus to other application. 
4>>  Error handle with th label point . Before "VAR-3" "3--VAR" will make the t3000 crash.  

2015 - 03 - 18
Update by Fance
1. For minipanel , user can login ,different level can access different data. Such as graphic mode only access graphic data.
2. DYNDNS service.
3. sync time. time zone.
4. Change point type.


2015 - 03 - 18
Update by Fance
1.Bacnet monitor finished.
2.Building config support remote connection.
3.Remote building scan.


2015 - 01 - 17
Update by Fance
1. Merge code. Database change!

2015 - 01 - 15
Update by Fance
1. Change the config icon and bacnet setting icon;
2. Bacnet setting UI change. Panel number and panel name change to normal font.

2015 - 01 - 12
Update by Fance
1. Optimize Bacnet read and write config .If user click new menu to read or write , don't interrupt the task. 
2. When the user delete some device , and the interface is show this device , it will exit and show the default building.

2015 - 01 - 07
Update by Fance
1. Bacnet program code -> If STATION_NUM == Mainpanel == SubPanel  .  243-243-VAR10 ----> Show VAR10 .
2. Program code will read 400 bytes all the time , no matter judge by the size of the program.
3. Fix the bugs all the bacnet list can't response the item change message.

2015 - 01 - 04
Update by Fance
1. ScreenEdit now support  go back to the last screen one by one.
2. Delete the tab in bacnet main screen. Add status bar icon menu.
3. Save the product status in DB every 30 seconds.
4. Bacnet input , Calibration change to int16.

.....
.....
.....


2014 - 11 - 18
Update by Fance
1.Add Bacnet device user login feature.
2.Recode the Screen and label ,make the graphic more beautiful.

2014-07-18
Update by Fance
1. Improve the quality of communication. 
2. Improve the network scan feature.
3. T3000 will scan device via serial port with baud rate both “9600” and “19200”.Before we only use 19200.
4. Add feature user can add device not through the scan command.
5. Bacnet programming feature support both lower-case and upper-case now.
6. All the Bacnet list (input/output/…), it will auto scroll, when user press up or down , the list item will automatically up and down.
7. Hide the graphic useless button. Add button feature to load factory default.(For Bacnet device)
8. Improve code to support read TSTAT6 through Zigbee module. Now it read smoothly.


2014-07-03
Update by Fance
1.All the Listctrl, it will auto scroll,when user press up or down , the list item will automatically up and down.
2.Hide the graphic useless button.(Maurice required)

2014-06-19
Update by Fance
1.Control Basic Editor: When finished editing and the users' program has been sent successfully, there is no need to show the exit Y/N confirmation dialog. Just exit right out with the 'Esc' key. 
2.Control Basic Editor: Update the error messages after each send of the program. 
3.Device Discovery: Fixed misc bugs related to auto discovery of bacnet devices and changes in the IP address. Sync the changes properly with the database. 
4.On line status of the nodes and the lower connection status bar now synchronized better. 
5.When user changes the bacnet "Auto/Manual" field the "value" column will refresh instantly, before it was slow.
6. Match number of inputs of the controller to the hardware model, ie if it has 16 Inptus T3000 will show only 16 inputs for the LB model, 32inputs for the LB model and so on. 
7. Change the multi flash update interface, TBD : Auto flash and support pause and resume.
8. Multi-flash update tool can now correctly handle 64k and 128k thermostats. 
9. Add feature that user can access the remote bacnet device by internet at a remote IP address. 
10. Add the bacnet interface to show the device software version , hardware version ,boot loader version,and other information.





2014-04-22
Update by Fance
1.Auto adjust different device show different Unit type.Take CM5 for example .
  CM5 contains 10 internal analog inputs and 8 internal digital inputs,So the inputs1 to input10,the "range" column only show "Analog",inputs11 to inputs18 only show "Digital"


2014-04-21
Update by Fance
1.The analog trend logging feature has been added. Digital data still to be done. 
2.Support to open the USB device "Testo-345" ,and read it's data.
TBD: add a small GUI screen and integrate to trend logs and other features
3.Add advanced setup screen for bacnet devices: IP address and others. 
4.Fix the modbus DLL .If the device disconnected , the "read_one" and
"write_one" used to fail if the node goes off line. Now it will automatically reconnect to the device.
5.Add a screen to show the Zigbee network RSSI signal strength now. TBD: add graphical version of zigbee network. 
6.Fix the bacnet panel number, max before was 32, now supports up to 255nodes on each rs485/mstp subnet and 255 on the main bacnet IP network

2014-04-16
Update by Fance
1.Fix the modbus DLL .If the device disconnected , the  "read_one" and "write_one" function will error until you connect again.
2.Add a new setting interface in the bacnet view.
2014-04-11
Update by Fance
1.Get the monitor data from the bacnet device.
2.Draw the graphic on the screen.

2014-03-28
Update by Fance
1.Update the Graphic view class.Now all the position and parameter can changed more easier. 
2014-03-27
Update by Fance
1.Finished the function ,Open the Testo USB port and read the Testo-435 data.Put the two function into the Dll,named "DllFunction.cpp"
2.Finished the Zigbee RSSI List , all the data will show on the list normally;

2014-03-20
Update by Fance
1.Change the graphic static,from staticex to own draw.
2.Change the ud_str.h ,some of the monitor struct.
2014-03-19
Update by Fance
1.Update the progress bar ,change the position to the button of the view.
2.Add read monitor data function.
2014-03-18
Update by Fance
1.Net cable disconnected ,the scan result also show the last connected device . ->Fix it.
2.If the Co2 external sensor count is a invalid value , the CO2 Window will crash . ->Fix it.
2014-03-17
Update by Fance
1.Support read Zigbee Tstat.(Not finished)
2014-03-14
Update by Fance
1.Program IDE user interface , panel upper 32 can't send program command,
	eg:  IF 36-OUT1 >5 THEN 87-IN2 = 3   (This shows 36-OUT1 is not defind)
	Fix this problem.
2.About program TIME-ON , TIME-OFF INTERVAL command ,now it can use normally.
3.Debug window don't always create when user don't needed.It will create when the user press SHIFT + 'D'
2014-03-07
Update by Fance
1.Allow the user to change the IP address and subnet and gateway.
2.Optimize the building graphic , user can add label (inputs ,outputs,variable) and monitor this label on the building view.
3.The device of CO2 show a wrong firmware version. Fix it.
4.Fix part of code which may crash the T3000.
2014-02-21
Update by Fance
1.BacnetTstat background color ,white and gray interval.
2.ISP tool in T3000 can't work , because the port 4321 is already bind by the T3000 .Already fix the problem
3.Modify the debug window hot key to shift + D

2014-02-20
Update by Fance
1.Code the BacnetScreenEdit.cpp , user can add PIC file and label in the pic file.user can monitor the value in the graphic.
2.Fix the BacnetScreenEdit.cpp memory leak.
3.Fix the BacnetScreen.cpp . No longer show  Text mode.
4.Fix the Bacnet object instance , T3000 no longer has the static instance ,use a random value.

2014-02-10
	Update by Fance
	1.TreeView update , support refresh different  com and network device.the Treeview of the device no longer
	  display a error status.

2014-01-13
	Update by Fance
	1.Add BacnetScreenEdit.cpp User can edit screen window,add label delete label.
	2.Modify the ListTree if one device is choose by user , It will change his color to red.
	3.Modify the range ,user can change the range such as  "ON/OFF" "OFF/ON" .In "ON/OFF" range the ON is 1 but in "OFF/ON" the ON it is 0;
	4.Change the Bacnet Main window. Insert all the property dialog into the view window.User can operated by the mouse click or keyboard input;
	5.Add Load config all and write config file .The file will be saved with "*.prg" and also user can loaded.
	6.Fix a bug that the program function "Interval" is not normally used;
2013-12-19
	Update by Fance
	1.Monify the list class,add function which we can select the list item.Also add list backcolor function;
	2.Add bacnet "Monitor" "Graphic" and "Screens".
	3.Use GDI to draw the bacnet monitor data.
	4.Add bacnet IP Scan,now T3000 can scan our own bacnet ip products.
	5.Add bacnet config file ,user can save the bacnet data to the config file,or load the config file;
	6.Fix the tree view can't show the bacnet products normally.
	7.Fix the transmission results,"RX" "TX" "ERROR".Now it can show bacnet ip transfer results normally;
	8.Fix all the bacnet listctrl , add function user can use keyboard operate it.
	9.Improve the function when user choose one of the bacnet list item ,if it is a combobox ,it will auto pull-down,and show all the items for user to choose.
	10.Improve  when user use bacnet function to change one of the value ,if the data change fail ,the data will resume to origin instead of show the error data on the screen.
	11.Fix program code dialog crashed,when the text length longer than the MAX_PATH;
	12.Fix when the label length can longer than the structure defined.
	13.Change the communication protocol from bacnetMSTP to BacnetIp;
	14.Save the bacnet instance id to mdb sw version and hw version,save the ip address to boudrate;
*/

#include "stdafx.h"
#include "T3000.h"
#include "DialogCM5_BacNet.h"
#include "CM5\MyOwnListCtrl.h"
#include "BacnetInput.h"
#include "BacnetOutput.h"
#include "BacnetProgram.h"
#include "BacnetVariable.h"
#include "global_function.h"
#include "Dialog_Progess.h"
#include "global_define.h"
#include "datalink.h"
#include "BacnetWait.h"
#include "Bacnet_Include.h"
#include "CM5\ud_str.h"
#include "BacnetWeeklyRoutine.h"
#include "BacnetAnnualRoutine.h"
#include "AnnualRout_InsertDia.h"
#include "BacnetController.h"
#include "BacnetScreen.h"
#include "BacnetMonitor.h"
#include "BacnetProgramEdit.h"
#include "rs485.h"
#include "BacnetScheduleTime.h"
#include "BacnetAlarmLog.h"
#include "BacnetAlarmWindow.h"
#include "BacnetTstat.h"
#include "BacnetSetting.h"
#include "BacnetCustomerDigitalRange.h"
#include "BacnetUserlogin.h"
#include "BacnetUserConfig.h"
#include "MainFrm.h"
#include "BacnetScreenEdit.h"
#include "BacnetRemotePoint.h"
#include "ShowMessageDlg.h"
int g_gloab_bac_comport = 1;
int g_gloab_bac_baudrate = 19200;
CString temp_device_id,temp_mac,temp_vendor_id;
HANDLE hwait_thread;
CString Re_Initial_Bac_Socket_IP;
BacnetWait *WaitDlg=NULL;
//extern int Station_NUM;
BacnetScreen *Screen_Window = NULL;
CBacnetProgram *Program_Window =NULL;
CBacnetInput *Input_Window = NULL;
CBacnetOutput *Output_Window = NULL;
CBacnetVariable *Variable_Window = NULL;
BacnetWeeklyRoutine *WeeklyRoutine_Window = NULL;
BacnetAnnualRoutine *AnnualRoutine_Window = NULL;
BacnetController *Controller_Window = NULL;
CBacnetMonitor *Monitor_Window = NULL;
CBacnetAlarmLog *AlarmLog_Window = NULL;
CBacnetTstat *Tstat_Window = NULL;
CBacnetSetting * Setting_Window = NULL;
CBacnetUserlogin* User_Login_Window = NULL;
CBacnetRemotePoint* Remote_Point_Window = NULL;
CDialog_Progess *WaitRead_Data_Dlg=NULL;
extern CBacnetScreenEdit * ScreenEdit_Window;
bool m_is_remote_device = false;
HANDLE hconnect_modbus_thread = NULL;
static int old_selected_item = WINDOW_PROGRAM; // 用于保存 上一个选中的 TAB; 用于 快捷键的操作;
extern CBacnetAlarmWindow * AlarmWindow_Window;
CBacnetProgramEdit *ProgramEdit_Window = NULL;
CBacnetScheduleTime *ScheduleEdit_Window = NULL;
AnnualRout_InsertDia *HolidayEdit_Window = NULL;

extern char mycode[2000];
int click_resend_time = 0;//当点击的时候，要切换device时 发送whois的次数;

// CDialogCM5_BacNet
CString IP_ADDRESS;
_Refresh_Info Bacnet_Refresh_Info;
CString remote_ip_address;

extern CString SaveConfigFilePath; //用来将资料存放至数据库，临时文件的存放目录;

extern SOCKET my_sokect;
extern bool show_user_list_window ;
HANDLE connect_mstp_thread = NULL; // 当点击MSTP的设备时开启 连接的线程;
HANDLE read_rs485_thread = NULL; // RS485的设备 通用;
HANDLE write_indb_thread = NULL; //将资料写入数据库的线程;

HANDLE hbip_whois_thread = NULL; //处理回复 I am 线程

int n_read_product_type = 0;
int n_read_list_flag = -1;

#define BAC_TIMER_2_WHOIS   2
#define BAC_TIMER_3_CHECKALARM         3
#define BAC_READ_SETTING_TIMER   4
#define BAC_RESET_WINDOW_TIMER   6
#define BAC_SET_LAST_UI			7

//#define WM_SEND_OVER     WM_USER + 1287
// int m_Input_data_length;
extern void  init_info_table( void );
extern void Init_table_bank();
CShowMessageDlg * ShowMessageDlg = NULL;
IMPLEMENT_DYNCREATE(CDialogCM5_BacNet, CFormView)

CDialogCM5_BacNet::CDialogCM5_BacNet()
	: CFormView(CDialogCM5_BacNet::IDD)
	, m_bac_obtain_ip_mode(0)
{
	m_MSTP_THREAD = true;
	m_cur_tab_sel = 0;
	//CM5_hThread = NULL;
}

CDialogCM5_BacNet::~CDialogCM5_BacNet()
{
}

void CDialogCM5_BacNet::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_LIST1, m_device_list_info);


	DDX_Control(pDX, IDC_BAC_MAINTAB, m_bac_main_tab);
}

BEGIN_MESSAGE_MAP(CDialogCM5_BacNet, CFormView)
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CDialogCM5_BacNet::OnBnClickedButtonTest)
	ON_MESSAGE(WM_FRESH_CM_LIST,Fresh_UI)
	ON_MESSAGE(MY_RESUME_DATA, AllMessageCallBack)
	ON_MESSAGE(WM_DELETE_NEW_MESSAGE_DLG,BacnetView_Message_Handle)	
	ON_MESSAGE(WM_SHOW_PROGRESS,Show_Progress)
	ON_MESSAGE(WM_RS485_MESSAGE,RS485_Read_Fun)
	ON_MESSAGE(WM_SHOW_PANELSTRING,Show_Panel_String)
	ON_MESSAGE(WM_CHANGE_NEXT_PANEL_MESSAGE,Change_Next_Panel)
	ON_WM_TIMER()
	ON_NOTIFY(TCN_SELCHANGE, IDC_BAC_MAINTAB, &CDialogCM5_BacNet::OnTcnSelchangeBacMaintab)
END_MESSAGE_MAP()


// CDialogCM5_BacNet diagnostics

#ifdef _DEBUG
void CDialogCM5_BacNet::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CDialogCM5_BacNet::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG



LRESULT CDialogCM5_BacNet::Change_Next_Panel(WPARAM wParam,LPARAM lParam)
{
	unsigned int temp_direction;
	unsigned int now_station_num;
	now_station_num = Station_NUM;
	temp_direction = (unsigned int )wParam;
	unsigned int next_lower_station_num = 0;
	unsigned int next_higher_station_num = 255;
	int next_index = -1;
	//获取目前的panel  ， 如果是向上键就 返回 上一个 panel;
	CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);

	if(temp_direction == LIST_UP)
	{
		for (int i=0;i< pFrame->m_product.size();i++)
		{
			if((pFrame->m_product.at(i).product_class_id != PM_MINIPANEL) &&
				(pFrame->m_product.at(i).product_class_id !=PM_CM5) &&
				(pFrame->m_product.at(i).product_class_id != PM_MINIPANEL_ARM))
			{
				continue;
			}

			if(pFrame->m_product.at(i).status == false)//下一个panel 不在线 就继续寻找下一个;
			{
				continue;
			}

			if((pFrame->m_product.at(i).panel_number > next_lower_station_num) &&
				(pFrame->m_product.at(i).panel_number < now_station_num) &&
				(pFrame->m_product.at(i).panel_number >= next_lower_station_num))
			{
				next_index = i;
				next_lower_station_num = pFrame->m_product.at(i).panel_number;
			}

		}
	}
	else if(temp_direction == LIST_DOWN)
	{
		for (int i=0;i< pFrame->m_product.size();i++)
		{
			if((pFrame->m_product.at(i).product_class_id != PM_MINIPANEL) &&
				(pFrame->m_product.at(i).product_class_id !=PM_CM5) &&
				(pFrame->m_product.at(i).product_class_id != PM_MINIPANEL_ARM))
			{
				continue;
			}

			if(pFrame->m_product.at(i).status == false)//下一个panel 不在线 就继续寻找下一个;
			{
				continue;
			}

			if((pFrame->m_product.at(i).panel_number < next_higher_station_num) &&
				(pFrame->m_product.at(i).panel_number > now_station_num)&&
				(pFrame->m_product.at(i).panel_number <= next_higher_station_num))
			{
				next_index = i;
				next_higher_station_num = pFrame->m_product.at(i).panel_number;
			}

		}
	}

	if(next_index >= 0)
	{
		//if(IDYES == MessageBox(_T("Do you want jump to the next panel ?"),_T("Notice"),MB_YESNO) )
			pFrame->DoConnectToANode(pFrame->m_product.at(next_index).product_item);
	}
	else
	{
		CString temp_cs;
		if(temp_direction == LIST_DOWN)
			temp_cs.Format(_T("This panel is %u , the next larger panel is not online."),now_station_num);
		else if(temp_direction == LIST_UP)
			temp_cs.Format(_T("This panel is %u , the next smaller panel is not online."),now_station_num);
		SetPaneString(BAC_SHOW_MISSION_RESULTS,temp_cs);
	}
	return 0;
}

//The window which created by the button ,will delete when the wait dialog send this message,to this window.
//It means ,it has done .we don't needed.
LRESULT CDialogCM5_BacNet::BacnetView_Message_Handle(WPARAM wParam,LPARAM lParam)
{
	int message_type = wParam;
	int n_lparam = (int)lParam;
	switch(message_type)
	{
	case 0:
		{
			if(WaitDlg!=NULL)
			{
				Sleep(50);
				if(WaitDlg)
					delete WaitDlg;
				WaitDlg = NULL;
			}
			if(bac_read_which_list == BAC_READ_ALL_LIST)
			{
				if(bac_read_all_results)
				{
					g_progress_persent = 100;
					SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Read data : OK!"));
					CString temp_file;
					CString temp_serial;
					temp_serial.Format(_T("%u.prog"),g_selected_serialnumber);
					temp_file = g_achive_folder + _T("\\") + temp_serial;
                    SaveBacnetBinaryFile(temp_file);
				}


				return 0;
			}

			if(bac_read_which_list == BAC_READ_SVAE_CONFIG)
			{
				if(bac_read_all_results)
				{
					::PostMessageW(MainFram_hwd,MY_BAC_CONFIG_READ_RESULTS,1,NULL);
					g_progress_persent = 100;
					SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Save config file success!"));
				}
				else
				{
					g_progress_persent = 0;
					SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Save config file failed! Timeout."));
					TRACE(_T("Failed\r\n"));
				}
				read_write_bacnet_config = false;
				return 0;
			}
			//if(bac_read_which_list != BAC_READ_USER_LOGIN_INFO)
			//{
			//	m_bac_main_tab.SetFocus();
			//}


			if(bac_read_which_list == BAC_READ_INPUT_LIST)
			{
				if(bac_input_read_results)
				{
					if(Input_Window == NULL)
					{
						Input_Window = new CBacnetInput;
						Input_Window->Create(IDD_DIALOG_BACNET_INPUT,this);	
					}
					else
					{
						if(Input_Window->IsWindowVisible())
							::PostMessage(m_input_dlg_hwnd,WM_REFRESH_BAC_INPUT_LIST,NULL,NULL);
					}
					//for (int i=0;i<WINDOW_TAB_COUNT;i++)
					//{
					//	pDialog[i]->ShowWindow(SW_HIDE);
					//}
					//Input_Window->ShowWindow(SW_SHOW);
					//m_bac_main_tab.SetCurSel(WINDOW_INPUT);
					//Input_Window->m_input_list.SetFocus();
				}
				else
				{
					if(hwait_thread)
					{
						TerminateThread(hwait_thread,0);
						hwait_thread = NULL;
					}
					SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Inputs list read time out!"));
					//MessageBox(_T("Inputs list read time out!"));	
				}
				return 0;
			}

			if(bac_read_which_list == BAC_READ_OUTPUT_LIST)
			{
				if(bac_output_read_results)
				{
					if(Output_Window == NULL)
					{
						Output_Window = new CBacnetOutput;
						Output_Window->Create(IDD_DIALOG_BACNET_OUTPUT,this);	
					}
					else
					{
						if(Output_Window->IsWindowVisible())
							::PostMessage(m_output_dlg_hwnd,WM_REFRESH_BAC_OUTPUT_LIST,REFRESH_LIST_NOW,NULL);
					}
					//for (int i=0;i<WINDOW_TAB_COUNT;i++)
					//{
					//	pDialog[i]->ShowWindow(SW_HIDE);
					//}
					//Output_Window->ShowWindow(SW_SHOW);
					//m_bac_main_tab.SetCurSel(WINDOW_OUTPUT);
					//Output_Window->m_output_list.SetFocus();

				}
				else
				{
					if(hwait_thread)
					{
						TerminateThread(hwait_thread,0);
						hwait_thread = NULL;
					}
					SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Outputs list read time out!"));
					//MessageBox(_T("Outputs list read time out!"));
				}
				return 0;
			}

			if(bac_read_which_list == BAC_READ_VARIABLE_LIST)
			{
				if(bac_variable_read_results)
				{
					if(Variable_Window == NULL)
					{
						Variable_Window = new CBacnetVariable;
						Variable_Window->Create(IDD_DIALOG_BACNET_VARIABLE,this);	
					}
					else
					{
						if(Variable_Window->IsWindowVisible())
							::PostMessage(m_variable_dlg_hwnd,WM_REFRESH_BAC_VARIABLE_LIST,REFRESH_LIST_NOW,NULL);
					}
					//for (int i=0;i<WINDOW_TAB_COUNT;i++)
					//{
					//	pDialog[i]->ShowWindow(SW_HIDE);
					//}
					//Variable_Window->ShowWindow(SW_SHOW);
					//m_bac_main_tab.SetCurSel(WINDOW_VARIABLE);

				}
				else
				{
					if(hwait_thread)
					{
						TerminateThread(hwait_thread,0);
						hwait_thread = NULL;
					}
					SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Variable list read time out!"));
					//MessageBox(_T("Variable list read time out!"));
				}
				return 0;
			}

			if(bac_read_which_list == BAC_READ_PROGRAM_LIST)
			{
				if(bac_program_read_results)
				{
					//CBacnetProgram DLG;
					//DLG.DoModal();

					if(Program_Window == NULL)
					{
						Program_Window = new CBacnetProgram;
						Program_Window->Create(IDD_DIALOG_BACNET_PROGRAM,this);	
					}
					else
					{
						::PostMessage(m_pragram_dlg_hwnd,WM_REFRESH_BAC_PROGRAM_LIST,NULL,NULL);
					}
					//for (int i=0;i<WINDOW_TAB_COUNT;i++)
					//{
					//	pDialog[i]->ShowWindow(SW_HIDE);
					//}
					//Program_Window->ShowWindow(SW_SHOW);
					//m_bac_main_tab.SetCurSel(WINDOW_PROGRAM);

				}
				else
				{
					if(hwait_thread)
					{
						TerminateThread(hwait_thread,0);
						hwait_thread = NULL;
					}
					SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Program list read time out!"));
					//MessageBox(_T("Program list read time out!"));
				}
				return 0;
			}

			if(bac_read_which_list == BAC_READ_WEEKLY_LIST)
			{
				if(bac_weekly_read_results)
				{
					//CBacnetProgram DLG;
					//DLG.DoModal();

					if(WeeklyRoutine_Window == NULL)
					{
						WeeklyRoutine_Window = new BacnetWeeklyRoutine;
						WeeklyRoutine_Window->Create(IDD_DIALOG_BACNET_WEEKLY_ROUTINES,this);	
					}
					else
					{
						::PostMessage(m_weekly_dlg_hwnd,WM_REFRESH_BAC_WEEKLY_LIST,NULL,NULL);
					}
					//for (int i=0;i<WINDOW_TAB_COUNT;i++)
					//{
					//	pDialog[i]->ShowWindow(SW_HIDE);
					//}
					//WeeklyRoutine_Window->ShowWindow(SW_SHOW);
					//m_bac_main_tab.SetCurSel(WINDOW_WEEKLY);
				}
				else
				{
					if(hwait_thread)
					{
						TerminateThread(hwait_thread,0);
						hwait_thread = NULL;
					}
					SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Weekly list read time out!"));
					//MessageBox(_T("Weekly list read time out!"));
				}
				return 0;
			}
			if(bac_read_which_list == BAC_READ_CUSTOMER_UNITS)
			{
				read_customer_unit = true;	//标志已经读过 单位了 不必再读，旧版本的设备 可能读不到;
				if(bac_customer_unit_read_results)
				{
					CBacnetCustomerDigitalRange Dlg;
					Dlg.DoModal();
				}
				return 0;
			}

			if(bac_read_which_list == BAC_READ_USER_LOGIN_INFO)
			{

				if((bac_user_login_read_results) && (show_user_list_window))
				{
					CBacnetUserConfig Dlg;
					Dlg.DoModal();
				}
				return 0;
			}
#if 0
			if(bac_read_which_list == BAC_READ_WEEKLY_LIST)
			{
				if(bac_weekly_read_results)
				{
					BacnetWeeklyRoutine Dlg;
					Dlg.DoModal();
				}
				else
					MessageBox(_T("Weekly Routine list read time out!"));
			}
#endif
			if(bac_read_which_list == BAC_READ_ANNUAL_LIST)
			{
				if(bac_annual_read_results)
				{
					if(AnnualRoutine_Window == NULL)
					{
						AnnualRoutine_Window = new BacnetAnnualRoutine;
						AnnualRoutine_Window->Create(IDD_DIALOG_BACNET_ANNUAL_ROUTINES,this);
					}
					else
					{
						::PostMessage(m_annual_dlg_hwnd,WM_REFRESH_BAC_ANNUAL_LIST,NULL,NULL);
					}
				}
				else
				{
					if(hwait_thread)
					{
						TerminateThread(hwait_thread,0);
						hwait_thread = NULL;
					}
					SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Annual Routine list read time out!"));
					//MessageBox(_T("Annual Routine list read time out!"));
				}
				return 0;
			}

			if(bac_read_which_list == BAC_READ_CONTROLLER_LIST)
			{
				if(bac_controller_read_results)
				{
					if(Controller_Window == NULL)
					{
						Controller_Window = new BacnetController;
						Controller_Window->Create(IDD_DIALOG_BACNET_CONTROLLER,this);
					}
					else
					{
						::PostMessage(m_controller_dlg_hwnd,WM_REFRESH_BAC_CONTROLLER_LIST,NULL,NULL);
					}
				}
				else
				{
					if(hwait_thread)
					{
						TerminateThread(hwait_thread,0);
						hwait_thread = NULL;
					}
					SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Controller list read time out!"));
					//MessageBox(_T("Controller list read time out!"));
				}
				return 0;
			}
			if(bac_read_which_list == BAC_READ_SCREEN_LIST)
			{
				if(bac_screen_read_results)
				{
					if(Screen_Window == NULL)
					{
						Screen_Window = new BacnetScreen;
						Screen_Window->Create(IDD_DIALOG_BACNET_SCREENS,this);	
					}
					else
					{
						::PostMessage(m_screen_dlg_hwnd,WM_REFRESH_BAC_SCREEN_LIST,NULL,NULL);
					}
				}
				else
				{
					if(hwait_thread)
					{
						TerminateThread(hwait_thread,0);
						hwait_thread = NULL;
					}
					SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Screens list read time out!"));
					//MessageBox(_T("Screens list read time out!"));
				}
				return 0;
			}

			if(bac_read_which_list == BAC_READ_TSTAT_LIST)
			{
				if(bac_tstat_read_results)
				{
					if(Tstat_Window == NULL)
					{
						Tstat_Window = new CBacnetTstat;
						Tstat_Window->Create(IDD_DIALOG_BACNET_TSTAT,this);
					}
					else
					{
						::PostMessage(m_tstat_dlg_hwnd,WM_REFRESH_BAC_TSTAT_LIST,NULL,NULL);
					}

				}
				else
				{
					if(hwait_thread)
					{
						TerminateThread(hwait_thread,0);
						hwait_thread = NULL;
					}
					SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Tstat list read time out!"));
				}
				return 0;
			}

			if(bac_read_which_list == BAC_READ_MONITOR_LIST)
			{
				if(bac_monitor_read_results)
				{
					if(Monitor_Window == NULL)
					{
						Monitor_Window = new CBacnetMonitor;
						Monitor_Window->Create(IDD_DIALOG_BACNET_MONITOR,this);
					}
					else
					{
						::PostMessage(m_screen_dlg_hwnd,WM_REFRESH_BAC_SCREEN_LIST,NULL,NULL);
					}
				}
				else
				{
					if(hwait_thread)
					{
						TerminateThread(hwait_thread,0);
						hwait_thread = NULL;
					}
					SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Monitor list read time out!"));
				}
				return 0;
			}

			if(bac_read_which_list == BAC_READ_ALARMLOG_LIST)
			{
				if(bac_alarmlog_read_results)
				{
					if(AlarmLog_Window == NULL)
					{
						AlarmLog_Window = new CBacnetAlarmLog;
						AlarmLog_Window->Create(IDD_DIALOG_BACNET_ALARMLOG,this);
					}
					else
					{
						if(AlarmLog_Window->IsWindowVisible())
							AlarmLog_Window->m_alarmlog_list.SetFocus();
					}

				}
				else
				{
					if(hwait_thread)
					{
						TerminateThread(hwait_thread,0);
						hwait_thread = NULL;
					}
					SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("AlarmLog read time out!"));
					//MessageBox(_T("AlarmLog read time out!"));
				}
				return 0;
			}



			if(bac_read_which_list == BAC_READ_GRAPHIC_LABEL_INFO)
			{
				if(bac_graphic_label_read_results)
				{
					need_read_bacnet_graphic_label_flag = false;
					bac_cm5_graphic = true;
					Screen_Window->Unreg_Hotkey();

					if(ScreenEdit_Window != NULL)
					{
						delete ScreenEdit_Window;
						ScreenEdit_Window = NULL;
					}
					ScreenEdit_Window = new CBacnetScreenEdit;
					ScreenEdit_Window->Create(IDD_DIALOG_BACNET_SCREENS_EDIT,this);	
					ScreenEdit_Window->ShowWindow(SW_SHOW);
				}
				else
				{
					SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Read data time out!"));
				}

				return 0;
			}

			if(bac_read_which_list == BAC_READ_WEEKLTCODE_LIST)
			{
				if(bac_weeklycode_read_results)
				{
					//显示非模态对话框;
					if(ScheduleEdit_Window != NULL)
					{
						delete ScheduleEdit_Window;
						ScheduleEdit_Window = NULL;
					}
					ScheduleEdit_Window = new CBacnetScheduleTime;
					ScheduleEdit_Window->Create(IDD_DIALOG_BACNET_SCHEDULE_TIME,this);	
					ScheduleEdit_Window->ShowWindow(SW_SHOW);

				}
				else
				{
					SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Weekly schedual time read time out!"));
					//MessageBox(_T("Weekly schedual time read time out!"));
				}

				return 0;
			}

			if(bac_read_which_list == BAC_READ_ANNUALCODE_LIST)
			{
				if(bac_annualcode_read_results)
				{
					//AnnualRout_InsertDia *HolidayEdit_Window = NULL;
					if(HolidayEdit_Window != NULL)
					{
						delete HolidayEdit_Window;
						HolidayEdit_Window = NULL;
					}
					HolidayEdit_Window = new AnnualRout_InsertDia;
					HolidayEdit_Window->Create(IDD_ANNUAL_ROUTINES_INSERT_DIA,this);	
					HolidayEdit_Window->ShowWindow(SW_SHOW);


					//AnnualRout_InsertDia Dlg;
					//Dlg.DoModal();
				}
				else
				{
					SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Holiday data read time out!"));
					//MessageBox(_T("Annual day time read time out!"));
				}

				return 0;
			}


			if((bac_read_which_list == BAC_READ_BASIC_SETTING_COMMAND) || (bac_read_which_list == BAC_READ_ALL_LIST))
			{
				if(bac_basic_setting_read_results)
				{
					//TRACE(_T("Read Setting success\r\n"));
					//PostMessage(WM_FRESH_CM_LIST,READ_SETTING_COMMAND,NULL);
				}
			}
		}
		break;
	case DELETE_WINDOW_MSG:
		//	m_bac_main_tab.SetFocus();
		//	m_bac_main_tab.SetCurSel(m_cur_tab_sel);
		break;
	case START_BACNET_TIMER:
		{
			DFTrace(_T("Connect to the device use the modbus ip and port success!"));
			m_bac_handle_Iam_data.clear();
			if(m_is_remote_device)
			{
				Send_WhoIs_remote_ip(remote_ip_address);
			}
			else
			{
				Send_WhoIs_Global(g_bac_instance, g_bac_instance);
			}
			//MessageBox(_T("Test2"));

			SetTimer(BAC_READ_SETTING_TIMER,500,NULL);
			click_resend_time = 10;
		}
		break;
	case CONNECT_TO_MODBUS_FAILED:
		{
			CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
			pFrame->m_pTreeViewCrl->turn_item_image(selected_tree_item ,false);
			MessageBox(_T("Connect to the device failed! No response from the TCP server!\
						  \r\nPlease Check the connection!\r\n"),_T("Notice"),MB_OK | MB_ICONINFORMATION);;
		}
		break;
	case PASSWORD_OK_INITIAL_UI:
		{
			m_user_level = n_lparam;
			Initial_Some_UI(n_lparam);
		}
		break;
	case SHOW_DEVICE_STATUS:
		{
			SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Reading data..."));
			bac_select_device_online = true;
			g_llTxCount ++ ;
			g_llRxCount ++;
		}
		break;
	case SHOW_PROGRAM_IDE:
		{
			if(bac_read_which_list == BAC_READ_PROGRAMCODE_LIST)
			{
				if(bac_programcode_read_results)
				{
					bac_read_which_list = -1;
					bac_programcode_read_results = false;

					//显示非模态对话框;
					if(ProgramEdit_Window != NULL)
					{
						delete ProgramEdit_Window;
						ProgramEdit_Window = NULL;
					}
					ProgramEdit_Window = new CBacnetProgramEdit;
					ProgramEdit_Window->Create(IDD_DIALOG_BACNET_PROGRAM_EDIT,this);	
					ProgramEdit_Window->ShowWindow(SW_SHOW);


				}

				return 0;
			}
		}
		break;
	}

	return 0;
}
// CDialogCM5_BacNet message handlers

LRESULT  CDialogCM5_BacNet::AllMessageCallBack(WPARAM wParam, LPARAM lParam)
{
	_MessageInvokeIDInfo *pInvoke =(_MessageInvokeIDInfo *)lParam;
	bool msg_result=WRITE_FAIL;
	msg_result = MKBOOL(wParam);
	CString Show_Results;
	CString temp_cs = pInvoke->task_info;
	if(msg_result)
	{
		for (int i=0;i<BAC_PROGRAM_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Program_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Program_Info[i].task_result = true;
		}

		for (int i=0;i<BAC_PROGRAMCODE_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Programcode_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Programcode_Info[i].task_result = true;
		}

		for (int i=0;i<BAC_OUTPUT_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Output_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Output_Info[i].task_result = true;
		}

		for (int i=0;i<BAC_INPUT_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Input_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Input_Info[i].task_result = true;
		}
		for (int i=0;i<BAC_VARIABLE_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Variable_Info[i].invoke_id)
			{
				Bacnet_Refresh_Info.Read_Variable_Info[i].task_result = true;
			}
		}

		for (int i=0;i<BAC_SCHEDULE_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Weekly_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Weekly_Info[i].task_result = true;
		}

		for (int i=0;i<BAC_SCHEDULECODE_GOUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Weeklycode_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Weeklycode_Info[i].task_result = true;
		}


		for (int i=0;i<BAC_HOLIDAY_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Annual_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Annual_Info[i].task_result = true;
		}

		for (int i=0;i<BAC_TIME_COMMAND_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Time_Command[i].invoke_id)
				Bacnet_Refresh_Info.Read_Time_Command[i].task_result = true;
		}

		for (int i=0;i<BAC_BASIC_SETTING_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_BasicSetting_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_BasicSetting_Info[i].task_result = true;
		}

		for (int i=0;i<BAC_PID_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Controller_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Controller_Info[i].task_result = true;
		}

		for (int i=0;i<BAC_SCREEN_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Screen_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Screen_Info[i].task_result = true;
		}

		for (int i=0;i<BAC_MONITOR_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Monitor_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Monitor_Info[i].task_result = true;
		}
		for (int i=0;i<BAC_HOLIDAYCODE_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Annualcode_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Annualcode_Info[i].task_result = true;
		}

		for (int i=0;i<BAC_ALARMLOG_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_AlarmLog_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_AlarmLog_Info[i].task_result = true;
		}



		for (int i=0;i<BAC_CUSTOMER_UNIT_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Customer_unit_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Customer_unit_Info[i].task_result = true;
		}

		for (int i=0;i<BAC_GRPHIC_LABEL_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].task_result = true;
		}

		Show_Results = temp_cs + _T("Success!");
		SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results);

	}
	else
	{
		for (int i=0;i<BAC_PROGRAM_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Program_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Program_Info[i].task_result = false;
		}

		for (int i=0;i<BAC_PROGRAMCODE_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Programcode_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Programcode_Info[i].task_result = false;
		}

		for (int i=0;i<BAC_OUTPUT_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Output_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Output_Info[i].task_result = false;
		}

		for (int i=0;i<BAC_INPUT_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Input_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Input_Info[i].task_result = false;
		}
		for (int i=0;i<BAC_VARIABLE_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Variable_Info[i].invoke_id)
			{
				Bacnet_Refresh_Info.Read_Variable_Info[i].task_result = false;
			}
		}

		for (int i=0;i<BAC_SCHEDULE_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Weekly_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Weekly_Info[i].task_result = false;
		}

		for (int i=0;i<BAC_SCHEDULECODE_GOUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Weeklycode_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Weeklycode_Info[i].task_result = false;
		}

		for (int i=0;i<BAC_HOLIDAY_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Annual_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Annual_Info[i].task_result = false;
		}

		for (int i=0;i<BAC_TIME_COMMAND_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Time_Command[i].invoke_id)
				Bacnet_Refresh_Info.Read_Time_Command[i].task_result = false;
		}

		for (int i=0;i<BAC_BASIC_SETTING_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_BasicSetting_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_BasicSetting_Info[i].task_result = false;
		}

		for (int i=0;i<BAC_PID_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Controller_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Controller_Info[i].task_result = false;
		}
		for (int i=0;i<BAC_SCREEN_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Screen_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Screen_Info[i].task_result = false;
		}
		for (int i=0;i<BAC_MONITOR_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Monitor_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Monitor_Info[i].task_result = false;
		}
		for (int i=0;i<BAC_HOLIDAYCODE_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Annualcode_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Annualcode_Info[i].task_result = false;
		}

		for (int i=0;i<BAC_ALARMLOG_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_AlarmLog_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_AlarmLog_Info[i].task_result = false;
		}


		for (int i=0;i<BAC_CUSTOMER_UNIT_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Customer_unit_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Customer_unit_Info[i].task_result = false;
		}

		for (int i=0;i<BAC_GRPHIC_LABEL_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].task_result = false;
		}
		g_progress_persent = 0 ; // 只要有一个读写失败 任务就会中断 ， 进度条就清零;
		Show_Results = temp_cs + _T("Fail!");
		SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results);

	}
	
	if(pInvoke)
		delete pInvoke;
	return 0;
}

void CDialogCM5_BacNet::OnBnClickedButtonTest()
{
	
	Send_WhoIs_Global(g_bac_instance, g_bac_instance);
}

static bool initial_once = true;
void CDialogCM5_BacNet::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	//Fresh();//Fance
	Initial_All_Point();
	
	//Tab_Initial();
	//initial_once = false;
	
}

void CDialogCM5_BacNet::Tab_Initial()
{
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	//为Tab Control增加两个页面;
	m_bac_main_tab.InsertItem(WINDOW_INPUT, _T("&Input   "));
	m_bac_main_tab.InsertItem(WINDOW_OUTPUT, _T("&Output   "));
	m_bac_main_tab.InsertItem(WINDOW_VARIABLE, _T("&Variable   "));
	m_bac_main_tab.InsertItem(WINDOW_PROGRAM, _T("&Program   "));
	m_bac_main_tab.InsertItem(WINDOW_CONTROLLER, _T("&Controller   "));
	m_bac_main_tab.InsertItem(WINDOW_SCREEN, _T("&Screen   "));
	m_bac_main_tab.InsertItem(WINDOW_WEEKLY, _T("&Weekly Routine   "));
	m_bac_main_tab.InsertItem(WINDOW_ANNUAL, _T("&Annual Routine   "));
	m_bac_main_tab.InsertItem(WINDOW_MONITOR, _T("&Monitor   "));
	m_bac_main_tab.InsertItem(WINDOW_ALARMLOG, _T("Alarm &Log   "));
	m_bac_main_tab.InsertItem(WINDOW_TSTAT, _T("&Tstat   "));
	m_bac_main_tab.InsertItem(WINDOW_SETTING, _T("Setting   "));
	m_bac_main_tab.InsertItem(12, _T("Remote Point   "));
	

	pDialog[WINDOW_INPUT] = Input_Window = new CBacnetInput;
	pDialog[WINDOW_OUTPUT] =Output_Window = new CBacnetOutput;
	pDialog[WINDOW_VARIABLE] = Variable_Window = new CBacnetVariable;
	pDialog[WINDOW_PROGRAM] = Program_Window = new CBacnetProgram;
	pDialog[WINDOW_CONTROLLER] = Controller_Window = new BacnetController;
	pDialog[WINDOW_SCREEN] = Screen_Window = new BacnetScreen;
	pDialog[WINDOW_WEEKLY] = WeeklyRoutine_Window = new BacnetWeeklyRoutine;
	pDialog[WINDOW_ANNUAL] = AnnualRoutine_Window = new BacnetAnnualRoutine;
	pDialog[WINDOW_MONITOR] = Monitor_Window = new CBacnetMonitor;
	pDialog[WINDOW_ALARMLOG] = AlarmLog_Window = new CBacnetAlarmLog;
	pDialog[WINDOW_TSTAT] = Tstat_Window = new CBacnetTstat;
	pDialog[WINDOW_SETTING] = Setting_Window = new CBacnetSetting;
	pDialog[WINDOW_USER_LOGIN] =  User_Login_Window = new CBacnetUserlogin;
	pDialog[WINDOW_REMOTE_POINT] =  Remote_Point_Window = new CBacnetRemotePoint;
	//创建两个对话框;
#if 0
	Input_Window->Create(IDD_DIALOG_BACNET_INPUT, &m_bac_main_tab);
	Output_Window->Create(IDD_DIALOG_BACNET_OUTPUT, &m_bac_main_tab);
	Variable_Window->Create(IDD_DIALOG_BACNET_VARIABLE, &m_bac_main_tab);
	Program_Window->Create(IDD_DIALOG_BACNET_PROGRAM, &m_bac_main_tab);
	Controller_Window->Create(IDD_DIALOG_BACNET_CONTROLLER, &m_bac_main_tab);
	Screen_Window->Create(IDD_DIALOG_BACNET_SCREENS, &m_bac_main_tab);
	WeeklyRoutine_Window->Create(IDD_DIALOG_BACNET_WEEKLY_ROUTINES, &m_bac_main_tab);
	AnnualRoutine_Window->Create(IDD_DIALOG_BACNET_ANNUAL_ROUTINES, &m_bac_main_tab);
	Monitor_Window->Create(IDD_DIALOG_BACNET_MONITOR, &m_bac_main_tab);
	AlarmLog_Window->Create(IDD_DIALOG_BACNET_ALARMLOG,&m_bac_main_tab);
	Tstat_Window->Create(IDD_DIALOG_BACNET_TSTAT,&m_bac_main_tab);
	Setting_Window->Create(IDD_DIALOG_BACNET_SETTING,&m_bac_main_tab);
	User_Login_Window->Create(IDD_DIALOG_BACNET_USER_LOGIN, this);
	Remote_Point_Window->Create(IDD_DIALOG_BACNET_REMOTE_POINT,&m_bac_main_tab);
#else
	Input_Window->Create(IDD_DIALOG_BACNET_INPUT, this);
	Output_Window->Create(IDD_DIALOG_BACNET_OUTPUT, this);
	Variable_Window->Create(IDD_DIALOG_BACNET_VARIABLE, this);
	Program_Window->Create(IDD_DIALOG_BACNET_PROGRAM, this);
	Controller_Window->Create(IDD_DIALOG_BACNET_CONTROLLER, this);
	Screen_Window->Create(IDD_DIALOG_BACNET_SCREENS, this);
	WeeklyRoutine_Window->Create(IDD_DIALOG_BACNET_WEEKLY_ROUTINES, this);
	AnnualRoutine_Window->Create(IDD_DIALOG_BACNET_ANNUAL_ROUTINES, this);
	Monitor_Window->Create(IDD_DIALOG_BACNET_MONITOR, this);
	AlarmLog_Window->Create(IDD_DIALOG_BACNET_ALARMLOG,this);
	Tstat_Window->Create(IDD_DIALOG_BACNET_TSTAT,this);
	Setting_Window->Create(IDD_DIALOG_BACNET_SETTING,this);
	User_Login_Window->Create(IDD_DIALOG_BACNET_USER_LOGIN, this);
	Remote_Point_Window->Create(IDD_DIALOG_BACNET_REMOTE_POINT,this);
#endif
	//设定在Tab内显示的范围;
	CRect rc;
	m_bac_main_tab.GetClientRect(rc);
	rc.top -= 400;
	rc.bottom -= 399;
	m_bac_main_tab.MoveWindow(&rc);

	GetClientRect(rc);
	//rc.top -= 20;

//	rc.top += 20;



	for (int i=0;i<WINDOW_TAB_COUNT;i++)
	{
		pDialog[i]->MoveWindow(&rc);
	}

	//CRect temp_mynew_rect
	//::GetWindowRect(BacNet_hwd,&temp_mynew_rect);	//获取 view的窗体大小;
	//MoveWindow(temp_mynew_rect.left,mynew_rect.top,mynew_rect.Width(),mynew_rect.Height(),1);

	//显示初始页面
	pDialog[WINDOW_INPUT]->ShowWindow(SW_HIDE);
	pDialog[WINDOW_OUTPUT]->ShowWindow(SW_HIDE);
	pDialog[WINDOW_VARIABLE]->ShowWindow(SW_HIDE);
	pDialog[WINDOW_PROGRAM]->ShowWindow(SW_HIDE);
	pDialog[WINDOW_CONTROLLER]->ShowWindow(SW_HIDE);
	pDialog[WINDOW_SCREEN]->ShowWindow(SW_HIDE);
	pDialog[WINDOW_WEEKLY]->ShowWindow(SW_HIDE);
	pDialog[WINDOW_ANNUAL]->ShowWindow(SW_HIDE);
	pDialog[WINDOW_MONITOR]->ShowWindow(SW_HIDE);
	pDialog[WINDOW_ALARMLOG]->ShowWindow(SW_HIDE);
	pDialog[WINDOW_TSTAT]->ShowWindow(SW_HIDE);
	pDialog[WINDOW_SETTING]->ShowWindow(SW_HIDE);
	pDialog[WINDOW_USER_LOGIN]->ShowWindow(SW_HIDE);
	pDialog[WINDOW_REMOTE_POINT]->ShowWindow(SW_HIDE);
    g_hwnd_now = m_input_dlg_hwnd;
	Input_Window->m_input_list.SetFocus();

	SetTimer(BAC_RESET_WINDOW_TIMER,10,NULL);
//保存当前选择
//	m_CurSelTab = WINDOW_INPUT;

}

void CDialogCM5_BacNet::Initial_All_Point()
{
	m_Input_data.clear();
	m_Variable_data.clear();
	m_Output_data.clear();
	m_Program_data.clear();
	m_Weekly_data.clear();
	m_Annual_data.clear();
	m_Schedual_Time_data.clear();
	m_controller_data.clear();
	m_screen_data.clear();
	m_monitor_data.clear();
	m_alarmlog_data.clear();
	m_Tstat_data.clear();
	m_customer_unit_data.clear();
	m_user_login_data.clear();
	m_tatat_schedule_data.clear();
	//vector <Str_TstatInfo_point> m_Tstat_data;
	for(int i=0;i<BAC_INPUT_ITEM_COUNT;i++)
	{
		Str_in_point temp_in;
		memset(temp_in.description,0,21);
		memset(temp_in.label,0,9);
		m_Input_data.push_back(temp_in);

	}
	for(int i=0;i<BAC_OUTPUT_ITEM_COUNT;i++)
	{
		Str_out_point temp_out;
		memset(&temp_out,0,sizeof(temp_out));
		m_Output_data.push_back(temp_out);
	}
	for (int i=0;i<BAC_VARIABLE_ITEM_COUNT;i++)
	{
		Str_variable_point temp_variable;
		memset(&temp_variable,0,sizeof(temp_variable));
		m_Variable_data.push_back(temp_variable);
	}
	for(int i=0;i<BAC_PROGRAM_ITEM_COUNT;i++)
	{
		Str_program_point temp_program;
		memset(&temp_program,0,sizeof(temp_program));
		temp_program.bytes = 0;//初始化时默认为400的长度，避免读不到数据;
		m_Program_data.push_back(temp_program);
	}
	for(int i=0;i<BAC_SCHEDULE_COUNT;i++)
	{
		Str_weekly_routine_point temp_weekly;
		memset(&temp_weekly,0,sizeof(temp_weekly));
		m_Weekly_data.push_back(temp_weekly);
	}
	for (int i=0;i<BAC_HOLIDAY_COUNT;i++)
	{
		Str_annual_routine_point temp_annual;
		memset(&temp_annual,0,sizeof(temp_annual));
		m_Annual_data.push_back(temp_annual);
	}

	for (int i=0;i<BAC_SCHEDULE_COUNT;i++)
	{
		Str_schedual_time_point temp_schedual;
		memset(&temp_schedual,0,sizeof(temp_schedual));
		m_Schedual_Time_data.push_back(temp_schedual);
	}

	for (int i=0;i<BAC_PID_COUNT;i++)
	{
		Str_controller_point temp_controller;
		memset(&temp_controller,0,sizeof(temp_controller));
		m_controller_data.push_back(temp_controller);
	}
	for (int i=0;i<BAC_SCREEN_COUNT;i++)
	{
		Control_group_point temp_screen;
		memset(&temp_screen,0,sizeof(temp_screen));
		m_screen_data.push_back(temp_screen);
	}
	for (int i=0;i<BAC_MONITOR_COUNT;i++)
	{
		Str_monitor_point temp_monitor;
		memset(&temp_monitor,0,sizeof(temp_monitor));
		m_monitor_data.push_back(temp_monitor);
	}

	for (int i=0;i<BAC_ALARMLOG_COUNT;i++)
	{
		Alarm_point temp_alarmpoint;
		memset(&temp_alarmpoint,0,sizeof(temp_alarmpoint));
		m_alarmlog_data.push_back(temp_alarmpoint);
	}
	for(int i=0;i<BAC_TSTAT_COUNT;i++)
	{
		Str_TstatInfo_point temp_tststpoint;
		memset(&temp_tststpoint,0,sizeof(temp_tststpoint));
		temp_tststpoint.product_model = 255;  //default 255  means no device;
		m_Tstat_data.push_back(temp_tststpoint);
		
	}

	for (int i=0;i<BAC_CUSTOMER_UNITS_COUNT;i++)
	{
		Str_Units_element temp_customer_units;
		memset(&temp_customer_units,0,sizeof(Str_Units_element));
		m_customer_unit_data.push_back(temp_customer_units);
	}
	
	for (int i=0;i<BAC_USER_LOGIN_COUNT;i++)
	{
		Str_userlogin_point temp_user_login;
		memset(&temp_user_login,0,sizeof(Str_userlogin_point));
		m_user_login_data.push_back(temp_user_login);
	}
	for(int i=0;i<BAC_GRPHIC_LABEL_COUNT;i++)
	{
		Str_label_point temp_label_point;
		memset(&temp_label_point,0,sizeof(Str_label_point));
		m_graphic_label_data.push_back(temp_label_point);
	}
	for (int i=0;i<BAC_REMOTE_POINT_COUNT;i++)
	{
		Str_remote_point temp_remote_point;
		memset(&temp_remote_point,0,sizeof(Str_remote_point));
		m_remote_point_data.push_back(temp_remote_point);
	}
	for (int i=0;i<BAC_ALALOG_CUSTMER_RANGE_TABLE_COUNT;i++)
	{
		Str_table_point temp_table_point;
		memset(&temp_table_point,0,sizeof(Str_table_point));
		m_analog_custmer_range.push_back(temp_table_point);
	}
	for (int i=0;i< BAC_VARIABLE_CUS_UNIT_COUNT ; i ++)
	{
			Str_variable_uint_point temp_var_unit_point;
			memset(&temp_var_unit_point,0,sizeof(Str_variable_uint_point));
			m_variable_analog_unite.push_back(temp_var_unit_point);
	}

	for (int i=0;i<BAC_EXTIO_COUNT;i++)
	{
		Str_Extio_point temp_extio_point;
		memset(&temp_extio_point,0,sizeof(Str_Extio_point));
		m_extio_config_data.push_back(temp_extio_point);
	}

	for (int i=0;i<BAC_TSTAT_SCHEDULE;i++)
	{
		Str_tstat_schedule temp_tstat_schedule;
		memset(&temp_tstat_schedule, 0, sizeof(Str_tstat_schedule));
		m_tatat_schedule_data.push_back(temp_tstat_schedule);

		//m_tatat_schedule_data.at(i).tstat.id = i + 1;
		//m_tatat_schedule_data.at(i).tstat.on_line = i % 2;
		//m_tatat_schedule_data.at(i).tstat.schedule = i % 9;
		//m_tatat_schedule_data.at(i).tstat.flag = 0x80;

	}

}


class Complex
{
public:
	Complex() {} //缺省构造函数
	Complex(double);	//带参数的构造函数
	Complex(double,double);
	Complex(int length, char *str);				 //构造函数;
	Complex operator * (const Complex&) const;   //重载操作符  *
	Complex operator / (const Complex&) const;	 //重载操作符  /
	Complex(const Complex&);					 //拷贝构造函数	->本例浅拷贝
	Complex &operator = (const Complex&);		//赋值构造函数 ->本例浅拷贝

	~Complex() {}
private:
	double real;
	double imag;
	int str_length;
	char * name;
};

Complex::Complex(int length, char * str)
{
	str_length = length;
	name = new char[str_length];
	if (name != NULL)
	{
		strcpy(name, str);
	}
}

Complex::Complex(double e)
{
	real = e;
	imag = 0;
	str_length = 0;
	name = NULL;

}

Complex::Complex(double treal, double timag)
{
	real = treal;
	imag = timag;
	str_length = 0;
	name = NULL;
}

Complex::Complex(const Complex &other)
{
	real = other.real;
	imag = other.imag;


	
}

Complex &Complex::operator= (const Complex & other)
{
	real = other.real;
	imag = other.imag;
	return *this;
}


Complex Complex::operator*(const Complex &other) const
{
	Complex Ret(real*other.real, imag*other.imag);
	return Ret;
}

Complex Complex::operator/(const Complex &other) const
{
	Complex Ret(real / other.real, imag / other.imag);
	return Ret;
}




typedef struct test_struct
{
	int index;
	int  name;
};



class FreshValue
{
public:
	FreshValue();   //缺省构造函数
	FreshValue(int, char*);
	FreshValue(double);
	FreshValue(double, double);
	FreshValue(const FreshValue&); //拷贝构造函数

	FreshValue &operator = (const FreshValue &); //赋值构造函数;
	FreshValue operator+  (const FreshValue &); //重载加号操作符;
	~FreshValue();
private:
	int str_length;
	char *name;
	double real;
	double imag;
};

FreshValue::~FreshValue()
{
	if(name!= NULL)
		delete name;
}

//重载操作符 + 号
FreshValue FreshValue::operator+ (const FreshValue &other)
{
	FreshValue Ret(real + other.real,imag + other.imag);
	return Ret;
	
}


//赋值构造函数 ，需要释放原有内存，开辟新内存接受 值.
FreshValue &FreshValue::operator= (const FreshValue& other)
{
	str_length = other.str_length;
	if(name!=NULL)
		delete name;
	else
	{
		return *this;
	}
	name = new char[str_length];
	if (name != NULL)
	{
		strcpy(name, other.name);
	}
	real = other.real;
	imag = other.imag;

	return *this;
}


FreshValue::FreshValue()
{
	str_length = 0;
	name = NULL;
	real = 0;
	imag = 0;
}

FreshValue::FreshValue(int temp_length, char *pchar)
{
	str_length = temp_length;
	name = new char[str_length];
	if (name != NULL)
	{
		strcpy(name, pchar);
	}
	real = 0;
	imag = 0;
}

FreshValue::FreshValue(double temp_real)
{
	real = temp_real;
	imag = 0;
	str_length = 0;
	name = NULL;
}


FreshValue::FreshValue(double temp_real, double temp_imag)
{
	real = temp_real;
	imag = temp_imag;
	str_length = 0;
	name = NULL;
}


//拷贝构造函数 -> 深拷贝.
FreshValue::FreshValue(const FreshValue&other)
{
	str_length = other.str_length;
	if ((other.name != NULL) && (other.str_length > 0))
	{
		name = new char[str_length];
		if (name != NULL)
		{
			strcpy(name, other.name);
		}
	}

	real = other.real;
	imag = other.imag;
}

typedef struct MyTree
{
	int index;
	MyTree * left;
	MyTree * right;

	MyTree()
	{
		left = NULL;
		right = NULL;
	}
};

int   isB(MyTree   *t)
{
	if (!t)   return   0;
	int   left = isB(t->left);
	int   right = isB(t->right);
	if (left >= 0 && right >= 0 && left - right <= 1 || left - right >= -1)
		return   (left < right) ? (right + 1) : (left + 1);
	else   return   -1;

}



class Bacnet
{
public:
	char * char_bac;

	Bacnet()
	{
		char_bac = NULL;
	}

	Bacnet(char *temp)
	{
		if (temp != NULL)
		{
			int length = strlen(temp);
			if (length > 0)
			{
				char_bac = new char[length+1];
				if (char_bac != NULL)
				{
					strcpy(char_bac, temp);
				}
			}

		}
	}

	Bacnet(const Bacnet &other)
	{
		if (other.char_bac != NULL)
		{
			int length = strlen(other.char_bac);
			if (length > 0)
			{
				char_bac = new char[length+1];
				if (char_bac != NULL)
				{
					strcpy(char_bac, other.char_bac);
				}
			}
		}
	}

	Bacnet &operator = (const Bacnet &other)
	{
		int length = strlen(other.char_bac);
		if (char_bac != NULL)
			delete[] char_bac;
		char_bac = new char[length+1];
		if (char_bac != NULL)
		{
			strcpy(char_bac, other.char_bac);
		}
		return *this;
	}

	Bacnet operator+ (const Bacnet &other)
	{
		int length = strlen(other.char_bac);
		if (length == 0)
		{
			return *this;
		}

		int new_length = length + strlen(char_bac);
		if (new_length > 0)
		{
			char *old_char = char_bac;
			char_bac = new char[new_length+1];
			if (char_bac != NULL)
			{
				if (old_char != NULL)
				{
					strcpy(char_bac, old_char);
					if (other.char_bac != NULL)
					{
						strcat(char_bac, other.char_bac);
						return *this;
					}
				}
			}
		}
		return *this;
	}

};

/*volatile*/ int test_thread = 0;
#if 0
HANDLE my_mutex = NULL;


DWORD WINAPI  Thread_1(LPVOID lpVoid)
{
	my_mutex = CreateMutex(NULL, true, _T("test_mutex"));
	ReleaseMutex(my_mutex);
	while (1)
	{
		WaitForSingleObject(my_mutex, INFINITE);
		test_thread++;
		TRACE(_T("thread1 %d \r\n"), test_thread);
		ReleaseMutex(my_mutex);
		Sleep(3000);
		
		
	}
	return 0;
}


DWORD WINAPI  Thread_2(LPVOID lpVoid)
{
	while (1)
	{
		WaitForSingleObject(my_mutex, INFINITE);
		test_thread++;
		TRACE(_T("thread2 %d\r\n"), test_thread);
		ReleaseMutex(my_mutex);
		Sleep(5000);
		
	}
	return 0;
}
#endif

#if 0
class String
{
private:
	char * str;
	int length;
	static int number ;
	enum {MAXNUMBER = 90};
public:
	String();
	String(const char *);
	String(const String&);
	String &operator = (const String&);
	String &operator = (const char *);
	char &operator[] (int);
	const char &operator[] (int) const;

	friend bool operator<(const String&, const String&);
	friend bool operator>(const String&, const String&);
	friend bool operator=(const String&, const String&);
	friend String operator+(const String&, const String&);

	friend ostream operator<<(ostream &os, const String&);
	friend istream operator >> (istream &is, const String&);
}
#endif





class Mystr
{
private:
	int length;
	char *str;
public:
	Mystr();
	Mystr(const char *);
	Mystr(const Mystr&);
	Mystr &operator=(const char *);
	Mystr &operator=(const Mystr&);
	char &operator[] (int);
	const char &operator[] (int) const;
	friend bool operator<(const Mystr&, const Mystr&);
	friend bool operator>(const Mystr&, const Mystr&);
	friend bool operator==(const Mystr&, const Mystr&);

	friend Mystr operator+(const Mystr&, const Mystr&);
};

char &Mystr::operator[](int index)
{
	return str[index];
}

const char &Mystr::operator[](int index) const
{
	return str[index];
}


bool operator<(const Mystr &str1, const Mystr &str2)
{
	return (strcmp(str1.str, str2.str) < 0);
}


Mystr &Mystr::operator=(const char *other)
{
	delete[] str;
	length = strlen(other);
	str = new char[length + 1];
	if(str!= NULL)
		strcpy(str, other);
	return *this;
}

Mystr &Mystr::operator=(const Mystr&other)
{
	if (this == &other)
		return *this;
	delete[] str;
	length = other.length;
	str = new char[length + 1];
	if (str != NULL)
	{
		strcpy(str, other.str);
	}

	return *this;
}

Mystr::Mystr()
{
	length = 0;
	str = new char[1];
	str[0] = '\0';
}

Mystr::Mystr(const char *other)
{
	length = strlen(other);
	str = new char[length + 1];
	strcpy_s(str, length, other);
}

Mystr::Mystr(const Mystr&other)
{
	length = other.length;
	str = new char[length + 1];
	if (str != NULL)
	{
		strcpy_s(str, length, other.str);
	}
}



void CDialogCM5_BacNet::InitialBacnetWindow()
{
	if (initial_once)
	{
		initial_once = false;
		Tab_Initial();
	}
}

//__declspec(dllexport) HANDLE	Get_RS485_Handle();
static bool already_retry = false;
bool has_change_connect_ip = true;
//INPUT int test_function_return_value();
void CDialogCM5_BacNet::Fresh()
{

	g_bPauseMultiRead = true; // 只要在minipanel的界面 就暂停 读 寄存器的那个线程;

	if ((g_protocol!=PROTOCOL_BACNET_IP) && 
        (g_protocol != MODBUS_BACNET_MSTP)  && 
        (g_protocol != PROTOCOL_BIP_TO_MSTP) && 
        (g_protocol != MODBUS_RS485) && 
        (g_protocol != MODBUS_TCPIP) &&
        (g_protocol != PROTOCOL_BIP_T0_MSTP_TO_MODBUS) &&
        (g_protocol != PROTOCOL_MSTP_TO_MODBUS))
	{
		return;
	}
//	g_NEED_MULTI_READ = FALSE;
	already_retry = false;
	read_write_bacnet_config = false;
	CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
	if(last_serial_number != pFrame->m_product.at(selected_product_index).serial_number) //如果上次的设备不是一样的就需要重读 Graphic label;
	{
		last_serial_number = pFrame->m_product.at(selected_product_index).serial_number;
		need_read_bacnet_graphic_label_flag = true;
	}

	if(initial_once)
	{
		initial_once = false;
		Tab_Initial();

	}
	m_user_level = LOGIN_SUCCESS_FULL_ACCESS;
	if(pFrame->m_product.at(selected_product_index).protocol == PROTOCOL_GSM)
	//if(0)	//GSM连接
	{
		Gsm_communication = true;
		g_bac_instance = pFrame->m_product.at(selected_product_index).hardware_version;
		bac_gloab_panel = g_mac = pFrame->m_product.at(selected_product_index).software_version;
		bacnet_device_type = SMALL_MINIPANEL;
		//Output_Window->Reload_Unit_Type();
		Inital_Tab_Loaded_Parameter();
		BacNet_hwd = this->m_hWnd;
		Station_NUM = bac_gloab_panel;
		if(initial_once)
		{
			initial_once = false;
			Tab_Initial();
		}


		PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,BAC_READ_INPUT_LIST);
		Input_Window->ShowWindow(SW_SHOW);
		m_bac_main_tab.SetCurSel(WINDOW_INPUT);
		if((CBacnetAlarmWindow *)AlarmWindow_Window->IsWindowVisible())
		{
			AlarmWindow_Window->ShowWindow(SW_HIDE);
		}
		return;
	}
	else if(pFrame->m_product.at(selected_product_index).protocol == MODBUS_BACNET_MSTP)
	{
		BacNet_hwd = this->m_hWnd;
		if(connect_mstp_thread == NULL)
			connect_mstp_thread =CreateThread(NULL,NULL,Mstp_Connect_Thread,this,NULL, NULL);
		return;
		set_datalink_protocol(2);
		Initial_bac(g_gloab_bac_comport,_T(""), g_gloab_bac_baudrate);
		if(g_bac_instance>0)
			Send_WhoIs_Global(-1, -1);
		::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,START_BACNET_TIMER,0);
		SetTimer(BAC_TIMER_2_WHOIS,20000,NULL);//定时器2用于间隔发送 whois;不知道设备什么时候会被移除;
		return;
	}
	else if((pFrame->m_product.at(selected_product_index).protocol == MODBUS_RS485) && (pFrame->m_product.at(selected_product_index).NetworkCard_Address.IsEmpty()))
	{
		BacNet_hwd = this->m_hWnd;
		if(read_rs485_thread == NULL)
			read_rs485_thread = CreateThread(NULL,NULL,RS485_Connect_Thread,this,NULL, NULL);
		return;
	}
	else if((pFrame->m_product.at(selected_product_index).protocol == MODBUS_TCPIP) && 
		((pFrame->m_product.at(selected_product_index).product_class_id == T38AI8AO6DO) ||
		 (pFrame->m_product.at(selected_product_index).product_class_id == PID_T322AI) ||
			(pFrame->m_product.at(selected_product_index).product_class_id == PWM_TRANSDUCER) ||
			(pFrame->m_product.at(selected_product_index).product_class_id == PID_T36CTA) ||
		 (pFrame->m_product.at(selected_product_index).product_class_id == PID_T3PT12)) ||
			(pFrame->m_product.at(selected_product_index).product_class_id == PM_T3_LC)  ||
		 (pFrame->m_product.at(selected_product_index).product_class_id == STM32_HUM_NET))
	{
		BacNet_hwd = this->m_hWnd;
		return;
	}
    else if (pFrame->m_product.at(selected_product_index).protocol == PROTOCOL_MSTP_TO_MODBUS)
    {
        BacNet_hwd = this->m_hWnd;
        return;
    }
    else if (pFrame->m_product.at(selected_product_index).protocol == PROTOCOL_BIP_T0_MSTP_TO_MODBUS)
    {
        BacNet_hwd = this->m_hWnd;
        return;
    }

	bip_setgsm(false);
	Gsm_communication = false;


    if (offline_mode == false)
    {
        if (initial_bip == false)
        {

            g_gloab_bac_comport = 0;
            set_datalink_protocol(2);

            Re_Initial_Bac_Socket_IP = pFrame->m_product.at(selected_product_index).NetworkCard_Address;

            if ((!offline_mode) && (Initial_bac(g_gloab_bac_comport, pFrame->m_product.at(selected_product_index).NetworkCard_Address)))
            {
                initial_bip = true;
            }
            else
            {
                DFTrace(_T("Initial_bac function failed!"));
                return;
            }
        }
        else
        {


            if (pFrame->m_product.at(selected_product_index).NetworkCard_Address.CompareNoCase(Re_Initial_Bac_Socket_IP) != 0)
            {
                closesocket(my_sokect);
                int ret_1 = Open_bacnetSocket2(pFrame->m_product.at(selected_product_index).NetworkCard_Address, BACNETIP_PORT, my_sokect);
                if (ret_1 >= 0)
                {
                    Re_Initial_Bac_Socket_IP = pFrame->m_product.at(selected_product_index).NetworkCard_Address;
                }
            }
            set_datalink_protocol(PROTOCOL_BACNET_IP);
            bip_set_socket(my_sokect);
            bip_set_port(htons(47808));
            in_addr BIP_Address;
            char temp_ip_2[100];
            memset(temp_ip_2, 0, 100);
            WideCharToMultiByte(CP_ACP, 0, pFrame->m_product.at(selected_product_index).NetworkCard_Address, -1, temp_ip_2, 255, NULL, NULL);
            BIP_Address.S_un.S_addr = inet_addr(temp_ip_2);
            bip_set_addr((uint32_t)BIP_Address.S_un.S_addr);



            if (g_bac_instance>0)
                Send_WhoIs_Global(g_bac_instance, g_bac_instance);
        }
    }
    else
    {
        receive_customer_unit = true;  // 虚拟设备很多地方判断是否有接收到了  客户的自定义range
        //虚拟设备也需要初始化bacnet 的相关参数;
        if (!bac_net_initial_once)
        {
            bac_net_initial_once = true;
            timesec1970 = (unsigned long)time(NULL);
            timestart = 0;
            init_info_table();
            Init_table_bank();
        }
        LoadBacnetBinaryFile(false, offline_prg_path);
    }


	//
	//SetTimer(1,500,NULL);
	SetTimer(BAC_TIMER_2_WHOIS,60000,NULL);//定时器2用于间隔发送 whois;不知道设备什么时候会被移除;
	SetTimer(BAC_TIMER_3_CHECKALARM,1000,NULL); //Check whether need  show Alarm dialog.

	BacNet_hwd = this->m_hWnd;


	CString temp_cs;
	temp_cs.Format(_T("%d"),g_bac_instance);
	GetDlgItem(IDC_STATIC_CM_DEVICE_ID)->SetWindowTextW(temp_cs);
	temp_cs.Format(_T("%d"),g_mac);
	GetDlgItem(IDC_STATIC_CM5_MAC)->SetWindowTextW(temp_cs);
	Station_NUM = g_mac;

	
	CString nconnectionip = pFrame->m_product.at(selected_product_index).BuildingInfo.strIp;
	int nport =	pFrame->m_product.at(selected_product_index).ncomport;

	g_CommunicationType = 1;
	SetCommunicationType(1);
	int ret = 0;
	if (ValidAddress(nconnectionip)==FALSE)  // 验证NC的IP
	{

		CString StringIP;
		if(!GetIPbyHostName(nconnectionip,StringIP))
		{
			AfxMessageBox(_T("Can not get a validate IP adreess from the domain name!"));
			return ;
		}
		nconnectionip = StringIP;
	}
	if(!offline_mode)
	{
		for (int i=0;i<3;i++)
		{
			ret = Open_Socket2(nconnectionip,nport);
			if(ret)
			{
				g_llTxCount ++;
				g_llRxCount ++;

				//if(pFrame->m_product.at(selected_product_index).status == false)
				//{
					//int ret_write = Write_One(g_tstat_id,33,151);	33write 151 is to reset the minipanel ethenet.
					//TRACE(_T("Write_One(g_tstat_id,33,151) == %d\r\n"),ret_write);
				//}
				bac_select_device_online = true;
				//TRACE(_T("Reconnected ~~~~~~~~\r\n"));
				break;
			}
			else 
			{
				g_llTxCount ++;
				g_llerrCount ++;
				bac_select_device_online = false;
			}
		}
	}
	else
	{
		::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,START_BACNET_TIMER,0);
		return;
	}
	
	if(ret)
	{
		unsigned int temp_instance;
		int temp_mac;
		int multy_ret = 0;
		unsigned short temp_buffer[5];

			::PostMessage(MainFram_hwd,MY_RX_TX_COUNT,1,0);

			memset(temp_buffer,0,5);

			multy_ret = Read_Multi(g_tstat_id,temp_buffer,32,5,5);
			if(multy_ret<=0)
			{
				pFrame->m_pTreeViewCrl->turn_item_image(selected_tree_item ,false);
				::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,CONNECT_TO_MODBUS_FAILED,0);
				g_llTxCount ++;
				g_llerrCount ++;
				bac_select_device_online = false;
				return;
			}

			temp_instance =temp_buffer[0]*65536 + temp_buffer[3];
			temp_mac = temp_buffer[4];
			has_change_connect_ip = true;

			if(pFrame->m_product.at(selected_product_index).product_class_id == PM_CM5)
				bacnet_device_type = PRODUCT_CM5;
			else if(pFrame->m_product.at(selected_product_index).product_class_id == T38AI8AO6DO)
				bacnet_device_type = T38AI8AO6DO;
			else if(pFrame->m_product.at(selected_product_index).product_class_id == PID_T322AI)
				bacnet_device_type = PID_T322AI;	
			else if (pFrame->m_product.at(selected_product_index).product_class_id == PWM_TRANSDUCER)
				bacnet_device_type = PWM_TRANSDUCER;
			else if(pFrame->m_product.at(selected_product_index).product_class_id == STM32_HUM_NET)
				bacnet_device_type = STM32_HUM_NET;	
			else if (pFrame->m_product.at(selected_product_index).product_class_id == PM_T3_LC)
				bacnet_device_type = PM_T3_LC;
			else
			{
				int ret = 0;
				ret = temp_buffer[2];
				if (ret == BIG_MINIPANEL)
					bacnet_device_type = BIG_MINIPANEL;
				else if (ret == SMALL_MINIPANEL)
					bacnet_device_type = SMALL_MINIPANEL;
				else if (ret == TINY_MINIPANEL)
					bacnet_device_type = TINY_MINIPANEL;
				else if (ret == TINY_EX_MINIPANEL)
					bacnet_device_type = TINY_EX_MINIPANEL;
				else if (ret == MINIPANELARM)
					bacnet_device_type = MINIPANELARM;
				else if (ret == MINIPANELARM_LB)
					bacnet_device_type = MINIPANELARM_LB;
				else if (ret == MINIPANELARM_TB)
					bacnet_device_type = MINIPANELARM_TB;
				else
					bacnet_device_type = PRODUCT_CM5;
			}
	
		memset(&m_temp_output_data,0,BAC_OUTPUT_ITEM_COUNT * sizeof(Str_out_point));

		memset(&m_temp_Input_data,0,BAC_INPUT_ITEM_COUNT * sizeof(Str_in_point));
		memset(&m_temp_variable_data,0,BAC_VARIABLE_ITEM_COUNT * sizeof(Str_variable_point));
		memset(&m_temp_program_data,0,BAC_PROGRAM_ITEM_COUNT * sizeof(Str_program_point));
		memset(&m_temp_controller_data,0,BAC_PID_COUNT * sizeof(Str_controller_point));
		memset(&m_temp_screen_data,0,BAC_SCREEN_COUNT * sizeof(Control_group_point));
		memset(&m_temp_weekly_data,0,BAC_SCHEDULE_COUNT * sizeof(Str_weekly_routine_point));
		memset(&m_temp_annual_data,0,BAC_HOLIDAY_COUNT * sizeof(Str_annual_routine_point));
		memset(&m_temp_monitor_data,0,BAC_MONITOR_COUNT * sizeof(Str_monitor_point));
		memset(&m_temp_alarmlog_data,0,BAC_ALARMLOG_COUNT * sizeof(Alarm_point));

		//temp_instance = read_one(g_tstat_id,35,5);
		//temp_mac = read_one(g_tstat_id,36,5);
		if(pFrame->m_product.at(selected_product_index).protocol == PROTOCOL_BIP_TO_MSTP)
		{
			//g_sub_instace = g_bac_instance;
			//g_bac_instance = temp_instance;
			
			if(WaitRead_Data_Dlg)
			{
				delete WaitRead_Data_Dlg;
				WaitRead_Data_Dlg = 0;
			}
			WaitRead_Data_Dlg = new CDialog_Progess(this,1,100);
			//创建对话框窗口
			WaitRead_Data_Dlg->Create(IDD_DIALOG10_Progress, this);
			WaitRead_Data_Dlg->ShowProgress(0,0);

			RECT RECT_SET1;
			GetClientRect(&RECT_SET1);
			ClientToScreen(&RECT_SET1);
			WaitRead_Data_Dlg->MoveWindow(RECT_SET1.left + 50,RECT_SET1.bottom - 19,800,20);
			WaitRead_Data_Dlg->ShowWindow(SW_SHOW);


			input_item_limit_count = 5;
			output_item_limit_count = 5;
			variable_item_limit_count = 5;
			program_item_limit_count = 4;
			controller_item_limit_count =4 ;
			screen_item_limit_count = 4;

			g_bac_need_read_setting = true;
			((CBacnetVariable *)pDialog[WINDOW_VARIABLE])->Initial_List();
			((CBacnetProgram *)pDialog[WINDOW_PROGRAM])->Initial_List();
			((BacnetController *)pDialog[WINDOW_CONTROLLER])->Initial_List();
			((BacnetScreen *)pDialog[WINDOW_SCREEN])->Initial_List();

			if(((CBacnetVariable *)pDialog[WINDOW_INPUT])->IsWindowVisible())
			{
				g_bac_read_type = TYPE_INPUT;
			}
			else if(((CBacnetVariable *)pDialog[WINDOW_OUTPUT])->IsWindowVisible())
			{
				g_bac_read_type = TYPE_OUTPUT;
			}
			else if(((CBacnetVariable *)pDialog[WINDOW_VARIABLE])->IsWindowVisible())
			{
				g_bac_read_type = TYPE_VARIABLE;
			}
			else if(((CBacnetVariable *)pDialog[WINDOW_PROGRAM])->IsWindowVisible())
			{
				g_bac_read_type = TYPE_PROGRAM;
			}
			else if(((CBacnetVariable *)pDialog[WINDOW_CONTROLLER])->IsWindowVisible())
			{
				g_bac_read_type = TYPE_CONTROLLER;
			}
			else if(((CBacnetVariable *)pDialog[WINDOW_SCREEN])->IsWindowVisible())
			{
				g_bac_read_type = TYPE_SCREENS;
			}
			else if(((CBacnetVariable *)pDialog[WINDOW_WEEKLY])->IsWindowVisible())
			{
				g_bac_read_type = TYPE_WEEKLY;
			}
			else if(((CBacnetVariable *)pDialog[WINDOW_ANNUAL])->IsWindowVisible())
			{
				g_bac_read_type = TYPE_ANNUAL;
			}
			else if(((CBacnetVariable *)pDialog[WINDOW_MONITOR])->IsWindowVisible())
			{
				g_bac_read_type = TYPE_MONITOR;
			}
			else if(((CBacnetVariable *)pDialog[WINDOW_ALARMLOG])->IsWindowVisible())
			{
				g_bac_read_type = TYPE_ALARMLOG;
			}
			else if(((CBacnetVariable *)pDialog[WINDOW_SETTING])->IsWindowVisible())
			{
				g_bac_read_type = TYPE_SETTING;
			}
			else
			{
				g_bac_read_type = TYPE_ALL;
			}


			if(click_read_thread==NULL)
			{
				click_read_thread =CreateThread(NULL,NULL,MSTP_Send_read_Command_Thread,this,NULL, NULL);

			}

			//::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,START_BACNET_TIMER,0);
		}
		else
		{
			input_item_limit_count = BAC_INPUT_ITEM_COUNT;
			output_item_limit_count = BAC_OUTPUT_ITEM_COUNT;
			variable_item_limit_count = BAC_VARIABLE_ITEM_COUNT;
			program_item_limit_count = BAC_PROGRAM_ITEM_COUNT;
			controller_item_limit_count = BAC_PID_COUNT ;
			screen_item_limit_count = BAC_SCREEN_COUNT;

			((CBacnetVariable *)pDialog[WINDOW_VARIABLE])->Initial_List();
			((CBacnetProgram *)pDialog[WINDOW_PROGRAM])->Initial_List();
			((BacnetController *)pDialog[WINDOW_CONTROLLER])->Initial_List();
			((BacnetScreen *)pDialog[WINDOW_SCREEN])->Initial_List();

			if((temp_instance<=0) || (temp_mac <=0))
			{
				pFrame->m_pTreeViewCrl->turn_item_image(selected_tree_item ,false);
				::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,CONNECT_TO_MODBUS_FAILED,0);
				g_llTxCount ++;
				g_llerrCount ++;
				bac_select_device_online = false;
			}
			else//更新数据库;如果是通过modbus扫描到得bacnet设备，因为不知道它的instance ID和panel number.所有读到后更新数据库;
			{
				bac_gloab_panel = g_mac;
				if((g_bac_instance != temp_instance) || (g_mac != temp_mac))
				{
					g_bac_instance = temp_instance;
					g_mac = temp_mac;
					pFrame->m_product.at(selected_product_index).hardware_version = (float)g_bac_instance;
					pFrame->m_product.at(selected_product_index).software_version = (float)g_mac;
					CString strSql;
					CString str_serialid;
					CString str_baudrate;
					CString hw_instance;
					CString sw_mac;
					hw_instance.Format(_T("%u"),g_bac_instance);
					sw_mac.Format(_T("%u"),g_mac);
					str_serialid.Format(_T("%u"),pFrame->m_product.at(selected_product_index).serial_number);
					str_baudrate =pFrame->m_product.at(selected_product_index).BuildingInfo.strIp;
					//TRACE(_T("update ALL_NODE set Software_Ver =\r\n"));
					CppSQLite3DB SqliteDBBuilding;
					CppSQLite3Table table;
					CppSQLite3Query q;
					SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);


					try
					{
						strSql.Format(_T("update ALL_NODE set Object_Instance ='%s' where Serial_ID = '%s' and Bautrate = '%s'"),hw_instance,str_serialid,str_baudrate);
						SqliteDBBuilding.execDML((UTF8MBSTR)strSql);	
						strSql.Format(_T("update ALL_NODE set Panal_Number ='%s' where Serial_ID = '%s' and Bautrate = '%s'"),sw_mac,str_serialid,str_baudrate);
						SqliteDBBuilding.execDML((UTF8MBSTR)strSql);	
					}
					catch(_com_error *e)
					{
						AfxMessageBox(e->ErrorMessage());
					}
					SqliteDBBuilding.closedb();
					//::PostMessage(pFrame->m_hWnd, WM_MYMSG_REFRESHBUILDING,0,0);
				}
				::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,START_BACNET_TIMER,0);
				CString temp_info;
				temp_info.Format(_T("Device is connected!"));
				CString* pstrInfo = new CString(temp_info); 
				::SendMessage(MainFram_hwd,WM_SHOW_PANNELINFOR,WPARAM(pstrInfo),LPARAM(3));
				bac_gloab_panel = g_mac = Station_NUM = temp_mac ;
				g_bac_instance = temp_instance;
				g_llTxCount ++;
				g_llRxCount ++;
				bac_select_device_online = true;
			}
		}

	}
	else
	{
		g_llTxCount ++;
		g_llerrCount ++;
		bac_select_device_online = false;
        for (int x = 0; x < 5; x++)
        {
            pFrame->m_product.at(selected_product_index).status_last_time[x] = false;//没有读到的话就将左边的list和状态都设置为false;
        }
		//pFrame->m_product.at(selected_product_index).status_last_time[0] = false;//没有读到的话就将左边的list和状态都设置为false;
		//pFrame->m_product.at(selected_product_index).status_last_time[1] = false;
		//pFrame->m_product.at(selected_product_index).status_last_time[2] = false;
		::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,CONNECT_TO_MODBUS_FAILED,0);
	}

}









typedef struct BACnet_Object_Property_Value_Own {
	BACNET_OBJECT_TYPE object_type;
	uint32_t object_instance;
	BACNET_PROPERTY_ID object_property;
	uint32_t array_index;
	BACNET_APPLICATION_DATA_VALUE value;
	HTREEITEM t_PropertyChild;
} BACNET_OBJECT_PROPERTY_VALUE_Own;

typedef struct _DEVICE_INFO
{
	uint32_t i_device_id;
	uint32_t i_vendor_id;
	uint32_t i_mac;
	HTREEITEM t_DeviceChild;
	vector	<BACnet_Object_Property_Value_Own> my_Property_value;

}DEVICE_INFO;


volatile struct mstp_port_struct_t MSTP_Port;
static void Read_Properties(
    void)
{
    uint32_t device_id = 0;
    bool status = false;
    unsigned max_apdu = 0;
    BACNET_ADDRESS src;
    bool next_device = false;
    static unsigned index = 0;
    static unsigned property = 0;
    /* list of required (and some optional) properties in the
       Device Object
       note: you could just loop through
       all the properties in all the objects. */
	 const int object_props[] = {
	PROP_OBJECT_LIST//PROP_MODEL_NAME//PROP_OBJECT_LIST
	 };
    //const int object_props[] = {
    //    PROP_OBJECT_IDENTIFIER,
    //    PROP_OBJECT_NAME,
    //    PROP_OBJECT_TYPE,
    //    PROP_SYSTEM_STATUS,
    //    PROP_VENDOR_NAME,
    //    PROP_VENDOR_IDENTIFIER,
    //    PROP_MODEL_NAME,
    //    PROP_FIRMWARE_REVISION,
    //    PROP_APPLICATION_SOFTWARE_VERSION,
    //    PROP_PROTOCOL_VERSION,
    //    PROP_PROTOCOL_SERVICES_SUPPORTED,
    //    PROP_PROTOCOL_OBJECT_TYPES_SUPPORTED,
    //    PROP_MAX_APDU_LENGTH_ACCEPTED,
    //    PROP_SEGMENTATION_SUPPORTED,
    //    PROP_LOCAL_TIME,
    //    PROP_LOCAL_DATE,
    //    PROP_UTC_OFFSET,
    //    PROP_DAYLIGHT_SAVINGS_STATUS,
    //    PROP_APDU_SEGMENT_TIMEOUT,
    //    PROP_APDU_TIMEOUT,
    //    PROP_NUMBER_OF_APDU_RETRIES,
    //    PROP_TIME_SYNCHRONIZATION_RECIPIENTS,
    //    PROP_MAX_MASTER,
    //    PROP_MAX_INFO_FRAMES,
    //    PROP_DEVICE_ADDRESS_BINDING,
    //    /* note: PROP_OBJECT_LIST is missing cause
    //       we need to get it with an index method since
    //       the list could be very large */
    //    /* some proprietary properties */
    //    514, 515,
    //    /* end of list */
    //    -1
    //};

    if (address_count()) {
        if (address_get_by_index(index, &device_id, &max_apdu, &src)) {
            if (object_props[property] < 0)
                next_device = true;
            else {
                /*status*/ g_invoke_id= Send_Read_Property_Request(device_id,  /* destination device */
                    OBJECT_DEVICE, device_id, (BACNET_PROPERTY_ID)object_props[property],
                    BACNET_ARRAY_ALL);
              //  if (status)	Fance
              //      property++;
            }
        } else
            next_device = true;
        if (next_device) {
            next_device = false;
           // index++;Fance
            if (index >= MAX_ADDRESS_CACHE)
                index = 0;
            property = 0;
        }
    }

    return;
}


LRESULT CDialogCM5_BacNet::Show_Panel_String(WPARAM wParam,LPARAM lParam)
{
	CString * temp_cstring;
	temp_cstring = (CString *)lParam;
//	CString temp_mystring = temp_cstring;

	SetPaneString(BAC_SHOW_MISSION_RESULTS,*temp_cstring);
		delete temp_cstring;
	return 0;
}

LRESULT CDialogCM5_BacNet::Show_Progress(WPARAM wParam,LPARAM lParam)
{
	int n_persent = wParam;
	int n_result = lParam;

	if(lParam)
	{
		delete WaitRead_Data_Dlg;
		WaitRead_Data_Dlg = 0;
		//SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Read items time out!"));
		return 0;
	}


	if(WaitRead_Data_Dlg!=NULL)
	{
		if(n_persent > 100)
			n_persent = 100;
		WaitRead_Data_Dlg->ShowProgress(n_persent,n_persent);
	}

	return 0;
}

LRESULT CDialogCM5_BacNet::Fresh_UI(WPARAM wParam,LPARAM lParam)
{
	int command_type = wParam;
	int button_click = 0;
	CString temp_cs;
	CTime	TimeTemp;
	switch(command_type)
	{
	//case WM_COMMAND_WHO_IS:
		//temp_cs.Format(_T("%d"),g_bac_instance);
		//GetDlgItem(IDC_STATIC_CM_DEVICE_ID)->SetWindowTextW(temp_cs);
		//temp_cs.Format(_T("%d"),g_mac);
		//GetDlgItem(IDC_STATIC_CM5_MAC)->SetWindowTextW(temp_cs);
		//Station_NUM = g_mac;

		//break;

	case MENU_CLICK:
        if ((User_Login_Window->IsWindowVisible()) && (lParam != BAC_READ_USER_LOGIN_INFO))
        {
            CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
            pFrame->HideBacnetWindow();
            User_Login_Window->ShowWindow(SW_NORMAL);
            break;
        }
            
		button_click = lParam;
		tsm_free_all_invoke_id();//每次点击的时候都将所有INVOKE ID 清零;
		if(button_click == WRITEPRGFLASH_COMMAND)
		{
			WriteFlash();
		}
		else
		{
			if(read_write_bacnet_config == false)
				Show_Wait_Dialog_And_SendMessage(button_click);
		}
		break;
	case TYPE_SVAE_CONFIG:
		read_write_bacnet_config = true;
		tsm_free_all_invoke_id();//每次点击的时候都将所有INVOKE ID 清零;
		Show_Wait_Dialog_And_SendMessage(TYPE_SVAE_CONFIG);
		break;
	default: 
		break;
	}

	
	return 0;
}




void CDialogCM5_BacNet::Show_Wait_Dialog_And_SendMessage(int read_list_type)
{
	bac_read_which_list = read_list_type;
	if(WaitDlg==NULL)
	{
			if(bac_read_which_list == BAC_READ_SVAE_CONFIG)
			{
				WaitDlg = new BacnetWait((int)BAC_WAIT_READ_DATA_WRITE_CONFIG);//如果是保存为ini文件 就要读取全部的data;
			}
			else
			{
				WaitDlg = new BacnetWait((int)BAC_WAIT_NORMAL_READ);
			}
			WaitDlg->Create(IDD_DIALOG_BACNET_WAIT,this);
			WaitDlg->ShowWindow(SW_HIDE);

            //2018 02 26 dufan 屏蔽  异常报错，这段应该没有用到.
			//RECT RECT_SET1;
			//GetClientRect(&RECT_SET1);
			//ClientToScreen(&RECT_SET1);
			//WaitDlg->MoveWindow(RECT_SET1.left + 50,RECT_SET1.bottom - 19,800,20);		
	}

	//::PostMessage(BacNet_hwd,WM_SEND_OVER,0,0);
	if(hwait_thread==NULL)
	{
		hwait_thread =CreateThread(NULL,NULL,Send_read_Command_Thread,this,NULL, NULL);

	}
}




DWORD WINAPI  MSTP_Write_Command_Thread(LPVOID lpVoid)
{
	
	int m_persent = 0;
	int m_finished_count = 0;
	int m_total_count = 0;
	bool find_exsit = false;
	for (int z=0;z<3;z++)
	{
		if(m_is_remote_device)
			Send_WhoIs_remote_ip(remote_ip_address);
		else
			Send_WhoIs_Global(-1, -1);
		Sleep(1000);
		for (int i=0;i<(int)m_bac_handle_Iam_data.size();i++)
		{
			if(m_bac_handle_Iam_data.at(i).device_id == g_bac_instance)
			{
				find_exsit = true;
				break;
			}
		}
		if(find_exsit)
			break;
	}

	if(!find_exsit)
	{
		CString *temp_cstring = new CString;
		temp_cstring->Format(_T("Device no response \"Who is Command\"!\r\n"));
		PostMessage(BacNet_hwd,WM_SHOW_PANELSTRING,0,(LPARAM)(temp_cstring));
		PostMessage(BacNet_hwd,WM_SHOW_PROGRESS,0,1);
		return -1;
	}

	m_total_count = input_item_limit_count + output_item_limit_count + variable_item_limit_count + BAC_PROGRAM_ITEM_COUNT + BAC_PID_COUNT +
		BAC_SCHEDULE_COUNT + BAC_HOLIDAY_COUNT + BAC_SCREEN_COUNT  ;

	if((g_bac_read_type == TYPE_ALL) || (g_bac_read_type == TYPE_INPUT))
	{
		for (int i=0;i<5;i++)	//read from start 0 to 4 ;
		{
			int ret_n;
			ret_n = Write_Private_Data_Blocking(READINPUT_T3000,i,i);
			CString *temp_cstring = new CString;
			if(ret_n < 0)
			{
				temp_cstring->Format(_T("Write input item %d timeout!\r\n"),i+1);
				PostMessage(BacNet_hwd,WM_SHOW_PANELSTRING,0,(LPARAM)(temp_cstring));
				PostMessage(BacNet_hwd,WM_SHOW_PROGRESS,0,1);
				click_read_thread = NULL;
				return -1;
			}
			else
			{
				temp_cstring->Format(_T("Write input item %d success!\r\n"),i+1);
				m_finished_count ++;
				m_persent = m_finished_count * 100 / m_total_count;
				PostMessage(BacNet_hwd,WM_SHOW_PROGRESS,m_persent,0);
				PostMessage(BacNet_hwd,WM_SHOW_PANELSTRING,0,(LPARAM)(temp_cstring));
				TRACE(_T("Write input %d success! m_finished_count = %d  m_total_count = %d  \r\n"),i,m_finished_count,m_total_count);

			}
		}
	}

	return 0;
}

DWORD WINAPI Write_Data_Into_Db(LPVOID lpVoid)
{
	for (int i = 0;i<BAC_INPUT_ITEM_COUNT;i++)
	{
		Save_InputData_to_db(i, g_selected_serialnumber);
	}

	for (int i = 0;i<BAC_OUTPUT_ITEM_COUNT;i++)
	{
		Save_OutputData_to_db(i, g_selected_serialnumber);
	}

	for (int i = 0;i<BAC_VARIABLE_ITEM_COUNT;i++)
	{
		Save_VariableData_to_db(i, g_selected_serialnumber);
	}

	write_indb_thread = NULL;
	return 0;
}

DWORD WINAPI  MSTP_Send_read_Command_Thread(LPVOID lpVoid)
{
	
	//CDialogCM5_BacNet *pParent = (CDialogCM5_BacNet *)lpVoid;
	int m_persent = 0;
	int m_finished_count = 0;
	int m_total_count = 0;

	//m_bac_handle_Iam_data.clear();
	bool find_exsit = false;
	for (int z=0;z<3;z++)
	{
		if(m_is_remote_device)
			Send_WhoIs_remote_ip(remote_ip_address);
		else
			Send_WhoIs_Global(-1, -1);
		Sleep(1000);
		for (int i=0;i<(int)m_bac_handle_Iam_data.size();i++)
		{
			if(m_bac_handle_Iam_data.at(i).device_id == g_bac_instance)
			{
				find_exsit = true;
				break;
			}
		}
		if(find_exsit)
			break;
	}

	if(!find_exsit)
	{
		CString *temp_cstring = new CString;
		temp_cstring->Format(_T("Device no response \"Who is Command\"!\r\n"));
		PostMessage(BacNet_hwd,WM_SHOW_PANELSTRING,0,(LPARAM)(temp_cstring));
		PostMessage(BacNet_hwd,WM_SHOW_PROGRESS,0,1);
		return -1;
	}

	if(g_bac_read_type == TYPE_ALL)
	{
		m_total_count = input_item_limit_count + output_item_limit_count + variable_item_limit_count + program_item_limit_count + controller_item_limit_count +
			BAC_SCHEDULE_COUNT + BAC_HOLIDAY_COUNT + screen_item_limit_count  ;
	}
	else if(g_bac_read_type == TYPE_INPUT)
	{
		m_total_count = input_item_limit_count ;
	}
	else if(g_bac_read_type == TYPE_OUTPUT)
	{
		m_total_count = output_item_limit_count ;
	}
	else if(g_bac_read_type == TYPE_VARIABLE)
	{
		m_total_count = variable_item_limit_count ;
	}
	else if(g_bac_read_type == TYPE_PROGRAM)
	{
		m_total_count = program_item_limit_count ;
	}
	else if(g_bac_read_type == TYPE_PROGRAMCODE)
	{
		m_total_count = 1 ;
	}
	else if(g_bac_read_type == TYPE_CONTROLLER)
	{
		m_total_count = controller_item_limit_count ;
	}
	else if(g_bac_read_type == TYPE_WEEKLY)
	{
		m_total_count = BAC_SCHEDULE_COUNT ;
	}
	else if(g_bac_read_type == TYPE_ANNUAL)
	{
		m_total_count = BAC_HOLIDAY_COUNT ;
	}
	else if(g_bac_read_type == TYPE_SCREENS)
	{
		m_total_count = screen_item_limit_count ;
	}
	else if(g_bac_read_type == TYPE_MONITOR)
	{
		m_total_count = BAC_MONITOR_COUNT ;
	}
	else if(g_bac_read_type == TYPE_ALARMLOG)
	{
		m_total_count = BAC_ALARMLOG_COUNT ;
	}
	else if(g_bac_read_type == TYPE_SETTING)
	{
		m_total_count =  1;
	}

#if 1
	if((g_bac_read_type == TYPE_ALL) || (g_bac_read_type == TYPE_SETTING) || g_bac_need_read_setting)
	{
		for (int i=0;i<1;i++)	//read from start 0 to 4 ;
		{
			int ret_n;
			ret_n = GetPrivateData_Blocking(g_bac_instance,READ_SETTING_COMMAND,i,i,sizeof(Str_Setting_Info));
			CString *temp_cstring = new CString;
			if(ret_n < 0)
			{
				temp_cstring->Format(_T("Read Setting information timeout!\r\n"));
				PostMessage(BacNet_hwd,WM_SHOW_PANELSTRING,0,(LPARAM)(temp_cstring));
				PostMessage(BacNet_hwd,WM_SHOW_PROGRESS,0,1);
				click_read_thread = NULL;
				g_progress_persent = 0;
				return -1;
			}
			else
			{
				temp_cstring->Format(_T("Read Setting information %d success!\r\n"),i+1);
				m_finished_count ++;
				m_persent = m_finished_count * 100 / m_total_count;
				g_progress_persent = m_persent;
				PostMessage(BacNet_hwd,WM_SHOW_PROGRESS,m_persent,0);
				PostMessage(BacNet_hwd,WM_SHOW_PANELSTRING,0,(LPARAM)(temp_cstring));
				
				//TRACE(_T("Read Setting %d ok!\r\n"),i);
				g_bac_need_read_setting = false;	//读过一次后 就不用在读了，下次点击Input 就直接读Input;
			}
		}
		::PostMessage(m_setting_dlg_hwnd,WM_FRESH_SETTING_UI,READ_SETTING_COMMAND,NULL);
	}
#endif
	if((g_bac_read_type == TYPE_ALL) || (g_bac_read_type == TYPE_INPUT))
	{
		for (int i=0;i<5;i++)	//read from start 0 to 4 ;
		{
			int ret_n;
			ret_n = GetPrivateData_Blocking(g_bac_instance,READINPUT_T3000,i,i,sizeof(Str_in_point));
			CString *temp_cstring = new CString;
			if(ret_n < 0)
			{
				temp_cstring->Format(_T("Read input item %d timeout!\r\n"),i+1);
				PostMessage(BacNet_hwd,WM_SHOW_PANELSTRING,0,(LPARAM)(temp_cstring));
				PostMessage(BacNet_hwd,WM_SHOW_PROGRESS,0,1);
				click_read_thread = NULL;
				g_progress_persent = 0;
				return -1;
			}
			else
			{
				temp_cstring->Format(_T("Read input item %d success!\r\n"),i+1);
				m_finished_count ++;
				m_persent = m_finished_count * 100 / m_total_count;
				g_progress_persent = m_persent;
				PostMessage(BacNet_hwd,WM_SHOW_PROGRESS,m_persent,0);
				PostMessage(BacNet_hwd,WM_SHOW_PANELSTRING,0,(LPARAM)(temp_cstring));
				TRACE(_T("Read input %d success! m_finished_count = %d  m_total_count = %d  \r\n"),i,m_finished_count,m_total_count);

			}
		}
		::PostMessage(m_input_dlg_hwnd,WM_REFRESH_BAC_INPUT_LIST,NULL,NULL);
	}

	if((g_bac_read_type == TYPE_ALL) || (g_bac_read_type == TYPE_OUTPUT))
	{
		for (int i=0;i<5;i++)
		{
			int ret_out;
			ret_out = GetPrivateData_Blocking(g_bac_instance,READOUTPUT_T3000,i,i,sizeof(Str_out_point));
			CString *temp_cstring = new CString;
			if(ret_out < 0)
			{
				temp_cstring->Format(_T("Read output item %d timeout!\r\n"),i+1);
				PostMessage(BacNet_hwd,WM_SHOW_PANELSTRING,0,(LPARAM)(temp_cstring));
				PostMessage(BacNet_hwd,WM_SHOW_PROGRESS,0,1);
				click_read_thread = NULL;
				g_progress_persent = 0;
				return -1;
			}
			else
			{
				temp_cstring->Format(_T("Read output item %d success!\r\n"),i+1);
				m_finished_count ++;
				m_persent = m_finished_count * 100 / m_total_count;
				g_progress_persent = m_persent;
				PostMessage(BacNet_hwd,WM_SHOW_PROGRESS,m_persent,0);
				PostMessage(BacNet_hwd,WM_SHOW_PANELSTRING,0,(LPARAM)(temp_cstring));
				TRACE(_T("Read output %d success!\r\n"),i);
			}
		}
		::PostMessage(m_output_dlg_hwnd,WM_REFRESH_BAC_OUTPUT_LIST,NULL,NULL);
	}

	if((g_bac_read_type == TYPE_ALL) || (g_bac_read_type == TYPE_VARIABLE))
	{
		for (int i=0;i<5;i++)
		{
			int ret_variable;
			ret_variable = GetPrivateData_Blocking(g_bac_instance,READVARIABLE_T3000,i,i,sizeof(Str_variable_point));
			CString *temp_cstring = new CString;
			if(ret_variable < 0)
			{
				temp_cstring->Format(_T("Read variable item %d timeout!\r\n"),i+1);
				PostMessage(BacNet_hwd,WM_SHOW_PANELSTRING,0,(LPARAM)(temp_cstring));
				PostMessage(BacNet_hwd,WM_SHOW_PROGRESS,0,1);
				click_read_thread = NULL;
				g_progress_persent = 0;
				return -1;
			}
			else
			{
				temp_cstring->Format(_T("Read variable item %d success!\r\n"),i+1);
				m_finished_count ++;
				m_persent = m_finished_count * 100 / m_total_count;
				g_progress_persent = m_persent;
				PostMessage(BacNet_hwd,WM_SHOW_PROGRESS,m_persent,0);
				PostMessage(BacNet_hwd,WM_SHOW_PANELSTRING,0,(LPARAM)(temp_cstring));
				TRACE(_T("Read variable %d success!\r\n"),i);
			}
		}
		::PostMessage(m_variable_dlg_hwnd,WM_REFRESH_BAC_VARIABLE_LIST,NULL,NULL);
	}

	if((g_bac_read_type == TYPE_ALL) || (g_bac_read_type == TYPE_CONTROLLER))
	{
		for (int i=0;i<controller_item_limit_count;i++)
		{
			int ret_variable;
			ret_variable = GetPrivateData_Blocking(g_bac_instance,READCONTROLLER_T3000,i,i,sizeof(Str_controller_point));
			CString *temp_cstring = new CString;
			if(ret_variable < 0)
			{
				temp_cstring->Format(_T("Read controller item %d timeout!\r\n"),i+1);
				PostMessage(BacNet_hwd,WM_SHOW_PANELSTRING,0,(LPARAM)(temp_cstring));
				PostMessage(BacNet_hwd,WM_SHOW_PROGRESS,0,1);
				click_read_thread = NULL;
				g_progress_persent = 0;
				return -1;
			}
			else
			{
				temp_cstring->Format(_T("Read controller item %d success!\r\n"),i+1);
				m_finished_count ++;
				m_persent = m_finished_count * 100 / m_total_count;
				g_progress_persent = m_persent;
				PostMessage(BacNet_hwd,WM_SHOW_PROGRESS,m_persent,0);
				PostMessage(BacNet_hwd,WM_SHOW_PANELSTRING,0,(LPARAM)(temp_cstring));
				TRACE(_T("Read controller %d success!\r\n"),i);
			}
		}
		::PostMessage(m_controller_dlg_hwnd,WM_REFRESH_BAC_CONTROLLER_LIST,NULL,NULL);
	}

	if((g_bac_read_type == TYPE_ALL) || (g_bac_read_type == TYPE_PROGRAM))
	{
		for (int i=0;i<program_item_limit_count;i++)
		{
			int ret_variable;
			ret_variable = GetPrivateData_Blocking(g_bac_instance,READPROGRAM_T3000,i,i,sizeof(Str_program_point));
			CString *temp_cstring = new CString;
			if(ret_variable < 0)
			{
				temp_cstring->Format(_T("Read program item %d timeout!\r\n"),i+1);
				PostMessage(BacNet_hwd,WM_SHOW_PANELSTRING,0,(LPARAM)(temp_cstring));
				PostMessage(BacNet_hwd,WM_SHOW_PROGRESS,0,1);
				click_read_thread = NULL;
				g_progress_persent = 0;
				return -1;
			}
			else
			{
				temp_cstring->Format(_T("Read program item %d success!\r\n"),i+1);
				m_finished_count ++;
				m_persent = m_finished_count * 100 / m_total_count;
				g_progress_persent = m_persent;
				PostMessage(BacNet_hwd,WM_SHOW_PROGRESS,m_persent,0);
				PostMessage(BacNet_hwd,WM_SHOW_PANELSTRING,0,(LPARAM)(temp_cstring));
				TRACE(_T("Read program %d success!\r\n"),i);
			}
		}
		::PostMessage(m_pragram_dlg_hwnd,WM_REFRESH_BAC_PROGRAM_LIST,NULL,NULL);
	}

	if((g_bac_read_type == TYPE_ALL) || (g_bac_read_type == TYPE_PROGRAMCODE))
	{
		if(g_bac_read_type == TYPE_PROGRAMCODE)
			bac_read_which_list = BAC_READ_PROGRAMCODE_LIST;
		for (int i=0;i<1;i++)
		{
			int ret_variable;
			ret_variable = GetProgramData_Blocking(g_bac_instance,program_list_line,program_list_line,i);
			CString *temp_cstring = new CString;
			if(ret_variable < 0)
			{
				temp_cstring->Format(_T("Read program item %d timeout!\r\n"),i+1);
				PostMessage(BacNet_hwd,WM_SHOW_PANELSTRING,0,(LPARAM)(temp_cstring));
				PostMessage(BacNet_hwd,WM_SHOW_PROGRESS,0,1);
				click_read_thread = NULL;
				g_progress_persent = 0;
				return -1;
			}
			else
			{
				temp_cstring->Format(_T("Read program code part%d success!\r\n"),i+1);
				m_finished_count ++;
				m_persent = m_finished_count * 100 / m_total_count;
				g_progress_persent = m_persent;
				PostMessage(BacNet_hwd,WM_SHOW_PROGRESS,m_persent,0);
				PostMessage(BacNet_hwd,WM_SHOW_PANELSTRING,0,(LPARAM)(temp_cstring));
				TRACE(_T("Read program code %d success!\r\n"),i);
			}
		}
		if(g_bac_read_type == TYPE_PROGRAMCODE)
		{
			bac_programcode_read_results = true;
			::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,SHOW_PROGRAM_IDE,0);
		}
		
	}


	if((g_bac_read_type == TYPE_ALL) || (g_bac_read_type == TYPE_SCREENS))
	{
		for (int i=0;i<screen_item_limit_count;i++)
		{
			int ret_variable;
			ret_variable = GetPrivateData_Blocking(g_bac_instance,READSCREEN_T3000,i,i,sizeof(Control_group_point));
			CString *temp_cstring = new CString;
			if(ret_variable < 0)
			{
				temp_cstring->Format(_T("Read screen item %d timeout!\r\n"),i+1);
				PostMessage(BacNet_hwd,WM_SHOW_PANELSTRING,0,(LPARAM)(temp_cstring));
				PostMessage(BacNet_hwd,WM_SHOW_PROGRESS,0,1);
				click_read_thread = NULL;
				g_progress_persent = 0;
				return -1;
			}
			else
			{
				temp_cstring->Format(_T("Read screen item %d success!\r\n"),i+1);
				m_finished_count ++;
				m_persent = m_finished_count * 100 / m_total_count;
				g_progress_persent = m_persent;
				PostMessage(BacNet_hwd,WM_SHOW_PROGRESS,m_persent,0);
				PostMessage(BacNet_hwd,WM_SHOW_PANELSTRING,0,(LPARAM)(temp_cstring));
				TRACE(_T("Read screen %d success!\r\n"),i);
			}
		}
		::PostMessage(m_screen_dlg_hwnd,WM_REFRESH_BAC_SCREEN_LIST,NULL,NULL);
	}

	if((g_bac_read_type == TYPE_ALL) || (g_bac_read_type == TYPE_WEEKLY))
	{
		for (int i=0;i<BAC_SCHEDULE_COUNT;i++)
		{
			int ret_variable;
			ret_variable = GetPrivateData_Blocking(g_bac_instance,READWEEKLYROUTINE_T3000,i,i,sizeof(Str_weekly_routine_point));
			CString *temp_cstring = new CString;
			if(ret_variable < 0)
			{
				temp_cstring->Format(_T("Read weekly routine item %d timeout!\r\n"),i+1);
				PostMessage(BacNet_hwd,WM_SHOW_PROGRESS,0,1);
				PostMessage(BacNet_hwd,WM_SHOW_PANELSTRING,0,(LPARAM)(temp_cstring));
				click_read_thread = NULL;
				g_progress_persent = 0;
				return -1;
			}
			else
			{
				temp_cstring->Format(_T("Read weekly routine item %d success!\r\n"),i+1);
				m_finished_count ++;
				m_persent = m_finished_count * 100 / m_total_count;
				g_progress_persent = m_persent;
				PostMessage(BacNet_hwd,WM_SHOW_PROGRESS,m_persent,0);
				PostMessage(BacNet_hwd,WM_SHOW_PANELSTRING,0,(LPARAM)(temp_cstring));
			}
		}
		::PostMessage(m_weekly_dlg_hwnd,WM_REFRESH_BAC_WEEKLY_LIST,NULL,NULL);
	}


	if((g_bac_read_type == TYPE_ALL) || (g_bac_read_type == TYPE_ANNUAL))
	{
		for (int i=0;i<BAC_HOLIDAY_COUNT;i++)
		{
			int ret_variable;
			ret_variable = GetPrivateData_Blocking(g_bac_instance,READANNUALROUTINE_T3000,i,i,sizeof(Str_annual_routine_point));
			CString *temp_cstring = new CString;
			if(ret_variable < 0)
			{
				temp_cstring->Format(_T("Read annual routine item %d timeout!\r\n"),i+1);
				PostMessage(BacNet_hwd,WM_SHOW_PROGRESS,0,1);
				PostMessage(BacNet_hwd,WM_SHOW_PANELSTRING,0,(LPARAM)(temp_cstring));
				click_read_thread = NULL;
				g_progress_persent = 0;
				return -1;
			}
			else
			{
				temp_cstring->Format(_T("Read annual routine item %d success!\r\n"),i+1);
				m_finished_count ++;
				m_persent = m_finished_count * 100 / m_total_count;
				g_progress_persent = m_persent;
				PostMessage(BacNet_hwd,WM_SHOW_PROGRESS,m_persent,0);
				PostMessage(BacNet_hwd,WM_SHOW_PANELSTRING,0,(LPARAM)(temp_cstring));
			}
		}
		::PostMessage(m_annual_dlg_hwnd,WM_REFRESH_BAC_ANNUAL_LIST,NULL,NULL);
	}
	
	if(g_bac_read_type == TYPE_ALL)
	{
		CString *temp_cstring = new CString;
		temp_cstring->Format(_T("Read data : OK!\r\n"));
		PostMessage(BacNet_hwd,WM_SHOW_PANELSTRING,0,(LPARAM)(temp_cstring));

		::PostMessageW(MainFram_hwd,MY_BAC_CONFIG_READ_RESULTS,1,NULL);
		::PostMessage(BacNet_hwd,WM_SHOW_PROGRESS,0,1);
		click_read_thread = NULL;
		return 0;
	}
	::PostMessage(BacNet_hwd,WM_SHOW_PROGRESS,0,1);
	::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,PASSWORD_OK_INITIAL_UI,LOGIN_SUCCESS_FULL_ACCESS);
	click_read_thread = NULL;
	return 0;
	//if(Device_Basic_Setting.reg.user_name == 2) //Enable user name
	//{
	//	m_bac_main_tab.ShowWindow(false);
	//	PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,BAC_READ_USER_LOGIN_INFO);
	//	User_Login_Window->ShowWindow(SW_NORMAL);

	//	::PostMessage(m_user_login_hwnd,MY_REDRAW_WINDOW,NULL,NULL);
	//	return;
	//}
	//else
	//{
	//	m_user_level = LOGIN_SUCCESS_FULL_ACCESS;
	//}
	//
	return 0;
}

static int resend_count=0;
//DWORD WINAPI  CDialogCM5_BacNet::Send_read_Command_Thread(LPVOID lpVoid)
DWORD WINAPI  Send_read_Command_Thread(LPVOID lpVoid)
{
	CDialogCM5_BacNet *pParent = (CDialogCM5_BacNet *)lpVoid;
	bac_read_all_results = false;
	int end_temp_instance = 0;
	bac_programcode_read_results = false;

	if(offline_mode)
	{
		LoadBacnetBinaryFile(false,offline_prg_path);
		hwait_thread = NULL;
		if(bac_read_which_list == BAC_READ_PROGRAMCODE_LIST)
		{
			bac_programcode_read_results = true;
			memcpy_s(mycode  ,2000 ,program_code[program_list_line],2000);
			::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,SHOW_PROGRAM_IDE,0);
		}
		else if(bac_read_which_list == BAC_READ_BASIC_SETTING_COMMAND)
		{
			::PostMessage(m_setting_dlg_hwnd,WM_FRESH_SETTING_UI,READ_SETTING_COMMAND,NULL);
		}
		return 0;

	}

	for (int i=0;i<BAC_GRPHIC_LABEL_GROUP;i++)
	{
		if(bac_read_which_list == BAC_READ_GRAPHIC_LABEL_INFO)
		{
			Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].command = 0;
			Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].device_id = 0;
			Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].start_instance =0;
			Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].end_instance = 0;
			Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].has_resend_yes_or_no = 0;
			Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].resend_count =0;
			Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].task_result = BAC_RESULTS_UNKONW;
			Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].invoke_id = -1;
			bac_graphic_label_read_results = 0;
			Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].timeout_count = 0;
		}
	}
	if(bac_read_which_list == BAC_READ_REMOTE_POINT_INFO)
	{
		for (int i=0;i<BAC_REMOTE_POINT_GROUP;i++)
		{
			Bacnet_Refresh_Info.Read_Remote_Point_Info[i].command = 0;
			Bacnet_Refresh_Info.Read_Remote_Point_Info[i].device_id = 0;
			Bacnet_Refresh_Info.Read_Remote_Point_Info[i].start_instance =0;
			Bacnet_Refresh_Info.Read_Remote_Point_Info[i].end_instance = 0;
			Bacnet_Refresh_Info.Read_Remote_Point_Info[i].has_resend_yes_or_no = 0;
			Bacnet_Refresh_Info.Read_Remote_Point_Info[i].resend_count =0;
			Bacnet_Refresh_Info.Read_Remote_Point_Info[i].task_result = BAC_RESULTS_UNKONW;
			Bacnet_Refresh_Info.Read_Remote_Point_Info[i].invoke_id = -1;
			bac_remote_point_read_results = 0;
			Bacnet_Refresh_Info.Read_Remote_Point_Info[i].timeout_count = 0;
		}
	}

	for (int i=0;i<BAC_USER_LOGIN_GROUP;i++)
	{
		if(bac_read_which_list == BAC_READ_USER_LOGIN_INFO)
		{
			Bacnet_Refresh_Info.Read_UserLogin[i].command = 0;
			Bacnet_Refresh_Info.Read_UserLogin[i].device_id = 0;
			Bacnet_Refresh_Info.Read_UserLogin[i].start_instance =0;
			Bacnet_Refresh_Info.Read_UserLogin[i].end_instance = 0;
			Bacnet_Refresh_Info.Read_UserLogin[i].has_resend_yes_or_no = 0;
			Bacnet_Refresh_Info.Read_UserLogin[i].resend_count =0;
			Bacnet_Refresh_Info.Read_UserLogin[i].task_result = BAC_RESULTS_UNKONW;
			Bacnet_Refresh_Info.Read_UserLogin[i].invoke_id = -1;
			bac_user_login_read_results = 0;
			Bacnet_Refresh_Info.Read_UserLogin[i].timeout_count = 0;
		}
	}

	
	for (int i=0;i<BAC_CUSTOMER_UNIT_GROUP;i++)
	{
		if(bac_read_which_list == BAC_READ_CUSTOMER_UNITS)
		{
			Bacnet_Refresh_Info.Read_Customer_unit_Info[i].command = 0;
			Bacnet_Refresh_Info.Read_Customer_unit_Info[i].device_id = 0;
			Bacnet_Refresh_Info.Read_Customer_unit_Info[i].start_instance =0;
			Bacnet_Refresh_Info.Read_Customer_unit_Info[i].end_instance = 0;
			Bacnet_Refresh_Info.Read_Customer_unit_Info[i].has_resend_yes_or_no = 0;
			Bacnet_Refresh_Info.Read_Customer_unit_Info[i].resend_count =0;
			Bacnet_Refresh_Info.Read_Customer_unit_Info[i].task_result = BAC_RESULTS_UNKONW;
			Bacnet_Refresh_Info.Read_Customer_unit_Info[i].invoke_id = -1;
			bac_customer_unit_read_results = 0;
			Bacnet_Refresh_Info.Read_Customer_unit_Info[i].timeout_count = 0;
		}
	}



	for (int i=0; i<BAC_MONITOR_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_MONITOR_LIST) || (bac_read_which_list == TYPE_SVAE_CONFIG)|| (bac_read_which_list ==BAC_READ_ALL_LIST))
		//if(bac_read_which_list == BAC_READ_MONITOR_LIST)
		{
			Bacnet_Refresh_Info.Read_Monitor_Info[i].command = 0;
			Bacnet_Refresh_Info.Read_Monitor_Info[i].device_id = 0;
			Bacnet_Refresh_Info.Read_Monitor_Info[i].start_instance =0;
			Bacnet_Refresh_Info.Read_Monitor_Info[i].end_instance = 0;
			Bacnet_Refresh_Info.Read_Monitor_Info[i].has_resend_yes_or_no = 0;
			Bacnet_Refresh_Info.Read_Monitor_Info[i].resend_count =0;
			Bacnet_Refresh_Info.Read_Monitor_Info[i].task_result = BAC_RESULTS_UNKONW;
			Bacnet_Refresh_Info.Read_Monitor_Info[i].invoke_id = -1;
			bac_monitor_read_results = 0;
			Bacnet_Refresh_Info.Read_Monitor_Info[i].timeout_count = 0;
		}
	}

	for (int i=0;i<BAC_SCREEN_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_SCREEN_LIST) || (bac_read_which_list == TYPE_SVAE_CONFIG)|| (bac_read_which_list ==BAC_READ_ALL_LIST))
		{
			Bacnet_Refresh_Info.Read_Screen_Info[i].command = 0;
			Bacnet_Refresh_Info.Read_Screen_Info[i].device_id = 0;
			Bacnet_Refresh_Info.Read_Screen_Info[i].start_instance =0;
			Bacnet_Refresh_Info.Read_Screen_Info[i].end_instance = 0;
			Bacnet_Refresh_Info.Read_Screen_Info[i].has_resend_yes_or_no = 0;
			Bacnet_Refresh_Info.Read_Screen_Info[i].resend_count =0;
			Bacnet_Refresh_Info.Read_Screen_Info[i].task_result = BAC_RESULTS_UNKONW;
			Bacnet_Refresh_Info.Read_Screen_Info[i].invoke_id = -1;
			bac_screen_read_results = 0;
			Bacnet_Refresh_Info.Read_Screen_Info[i].timeout_count = 0;
		}
	}

	for (int i=0;i<BAC_TIME_COMMAND_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_TIME_COMMAND) || (bac_read_which_list ==BAC_READ_ALL_LIST) || (bac_read_which_list == TYPE_SVAE_CONFIG))
		{
			Bacnet_Refresh_Info.Read_Time_Command[i].command = 0;
			Bacnet_Refresh_Info.Read_Time_Command[i].device_id = 0;
			Bacnet_Refresh_Info.Read_Time_Command[i].start_instance =0;
			Bacnet_Refresh_Info.Read_Time_Command[i].end_instance = 0;
			Bacnet_Refresh_Info.Read_Time_Command[i].has_resend_yes_or_no = 0;
			Bacnet_Refresh_Info.Read_Time_Command[i].resend_count =0;
			Bacnet_Refresh_Info.Read_Time_Command[i].task_result = BAC_RESULTS_UNKONW;
			Bacnet_Refresh_Info.Read_Time_Command[i].invoke_id = -1;
			bac_time_command_read_results = 0;
			Bacnet_Refresh_Info.Read_Time_Command[i].timeout_count = 0;
		}
	}

	for (int i=0;i<BAC_BASIC_SETTING_GROUP;i++)//prg文件暂时不保存这些基本设置的东西，像IP地址和mac地址;
	{
		if((bac_read_which_list == BAC_READ_BASIC_SETTING_COMMAND) || (bac_read_which_list ==BAC_READ_ALL_LIST) || (bac_read_which_list == TYPE_SVAE_CONFIG))
		{
			Bacnet_Refresh_Info.Read_BasicSetting_Info[i].command = 0;
			Bacnet_Refresh_Info.Read_BasicSetting_Info[i].device_id = 0;
			Bacnet_Refresh_Info.Read_BasicSetting_Info[i].start_instance =0;
			Bacnet_Refresh_Info.Read_BasicSetting_Info[i].end_instance = 0;
			Bacnet_Refresh_Info.Read_BasicSetting_Info[i].has_resend_yes_or_no = 0;
			Bacnet_Refresh_Info.Read_BasicSetting_Info[i].resend_count =0;
			Bacnet_Refresh_Info.Read_BasicSetting_Info[i].task_result = BAC_RESULTS_UNKONW;
			Bacnet_Refresh_Info.Read_BasicSetting_Info[i].invoke_id = -1;
			bac_basic_setting_read_results = 0;
			Bacnet_Refresh_Info.Read_BasicSetting_Info[i].timeout_count = 0;
		}
	}

	

	for(int i=0;i<BAC_SCHEDULE_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_WEEKLY_LIST) || (bac_read_which_list ==BAC_READ_ALL_LIST) || (bac_read_which_list ==TYPE_SVAE_CONFIG))
		{
			Bacnet_Refresh_Info.Read_Weekly_Info[i].command = 0;
			Bacnet_Refresh_Info.Read_Weekly_Info[i].device_id = 0;
			Bacnet_Refresh_Info.Read_Weekly_Info[i].start_instance =0;
			Bacnet_Refresh_Info.Read_Weekly_Info[i].end_instance =0;
			Bacnet_Refresh_Info.Read_Weekly_Info[i].has_resend_yes_or_no = 0;
			Bacnet_Refresh_Info.Read_Weekly_Info[i].resend_count = 0;
			Bacnet_Refresh_Info.Read_Weekly_Info[i].task_result = BAC_RESULTS_UNKONW;
			Bacnet_Refresh_Info.Read_Weekly_Info[i].invoke_id = -1;
			bac_weekly_read_results = 0;
			Bacnet_Refresh_Info.Read_Weekly_Info[i].timeout_count = 0;
		}
	}

	for(int i=0;i<BAC_HOLIDAY_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_ANNUAL_LIST) ||(bac_read_which_list ==BAC_READ_ALL_LIST) || (bac_read_which_list ==TYPE_SVAE_CONFIG))
		{
			Bacnet_Refresh_Info.Read_Annual_Info[i].command = 0;
			Bacnet_Refresh_Info.Read_Annual_Info[i].device_id = 0;
			Bacnet_Refresh_Info.Read_Annual_Info[i].start_instance =0;
			Bacnet_Refresh_Info.Read_Annual_Info[i].end_instance =0;
			Bacnet_Refresh_Info.Read_Annual_Info[i].has_resend_yes_or_no = 0;
			Bacnet_Refresh_Info.Read_Annual_Info[i].resend_count = 0;

			Bacnet_Refresh_Info.Read_Annual_Info[i].task_result = BAC_RESULTS_UNKONW;
			Bacnet_Refresh_Info.Read_Annual_Info[i].invoke_id = -1;

			bac_annual_read_results = 0;
			Bacnet_Refresh_Info.Read_Annual_Info[i].timeout_count = 0;
		}
	}
	
	for (int i=0;i<BAC_PROGRAMCODE_GROUP;i++)
	{

		if((bac_read_which_list == BAC_READ_PROGRAMCODE_LIST) || (bac_read_which_list ==TYPE_SVAE_CONFIG))
		{
			Bacnet_Refresh_Info.Read_Programcode_Info[i].command = 0;
			Bacnet_Refresh_Info.Read_Programcode_Info[i].device_id = 0;
			Bacnet_Refresh_Info.Read_Programcode_Info[i].start_instance =0;
			Bacnet_Refresh_Info.Read_Programcode_Info[i].end_instance =0;
			Bacnet_Refresh_Info.Read_Programcode_Info[i].has_resend_yes_or_no = 0;
			Bacnet_Refresh_Info.Read_Programcode_Info[i].resend_count = 0;

			Bacnet_Refresh_Info.Read_Programcode_Info[i].task_result = BAC_RESULTS_UNKONW;
			Bacnet_Refresh_Info.Read_Programcode_Info[i].invoke_id = -1;
			bac_programcode_read_results = 0;
			//bac_program_read_results = 0;
			Bacnet_Refresh_Info.Read_Programcode_Info[i].timeout_count = 0;
		}
	}

	for (int i=0;i<BAC_SCHEDULECODE_GOUP;i++)
	{

		if((bac_read_which_list == BAC_READ_WEEKLTCODE_LIST) || (bac_read_which_list ==TYPE_SVAE_CONFIG))
		{
			Bacnet_Refresh_Info.Read_Weeklycode_Info[i].command = 0;
			Bacnet_Refresh_Info.Read_Weeklycode_Info[i].device_id = 0;
			Bacnet_Refresh_Info.Read_Weeklycode_Info[i].start_instance =0;
			Bacnet_Refresh_Info.Read_Weeklycode_Info[i].end_instance =0;
			Bacnet_Refresh_Info.Read_Weeklycode_Info[i].has_resend_yes_or_no = 0;
			Bacnet_Refresh_Info.Read_Weeklycode_Info[i].resend_count = 0;

			Bacnet_Refresh_Info.Read_Weeklycode_Info[i].task_result = BAC_RESULTS_UNKONW;
			Bacnet_Refresh_Info.Read_Weeklycode_Info[i].invoke_id = -1;
			bac_weeklycode_read_results = 0;
			Bacnet_Refresh_Info.Read_Weeklycode_Info[i].timeout_count = 0;
		}
	}

	for (int i=0;i<BAC_PROGRAM_GROUP;i++)
	{

		if((bac_read_which_list == BAC_READ_PROGRAM_LIST) || (bac_read_which_list ==BAC_READ_ALL_LIST) ||(bac_read_which_list ==TYPE_SVAE_CONFIG))
		{
			Bacnet_Refresh_Info.Read_Program_Info[i].command = 0;
			Bacnet_Refresh_Info.Read_Program_Info[i].device_id = 0;
			Bacnet_Refresh_Info.Read_Program_Info[i].start_instance =0;
			Bacnet_Refresh_Info.Read_Program_Info[i].end_instance =0;
			Bacnet_Refresh_Info.Read_Program_Info[i].has_resend_yes_or_no = 0;
			Bacnet_Refresh_Info.Read_Program_Info[i].resend_count = 0;

			Bacnet_Refresh_Info.Read_Program_Info[i].task_result = BAC_RESULTS_UNKONW;
			Bacnet_Refresh_Info.Read_Program_Info[i].invoke_id = -1;

			bac_program_read_results = 0;
			Bacnet_Refresh_Info.Read_Program_Info[i].timeout_count = 0;
		}
	}

	for (int i=0;i<BAC_INPUT_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_INPUT_LIST) || (bac_read_which_list ==BAC_READ_ALL_LIST) ||(bac_read_which_list ==TYPE_SVAE_CONFIG) || (bac_read_which_list == BAC_READ_IN_OUT_VAR_LIST))
		{
			Bacnet_Refresh_Info.Read_Input_Info[i].command = 0;
			Bacnet_Refresh_Info.Read_Input_Info[i].device_id = 0;
			Bacnet_Refresh_Info.Read_Input_Info[i].start_instance =0;
			Bacnet_Refresh_Info.Read_Input_Info[i].end_instance =0;
			Bacnet_Refresh_Info.Read_Input_Info[i].has_resend_yes_or_no = 0;
			Bacnet_Refresh_Info.Read_Input_Info[i].resend_count = 0;
			Bacnet_Refresh_Info.Read_Input_Info[i].task_result = BAC_RESULTS_UNKONW;
			Bacnet_Refresh_Info.Read_Input_Info[i].invoke_id = -1;
			bac_input_read_results = 0;
			Bacnet_Refresh_Info.Read_Input_Info[i].timeout_count = 0;

		}
	}
	for (int i=0;i<BAC_HOLIDAYCODE_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_ANNUALCODE_LIST) || (bac_read_which_list ==BAC_READ_ALL_LIST) ||(bac_read_which_list ==TYPE_SVAE_CONFIG))
		{
			Bacnet_Refresh_Info.Read_Annualcode_Info[i].command = 0;
			Bacnet_Refresh_Info.Read_Annualcode_Info[i].device_id = 0;
			Bacnet_Refresh_Info.Read_Annualcode_Info[i].start_instance =0;
			Bacnet_Refresh_Info.Read_Annualcode_Info[i].end_instance =0;
			Bacnet_Refresh_Info.Read_Annualcode_Info[i].has_resend_yes_or_no = 0;
			Bacnet_Refresh_Info.Read_Annualcode_Info[i].resend_count = 0;
			Bacnet_Refresh_Info.Read_Annualcode_Info[i].task_result = BAC_RESULTS_UNKONW;
			Bacnet_Refresh_Info.Read_Annualcode_Info[i].invoke_id = -1;
			bac_annualcode_read_results = 0;
			Bacnet_Refresh_Info.Read_Annualcode_Info[i].timeout_count = 0;

		}
	}

	for (int i=0;i<BAC_OUTPUT_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_OUTPUT_LIST) || (bac_read_which_list ==BAC_READ_ALL_LIST) ||(bac_read_which_list ==TYPE_SVAE_CONFIG) || (bac_read_which_list == BAC_READ_IN_OUT_VAR_LIST))
		{
			Bacnet_Refresh_Info.Read_Output_Info[i].command = 0;
			Bacnet_Refresh_Info.Read_Output_Info[i].device_id = 0;
			Bacnet_Refresh_Info.Read_Output_Info[i].start_instance =0;
			Bacnet_Refresh_Info.Read_Output_Info[i].end_instance =0;
			Bacnet_Refresh_Info.Read_Output_Info[i].has_resend_yes_or_no = 0;
			Bacnet_Refresh_Info.Read_Output_Info[i].resend_count = 0;
			Bacnet_Refresh_Info.Read_Output_Info[i].task_result = BAC_RESULTS_UNKONW;
			Bacnet_Refresh_Info.Read_Output_Info[i].invoke_id = -1;
			bac_output_read_results = 0;
			Bacnet_Refresh_Info.Read_Output_Info[i].timeout_count = 0;

		}
	}

	for (int i=0;i<BAC_VARIABLE_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_VARIABLE_LIST) || (bac_read_which_list ==BAC_READ_ALL_LIST) ||(bac_read_which_list ==TYPE_SVAE_CONFIG) || (bac_read_which_list == BAC_READ_IN_OUT_VAR_LIST))
		{
			Bacnet_Refresh_Info.Read_Variable_Info[i].command = 0;
			Bacnet_Refresh_Info.Read_Variable_Info[i].device_id = 0;
			Bacnet_Refresh_Info.Read_Variable_Info[i].start_instance =0;
			Bacnet_Refresh_Info.Read_Variable_Info[i].end_instance =0;
			Bacnet_Refresh_Info.Read_Variable_Info[i].has_resend_yes_or_no = 0;
			Bacnet_Refresh_Info.Read_Variable_Info[i].task_result = BAC_RESULTS_UNKONW;
			Bacnet_Refresh_Info.Read_Variable_Info[i].invoke_id = -1;
			Bacnet_Refresh_Info.Read_Variable_Info[i].resend_count = 0;
			bac_variable_read_results = 0;
			Bacnet_Refresh_Info.Read_Variable_Info[i].timeout_count = 0;
		}
	}

	for (int i=0;i<BAC_PID_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_CONTROLLER_LIST) || (bac_read_which_list == TYPE_SVAE_CONFIG)|| (bac_read_which_list ==BAC_READ_ALL_LIST))
		{
			Bacnet_Refresh_Info.Read_Controller_Info[i].command = 0;
			Bacnet_Refresh_Info.Read_Controller_Info[i].device_id = 0;
			Bacnet_Refresh_Info.Read_Controller_Info[i].start_instance =0;
			Bacnet_Refresh_Info.Read_Controller_Info[i].end_instance =0;
			Bacnet_Refresh_Info.Read_Controller_Info[i].has_resend_yes_or_no = 0;
			Bacnet_Refresh_Info.Read_Controller_Info[i].task_result = BAC_RESULTS_UNKONW;
			Bacnet_Refresh_Info.Read_Controller_Info[i].invoke_id = -1;
			Bacnet_Refresh_Info.Read_Controller_Info[i].resend_count = 0;
			bac_controller_read_results = 0;
				Bacnet_Refresh_Info.Read_Controller_Info[i].timeout_count = 0;
		}
	}


	for (int i=0;i<BAC_ALARMLOG_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_ALARMLOG_LIST)/* || (bac_read_which_list ==BAC_READ_ALL_LIST)*/)
		{
			Bacnet_Refresh_Info.Read_AlarmLog_Info[i].command = 0;
			Bacnet_Refresh_Info.Read_AlarmLog_Info[i].device_id = 0;
			Bacnet_Refresh_Info.Read_AlarmLog_Info[i].start_instance =0;
			Bacnet_Refresh_Info.Read_AlarmLog_Info[i].end_instance =0;
			Bacnet_Refresh_Info.Read_AlarmLog_Info[i].has_resend_yes_or_no = 0;
			Bacnet_Refresh_Info.Read_AlarmLog_Info[i].task_result = BAC_RESULTS_UNKONW;
			Bacnet_Refresh_Info.Read_AlarmLog_Info[i].invoke_id = -1;
			Bacnet_Refresh_Info.Read_AlarmLog_Info[i].resend_count = 0;
			bac_alarmlog_read_results = 0;
			Bacnet_Refresh_Info.Read_AlarmLog_Info[i].timeout_count = 0;
		}
	}

	for (int i=0;i<BAC_GRPHIC_LABEL_GROUP;i++)
	{
		if(bac_read_which_list == BAC_READ_GRAPHIC_LABEL_INFO)
		{
			resend_count = 0;
			do 
			{
				resend_count ++;
				if(resend_count>RESEND_COUNT)
					goto myend;
				end_temp_instance = BAC_READ_GRPHIC_LABEL_REMAINDER + (BAC_READ_GRPHIC_LABEL_GROUP_NUMBER)*i ;
				if(end_temp_instance >= BAC_GRPHIC_LABEL_COUNT)
					end_temp_instance = BAC_GRPHIC_LABEL_COUNT - 1;

				g_invoke_id = GetPrivateData(g_bac_instance,READ_GRPHIC_LABEL_COMMAND,(BAC_READ_GRPHIC_LABEL_GROUP_NUMBER)*i,
					end_temp_instance,sizeof(Str_label_point));
				Sleep(SEND_COMMAND_DELAY_TIME);	
			} while (g_invoke_id<0);

			Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].command = READ_GRPHIC_LABEL_COMMAND;
			Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].device_id = g_bac_instance;
			Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].start_instance = (BAC_READ_GRPHIC_LABEL_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].end_instance = end_temp_instance;
			Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].invoke_id = g_invoke_id;
			CString temp_cs;
			temp_cs.Format(_T("Read Graphic Label From %d to %d "),
				Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].start_instance,
				Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].end_instance);
			Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs);
		}
	}

	if(bac_read_which_list == BAC_READ_REMOTE_POINT_INFO)
	{
		for (int i=0;i<BAC_REMOTE_POINT_GROUP;i++)
		{
			resend_count = 0;
			do 
			{
				resend_count ++;
				if(resend_count>RESEND_COUNT)
					goto myend;
				end_temp_instance = BAC_REMOTE_POINT_GROUP_REMAINDER + (BAC_REMOTE_POINT_GROUP_NUMBER*i) ;
				if(end_temp_instance >= BAC_REMOTE_POINT_COUNT)
					end_temp_instance = BAC_REMOTE_POINT_COUNT - 1;

				g_invoke_id = GetPrivateData(g_bac_instance,READ_REMOTE_POINT,(BAC_REMOTE_POINT_GROUP_NUMBER)*i,
					end_temp_instance,sizeof(Str_remote_point));
				Sleep(SEND_COMMAND_DELAY_TIME);	
			} while (g_invoke_id<0);
			Bacnet_Refresh_Info.Read_Remote_Point_Info[i].command = READ_REMOTE_POINT;
			Bacnet_Refresh_Info.Read_Remote_Point_Info[i].device_id = g_bac_instance;
			Bacnet_Refresh_Info.Read_Remote_Point_Info[i].start_instance = (BAC_REMOTE_POINT_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Remote_Point_Info[i].end_instance = end_temp_instance;
			Bacnet_Refresh_Info.Read_Remote_Point_Info[i].invoke_id = g_invoke_id;
			CString temp_cs;
			temp_cs.Format(_T("Read Remote point From %d to %d "),
				Bacnet_Refresh_Info.Read_Remote_Point_Info[i].start_instance,
				Bacnet_Refresh_Info.Read_Remote_Point_Info[i].end_instance);
			Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs);
		}
	}



	for (int i=0;i<BAC_USER_LOGIN_GROUP;i++)
	{
		if(bac_read_which_list == BAC_READ_USER_LOGIN_INFO)
		{
			resend_count = 0;
			do 
			{
				resend_count ++;
				if(resend_count>RESEND_COUNT)
					goto myend;
				end_temp_instance = BAC_READ_USER_LOGIN_INFO_REMAINDER + (BAC_READ_USER_LOGIN_INFO_GROUP_NUMBER)*i ;
				if(end_temp_instance >= BAC_USER_LOGIN_COUNT)
					end_temp_instance = BAC_USER_LOGIN_COUNT - 1;

				g_invoke_id = GetPrivateData(g_bac_instance,READUSER_T3000,(BAC_READ_USER_LOGIN_INFO_GROUP_NUMBER)*i,
					end_temp_instance,sizeof(Str_userlogin_point));
				Sleep(SEND_COMMAND_DELAY_TIME);	
			} while (g_invoke_id<0);

			Bacnet_Refresh_Info.Read_UserLogin[i].command = READUSER_T3000;
			Bacnet_Refresh_Info.Read_UserLogin[i].device_id = g_bac_instance;
			Bacnet_Refresh_Info.Read_UserLogin[i].start_instance = (BAC_READ_USER_LOGIN_INFO_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_UserLogin[i].end_instance = end_temp_instance;
			Bacnet_Refresh_Info.Read_UserLogin[i].invoke_id = g_invoke_id;
			CString temp_cs;
			temp_cs.Format(_T("Read Tstat From %d to %d "),
				Bacnet_Refresh_Info.Read_UserLogin[i].start_instance,
				Bacnet_Refresh_Info.Read_UserLogin[i].end_instance);
			Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs);
		}
	}


	for (int i=0;i<BAC_CUSTOMER_UNIT_GROUP;i++)
	{
		if(bac_read_which_list == BAC_READ_CUSTOMER_UNITS) 
		{
			resend_count = 0;
			do 
			{
				resend_count ++;
				if(resend_count>RESEND_COUNT)
					goto myend;
				g_invoke_id = GetPrivateData(g_bac_instance,READUNIT_T3000,(BAC_READ_CUSTOMER_UNITS_GROUP_NUMBER)*i,
					3+(BAC_READ_GROUP_NUMBER)*i,sizeof(Str_Units_element));
				Sleep(SEND_COMMAND_DELAY_TIME);	
			} while (g_invoke_id<0);
			Bacnet_Refresh_Info.Read_Customer_unit_Info[i].command = READUNIT_T3000;
			Bacnet_Refresh_Info.Read_Customer_unit_Info[i].device_id = g_bac_instance;
			Bacnet_Refresh_Info.Read_Customer_unit_Info[i].start_instance = (BAC_READ_CUSTOMER_UNITS_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Customer_unit_Info[i].end_instance =BAC_READ_CUSTOMER_UNITS_GROUP_NUMBER+(BAC_READ_CUSTOMER_UNITS_GROUP_NUMBER)*i;
			//Bacnet_Refresh_Info.Read_Monitor_Info[i].end_instance =2+(BAC_READ_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Customer_unit_Info[i].invoke_id = g_invoke_id;
			CString temp_cs;
			temp_cs.Format(_T("Read Custom units From %d to %d "),
				Bacnet_Refresh_Info.Read_Customer_unit_Info[i].start_instance,
				Bacnet_Refresh_Info.Read_Customer_unit_Info[i].end_instance);
			Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs);
		}
	}


	for (int i=0;i<BAC_MONITOR_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_MONITOR_LIST) || (bac_read_which_list == TYPE_SVAE_CONFIG) || (bac_read_which_list ==BAC_READ_ALL_LIST))
		//if(bac_read_which_list == BAC_READ_MONITOR_LIST)
		{
			resend_count = 0;
			do 
			{
				resend_count ++;
				if(resend_count>RESEND_COUNT)
					goto myend;
				end_temp_instance = BAC_READ_MONOTOR_REMAINDER + (BAC_READ_MONITOR_GROUP_NUMBER)*i ;
				if(end_temp_instance >= BAC_MONITOR_COUNT)
					end_temp_instance = BAC_MONITOR_COUNT - 1;

				g_invoke_id = GetPrivateData(g_bac_instance,READMONITOR_T3000,(BAC_READ_MONITOR_GROUP_NUMBER)*i,
					end_temp_instance,
				//	2+(BAC_READ_GROUP_NUMBER)*i,
					sizeof(Str_monitor_point));
				Sleep(SEND_COMMAND_DELAY_TIME);	
			} while (g_invoke_id<0);
			Bacnet_Refresh_Info.Read_Monitor_Info[i].command = READMONITOR_T3000;
			Bacnet_Refresh_Info.Read_Monitor_Info[i].device_id = g_bac_instance;
			Bacnet_Refresh_Info.Read_Monitor_Info[i].start_instance = (BAC_READ_MONITOR_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Monitor_Info[i].end_instance = end_temp_instance;
			//Bacnet_Refresh_Info.Read_Monitor_Info[i].end_instance =2+(BAC_READ_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Monitor_Info[i].invoke_id = g_invoke_id;
			CString temp_cs;
			temp_cs.Format(_T("Read Monitor Item From %d to %d "),
				Bacnet_Refresh_Info.Read_Monitor_Info[i].start_instance,
				Bacnet_Refresh_Info.Read_Monitor_Info[i].end_instance);
			Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs);
		}
	}

	for (int i=0;i<BAC_SCREEN_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_SCREEN_LIST) || (bac_read_which_list == TYPE_SVAE_CONFIG)|| (bac_read_which_list ==BAC_READ_ALL_LIST))
		//if(bac_read_which_list == BAC_READ_SCREEN_LIST)
		{
			resend_count = 0;
			do 
			{
				resend_count ++;
				if(resend_count>RESEND_COUNT)
					goto myend;
				end_temp_instance = BAC_READ_SCREEN_REMAINDER + (BAC_READ_SCREEN_GROUP_NUMBER)*i ;
				if(end_temp_instance >= BAC_SCREEN_COUNT)
					end_temp_instance = BAC_SCREEN_COUNT - 1;

				g_invoke_id = GetPrivateData(g_bac_instance,READSCREEN_T3000,(BAC_READ_SCREEN_GROUP_NUMBER)*i,
					end_temp_instance,
					sizeof(Control_group_point));
				Sleep(SEND_COMMAND_DELAY_TIME);	
			} while (g_invoke_id<0);
			Bacnet_Refresh_Info.Read_Screen_Info[i].command = READSCREEN_T3000;
			Bacnet_Refresh_Info.Read_Screen_Info[i].device_id = g_bac_instance;
			Bacnet_Refresh_Info.Read_Screen_Info[i].start_instance = (BAC_READ_SCREEN_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Screen_Info[i].end_instance = end_temp_instance;
			Bacnet_Refresh_Info.Read_Screen_Info[i].invoke_id = g_invoke_id;
			CString temp_cs;
			temp_cs.Format(_T("Read Screens Item From %d to %d "),
				Bacnet_Refresh_Info.Read_Screen_Info[i].start_instance,
				Bacnet_Refresh_Info.Read_Screen_Info[i].end_instance);
			Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs);
		}
	}

	for (int i=0;i<BAC_TIME_COMMAND_GROUP;i++)
	{
		//如果是在读Setting 界面的时候也要同时读 Time，因为time在setting界面值;
		if((bac_read_which_list == BAC_READ_TIME_COMMAND) ||(bac_read_which_list == BAC_READ_BASIC_SETTING_COMMAND)|| (bac_read_which_list ==BAC_READ_ALL_LIST) || (bac_read_which_list == TYPE_SVAE_CONFIG))
		{
			resend_count = 0;
			do 
			{
				resend_count ++;
				if(resend_count>RESEND_COUNT)
					goto myend;
				g_invoke_id = GetPrivateData(g_bac_instance,
					TIME_COMMAND,0,
					0,
					sizeof(Time_block_mini));
				Sleep(SEND_COMMAND_DELAY_TIME);
			} while (g_invoke_id<0);
			Bacnet_Refresh_Info.Read_Time_Command[i].command = TIME_COMMAND;
			Bacnet_Refresh_Info.Read_Time_Command[i].device_id = g_bac_instance;
			Bacnet_Refresh_Info.Read_Time_Command[i].start_instance = 0;
			Bacnet_Refresh_Info.Read_Time_Command[i].end_instance =0;
			Bacnet_Refresh_Info.Read_Time_Command[i].invoke_id = g_invoke_id;

			CString temp_cs_show;
			temp_cs_show.Format(_T("Read device time "));
			Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs_show);

			//Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,_T("1111111111111"));
			//Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd);
		}
	}



	for (int i=0;i<BAC_BASIC_SETTING_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_BASIC_SETTING_COMMAND) || (bac_read_which_list ==BAC_READ_ALL_LIST) || (bac_read_which_list == TYPE_SVAE_CONFIG))
		{
			resend_count = 0;
			do 
			{
				resend_count ++;
				if(resend_count>RESEND_COUNT)
					goto myend;
				g_invoke_id = GetPrivateData(g_bac_instance,
					READ_SETTING_COMMAND,0,
					0,
					sizeof(Str_Setting_Info));
				Sleep(SEND_COMMAND_DELAY_TIME);
			} while (g_invoke_id<0);
			Bacnet_Refresh_Info.Read_BasicSetting_Info[i].command = READ_SETTING_COMMAND;
			Bacnet_Refresh_Info.Read_BasicSetting_Info[i].device_id = g_bac_instance;
			Bacnet_Refresh_Info.Read_BasicSetting_Info[i].start_instance = 0;
			Bacnet_Refresh_Info.Read_BasicSetting_Info[i].end_instance =0;
			Bacnet_Refresh_Info.Read_BasicSetting_Info[i].invoke_id = g_invoke_id;

			CString temp_cs_show;
			temp_cs_show.Format(_T("Read Basic Setting "));
			Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs_show);

		}
	}

	
	for (int i=0;i<BAC_SCHEDULE_GROUP;i++)
	{

		if((bac_read_which_list == BAC_READ_WEEKLY_LIST) || (bac_read_which_list ==BAC_READ_ALL_LIST) || (bac_read_which_list ==TYPE_SVAE_CONFIG))
		//if(bac_read_which_list == BAC_READ_WEEKLY_LIST)
		{
			resend_count = 0;
			do 
			{
				resend_count ++;
				if(resend_count>RESEND_COUNT)
					goto myend;
				end_temp_instance = BAC_READ_SCHEDULE_REMAINDER + (BAC_READ_SCHEDULE_GROUP_NUMBER)*i ;
				if(end_temp_instance >= BAC_SCHEDULE_COUNT)
					end_temp_instance = BAC_SCHEDULE_COUNT - 1;

				g_invoke_id = GetPrivateData(g_bac_instance,
					READWEEKLYROUTINE_T3000,(BAC_READ_SCHEDULE_GROUP_NUMBER)*i,
					end_temp_instance,
					sizeof(Str_weekly_routine_point));
				Sleep(SEND_COMMAND_DELAY_TIME);
			} while (g_invoke_id<0);
			Bacnet_Refresh_Info.Read_Weekly_Info[i].command = READWEEKLYROUTINE_T3000;
			Bacnet_Refresh_Info.Read_Weekly_Info[i].device_id = g_bac_instance;
			Bacnet_Refresh_Info.Read_Weekly_Info[i].start_instance = (BAC_READ_SCHEDULE_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Weekly_Info[i].end_instance =end_temp_instance;
			Bacnet_Refresh_Info.Read_Weekly_Info[i].invoke_id = g_invoke_id;

			CString temp_cs;
			temp_cs.Format(_T("Read Weekly Routine Item From %d to %d "),
				Bacnet_Refresh_Info.Read_Weekly_Info[i].start_instance,
				Bacnet_Refresh_Info.Read_Weekly_Info[i].end_instance);
			Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs);
		}
	}

	for (int i=0;i<BAC_HOLIDAY_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_ANNUAL_LIST) ||(bac_read_which_list ==BAC_READ_ALL_LIST) || (bac_read_which_list ==TYPE_SVAE_CONFIG))
		//if(bac_read_which_list == BAC_READ_ANNUAL_LIST)
		{
			resend_count = 0;
			do 
			{
				resend_count ++;
				if(resend_count>RESEND_COUNT)
					goto myend;
				end_temp_instance = BAC_READ_HOLIDAY_REMAINDER + (BAC_READ_HOLIDAY_GROUP_NUMBER)*i ;
				if(end_temp_instance >= BAC_HOLIDAY_COUNT)
					end_temp_instance = BAC_HOLIDAY_COUNT - 1;

				g_invoke_id = GetPrivateData(g_bac_instance,
					READANNUALROUTINE_T3000,(BAC_READ_HOLIDAY_GROUP_NUMBER)*i,
					end_temp_instance,
					sizeof(Str_annual_routine_point));
				Sleep(SEND_COMMAND_DELAY_TIME);
			} while (g_invoke_id<0);
			Bacnet_Refresh_Info.Read_Annual_Info[i].command = READANNUALROUTINE_T3000;
			Bacnet_Refresh_Info.Read_Annual_Info[i].device_id = g_bac_instance;
			Bacnet_Refresh_Info.Read_Annual_Info[i].start_instance = (BAC_READ_HOLIDAY_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Annual_Info[i].end_instance =end_temp_instance;
			Bacnet_Refresh_Info.Read_Annual_Info[i].invoke_id = g_invoke_id;


			CString temp_cs;
			temp_cs.Format(_T("Read Annual Routine Item From %d to %d "),
				Bacnet_Refresh_Info.Read_Annual_Info[i].start_instance,
				Bacnet_Refresh_Info.Read_Annual_Info[i].end_instance);
			Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs);
		}
	}


	

	for (int i=0;i<BAC_ALARMLOG_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_ALARMLOG_LIST) /*|| (bac_read_which_list ==BAC_READ_ALL_LIST)*/)
		{
			resend_count = 0;
			do 
			{
				resend_count ++;
				if(resend_count>RESEND_COUNT)
					goto myend;
				end_temp_instance = BAC_READ_ALARMLOG_REMAINDER + (BAC_READ_ALARMLOG_GROUP_NUMBER)*i ;
				if(end_temp_instance >= BAC_ALARMLOG_COUNT)
					end_temp_instance = BAC_ALARMLOG_COUNT - 1;
				g_invoke_id = GetPrivateData(g_bac_instance,READALARM_T3000,(BAC_READ_ALARMLOG_GROUP_NUMBER)*i,
					end_temp_instance,sizeof(Alarm_point));
				Sleep(SEND_COMMAND_DELAY_TIME);
				//if(g_invoke_id<0)
				//	TRACE(_T("Input g_invoke_id = %d ,resend count = %d\r\n"),g_invoke_id,resend_count);
			} while (g_invoke_id<0);
			Bacnet_Refresh_Info.Read_AlarmLog_Info[i].command = READALARM_T3000;
			Bacnet_Refresh_Info.Read_AlarmLog_Info[i].device_id = g_bac_instance;
			Bacnet_Refresh_Info.Read_AlarmLog_Info[i].start_instance = (BAC_READ_ALARMLOG_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_AlarmLog_Info[i].end_instance = end_temp_instance;
			Bacnet_Refresh_Info.Read_AlarmLog_Info[i].invoke_id = g_invoke_id;


			CString temp_cs;
			temp_cs.Format(_T("Read Alarm Log List %d"),
				Bacnet_Refresh_Info.Read_AlarmLog_Info[i].start_instance);
			Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs);
		}
	}



	for (int i=0;i<BAC_INPUT_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_INPUT_LIST) || (bac_read_which_list ==BAC_READ_ALL_LIST) || (bac_read_which_list ==TYPE_SVAE_CONFIG) || (bac_read_which_list == BAC_READ_IN_OUT_VAR_LIST))
		{
			resend_count = 0;
			do 
			{
				resend_count ++;
				if(resend_count>RESEND_COUNT)
					goto myend;
				end_temp_instance = BAC_READ_INPUT_REMAINDER + (BAC_READ_INPUT_GROUP_NUMBER)*i ;
				if(end_temp_instance >= BAC_INPUT_ITEM_COUNT)
					end_temp_instance = BAC_INPUT_ITEM_COUNT - 1;
				g_invoke_id = GetPrivateData(g_bac_instance,READINPUT_T3000,(BAC_READ_INPUT_GROUP_NUMBER)*i,end_temp_instance,sizeof(Str_in_point));
				Sleep(SEND_COMMAND_DELAY_TIME);
				//if(g_invoke_id<0)
				//	TRACE(_T("Input g_invoke_id = %d ,resend count = %d\r\n"),g_invoke_id,resend_count);
			} while (g_invoke_id<0);
			Bacnet_Refresh_Info.Read_Input_Info[i].command = READINPUT_T3000;
			Bacnet_Refresh_Info.Read_Input_Info[i].device_id = g_bac_instance;
			Bacnet_Refresh_Info.Read_Input_Info[i].start_instance = (BAC_READ_INPUT_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Input_Info[i].end_instance =end_temp_instance;
			Bacnet_Refresh_Info.Read_Input_Info[i].invoke_id = g_invoke_id;
			if(Bacnet_Refresh_Info.Read_Input_Info[i].end_instance >= BAC_INPUT_ITEM_COUNT)
				Bacnet_Refresh_Info.Read_Input_Info[i].end_instance = BAC_INPUT_ITEM_COUNT - 1;

			CString temp_cs;
			temp_cs.Format(_T("Read Input List Item From %d to %d "),
				Bacnet_Refresh_Info.Read_Input_Info[i].start_instance,
				Bacnet_Refresh_Info.Read_Input_Info[i].end_instance);
			Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs);
		}
	}

	for (int i=0;i<BAC_OUTPUT_GROUP;i++)
	{

		if((bac_read_which_list == BAC_READ_OUTPUT_LIST) || (bac_read_which_list ==BAC_READ_ALL_LIST) || (bac_read_which_list ==TYPE_SVAE_CONFIG) || (bac_read_which_list == BAC_READ_IN_OUT_VAR_LIST))
		{
			resend_count = 0;
			do 
			{
				resend_count ++;
				if(resend_count>RESEND_COUNT)
					goto myend;
				end_temp_instance = BAC_READ_OUTPUT_REMAINDER + (BAC_READ_OUTPUT_GROUP_NUMBER)*i ;
				if(end_temp_instance >= BAC_OUTPUT_ITEM_COUNT)
					end_temp_instance = BAC_OUTPUT_ITEM_COUNT - 1;
				g_invoke_id = GetPrivateData(g_bac_instance,
					READOUTPUT_T3000,(BAC_READ_OUTPUT_GROUP_NUMBER)*i,
					end_temp_instance,
					sizeof(Str_out_point));
				Sleep(SEND_COMMAND_DELAY_TIME);
			} while (g_invoke_id<0);
			Bacnet_Refresh_Info.Read_Output_Info[i].command = READOUTPUT_T3000;
			Bacnet_Refresh_Info.Read_Output_Info[i].device_id = g_bac_instance;
			Bacnet_Refresh_Info.Read_Output_Info[i].start_instance = (BAC_READ_OUTPUT_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Output_Info[i].end_instance = end_temp_instance;
			Bacnet_Refresh_Info.Read_Output_Info[i].invoke_id = g_invoke_id;

			CString temp_cs;
			temp_cs.Format(_T("Read Output List Item From %d to %d "),
				Bacnet_Refresh_Info.Read_Output_Info[i].start_instance,
				Bacnet_Refresh_Info.Read_Output_Info[i].end_instance);
			Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs);
		}
	}

	for (int i=0;i<BAC_VARIABLE_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_VARIABLE_LIST) || (bac_read_which_list ==BAC_READ_ALL_LIST) || (bac_read_which_list ==TYPE_SVAE_CONFIG) || (bac_read_which_list == BAC_READ_IN_OUT_VAR_LIST))
		{
			int resend_count = 0;
			do 
			{
				resend_count ++;
				if(resend_count>RESEND_COUNT)
					goto myend;
				end_temp_instance = BAC_READ_VARIABLE_REMAINDER + (BAC_READ_VARIABLE_GROUP_NUMBER)*i ;
				if(end_temp_instance >= BAC_VARIABLE_ITEM_COUNT)
					end_temp_instance = BAC_VARIABLE_ITEM_COUNT - 1;
				g_invoke_id = GetPrivateData(g_bac_instance,READVARIABLE_T3000,(BAC_READ_VARIABLE_GROUP_NUMBER)*i,end_temp_instance,sizeof(Str_variable_point));
				//TRACE(_T("g_invoke_id = %d\r\n",g_invoke_id));
				Sleep(SEND_COMMAND_DELAY_TIME);
			} while (g_invoke_id<0);

			Bacnet_Refresh_Info.Read_Variable_Info[i].command = READVARIABLE_T3000;
			Bacnet_Refresh_Info.Read_Variable_Info[i].device_id = g_bac_instance;
			Bacnet_Refresh_Info.Read_Variable_Info[i].start_instance = (BAC_READ_VARIABLE_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Variable_Info[i].end_instance = end_temp_instance;
			Bacnet_Refresh_Info.Read_Variable_Info[i].invoke_id = g_invoke_id;
			CString temp_cs;
			temp_cs.Format(_T("Read Variable List Item From %d to %d "),
				Bacnet_Refresh_Info.Read_Variable_Info[i].start_instance,
				Bacnet_Refresh_Info.Read_Variable_Info[i].end_instance);
			Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs);
		}
	}

	for (int i=0;i<BAC_PROGRAM_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_PROGRAM_LIST) || (bac_read_which_list ==BAC_READ_ALL_LIST) || (bac_read_which_list ==TYPE_SVAE_CONFIG))
		{
			int resend_count = 0;
			do 
			{
				resend_count ++;
				if(resend_count>RESEND_COUNT)
					goto myend;
				end_temp_instance = BAC_READ_PROGRAM_REMAINDER + (BAC_READ_PROGRAM_GROUP_NUMBER)*i ;
				if(end_temp_instance >= BAC_PROGRAM_ITEM_COUNT)
					end_temp_instance = BAC_PROGRAM_ITEM_COUNT - 1;
				g_invoke_id = GetPrivateData(g_bac_instance,READPROGRAM_T3000,(BAC_READ_PROGRAM_GROUP_NUMBER)*i,end_temp_instance,sizeof(Str_program_point));
				Sleep(SEND_COMMAND_DELAY_TIME);
			} while (g_invoke_id<0);

			Bacnet_Refresh_Info.Read_Program_Info[i].command = READPROGRAM_T3000;
			Bacnet_Refresh_Info.Read_Program_Info[i].device_id = g_bac_instance;
			Bacnet_Refresh_Info.Read_Program_Info[i].start_instance = (BAC_READ_PROGRAM_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Program_Info[i].end_instance = end_temp_instance;
			Bacnet_Refresh_Info.Read_Program_Info[i].invoke_id = g_invoke_id;
			CString temp_cs;
			temp_cs.Format(_T("Read Program List Item From %d to %d "),
				Bacnet_Refresh_Info.Read_Program_Info[i].start_instance,
				Bacnet_Refresh_Info.Read_Program_Info[i].end_instance);
			Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs);
		}
	}


	for (int i=0;i<BAC_PID_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_CONTROLLER_LIST) || (bac_read_which_list ==TYPE_SVAE_CONFIG)|| (bac_read_which_list ==BAC_READ_ALL_LIST))
		{
			int resend_count = 0;
			do 
			{
				resend_count ++;
				if(resend_count>RESEND_COUNT)
					goto myend;
				end_temp_instance = BAC_READ_PID_REMAINDER + (BAC_READ_PID_GROUP_NUMBER)*i ;
				if(end_temp_instance >= BAC_PID_COUNT)
					end_temp_instance = BAC_PID_COUNT - 1;
				g_invoke_id = GetPrivateData(g_bac_instance,READCONTROLLER_T3000,(BAC_READ_PID_GROUP_NUMBER)*i,end_temp_instance,sizeof(Str_controller_point));
				Sleep(SEND_COMMAND_DELAY_TIME);
			} while (g_invoke_id<0);

			Bacnet_Refresh_Info.Read_Controller_Info[i].command = READCONTROLLER_T3000;
			Bacnet_Refresh_Info.Read_Controller_Info[i].device_id = g_bac_instance;
			Bacnet_Refresh_Info.Read_Controller_Info[i].start_instance = (BAC_READ_PID_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Controller_Info[i].end_instance = end_temp_instance;
			Bacnet_Refresh_Info.Read_Controller_Info[i].invoke_id = g_invoke_id;
			CString temp_cs;
			temp_cs.Format(_T("Read Controller List Item From %d to %d "),
				Bacnet_Refresh_Info.Read_Controller_Info[i].start_instance,
				Bacnet_Refresh_Info.Read_Controller_Info[i].end_instance);
			Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs);
		}
	}


	if(bac_read_which_list == BAC_READ_WEEKLTCODE_LIST)
	{
		int resend_count = 0;
		do 
		{
			resend_count ++;
			if(resend_count>RESEND_COUNT)
				goto myend;
			g_invoke_id = GetPrivateData(g_bac_instance,READTIMESCHEDULE_T3000,weekly_list_line,weekly_list_line,WEEKLY_SCHEDULE_SIZE);
			Sleep(SEND_COMMAND_DELAY_TIME);
		} while (g_invoke_id<0);

		Bacnet_Refresh_Info.Read_Weeklycode_Info[weekly_list_line].command = READTIMESCHEDULE_T3000;
		Bacnet_Refresh_Info.Read_Weeklycode_Info[weekly_list_line].device_id = g_bac_instance;
		Bacnet_Refresh_Info.Read_Weeklycode_Info[weekly_list_line].start_instance = weekly_list_line;
		Bacnet_Refresh_Info.Read_Weeklycode_Info[weekly_list_line].end_instance = weekly_list_line;
		Bacnet_Refresh_Info.Read_Weeklycode_Info[weekly_list_line].invoke_id = g_invoke_id;
		CString temp_cs;
		temp_cs.Format(_T("Read annuale Schedule time List Item From %d to %d "),
			Bacnet_Refresh_Info.Read_Weeklycode_Info[weekly_list_line].start_instance,
			Bacnet_Refresh_Info.Read_Weeklycode_Info[weekly_list_line].end_instance);
		Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs);
	}
	else if(bac_read_which_list ==TYPE_SVAE_CONFIG)
	{
		for (int i=0;i<BAC_SCHEDULECODE_GOUP;i++)
		{
			int resend_count = 0;
			do 
			{
				resend_count ++;
				if(resend_count>RESEND_COUNT)
					goto myend;
				g_invoke_id = GetPrivateData(g_bac_instance,READTIMESCHEDULE_T3000,i,i,WEEKLY_SCHEDULE_SIZE);
				Sleep(SEND_COMMAND_DELAY_TIME);
			} while (g_invoke_id<0);

			Bacnet_Refresh_Info.Read_Weeklycode_Info[i].command = READTIMESCHEDULE_T3000;
			Bacnet_Refresh_Info.Read_Weeklycode_Info[i].device_id = g_bac_instance;
			Bacnet_Refresh_Info.Read_Weeklycode_Info[i].start_instance = i;
			Bacnet_Refresh_Info.Read_Weeklycode_Info[i].end_instance = i;
			Bacnet_Refresh_Info.Read_Weeklycode_Info[i].invoke_id = g_invoke_id;
			CString temp_cs;
			temp_cs.Format(_T("Read schedule time List Item From %d to %d "),
				Bacnet_Refresh_Info.Read_Weeklycode_Info[i].start_instance,
				Bacnet_Refresh_Info.Read_Weeklycode_Info[i].end_instance);
			Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs);
		}
	}


	if(bac_read_which_list == BAC_READ_ANNUALCODE_LIST)
	{
		int resend_count = 0;
		do 
		{
			resend_count ++;
			if(resend_count>RESEND_COUNT)
				goto myend;
			g_invoke_id = GetPrivateData(g_bac_instance,READANNUALSCHEDULE_T3000,annual_list_line,annual_list_line,ANNUAL_CODE_SIZE);
			Sleep(SEND_COMMAND_DELAY_TIME);
		} while (g_invoke_id<0);

		Bacnet_Refresh_Info.Read_Annualcode_Info[annual_list_line].command = READANNUALSCHEDULE_T3000;
		Bacnet_Refresh_Info.Read_Annualcode_Info[annual_list_line].device_id = g_bac_instance;
		Bacnet_Refresh_Info.Read_Annualcode_Info[annual_list_line].start_instance = annual_list_line;
		Bacnet_Refresh_Info.Read_Annualcode_Info[annual_list_line].end_instance = annual_list_line;
		Bacnet_Refresh_Info.Read_Annualcode_Info[annual_list_line].invoke_id = g_invoke_id;
		CString temp_cs;
		temp_cs.Format(_T("Read Annual day List Item From %d to %d "),
			Bacnet_Refresh_Info.Read_Annualcode_Info[annual_list_line].start_instance,
			Bacnet_Refresh_Info.Read_Annualcode_Info[annual_list_line].end_instance);
		Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs);
	}
	else if(bac_read_which_list ==TYPE_SVAE_CONFIG)
	{
		for (int i=0;i<BAC_HOLIDAYCODE_GROUP;i++)
		{
			int resend_count = 0;
			do 
			{
				resend_count ++;
				if(resend_count>RESEND_COUNT)
					goto myend;
				g_invoke_id = GetPrivateData(g_bac_instance,READANNUALSCHEDULE_T3000,i,i,ANNUAL_CODE_SIZE);
				Sleep(SEND_COMMAND_DELAY_TIME);
			} while (g_invoke_id<0);

			Bacnet_Refresh_Info.Read_Annualcode_Info[i].command = READTIMESCHEDULE_T3000;
			Bacnet_Refresh_Info.Read_Annualcode_Info[i].device_id = g_bac_instance;
			Bacnet_Refresh_Info.Read_Annualcode_Info[i].start_instance = i;
			Bacnet_Refresh_Info.Read_Annualcode_Info[i].end_instance = i;
			Bacnet_Refresh_Info.Read_Annualcode_Info[i].invoke_id = g_invoke_id;
			CString temp_cs;
			temp_cs.Format(_T("Read Holiday List Item From %d to %d "),
				Bacnet_Refresh_Info.Read_Annualcode_Info[i].start_instance,
				Bacnet_Refresh_Info.Read_Annualcode_Info[i].end_instance);
			Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs);
		}
	}



#if 1
	if(bac_read_which_list == BAC_READ_PROGRAMCODE_LIST)
	{
		memset(mycode,0,2000);



		for (int x=0;x<5;x++)
		{
			int send_status = true;
			int resend_count = 0;
			int temp_invoke_id = -1;
			do 
			{
				resend_count ++;
				if(resend_count>RESEND_COUNT)
					goto myend;
				temp_invoke_id = GetProgramData(g_bac_instance,program_list_line,program_list_line,x);
				Sleep(SEND_COMMAND_DELAY_TIME);
			} while (temp_invoke_id<0);

			Sleep(SEND_COMMAND_DELAY_TIME);
			if(send_status)
			{
				for (int i=0;i<2000;i++)
				{
					Sleep(1);
					if(tsm_invoke_id_free(temp_invoke_id))
					{
						goto	part_success;
					}
				}
				goto myend;

part_success:
				continue;
			}



		}

		bac_programcode_read_results = true;
		::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,SHOW_PROGRAM_IDE,0);


		//::PostMessage(m_program_edit_hwnd,WM_REFRESH_BAC_PROGRAM_RICHEDIT,NULL,NULL);

	}
	else if(bac_read_which_list ==TYPE_SVAE_CONFIG)
	{
		for (int z=0;z<BAC_PROGRAM_ITEM_COUNT;z++)
		{
			memset(program_code[z],0,2000);		 //清零;
			for (int x=0;x<5;x++)
			{
				int send_status = true;
				int resend_count = 0;
				int temp_invoke_id = -1;
				do 
				{
					resend_count ++;
					if(resend_count>RESEND_COUNT)
						goto myend;
					temp_invoke_id = GetProgramData(g_bac_instance,z,z,x);
					Sleep(SEND_COMMAND_DELAY_TIME);
				} while (temp_invoke_id<0);


				if(send_status)
				{
					for (int i=0;i<2000;i++)
					{
						Sleep(1);
						if(tsm_invoke_id_free(temp_invoke_id))
						{
							goto	program_part_success;
						}
					}
					Bacnet_Refresh_Info.Read_Programcode_Info[z].task_result = BAC_RESULTS_FAIL;
					goto myend;
					
program_part_success:
					

					
					continue;
				}



			}
			Bacnet_Refresh_Info.Read_Programcode_Info[z].task_result = BAC_RESULTS_OK;
		}
		//bac_programcode_read_results = true;
	}
#endif
	//if( bac_read_which_list == BAC_READ_ALL_LIST)
	//{
	//	g_CommunicationType = 1;
	//	SetCommunicationType(1);
	//	CreateThread(NULL,NULL,ConnectToDevice,NULL,NULL,NULL);
	//}
	
	hwait_thread = NULL;

	return 0;
		//::PostMessage(BacNet_hwd,WM_SEND_OVER,0,0);

myend:	hwait_thread = NULL;
		//AfxMessageBox(_T("Send Command Timeout!!!!!!!!!!"));
		::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,0,0);
	return 0;
}






void CDialogCM5_BacNet::SetConnected_IP(LPCTSTR myip)
{
	IP_ADDRESS.Empty();
	IP_ADDRESS.Format(_T("%s"),myip);
	//IP_ADDRESS = myip;
}

void CDialogCM5_BacNet::WriteFlash()
{
	
	//WRITEPRGFLASH_COMMAND
	int resend_count = 0;
	do 
	{
		resend_count ++;
		if(resend_count>10)
			break;
		g_invoke_id = GetPrivateData(g_bac_instance,WRITEPRGFLASH_COMMAND,0,0,0);
		Sleep(100);
	} while (g_invoke_id<0);

	if(g_invoke_id>0)
	{
		CString temp_cs_show;
		temp_cs_show.Format(_T("Write into flash "));
		Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs_show);
	}


	//Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd);
}





void CDialogCM5_BacNet::Set_Device_Type(bool is_remote_device)
{
	m_is_remote_device = is_remote_device;
}

void CDialogCM5_BacNet::Set_remote_device_IP(LPCTSTR ipaddress)
{
	remote_ip_address.Format(_T("%s"),ipaddress);
}


void CDialogCM5_BacNet::OnTimer(UINT_PTR nIDEvent)
{
	 

	bool is_connected = false;
	switch(nIDEvent)
	{
	case BAC_TIMER_2_WHOIS:
		{
			if(offline_mode)
				break;
			if(this->IsWindowVisible())
			{
				g_bPauseMultiRead = true; // 只要在minipanel的界面 就暂停 读 寄存器的那个线程;
				if(!Gsm_communication)
					m_bac_handle_Iam_data.clear();




				if(m_is_remote_device)
					Send_WhoIs_remote_ip(remote_ip_address);
                else
                {
                    if (g_bac_instance>0)
                        Send_WhoIs_Global(-1, -1);
                }

				if(bac_select_device_online)
				{
					CString temp_file;
					CString temp_serial;
					temp_serial.Format(_T("%u.prog"),g_selected_serialnumber);
					temp_file = g_achive_folder + _T("\\") + temp_serial;
                    SaveBacnetBinaryFile(temp_file);
				}
			}

		}
		break;
	case BAC_TIMER_3_CHECKALARM:
		{
			if((AlarmWindow_Window!=NULL) && ((CBacnetAlarmWindow *)AlarmWindow_Window->m_hWnd !=NULL))
			{
				if(bac_show_alarm_window)//显示;
				{
					if(!(CBacnetAlarmWindow *)AlarmWindow_Window->IsWindowVisible())
					{
						AlarmWindow_Window->ShowWindow(SW_SHOW);
					}
				}
				else//Not show
				{
					if((CBacnetAlarmWindow *)AlarmWindow_Window->IsWindowVisible())
					{
						AlarmWindow_Window->ShowWindow(SW_HIDE);
					}
				}
			}
		}
		break;
	case BAC_READ_SETTING_TIMER:
		{
			if(offline_mode)
			{
				Initial_Some_UI(LOGIN_SUCCESS_FULL_ACCESS);
				KillTimer(BAC_READ_SETTING_TIMER);
				break;
			}
			click_resend_time --;
			if(m_is_remote_device)
				Send_WhoIs_remote_ip(remote_ip_address);
			else
				Send_WhoIs_Global(g_bac_instance, g_bac_instance);
			bool find_exsit = false;
			for (int i=0;i<(int)m_bac_handle_Iam_data.size();i++)
			{
				if(m_bac_handle_Iam_data.at(i).device_id == g_bac_instance)
				{
					find_exsit = true;
					KillTimer(BAC_READ_SETTING_TIMER);
					break;
				}
			}

			if(click_resend_time == 9)
			{
				SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Please wait ...."));
			}

			if(find_exsit)
			{
                if(hbip_whois_thread == NULL)
                    hbip_whois_thread = CreateThread(NULL, NULL, Handle_Bip_whois_Thread, this, NULL, NULL);

				int temp_invoke_id = -1;
				int send_status = true;
				int	resend_count = 0;
				for (int z=0;z<3;z++)
				{
					do 
					{
						resend_count ++;
						if(resend_count>5)
						{
							send_status = false;
							break;
						}
						temp_invoke_id = GetPrivateData(
							g_bac_instance,
							READ_SETTING_COMMAND,
							0,
							0,
							sizeof(Str_Setting_Info));		

						Sleep(SEND_COMMAND_DELAY_TIME);
					} while (temp_invoke_id<0);
					if(send_status)
					{
						bool need_break = false;
						for (int z=0;z<100;z++)
						{
							Sleep(10);
							if(tsm_invoke_id_free(temp_invoke_id))
							{
								SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Read data : OK  "));
								g_llRxCount ++;
								g_llTxCount ++;
								is_connected = true;
								//Device_Basic_Setting.reg.user_name = 2;
								if(Device_Basic_Setting.reg.user_name == 2) //Enable user name
								{
									m_bac_main_tab.ShowWindow(false);
									PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,BAC_READ_USER_LOGIN_INFO);
									User_Login_Window->ShowWindow(SW_NORMAL);

									::PostMessage(m_user_login_hwnd,MY_REDRAW_WINDOW,NULL,NULL);
									return;
								}
								else
								{
									m_user_level = LOGIN_SUCCESS_FULL_ACCESS;
								}
								need_break = true;
								break;
							}
							else
								continue;
						}
						if(need_break)
							break;
						
					}

				}
				if(is_connected)	
				{
					Initial_Some_UI(LOGIN_SUCCESS_FULL_ACCESS);
					KillTimer(BAC_READ_SETTING_TIMER);
					break;
				}
				else
				{
					bac_select_device_online = false;
					KillTimer(BAC_READ_SETTING_TIMER);
					CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
					pFrame->m_pTreeViewCrl->turn_item_image(selected_tree_item ,false);
					//pFrame->m_product.at(selected_product_index).status_last_time[0] = false;
					//pFrame->m_product.at(selected_product_index).status_last_time[1] = false;
					//pFrame->m_product.at(selected_product_index).status_last_time[2] = false;

                    for (int x = 0; x < 5; x++)
                    {
                        pFrame->m_product.at(selected_product_index).status_last_time[x] = false;
                    }
					SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Read data timeout. "));
					break;
				}



			}

			if(!find_exsit)
			{
				if(click_resend_time == 0)
				{
					if(already_retry == false)
					{
						//如果 TCP能连接上， 而没有回复UDP的包，就用TCP 发送软复位命令给板子;
						SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("No bacnet command response!"));
						//if(IDYES == MessageBox(_T("No bacnet command reply from panel .Do you want reset the network of this panel"),_T("Warning"),MB_YESNO))
						//{
						//	write_one(g_tstat_id,33,151,1);

						//	SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Reset the network , please wait!"));

						//}
						click_resend_time = 10;
						already_retry = true;


					}
					//else
					//{
					//	bac_select_device_online = false;
					//	KillTimer(4);
					//	//SetPaneString(2,_T("Offline"));
					//	CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
					//	pFrame->m_pTreeViewCrl->turn_item_image(selected_tree_item ,false);
					//	pFrame->m_product.at(selected_product_index).status_last_time[0] = false;
					//	pFrame->m_product.at(selected_product_index).status_last_time[1] = false;
					//	pFrame->m_product.at(selected_product_index).status_last_time[2] = false;

					//	HKEY hkey;
					//	char sz[256];
					//	DWORD dwtype, sl = 256;

					//	RegOpenKeyEx(HKEY_LOCAL_MACHINE,	_T("SYSTEM\\CurrentControlSet\\Services\\SharedAccess\\Parameters\\FirewallPolicy\\StandardProfile"),	NULL, KEY_READ, &hkey);
					//	RegQueryValueEx(hkey, _T("EnableFirewall"), NULL, &dwtype, (LPBYTE)sz, &sl);
					//	DWORD dw_firewall;
					//	dw_firewall = sz[0] + sz[1] * 256 + sz[2] * 256*256 + sz[3] * 256*256*256;
					//	if(dw_firewall != 0 )
					//	{
					//		//AfxMessageBox(_T("Please turn off your firewall .If not , some broadcast communication may fail."));
					//		pFrame->m_pDialogInfo->ShowWindow(SW_SHOW);
					//		pFrame->m_pDialogInfo->GetDlgItem(IDC_STATIC_INFO)->SetWindowText(_T("Please turn off your firewall or add 'T3000.exe' to firewall exception list.\r\nIf not , some broadcast communication may fail."));
					//	}

					//	RegCloseKey(hkey);

					//}

					
				}
				else
				{
					//TRACE(_T("Resend Who is count = %d\r\n"),10 - click_resend_time);
				}

			}

			if(click_resend_time <= 0)
			{
				SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("No Bacnet 'Who is?' Command response"));
				KillTimer(BAC_READ_SETTING_TIMER);
			}
			
		}
		break;
	case BAC_RESET_WINDOW_TIMER:
		{
			KillTimer(BAC_RESET_WINDOW_TIMER);
			Input_Window->Reset_Input_Rect();
			Output_Window->Reset_Output_Rect();
			Variable_Window->Reset_Variable_Rect();
			Program_Window->Reset_Program_Rect();
			Controller_Window->Reset_Controller_Rect();
			Screen_Window->Reset_Screen_Rect();
			WeeklyRoutine_Window->Reset_Weekly_Rect();
			AnnualRoutine_Window->Reset_Annual_Rect();
			Monitor_Window->Reset_Monitor_Rect();
			AlarmLog_Window->Reset_Alarm_Rect();
			Remote_Point_Window->Reset_RemotePoint_Rect();
			Setting_Window->Reset_Setting_Rect();
		}
		break;
	case BAC_SET_LAST_UI:
		{
			KillTimer(BAC_SET_LAST_UI);
            switch_product_last_view();
		}
		break;
	default:
		break;
	}
	

	CFormView::OnTimer(nIDEvent);
}

void	CDialogCM5_BacNet::Initial_Some_UI(int ntype)
{


	//先用宏定义标记起来;
#ifdef NEED_ANALOG_DIGITAL_ONLY
	Input_Window->Reload_Unit_Type();
#endif
	Output_Window->Reload_Unit_Type();
	Inital_Tab_Loaded_Parameter();
	//PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,TYPE_READ_CUSTOMER_UNIT);//先读下cutomer unit ，在range里面要显示出来;


	CString temp_cstring;
	CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
	pFrame->m_pTreeViewCrl->turn_item_image(selected_tree_item ,true);

	CString achive_file_path;
	CString temp_serial;
	temp_serial.Format(_T("%d.prog"),g_selected_serialnumber);
	achive_file_path = g_achive_folder + _T("\\") + temp_serial;

	if(m_user_level == LOGIN_SUCCESS_GRAPHIC_MODE)
	{

		if(LoadBacnetBinaryFile(false,achive_file_path) < 0 )
			PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,TYPE_ALL);
		m_bac_main_tab.SetCurSel(WINDOW_SCREEN);
		for (int i=0;i<WINDOW_TAB_COUNT;i++)
		{
			if(i==WINDOW_SCREEN)
			{
				pDialog[i]->ShowWindow(SW_SHOW);
			}
			else
			{
				pDialog[i]->ShowWindow(SW_HIDE);
			}
		}

		((BacnetScreen *)pDialog[WINDOW_SCREEN])->Fresh_Screen_List(NULL,NULL);
		((BacnetScreen*)pDialog[WINDOW_SCREEN])->Reg_Hotkey();
		g_hwnd_now = m_screen_dlg_hwnd;
		bacnet_view_number = TYPE_SCREENS;
		old_selected_item = WINDOW_SCREEN;
	}
	else if(m_user_level == LOGIN_SUCCESS_ROUTINE_MODE)
	{
		if(LoadBacnetBinaryFile(false,achive_file_path) < 0 )
			PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,BAC_READ_ALL_LIST);
		m_bac_main_tab.SetCurSel(WINDOW_WEEKLY);
		for (int i=0;i<WINDOW_TAB_COUNT;i++)
		{
			if(i==WINDOW_WEEKLY)
			{
				pDialog[i]->ShowWindow(SW_SHOW);
			}
			else
			{
				pDialog[i]->ShowWindow(SW_HIDE);
			}
		}

		((BacnetWeeklyRoutine *)pDialog[WINDOW_WEEKLY])->Fresh_Weekly_List(NULL,NULL);
		((BacnetWeeklyRoutine*)pDialog[WINDOW_WEEKLY])->Reg_Hotkey();
		g_hwnd_now = m_weekly_dlg_hwnd;
		bacnet_view_number = TYPE_WEEKLY;
		old_selected_item = WINDOW_WEEKLY;
	}
	else
	{	
		if(g_protocol != PROTOCOL_BIP_TO_MSTP)
		{
			if(offline_mode)
			{
				LoadBacnetBinaryFile(false,offline_prg_path);
				PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,BAC_READ_ALL_LIST);
			}
            else if(LoadBacnetBinaryFile(false, achive_file_path) < 0)
			//else if(LoadBacnetBinaryFile_Cache(achive_file_path) < 0 )
			{
                if (g_protocol == PROTOCOL_BACNET_IP)
                {
                    pFrame->Show_Wait_Dialog_And_ReadBacnet();
#if 0
                    if (ShowMessageDlg != NULL)
                    {
                        delete ShowMessageDlg;
                        ShowMessageDlg = NULL;
                    }
                    ShowMessageDlg = new CShowMessageDlg;



                    //CShowMessageDlg dlg;
                    CString temp_task_info;
                    temp_task_info.Format(_T("This is the first time the device has been accessed , the data is being read"));
                    ShowMessageDlg->SetStaticText(temp_task_info);
                    //dlg.SetStaticTextBackgroundColor(RGB(222, 222, 222));
                    ShowMessageDlg->SetStaticTextColor(RGB(0, 0, 255));
                    ShowMessageDlg->SetStaticTextSize(25, 20);
                    ShowMessageDlg->SetProgressAutoClose(250, 100, EVENT_FIRST_LOAD_PROG);
                    //dlg.DoModal();
                    ShowMessageDlg->Create(IDD_AA_SHOWMESSAGE, this);
                    ShowMessageDlg->ShowWindow(SW_HIDE);
#endif
                }
				//PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,BAC_READ_ALL_LIST);
			}
		}

		Set_Tab_Loaded_Parameter(WINDOW_INPUT);
		Set_Tab_Loaded_Parameter(WINDOW_OUTPUT);
		Set_Tab_Loaded_Parameter(WINDOW_VARIABLE);
	}

	if(m_bac_main_tab.IsWindowVisible() == false)
		m_bac_main_tab.ShowWindow(true);
	

	switch(bacnet_view_number)
	{
	case TYPE_INPUT:
		g_hwnd_now = m_input_dlg_hwnd;
		Input_Window->m_input_list.SetFocus();
		::PostMessage(m_input_dlg_hwnd, WM_REFRESH_BAC_INPUT_LIST,NULL,NULL);
		break;
	case TYPE_OUTPUT:
		g_hwnd_now = m_output_dlg_hwnd;
		Output_Window->m_output_list.SetFocus();
		::PostMessage(m_output_dlg_hwnd, WM_REFRESH_BAC_OUTPUT_LIST,NULL,REFRESH_LIST_NOW);
		break;
	case TYPE_VARIABLE:
		g_hwnd_now =  m_variable_dlg_hwnd;
		Variable_Window->m_variable_list.SetFocus();
		::PostMessage(m_variable_dlg_hwnd, WM_REFRESH_BAC_VARIABLE_LIST,NULL,NULL);
		break;
	case TYPE_PROGRAM:
		g_hwnd_now =  m_pragram_dlg_hwnd;
		Program_Window->m_program_list.SetFocus();
		::PostMessage(m_pragram_dlg_hwnd, WM_REFRESH_BAC_PROGRAM_LIST,NULL,NULL);
		break;
	case TYPE_CONTROLLER:
		g_hwnd_now =  m_controller_dlg_hwnd;
		Controller_Window->m_controller_list.SetFocus();
		::PostMessage(m_controller_dlg_hwnd, WM_REFRESH_BAC_CONTROLLER_LIST,NULL,NULL);
		break;
	case TYPE_SCREENS:
		g_hwnd_now =  m_screen_dlg_hwnd;
		Screen_Window->m_screen_list.SetFocus();
		::PostMessage(m_screen_dlg_hwnd, WM_REFRESH_BAC_SCREEN_LIST,NULL,NULL);
		break;
	case TYPE_WEEKLY:
		g_hwnd_now =  m_weekly_dlg_hwnd;
		WeeklyRoutine_Window->m_weeklyr_list.SetFocus();
		::PostMessage(m_weekly_dlg_hwnd, WM_REFRESH_BAC_WEEKLY_LIST,NULL,NULL);
		break;
	case TYPE_ANNUAL:
		g_hwnd_now =  m_annual_dlg_hwnd;
		AnnualRoutine_Window->m_annualr_list.SetFocus();
		::PostMessage(m_annual_dlg_hwnd, WM_REFRESH_BAC_ANNUAL_LIST,NULL,NULL);
		break;
	case TYPE_MONITOR:
		g_hwnd_now =  m_monitor_dlg_hwnd;
		Monitor_Window->m_monitor_list.SetFocus();
		::PostMessage(m_monitor_dlg_hwnd, WM_REFRESH_BAC_MONITOR_LIST,NULL,NULL);
		::PostMessage(m_monitor_dlg_hwnd, WM_REFRESH_BAC_MONITOR_INPUT_LIST,NULL,NULL);
		break;
	case TYPE_ALARMLOG:
		g_hwnd_now =  m_alarmlog_dlg_hwnd;
		AlarmLog_Window->m_alarmlog_list.SetFocus();
		::PostMessage(m_alarmlog_dlg_hwnd, WM_REFRESH_BAC_ALARMLOG_LIST,NULL,NULL);
		break;
	case TYPE_READ_REMOTE_POINT_INFO:
		g_hwnd_now =  m_remote_point_hwnd;
		Remote_Point_Window->m_remote_point_list.SetFocus();
		::PostMessage(m_remote_point_hwnd, WM_REFRESH_BAC_REMOTE_POINT_LIST,NULL,NULL);
		break;
	default:
		break;
	}

	bac_select_device_online = true;
	CString temp_serial_number;
	temp_serial_number.Format(_T("%u"),g_selected_serialnumber);
	
	int temp_write_flag = GetPrivateProfileInt(temp_serial_number,_T("WriteFlag"),0,g_achive_device_name_path);

	if(Device_Basic_Setting.reg.en_panel_name == 1)
	{
		if(temp_write_flag)	//本地的 name 比较新  更新至 设备; 
		{
			CString Write_String;
			char temp_buffer[20];
			GetPrivateProfileStringW(temp_serial_number,_T("NewName"),_T(""),Write_String.GetBuffer(MAX_PATH),MAX_PATH,g_achive_device_name_path);
			if(Write_String.GetLength()>=20)
			{
				Write_String = Write_String.Left(19);
			}
			WideCharToMultiByte( CP_ACP, 0, Write_String.GetBuffer(), -1, temp_buffer, 255, NULL, NULL );
			memcpy_s(Device_Basic_Setting.reg.panel_name,20,temp_buffer,20);
			if(Write_Private_Data_Blocking(WRITE_SETTING_COMMAND,0,0) <= 0)
			{
				SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Update panel name failed!"));
			}
			else
			{
				SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Update panel name success!"));
				WritePrivateProfileStringW(temp_serial_number,NULL,NULL,g_achive_device_name_path);
			}
		}
		else	//设备的比较新
		{
            //20171219 杜帆屏蔽  //不在去判断更新，有时候点击的时候会出现两个同名的设备，可能是Setting结构重名了。
			//CString temp_device_panel_name;
			//MultiByteToWideChar( CP_ACP, 0, Device_Basic_Setting.reg.panel_name, (int)strlen(Device_Basic_Setting.reg.panel_name)+1,temp_device_panel_name.GetBuffer(MAX_PATH), MAX_PATH );
			//temp_device_panel_name.ReleaseBuffer();
			//temp_device_panel_name.Remove('\'');
			//temp_device_panel_name.Remove('\%');
			//if((!temp_device_panel_name.IsEmpty()) && (temp_device_panel_name.GetLength() <20))
			//{
			//	if(temp_device_panel_name.CompareNoCase(pFrame->m_product.at(selected_product_index).NameShowOnTree) != 0)
			//	{
			//		CppSQLite3DB SqliteDBBuilding;
			//		CppSQLite3Table table;
			//		CppSQLite3Query q;
			//		SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);
			//		CString strSql;
			//		strSql.Format(_T("update ALL_NODE set Product_name='%s' where Serial_ID ='%s'"),temp_device_panel_name,temp_serial_number);
			//		SqliteDBBuilding.execDML((UTF8MBSTR)strSql); 
			//		if(selected_product_index < pFrame->m_product.size())
			//		{
			//			pFrame->m_pTreeViewCrl->SetItemText(pFrame->m_product.at(selected_product_index).product_item,temp_device_panel_name);
			//			pFrame->m_product.at(selected_product_index).NameShowOnTree = temp_device_panel_name;
			//		}
			//		SqliteDBBuilding.closedb();
			//	}
			//}
			
		}
	}
	if((!read_customer_unit) && pFrame->m_product.at(selected_product_index).protocol != MODBUS_RS485 
		&& (
			(pFrame->m_product.at(selected_product_index).product_class_id ==PM_CM5 ) 
			||
			(pFrame->m_product.at(selected_product_index).product_class_id ==PM_MINIPANEL )	
			||
			(pFrame->m_product.at(selected_product_index).product_class_id == PM_MINIPANEL_ARM)
			)
	  )
	{
		if(GetPrivateData_Blocking(g_bac_instance,READUNIT_T3000,0,BAC_CUSTOMER_UNITS_COUNT - 1,sizeof(Str_Units_element)) > 0)
		{
			read_customer_unit = true;
		}
	}

	//if ((!read_analog_customer_unit) && pFrame->m_product.at(selected_product_index).protocol != MODBUS_RS485 && ((pFrame->m_product.at(selected_product_index).product_class_id == PM_CM5) ||
	//	(pFrame->m_product.at(selected_product_index).product_class_id == PM_MINIPANEL)))
      if((!read_analog_customer_unit) && 
		pFrame->m_product.at(selected_product_index).protocol != MODBUS_RS485 && 
		((pFrame->m_product.at(selected_product_index).product_class_id ==PM_CM5 ) ||
		(pFrame->m_product.at(selected_product_index).product_class_id ==PM_MINIPANEL )
			||
			(pFrame->m_product.at(selected_product_index).product_class_id == PM_MINIPANEL_ARM)
			))
	{
		CString temp_cs;
		if(GetPrivateData_Blocking(g_bac_instance,READANALOG_CUS_TABLE_T3000,0,3,sizeof(Str_table_point)) > 0)
		{
			GetPrivateData_Blocking(g_bac_instance,READANALOG_CUS_TABLE_T3000,4,4,sizeof(Str_table_point));
			temp_cs.Format(_T("Read Analog custmer table OK."));
			SetPaneString(BAC_SHOW_MISSION_RESULTS,temp_cs);
			read_analog_customer_unit = true;		
		}

		if(GetPrivateData_Blocking(g_bac_instance,READVARUNIT_T3000,0,4,sizeof(Str_variable_uint_point)) > 0)
		{
			temp_cs.Format(_T("Read Data OK."));
			SetPaneString(BAC_SHOW_MISSION_RESULTS,temp_cs);
		}


	}
	  /*if (write_indb_thread == NULL)
		  write_indb_thread = CreateThread(NULL, NULL, Write_Data_Into_Db, this, NULL, NULL);*/
	WritePrivateProfileStringW(_T("LastView"),_T("ViewSerialNumber"),temp_serial_number,g_cstring_ini_path);
	WritePrivateProfileStringW(_T("LastView"),_T("ViewPid"),_T("35"),g_cstring_ini_path);
	SetTimer(BAC_SET_LAST_UI,1000,NULL);

}
void	CDialogCM5_BacNet::Set_Tab_Loaded_Parameter(int ntab)
{
	ASSERT(ntab < WINDOW_TAB_COUNT);
	tab_loaded[ntab] = true;
}

void CDialogCM5_BacNet::Inital_Tab_Loaded_Parameter()
{
	for (int i=0;i<WINDOW_TAB_COUNT;i++)
	{
		tab_loaded[i] = false;
	}
	static unsigned int static_value_read = 0;
	if(static_value_read == 0)
	{
        if (offline_mode == false)
        {
            read_customer_unit = false;
            receive_customer_unit = false;
            read_analog_customer_unit = false;
        }
		static_value_read = g_selected_serialnumber ;
	}
	else if(static_value_read != g_selected_serialnumber) 
	{
        if (offline_mode == false)
        {
            read_customer_unit = false;
            read_analog_customer_unit = false;
            receive_customer_unit = false;
        }
		static_value_read = g_selected_serialnumber;
	}

}

void CDialogCM5_BacNet::OnTcnSelchangeBacMaintab(NMHDR *pNMHDR, LRESULT *pResult)
{
	
	
	int selected = m_bac_main_tab.GetCurSel();



	switch(old_selected_item)
	{
	case WINDOW_PROGRAM:
		((CBacnetProgram*)pDialog[WINDOW_PROGRAM])->Unreg_Hotkey();
		break;
	case WINDOW_SCREEN:
		((BacnetScreen*)pDialog[WINDOW_SCREEN])->Unreg_Hotkey();
		break;
	case WINDOW_WEEKLY:
		((BacnetWeeklyRoutine*)pDialog[WINDOW_WEEKLY])->Unreg_Hotkey();
		break;
	case WINDOW_ANNUAL:
		((BacnetAnnualRoutine*)pDialog[WINDOW_ANNUAL])->Unreg_Hotkey();
		break;
	case WINDOW_MONITOR:
		((CBacnetMonitor*)pDialog[WINDOW_MONITOR])->Unreg_Hotkey();
		break;
	default:
		break;
	}

	if(m_user_level == LOGIN_SUCCESS_GRAPHIC_MODE)	//如果是图片模式只让显示图片;
	{
		if(selected != WINDOW_SCREEN)
		{
			for (int i=0;i<WINDOW_TAB_COUNT;i++)
			{
				if(i==WINDOW_SCREEN)
				{
					pDialog[i]->ShowWindow(SW_SHOW);
				}
				else
				{
					pDialog[i]->ShowWindow(SW_HIDE);
				}
			}
			if(!tab_loaded[WINDOW_SCREEN])
			{
				PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,TYPE_SCREENS);
				Set_Tab_Loaded_Parameter(WINDOW_SCREEN);
			}
			((BacnetScreen *)pDialog[WINDOW_SCREEN])->Fresh_Screen_List(NULL,NULL);
			((BacnetScreen*)pDialog[WINDOW_SCREEN])->Reg_Hotkey();
			g_hwnd_now = m_screen_dlg_hwnd;
			bacnet_view_number = TYPE_SCREENS;
			m_bac_main_tab.SetCurSel(WINDOW_SCREEN);
			MessageBox(_T("Insufficient User Permissions"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
			Invalidate(TRUE);
		}
		return;
	}
	else if(m_user_level == LOGIN_SUCCESS_ROUTINE_MODE)
	{
		if((selected != WINDOW_WEEKLY) && (selected != WINDOW_ANNUAL))
		{
			for (int i=0;i<WINDOW_TAB_COUNT;i++)
			{
				if(i==WINDOW_WEEKLY)
				{
					pDialog[i]->ShowWindow(SW_SHOW);
				}
				else
				{
					pDialog[i]->ShowWindow(SW_HIDE);
				}
			}
			if(!tab_loaded[WINDOW_WEEKLY])
			{
				PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,TYPE_WEEKLY);
				Set_Tab_Loaded_Parameter(WINDOW_WEEKLY);
			}
			((BacnetWeeklyRoutine *)pDialog[WINDOW_WEEKLY])->Fresh_Weekly_List(NULL,NULL);
			((BacnetWeeklyRoutine*)pDialog[WINDOW_WEEKLY])->Reg_Hotkey();
			g_hwnd_now = m_weekly_dlg_hwnd;
			bacnet_view_number = TYPE_WEEKLY;
			m_bac_main_tab.SetCurSel(WINDOW_WEEKLY);
			MessageBox(_T("Insufficient User Permissions"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
			Invalidate(TRUE);
			return;
		}
		
	}

	for (int i=0;i<WINDOW_TAB_COUNT;i++)
	{
		if(i==selected)
		{
			pDialog[i]->ShowWindow(SW_SHOW);
			switch(i)
			{
			case WINDOW_INPUT:
				//PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,TYPE_INPUT);
				if(!tab_loaded[WINDOW_INPUT])
				{
					PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,TYPE_INPUT);
					Set_Tab_Loaded_Parameter(WINDOW_INPUT);
				}
				((CBacnetInput *)pDialog[i])->Fresh_Input_List(NULL,NULL);
				g_hwnd_now = m_input_dlg_hwnd;
				bacnet_view_number = TYPE_INPUT;
				break;
			case WINDOW_OUTPUT:
				//PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,TYPE_OUTPUT);
				if(!tab_loaded[WINDOW_OUTPUT])
				{
					PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,TYPE_OUTPUT);
					Set_Tab_Loaded_Parameter(WINDOW_OUTPUT);
				}

				((CBacnetOutput *)pDialog[i])->Fresh_Output_List(NULL,NULL);
				g_hwnd_now = m_output_dlg_hwnd;
				bacnet_view_number = TYPE_OUTPUT;
				break;
			case WINDOW_VARIABLE:
				if(!tab_loaded[WINDOW_VARIABLE])
				{
					PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,TYPE_VARIABLE);
					Set_Tab_Loaded_Parameter(WINDOW_VARIABLE);
				}
				((CBacnetVariable *)pDialog[i])->Fresh_Variable_List(NULL,NULL);
				g_hwnd_now = m_variable_dlg_hwnd;
				bacnet_view_number = TYPE_VARIABLE;
				break;
			case WINDOW_PROGRAM:
				if(!tab_loaded[WINDOW_PROGRAM])
				{
					PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,TYPE_PROGRAM);
					Set_Tab_Loaded_Parameter(WINDOW_PROGRAM);
				}

				((CBacnetProgram *)pDialog[i])->Fresh_Program_List(NULL,NULL);
				((CBacnetProgram*)pDialog[i])->Reg_Hotkey();
				g_hwnd_now = m_pragram_dlg_hwnd;
				bacnet_view_number = TYPE_PROGRAM;
				break;
			case WINDOW_CONTROLLER:
				if(!tab_loaded[WINDOW_CONTROLLER])
				{
					PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,TYPE_CONTROLLER);
					Set_Tab_Loaded_Parameter(WINDOW_CONTROLLER);
				}
				//PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,TYPE_CONTROLLER);
				((BacnetController *)pDialog[i])->Fresh_Controller_List(NULL,NULL);
				g_hwnd_now = m_controller_dlg_hwnd;
				bacnet_view_number = TYPE_CONTROLLER;
				break;
			case WINDOW_SCREEN:
				if(!tab_loaded[WINDOW_SCREEN])
				{
					PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,TYPE_SCREENS);
					Set_Tab_Loaded_Parameter(WINDOW_SCREEN);
				}
				((BacnetScreen *)pDialog[i])->Fresh_Screen_List(NULL,NULL);
				((BacnetScreen*)pDialog[i])->Reg_Hotkey();
				g_hwnd_now = m_screen_dlg_hwnd;
				bacnet_view_number = TYPE_SCREENS;
				break;
			case WINDOW_WEEKLY:
				if(!tab_loaded[WINDOW_WEEKLY])
				{
					PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,TYPE_WEEKLY);
					Set_Tab_Loaded_Parameter(WINDOW_WEEKLY);
				}
				((BacnetWeeklyRoutine *)pDialog[i])->Fresh_Weekly_List(NULL,NULL);
				((BacnetWeeklyRoutine*)pDialog[i])->Reg_Hotkey();
				g_hwnd_now = m_weekly_dlg_hwnd;
				bacnet_view_number = TYPE_WEEKLY;
				break;
			case WINDOW_ANNUAL:
				if(!tab_loaded[WINDOW_ANNUAL])
				{
					PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,TYPE_ANNUAL);
					Set_Tab_Loaded_Parameter(WINDOW_ANNUAL);
				}
				((BacnetAnnualRoutine *)pDialog[i])->Fresh_Annual_Routine_List(NULL,NULL);
				((BacnetAnnualRoutine*)pDialog[i])->Reg_Hotkey();
				g_hwnd_now = m_annual_dlg_hwnd;
				bacnet_view_number = TYPE_ANNUAL;
				break;
			case WINDOW_MONITOR:
				if(!tab_loaded[WINDOW_MONITOR])
				{
					PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,TYPE_MONITOR);
					Set_Tab_Loaded_Parameter(WINDOW_MONITOR);
				}
				((CBacnetMonitor *)pDialog[i])->Fresh_Monitor_List(NULL,NULL);
				((CBacnetMonitor *)pDialog[i])->Fresh_Monitor_Input_List(NULL,NULL);
				((CBacnetMonitor*)pDialog[i])->Reg_Hotkey();
				g_hwnd_now = m_monitor_dlg_hwnd;
				bacnet_view_number = TYPE_MONITOR;
				break;
			case WINDOW_ALARMLOG:
				//增加刷新list和改变当前window 的hwnd;
				if(!tab_loaded[WINDOW_ALARMLOG])
				{
					PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,TYPE_ALARMLOG);
					Set_Tab_Loaded_Parameter(WINDOW_ALARMLOG);
				}

				((CBacnetAlarmLog *)pDialog[i])->Fresh_Alarmlog_List(NULL,NULL);
				g_hwnd_now = m_alarmlog_dlg_hwnd;
				bacnet_view_number = TYPE_ALARMLOG;
				break;
			case WINDOW_TSTAT:

				if(!tab_loaded[WINDOW_TSTAT])
				{
					PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,TYPE_TSTAT);
					Set_Tab_Loaded_Parameter(WINDOW_TSTAT);
				}
				((CBacnetTstat *)pDialog[i])->Fresh_Tstat_List(NULL,NULL);
				
				g_hwnd_now = m_tstat_dlg_hwnd;
				bacnet_view_number = TYPE_TSTAT;
				break;
			case WINDOW_SETTING:
				if(!tab_loaded[WINDOW_SETTING])
				{
					PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,TYPE_SETTING);
					Set_Tab_Loaded_Parameter(WINDOW_SETTING);
				}
				((CBacnetSetting *)pDialog[i])->Fresh_Setting_UI(READ_SETTING_COMMAND,NULL);
				g_hwnd_now = m_setting_dlg_hwnd;
				bacnet_view_number = TYPE_SETTING;
				break;
			}
			
//			pDialog[i]->OnInitDialog();
		}
		else
		{
			pDialog[i]->ShowWindow(SW_HIDE);
		}
	}

	pDialog[selected]->SetFocus();

	if(old_selected_item!= selected)
		old_selected_item = selected;
	*pResult = 0;
}

DWORD WINAPI RS485_Read_Each_List_Thread(LPVOID lpvoid)
{
	unsigned char  read_device_id = 0;
	CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
    if ((pFrame->m_product.size() == 0) || (selected_product_index  >= pFrame->m_product.size()))
        return 0;
	read_device_id = pFrame->m_product.at(selected_product_index).product_id;
	bool read_result = true;
	unsigned short read_data_buffer[3200];
	memset(read_data_buffer,0,sizeof(unsigned short)*3200);

	int output_reg = 0;
	int input_reg = 0;
	int pid_con_reg = 0;
	if(n_read_product_type == T38AI8AO6DO)
	{
		output_reg = 4; // (6+8)*23 = 322/100 = 4
		input_reg =  2; //  23 * 8 = 184
	}
	else if(n_read_product_type == PID_T322AI)
	{
		output_reg = 0; // (6+8)*23 = 322
		input_reg =  6; //  23 * 22 = 506
	}
	else if (n_read_product_type == PWM_TRANSDUCER)
	{
		output_reg = 2; // (0 + 6)*23 = 138
		input_reg = 2; //  (0+6) * 22 = 132
	}
	else if(n_read_product_type == PID_T3PT12)
	{
		output_reg = 0; // (6+8)*23 = 322
		input_reg =  3; //  23 * 12 = 506
	}
	else if (n_read_product_type == PM_T3_LC)
	{
		output_reg =2 ; // (6+8)*23 = 322
		input_reg = 2; //  23 * 12 = 506
	}
	else if (n_read_product_type == PID_T36CTA)
	{
		output_reg = 1;       //(1)*23 = 322
		input_reg = 6;       //23 * 12 = 506
	}
	else if(n_read_product_type == STM32_HUM_NET)
	{
		pid_con_reg = 1;		//28*3
	}
	else
	{
		output_reg = 15; //默认是读minipanel 的所有的寄存器 ;
		input_reg = 15;
	}
	
	
	int read_type = -1;

	switch(n_read_list_flag)
	{
	case BAC_OUT:   //OUT
		{
			//output 45 按46算  *64  + input 46  *64  需要读2944;
			for(int i=0; i<output_reg; i++)
			{
				int itemp = 0;
				itemp = Read_Multi(read_device_id,&read_data_buffer[i*100],BAC_OUT_START_REG+i*100,100,4);
				if(itemp < 0 )
				{
					read_result = false;
					break; 
				}
				else
				{
					if(!hide_485_progress)
						g_progress_persent = (i+1)*100 / output_reg;	
					else
					{
						SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Reading Outputs..."));
					}
				}
				Sleep(100);
			}
			if(read_result)
			{
				SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Read Outputs OK!"));
				for (int i=0;i<BAC_OUTPUT_ITEM_COUNT;i++)
				{
					memcpy( &m_Output_data.at(i),&read_data_buffer[i*23],sizeof(Str_out_point));//因为Output 只有45个字节，两个byte放到1个 modbus的寄存器里面;
				}

				CString str_serialid;
				str_serialid.Format(_T("%u"), pFrame->m_product.at(selected_product_index).serial_number);
				CString achive_file_path;
				CString temp_serial;
				achive_file_path = g_achive_folder + _T("\\") + _T("Modbus_") + str_serialid + _T(".prog");

				SaveModbusConfigFile_Cache(achive_file_path,NULL,3200*2);

				if(Output_Window->IsWindowVisible())
					::PostMessage(m_output_dlg_hwnd,WM_REFRESH_BAC_OUTPUT_LIST,NULL,NULL);
			}
            g_progress_persent = 100;
		}
		break;
	case BAC_IN:   //IN
		{
			//output 45 按46算  *64  + input 46  *64  需要读2944;
			for(int i=0; i<input_reg; i++)
			{

				int itemp = 0;
				itemp = Read_Multi(read_device_id,&read_data_buffer[i*100],BAC_IN_START_REG+i*100,100,4);
				if(itemp < 0 )
				{
					read_result = false;
					break; 
				}
				else
				{
					if(!hide_485_progress)
						g_progress_persent = (i+1)*100 / input_reg;	
					else
					{
						SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Reading Inputs..."));
					}
				}
				Sleep(100);
			}
			if(read_result)
			{
				SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Read Iutputs OK!"));

				for (int i=0;i<BAC_INPUT_ITEM_COUNT;i++)
				{
					memcpy( &m_Input_data.at(i),&read_data_buffer[i*23],sizeof(Str_in_point));//因为Input 只有45个字节，两个byte放到1个 modbus的寄存器里面;
				}

				CString str_serialid;
				str_serialid.Format(_T("%u"), pFrame->m_product.at(selected_product_index).serial_number);
				CString achive_file_path;
				CString temp_serial;
				achive_file_path = g_achive_folder + _T("\\") + _T("Modbus_") + str_serialid + _T(".prog");

				SaveModbusConfigFile_Cache(achive_file_path,NULL,3200*2);

			
				if(Input_Window->IsWindowVisible())
					::PostMessage(m_input_dlg_hwnd,WM_REFRESH_BAC_INPUT_LIST,NULL,NULL);
			}
            g_progress_persent = 100;
			
		}
		for (int i = 0;i < 3;i++)
		{
			int itemp = 0;
			itemp = Read_Multi(read_device_id, &read_data_buffer[i * 100], BAC_CUSTOMER_TABLE_START + i * 100, 100, 4);
			if (itemp < 0)
			{
				read_result = false;
				break;
			}
			else
			{
				if (!hide_485_progress)
					g_progress_persent = (i + 1) * 100 / 2;
			}
			Sleep(100);

		}
		if (read_result)
		{
			SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Read Analog table OK!"));


			for (int i = 0;i<BAC_ALALOG_CUSTMER_RANGE_TABLE_COUNT;i++)
			{
				memcpy(&m_analog_custmer_range.at(i), &read_data_buffer[i * 53], sizeof(Str_table_point));//因为Input 只有45个字节，两个byte放到1个 modbus的寄存器里面;
			}

		}

	break;
	case BAC_PID:
		{
			//output 45 按46算  *64  + input 46  *64  需要读2944;
			for(int i=0; i<pid_con_reg; i++)
			{

				int itemp = 0;
				itemp = Read_Multi(read_device_id,&read_data_buffer[i*100],BAC_PID_CONTROL_START_REG+i*100,100,4);
				if(itemp < 0 )
				{
					read_result = false;
					break; 
				}
				else
				{
					if(!hide_485_progress)
						g_progress_persent = (i+1)*100 / pid_con_reg;	
					else
					{
						SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Reading pid controlls..."));
					}
				}
				Sleep(100);
			}
			if(read_result)
			{
				SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Read PID OK!"));

				for (int i=0;i<BAC_PID_COUNT;i++)
				{
					memcpy( &m_controller_data.at(i),&read_data_buffer[i*14],sizeof(Str_controller_point));
				}

				if(Controller_Window->IsWindowVisible())
					::PostMessage(m_controller_dlg_hwnd,WM_REFRESH_BAC_CONTROLLER_LIST,NULL,NULL);
			}
		}
		break;

	case   READ_SETTING_COMMAND:
		{
			//Setting 400 个字节
			for(int i=0; i<2; i++)
			{
				int itemp = 0;
				itemp = Read_Multi(read_device_id,&read_data_buffer[i*100],BAC_SETTING_START_REG+i*100,100,4);
				if(itemp < 0 )
				{
					read_result = false;
					break; 
				}
				else
				{
					if(!hide_485_progress)
						g_progress_persent = (i+1)*100 / 2;	
				}
				Sleep(100);
			}

			if(read_result)
			{
				SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Read Setting infomation OK!"));

				memcpy(&Device_Basic_Setting.reg,read_data_buffer,400); //Setting 的400个字节;


				CString str_serialid;
				str_serialid.Format(_T("%u"), pFrame->m_product.at(selected_product_index).serial_number);
				CString achive_file_path;
				CString temp_serial;
				achive_file_path = g_achive_folder + _T("\\") + _T("Modbus_") + str_serialid + _T(".prog");

				SaveModbusConfigFile_Cache(achive_file_path,NULL,3200*2);


				if(Setting_Window->IsWindowVisible())
					 ::PostMessage(m_setting_dlg_hwnd,WM_FRESH_SETTING_UI,READ_SETTING_COMMAND,NULL);

			}

		}
		break;
	default:
		break;
	}
	hide_485_progress = false;
	read_each_485_fun_thread = NULL;
	return 0;
}


DWORD WINAPI RS485_Connect_Thread(LPVOID lpvoid)
{
	close_com();
	CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);

	if((selected_product_index > pFrame->m_product.size()) || (pFrame->m_product.size() == 0))
	{
		read_rs485_thread = NULL;
		return 2;
	}

	int n_comport = pFrame->m_product.at(selected_product_index).ncomport;
	unsigned char  read_device_id = 0;
	read_device_id =  pFrame->m_product.at(selected_product_index).product_id;
	int m_nbaudrat;
	m_nbaudrat = pFrame->m_product.at(selected_product_index).baudrate;
	if((n_comport==0)|| (n_comport >50))
	{
		AfxMessageBox(_T("Serial Port error!"));
		read_rs485_thread = NULL;
		return 1;
	}
	bool bret;
	bret =	open_com(n_comport);
	if(bret == false)
	{
		 read_rs485_thread = NULL;
		AfxMessageBox(_T("Open serial port failed!"));
		SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Open serial port failed!"));
		return 2;
	}
	 Change_BaudRate(m_nbaudrat);
	 unsigned short read_data[100];
	 int nmultyRet=Read_Multi(g_tstat_id,&read_data[0],0,100,3);
	 if(nmultyRet < 0)
	 {
		 SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Read Register 0-100 timeout!"));
		 read_rs485_thread = NULL;
		 return 3;
	 }
	 g_llTxCount ++;
	 g_llRxCount ++;

	 SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Reading data..."));

	 Sleep(100);
	 unsigned short temp_panel_number = 0 ;
	 unsigned int   temp_object_instance = 0;
	 bool temp_update =false;

	 temp_panel_number = read_data[36];
	 temp_object_instance = read_data[35];

	 CString str_object_instance;
	 CString str_panel_number;
	 CString str_serialid;
	 CString strSql;
	 str_object_instance.Format(_T("%u"),temp_object_instance);
	 str_panel_number.Format(_T("%u"),temp_panel_number);
	 str_serialid.Format(_T("%u"), pFrame->m_product.at(selected_product_index).serial_number);
	 if(read_data[MODBUS_PRODUCT_MODEL] == PM_MINIPANEL|| read_data[MODBUS_PRODUCT_MODEL] == PM_MINIPANEL_ARM)
	 {
		 // 同步 本地数据库 的资料 ， panel number 和 实例号需与设备匹配;
		 if((temp_panel_number != pFrame->m_product.at(selected_product_index).panel_number) || (temp_object_instance != pFrame->m_product.at(selected_product_index).object_instance))
		 {
			 CppSQLite3DB SqliteDBBuilding;
			 CppSQLite3Table table;
			 CppSQLite3Query q;
			 SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);

			 CString temp_pro;
			 temp_pro.Format(_T("%u"),PROTOCOL_BIP_TO_MSTP);
			 strSql.Format(_T("update ALL_NODE set  Object_Instance = '%s' , Panal_Number = '%s' ,Online_Status = 1  where Serial_ID = '%s'"),str_object_instance,str_panel_number,str_serialid);
			 temp_update = true;
			 SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
		 }
	 }

	 /*
	 从缓存中得到上次的数据，如果版本不对就重新读一遍.
	 */

	 CString achive_file_path;
	 CString temp_serial;
	 achive_file_path = g_achive_folder + _T("\\") + _T("Modbus_") + str_serialid + _T(".prog");


	 if(LoadModbusConfigFile_Cache(achive_file_path)< 0)
	 {

		 bool read_result = true;
		 unsigned int buffer_length = 3200;
		 unsigned short read_data_buffer[3200];
		 memset(read_data_buffer,0,sizeof(unsigned short)*3200);
		 //output 45 按46算  *64  + input 46  *64  需要读2944;
		 for(int i=0; i<32; i++)
		 {
			 int itemp = 0;
			 itemp = Read_Multi(read_device_id,&read_data_buffer[i*100],BAC_SETTING_START_REG+i*100,100,4);
			 if(itemp < 0 )
			 {
				 read_result = false;
				 break; 
			 }
			 else
			 {
				 g_progress_persent = (i+1)*100 / 32;	
			 }
			 Sleep(100);
		 }
		 if(read_result)
		 {
			 SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Read Inputs and Outputs OK!"));
		 }

		 Copy_Data_From_485_to_Bacnet(&read_data_buffer[0]);
		 SaveModbusConfigFile_Cache(achive_file_path,(char *)read_data_buffer,buffer_length*2);

		 if(temp_update)
			 ::PostMessage(pFrame->m_hWnd,WM_MYMSG_REFRESHBUILDING,0,0);
	 }
	 else
	 {
		 g_llTxCount ++;
		 g_llRxCount ++;

		 SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Reading data :OK"));
		 if(Input_Window->IsWindowVisible())
			::PostMessage(m_input_dlg_hwnd, WM_REFRESH_BAC_INPUT_LIST,NULL,NULL);
		 else if(Output_Window->IsWindowVisible())
			::PostMessage(m_output_dlg_hwnd, WM_REFRESH_BAC_OUTPUT_LIST,NULL,NULL);
		 else if(Setting_Window->IsWindowVisible())
			  ::PostMessage(m_setting_dlg_hwnd,WM_FRESH_SETTING_UI,READ_SETTING_COMMAND,NULL);

	 }
	 //PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,TYPE_ALL);





	 read_rs485_thread = NULL;
	return 0;
}

DWORD WINAPI  Mstp_Connect_Thread(LPVOID lpVoid)
{
	set_datalink_protocol(2);
	Initial_bac(g_gloab_bac_comport,_T(""), g_gloab_bac_baudrate);

	bool find_exsit = false;
	for (int i=0;i<10;i++)
	{
		if(g_bac_instance<=0)
		{
			connect_mstp_thread = NULL;
			return 0;
		}
		Send_WhoIs_Global(-1, -1);
		Sleep(1500);

		for (int i=0;i<(int)m_bac_handle_Iam_data.size();i++)
		{
			if(m_bac_handle_Iam_data.at(i).device_id == g_bac_instance)
			{
				find_exsit = true;
				break;
			}
		}

		if(find_exsit)
			break;
		else
		{
			CString temp_cs_2;
			temp_cs_2.Format(_T("Wait for response.(%ds)"),10-i);
			SetPaneString(BAC_SHOW_MISSION_RESULTS,temp_cs_2);
		}
	}


	if(find_exsit == false)
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("No Who is response!"));
		connect_mstp_thread = NULL;
		return 0;
	}
	else
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Receive bacnet 'who is ' command!"));
	}


	if(GetPrivateData_Blocking(g_bac_instance,READ_SETTING_COMMAND,0,0,sizeof(Str_Setting_Info)) > 0)
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Read data success!"));
	}

	
	connect_mstp_thread = NULL;
	return 1;
}


LRESULT CDialogCM5_BacNet::RS485_Read_Fun(WPARAM wParam,LPARAM lParam)
{
	n_read_product_type = wParam;
	//int n_read_list = lParam;
	n_read_list_flag = lParam;
	if(read_each_485_fun_thread == NULL)
		read_each_485_fun_thread = CreateThread(NULL,NULL,RS485_Read_Each_List_Thread,this,NULL, NULL);
	else
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Reading data .  Please wait!"));
	}
	return 0;
}


BOOL CDialogCM5_BacNet::PreTranslateMessage(MSG* pMsg)
{
	
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_TAB) 
	{
		if(m_bac_main_tab.IsWindowVisible() == false)
		{
			if(User_Login_Window->IsWindowVisible())
				User_Login_Window->SetFocus();
			return 1;
		}
		if(m_user_level == LOGIN_SUCCESS_GRAPHIC_MODE)	//如果是图片模式只让显示图片;
		{
			return 1;
		}

		return TRUE;
	}
	else if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_DOWN)
	{
		m_cur_tab_sel = m_bac_main_tab.GetCurSel();
		pDialog[m_cur_tab_sel]->SetFocus();
	}
    if (pMsg->message == WM_LBUTTONDOWN||pMsg->message == WM_RBUTTONDOWN)
    {
        //  pMain->m_pDialogInfo->ShowWindow(SW_HIDE);
        ::PostMessage(MainFram_hwd,WM_LBUTTONDOWN,0,0);
    }
	return CFormView::PreTranslateMessage(pMsg);
}

DWORD WINAPI Handle_Bip_whois_Thread(LPVOID lpvoid)
{
    return 1;
    Sleep(1);
    vector <_Bac_Scan_Com_Info> temp_vector;
    for (int i = 0; i < m_bac_handle_Iam_data.size(); i++)
    {
        CString temp_ip;
        temp_ip.Format(_T("%d.%d.%d.%d"), m_bac_handle_Iam_data.at(i).ipaddress[0],
            m_bac_handle_Iam_data.at(i).ipaddress[1],
            m_bac_handle_Iam_data.at(i).ipaddress[2],
            m_bac_handle_Iam_data.at(i).ipaddress[3]);
        if ((IP_ADDRESS.CompareNoCase(temp_ip) == 0) && m_bac_handle_Iam_data.at(i).device_id != g_bac_instance)
        {
            //就说明下面挂载了 MSTP的设备;
            temp_vector.push_back(m_bac_handle_Iam_data.at(i));
        }
    }
    CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);

    //pFrame->m_product.at(selected_product_index).object_instance

    for (int i = 0; i < temp_vector.size(); i++)
    {
        bool find_device_instance = false;
        for (int j = 0; j < pFrame->m_product.size(); j++)
        {
            if (temp_vector.at(i).device_id == pFrame->m_product.at(j).object_instance)
            {
                find_device_instance = true;
                break;
            }
        }
        if (find_device_instance)
            continue;

        unsigned short test_array[1000];
        int nret = 0;
        nret = GetPrivateBacnetToModbusData(temp_vector.at(i).device_id, 0, 100, test_array);
        Sleep(1);
    }
    //IP_ADDRESS
    hbip_whois_thread = NULL;
    return 0;
}