# Microsoft Developer Studio Project File - Name="TeleStation" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=TeleStation - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TeleStation.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TeleStation.mak" CFG="TeleStation - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TeleStation - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "TeleStation - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TeleStation - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x41d /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x41d /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "TeleStation - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x41d /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x41d /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winmm.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "TeleStation - Win32 Release"
# Name "TeleStation - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AnswerScriptDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\AnswerScriptView.cpp
# End Source File
# Begin Source File

SOURCE=.\BranchPropDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\EditCategoriesDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EnterCodeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FileGenDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LabelPropDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\NewPBEntryDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\NewProjectDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\NoPropDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PhoneBookDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\PhoneBookView.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayFilePropDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjectDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjectView.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjItemPropDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ScriptSimulatorDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectTransDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "TeleStation - Win32 Release"

!ELSEIF  "$(CFG)" == "TeleStation - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TeleStation.cpp
# End Source File
# Begin Source File

SOURCE=.\TeleStation.rc
# End Source File
# Begin Source File

SOURCE=.\TSDocument.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AnswerScriptDoc.h
# End Source File
# Begin Source File

SOURCE=.\AnswerScriptView.h
# End Source File
# Begin Source File

SOURCE=.\BranchPropDlg.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\EditCategoriesDlg.h
# End Source File
# Begin Source File

SOURCE=.\EnterCodeDlg.h
# End Source File
# Begin Source File

SOURCE=.\FileGenDlg.h
# End Source File
# Begin Source File

SOURCE=.\LabelPropDlg.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\NewPBEntryDlg.h
# End Source File
# Begin Source File

SOURCE=.\NewProjectDlg.h
# End Source File
# Begin Source File

SOURCE=.\NoPropDlg.h
# End Source File
# Begin Source File

SOURCE=.\PhoneBookDoc.h
# End Source File
# Begin Source File

SOURCE=.\PhoneBookView.h
# End Source File
# Begin Source File

SOURCE=.\PlayFilePropDlg.h
# End Source File
# Begin Source File

SOURCE=.\ProjectDoc.h
# End Source File
# Begin Source File

SOURCE=.\ProjectView.h
# End Source File
# Begin Source File

SOURCE=.\ProjItemPropDlg.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\ScriptSimulatorDlg.h
# End Source File
# Begin Source File

SOURCE=.\SelectTransDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TeleStation.h
# End Source File
# Begin Source File

SOURCE=.\TSDocument.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\AnswerScriptDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00005.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00006.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00007.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00008.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00009.bmp
# End Source File
# Begin Source File

SOURCE=.\res\happy_su.bmp
# End Source File
# Begin Source File

SOURCE=.\res\idr_answ.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_cphn.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_work.ico
# End Source File
# Begin Source File

SOURCE=.\res\mainfram.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Palette.bmp
# End Source File
# Begin Source File

SOURCE=.\res\script_a.bmp
# End Source File
# Begin Source File

SOURCE=.\res\script_l.bmp
# End Source File
# Begin Source File

SOURCE=.\res\script_p.bmp
# End Source File
# Begin Source File

SOURCE=.\res\script_r.bmp
# End Source File
# Begin Source File

SOURCE=.\res\script_t.bmp
# End Source File
# Begin Source File

SOURCE=.\res\simulati.bmp
# End Source File
# Begin Source File

SOURCE=.\res\TeleStation.ico
# End Source File
# Begin Source File

SOURCE=.\res\TeleStation.rc2
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
