@echo off&color 17
if exist "%SystemRoot%\SysWOW64" path %path%;%windir%\SysNative;%SystemRoot%\SysWOW64;%~dp0
bcdedit >nul
if '%errorlevel%' NEQ '0' (goto UACPrompt) else (goto UACAdmin)
:UACPrompt
%1 start "" mshta vbscript:createobject("shell.application").shellexecute("""%~0""","::",,"runas",1)(window.close)&exit
exit /B
:UACAdmin
cd /d "%~dp0"
echo ��ǰ����·���ǣ�%CD%
echo �ѻ�ȡ����ԱȨ��
echo ������������
echo �������
Kdmapper.exe Driver.sys
echo ���������������
pause
cd program
ren *.exe overlay_ap.exe
taskkill /F /T /IM overlay_ap.exe
::start overlay_ap.exe
cd main
ren *.exe SIMPLE-APEX-CHEAT.exe
taskkill /F /T /IM NVIDIA Share.exe
start SIMPLE-APEX-CHEAT.exe
exit