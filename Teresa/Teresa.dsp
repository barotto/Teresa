# Microsoft Developer Studio Project File - Name="Teresa" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Teresa - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Teresa.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Teresa.mak" CFG="Teresa - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Teresa - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Teresa - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Teresa - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../trscore" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "TRS_AFX_USED" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x410 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x410 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 dockbars.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "Teresa - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../trscore" /D "_DEBUG" /D "TRS_AFX_USED" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x410 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x410 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 dockbars_dbg.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Teresa - Win32 Release"
# Name "Teresa - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\DescDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EquProp.cpp
# End Source File
# Begin Source File

SOURCE=.\Globals.cpp
# End Source File
# Begin Source File

SOURCE=.\HuetDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\InputBar.cpp
# End Source File
# Begin Source File

SOURCE=.\InputEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\OrderDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\OrderProbDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\OutputBar.cpp
# End Source File
# Begin Source File

SOURCE=.\OutputEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\SIDView.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TeoriaProp.cpp
# End Source File
# Begin Source File

SOURCE=.\Teresa.cpp
# End Source File
# Begin Source File

SOURCE=.\Teresa.rc
# End Source File
# Begin Source File

SOURCE=.\TeresaDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\TermsView.cpp
# End Source File
# Begin Source File

SOURCE=..\trscore\trs.cpp
# End Source File
# Begin Source File

SOURCE=..\trscore\trscriticalpairs.cpp
# End Source File
# Begin Source File

SOURCE=..\trscore\trsexception.cpp
# End Source File
# Begin Source File

SOURCE=..\trscore\trshuet.cpp
# End Source File
# Begin Source File

SOURCE=..\trscore\trslpoorder.cpp
# End Source File
# Begin Source File

SOURCE=..\trscore\trsparser.cpp
# End Source File
# Begin Source File

SOURCE=..\trscore\trsrewrite.cpp
# End Source File
# Begin Source File

SOURCE=..\trscore\trssapairs.cpp
# End Source File
# Begin Source File

SOURCE=..\trscore\trssubst.cpp
# End Source File
# Begin Source File

SOURCE=..\trscore\trsterm.cpp
# End Source File
# Begin Source File

SOURCE=..\trscore\trstermpair.cpp
# End Source File
# Begin Source File

SOURCE=..\trscore\trsunify.cpp
# End Source File
# Begin Source File

SOURCE=.\TRSView.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\DescDlg.h
# End Source File
# Begin Source File

SOURCE=.\EquProp.h
# End Source File
# Begin Source File

SOURCE=.\Globals.h
# End Source File
# Begin Source File

SOURCE=.\HuetDlg.h
# End Source File
# Begin Source File

SOURCE=.\InputBar.h
# End Source File
# Begin Source File

SOURCE=.\InputEdit.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\OrderDlg.h
# End Source File
# Begin Source File

SOURCE=.\OrderProbDlg.h
# End Source File
# Begin Source File

SOURCE=.\OutputBar.h
# End Source File
# Begin Source File

SOURCE=.\OutputEdit.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SIDView.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TeoriaProp.h
# End Source File
# Begin Source File

SOURCE=.\Teresa.h
# End Source File
# Begin Source File

SOURCE=.\TeresaDoc.h
# End Source File
# Begin Source File

SOURCE=.\TermsView.h
# End Source File
# Begin Source File

SOURCE=..\trscore\trs.h
# End Source File
# Begin Source File

SOURCE=.\TRSView.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\huetfunc.bmp
# End Source File
# Begin Source File

SOURCE=.\res\huetfunc1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\huetfunc2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\huetfunc3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\huetfunc4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\huetfunc5.bmp
# End Source File
# Begin Source File

SOURCE=.\res\info.txt
# End Source File
# Begin Source File

SOURCE=.\res\InputBar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\sigma.ico
# End Source File
# Begin Source File

SOURCE=.\res\start.ico
# End Source File
# Begin Source File

SOURCE=.\res\stop.ico
# End Source File
# Begin Source File

SOURCE=.\res\Teresa.ico
# End Source File
# Begin Source File

SOURCE=.\res\Teresa.rc2
# End Source File
# Begin Source File

SOURCE=.\res\TeresaDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=..\COPYING.txt
# End Source File
# End Target
# End Project
