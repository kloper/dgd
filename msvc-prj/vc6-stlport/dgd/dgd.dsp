# Microsoft Developer Studio Project File - Name="dgd" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=dgd - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "dgd.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "dgd.mak" CFG="dgd - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "dgd - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "dgd - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "dgd - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MTd /W3 /GR /GX /O2 /I "../../../../STLPort-4.5/stlport" /I "../../../../boost" /I "../../../getopt/src" /I "../../../regex/src" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_DG_CPP_FILE_" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "dgd - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GR /GX /ZI /Od /I "../../../../STLPort-4.5/stlport" /I "../../../../boost" /I "../../../getopt/src" /I "../../../regex/src" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_DG_CPP_FILE_" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "dgd - Win32 Release"
# Name "dgd - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\dgd\src\dgChannel.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\dgd\src\dgChannelBuf.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\dgd\src\dgDebug.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\dgd\src\dgDebugOpt.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\dgd\src\dgFunnel.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\dgd\src\dgMultifileLog.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\dgd\src\dgOptionFilter.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\dgd\src\dgChannel.h
# End Source File
# Begin Source File

SOURCE=..\..\..\dgd\src\dgChannelBuf.h
# End Source File
# Begin Source File

SOURCE=..\..\..\dgd\src\dgChannelManip.h
# End Source File
# Begin Source File

SOURCE=..\..\..\dgd\src\dgConfig.h
# End Source File
# Begin Source File

SOURCE=..\..\..\dgd\src\dgDebug.h
# End Source File
# Begin Source File

SOURCE=..\..\..\dgd\src\dgDebugExtra.h
# End Source File
# Begin Source File

SOURCE=..\..\..\dgd\src\dgDebugOpt.h
# End Source File
# Begin Source File

SOURCE=..\..\..\dgd\src\dgDebugStd.h
# End Source File
# Begin Source File

SOURCE=..\..\..\dgd\src\dgFunnel.h
# End Source File
# Begin Source File

SOURCE=..\..\..\dgd\src\dgMultifileLog.h
# End Source File
# Begin Source File

SOURCE=..\..\..\dgd\src\dgOptionFilter.h
# End Source File
# Begin Source File

SOURCE=..\..\..\dgd\src\dgStream.h
# End Source File
# End Group
# End Target
# End Project
