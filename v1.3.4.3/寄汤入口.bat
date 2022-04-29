@echo off
::color 17
if exist "%SystemRoot%\SysWOW64" path %path%;%windir%\SysNative;%SystemRoot%\SysWOW64;%~dp0
bcdedit >nul
if '%errorlevel%' NEQ '0' (goto UACPrompt) else (goto UACAdmin)
:UACPrompt
%1 start "" mshta vbscript:createobject("shell.application").shellexecute("""%~0""","::",,"runas",1)(window.close)&exit
exit /B
:UACAdmin
title 内部专用——不得外传
cd /d "%~dp0"
echo 当前运行路径是：%CD%
echo 已获取管理员权限
echo 		啊，哈哈哈哈
echo 		寄汤来喽
Kdmapper.exe Driver.sys
echo 按任意键启动程序
pause
cd program
del /f /s /q overlay_ap.exe
cls
echo .
echo .
echo 	请选择您的游戏内分辨率
echo		1	1920 x 1080	（默认）
echo		2	2560 x 1440
echo 请输入分辨率的序号
set /p ID=

if “%id%”==“1” (
	goto 1920x1080
)else if “%id%”==“2” (
	goto 2560x1440
)else goto 1920x1080

:1920x1080
echo 1920x1080
set "dir=%CD%\分辨率\1920x1080.exe"
set "goal=%CD%\"
if not exist "%dir%" (
	echo 文件不存在，请检查文件完整性后重试
	exit
)
copy "%dir%" "%goal%"
goto cheat

:2560x1440
echo 2560x1440
set "dir=%CD%\分辨率\2560x1440.exe”
set goal="%CD%"
if not exist "%dir%" (
	exit
)
copy "%dir%" "%goal%"
goto cheat
:cheat
ren *.exe overlay_ap.exe
taskkill /F /T /IM overlay_ap.exe
start overlay_ap.exe
cd main
ren *.exe SIMPLE-APEX-CHEAT.exe
taskkill /F /T /IM NVIDIA Share.exe
start SIMPLE-APEX-CHEAT.exe
exit