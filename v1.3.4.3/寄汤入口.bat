@echo off
::color 17
if exist "%SystemRoot%\SysWOW64" path %path%;%windir%\SysNative;%SystemRoot%\SysWOW64;%~dp0
bcdedit >nul
if '%errorlevel%' NEQ '0' (goto UACPrompt) else (goto UACAdmin)
:UACPrompt
%1 start "" mshta vbscript:createobject("shell.application").shellexecute("""%~0""","::",,"runas",1)(window.close)&exit
exit /B
:UACAdmin
title �ڲ�ר�á��������⴫
cd /d "%~dp0"
echo ��ǰ����·���ǣ�%CD%
echo �ѻ�ȡ����ԱȨ��
echo 		������������
echo 		�������
Kdmapper.exe Driver.sys
echo ���������������
pause
cd program
del /f /s /q overlay_ap.exe
cls
echo .
echo .
echo 	��ѡ��������Ϸ�ڷֱ���
echo		1	1920 x 1080	��Ĭ�ϣ�
echo		2	2560 x 1440
echo ������ֱ��ʵ����
set /p ID=

if ��%id%��==��1�� (
	goto 1920x1080
)else if ��%id%��==��2�� (
	goto 2560x1440
)else goto 1920x1080

:1920x1080
echo 1920x1080
set "dir=%CD%\�ֱ���\1920x1080.exe"
set "goal=%CD%\"
if not exist "%dir%" (
	echo �ļ������ڣ������ļ������Ժ�����
	exit
)
copy "%dir%" "%goal%"
goto cheat

:2560x1440
echo 2560x1440
set "dir=%CD%\�ֱ���\2560x1440.exe��
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