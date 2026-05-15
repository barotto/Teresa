@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by TERESA.HPJ. >"hlp\Teresa.hm"
echo. >>"hlp\Teresa.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\Teresa.hm"
C:\MSVS\Common\Tools\makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\Teresa.hm"
echo. >>"hlp\Teresa.hm"
echo // Prompts (IDP_*) >>"hlp\Teresa.hm"
C:\MSVS\Common\Tools\makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\Teresa.hm"
echo. >>"hlp\Teresa.hm"
echo // Resources (IDR_*) >>"hlp\Teresa.hm"
C:\MSVS\Common\Tools\makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\Teresa.hm"
echo. >>"hlp\Teresa.hm"
echo // Dialogs (IDD_*) >>"hlp\Teresa.hm"
C:\MSVS\Common\Tools\makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\Teresa.hm"
echo. >>"hlp\Teresa.hm"
echo // Frame Controls (IDW_*) >>"hlp\Teresa.hm"
C:\MSVS\Common\Tools\makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\Teresa.hm"
REM -- Make help for Project TERESA


echo Building Win32 Help files
start "" /wait "C:\Programmi\Microsoft Visual Studio\Common\Tools\hcw" /C /E /M "hlp\Teresa.hpj"
if errorlevel 1 goto :Error
if not exist "hlp\Teresa.hlp" goto :Error
if not exist "hlp\Teresa.cnt" goto :Error
echo.
if exist Debug\nul copy "hlp\Teresa.hlp" Debug
if exist Debug\nul copy "hlp\Teresa.cnt" Debug
if exist Release\nul copy "hlp\Teresa.hlp" Release
if exist Release\nul copy "hlp\Teresa.cnt" Release
echo.
goto :done

:Error
echo hlp\Teresa.hpj(1) : error: Problem encountered creating help file

:done
echo.
