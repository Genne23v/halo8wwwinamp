# Microsoft Developer Studio Project File - Name="wwwinamp" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=wwwinamp - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "wwwinamp.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "wwwinamp.mak" CFG="wwwinamp - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "wwwinamp - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "wwwinamp - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "wwwinamp - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib /nologo /subsystem:console /machine:I386
# Begin Special Build Tool
OutDir=.\Release
SOURCE="$(InputPath)"
PostBuild_Cmds=echo on	copy $(OutDir)\halo8.url "$(OutDir)\Halo 8 Productions.url"
# End Special Build Tool

!ELSEIF  "$(CFG)" == "wwwinamp - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MT /W3 /GX /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib th32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "wwwinamp - Win32 Release"
# Name "wwwinamp - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\MAIN.C

!IF  "$(CFG)" == "wwwinamp - Win32 Release"

# ADD CPP /W3

!ELSEIF  "$(CFG)" == "wwwinamp - Win32 Debug"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\FRONTEND.H
# End Source File
# Begin Source File

SOURCE=.\IPC_PE.H
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "static files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\controls.bg.gif

!IF  "$(CFG)" == "wwwinamp - Win32 Release"

# Begin Custom Build - copy $(InputPath)
OutDir=.\Release
InputPath=.\controls.bg.gif

"$(OutDir)/$(InputPath)" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "$(OutDir)/$(InputPath)"

# End Custom Build

!ELSEIF  "$(CFG)" == "wwwinamp - Win32 Debug"

# Begin Custom Build - copy $(InputPath)
OutDir=.\Debug
InputPath=.\controls.bg.gif

"$(OutDir)/$(InputPath)" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "$(OutDir)/$(InputPath)"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\controls.gif

!IF  "$(CFG)" == "wwwinamp - Win32 Release"

# Begin Custom Build - copy $(InputPath)
OutDir=.\Release
InputPath=.\controls.gif

"$(OutDir)/$(InputPath)" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "$(OutDir)/$(InputPath)"

# End Custom Build

!ELSEIF  "$(CFG)" == "wwwinamp - Win32 Debug"

# Begin Custom Build - copy $(InputPath)
OutDir=.\Debug
InputPath=.\controls.gif

"$(OutDir)/$(InputPath)" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "$(OutDir)/$(InputPath)"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\controls.mp3j.gif

!IF  "$(CFG)" == "wwwinamp - Win32 Release"

# Begin Custom Build - copy $(InputPath)
OutDir=.\Release
InputPath=.\controls.mp3j.gif

"$(OutDir)/$(InputPath)" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "$(OutDir)/$(InputPath)"

# End Custom Build

!ELSEIF  "$(CFG)" == "wwwinamp - Win32 Debug"

# Begin Custom Build - copy $(InputPath)
OutDir=.\Debug
InputPath=.\controls.mp3j.gif

"$(OutDir)/$(InputPath)" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "$(OutDir)/$(InputPath)"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\folder.gif

!IF  "$(CFG)" == "wwwinamp - Win32 Release"

# Begin Custom Build - copy $(InputPath)
OutDir=.\Release
InputPath=.\folder.gif

"$(OutDir)/$(InputPath)" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "$(OutDir)/$(InputPath)"

# End Custom Build

!ELSEIF  "$(CFG)" == "wwwinamp - Win32 Debug"

# Begin Custom Build - copy $(InputPath)
OutDir=.\Debug
InputPath=.\folder.gif

"$(OutDir)/$(InputPath)" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "$(OutDir)/$(InputPath)"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\halo8.ico

!IF  "$(CFG)" == "wwwinamp - Win32 Release"

# Begin Custom Build - copy $(InputPath)
OutDir=.\Release
InputPath=.\halo8.ico

"$(OutDir)/$(InputPath)" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "$(OutDir)/$(InputPath)"

# End Custom Build

!ELSEIF  "$(CFG)" == "wwwinamp - Win32 Debug"

# Begin Custom Build - copy $(InputPath)
OutDir=.\Debug
InputPath=.\halo8.ico

"$(OutDir)/$(InputPath)" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "$(OutDir)/$(InputPath)"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\halo8.url

!IF  "$(CFG)" == "wwwinamp - Win32 Release"

# Begin Custom Build - copy $(InputPath)
OutDir=.\Release
InputPath=.\halo8.url

"$(OutDir)/$(InputPath)" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "$(OutDir)/$(InputPath)"

# End Custom Build

!ELSEIF  "$(CFG)" == "wwwinamp - Win32 Debug"

# Begin Custom Build - copy $(InputPath)
OutDir=.\Debug
InputPath=.\halo8.url

"$(OutDir)/$(InputPath)" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "$(OutDir)/$(InputPath)"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\readme.txt

!IF  "$(CFG)" == "wwwinamp - Win32 Release"

# Begin Custom Build - copy $(InputPath)
OutDir=.\Release
InputPath=.\readme.txt

"$(OutDir)/$(InputPath)" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "$(OutDir)/$(InputPath)"

# End Custom Build

!ELSEIF  "$(CFG)" == "wwwinamp - Win32 Debug"

# Begin Custom Build - copy $(InputPath)
OutDir=.\Debug
InputPath=.\readme.txt

"$(OutDir)/$(InputPath)" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "$(OutDir)/$(InputPath)"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\readme_nullsoft.txt

!IF  "$(CFG)" == "wwwinamp - Win32 Release"

# Begin Custom Build - copy $(InputPath)
OutDir=.\Release
InputPath=.\readme_nullsoft.txt

"$(OutDir)/$(InputPath)" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "$(OutDir)/$(InputPath)"

# End Custom Build

!ELSEIF  "$(CFG)" == "wwwinamp - Win32 Debug"

# Begin Custom Build - copy $(InputPath)
OutDir=.\Debug
InputPath=.\readme_nullsoft.txt

"$(OutDir)/$(InputPath)" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "$(OutDir)/$(InputPath)"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\spacer.gif

!IF  "$(CFG)" == "wwwinamp - Win32 Release"

# Begin Custom Build - copy $(InputPath)
OutDir=.\Release
InputPath=.\spacer.gif

"$(OutDir)/$(InputPath)" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "$(OutDir)/$(InputPath)"

# End Custom Build

!ELSEIF  "$(CFG)" == "wwwinamp - Win32 Debug"

# Begin Custom Build - copy $(InputPath)
OutDir=.\Debug
InputPath=.\spacer.gif

"$(OutDir)/$(InputPath)" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "$(OutDir)/$(InputPath)"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\wwwinamp.css

!IF  "$(CFG)" == "wwwinamp - Win32 Release"

# Begin Custom Build - copy $(InputPath)
OutDir=.\Release
InputPath=.\wwwinamp.css

"$(OutDir)/$(InputPath)" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "$(OutDir)/$(InputPath)"

# End Custom Build

!ELSEIF  "$(CFG)" == "wwwinamp - Win32 Debug"

# Begin Custom Build - copy $(InputPath)
OutDir=.\Debug
InputPath=.\wwwinamp.css

"$(OutDir)/$(InputPath)" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "$(OutDir)/$(InputPath)"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\wwwinamp.ini

!IF  "$(CFG)" == "wwwinamp - Win32 Release"

# Begin Custom Build - copy $(InputPath)
OutDir=.\Release
InputPath=.\wwwinamp.ini

"$(OutDir)/$(InputPath)" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "$(OutDir)/$(InputPath)"

# End Custom Build

!ELSEIF  "$(CFG)" == "wwwinamp - Win32 Debug"

# Begin Custom Build - copy $(InputPath)
OutDir=.\Debug
InputPath=.\wwwinamp.ini

"$(OutDir)/$(InputPath)" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "$(OutDir)/$(InputPath)"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\wwwinamp.js

!IF  "$(CFG)" == "wwwinamp - Win32 Release"

# Begin Custom Build - copy $(InputPath)
OutDir=.\Release
InputPath=.\wwwinamp.js

"$(OutDir)/$(InputPath)" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "$(OutDir)/$(InputPath)"

# End Custom Build

!ELSEIF  "$(CFG)" == "wwwinamp - Win32 Debug"

# Begin Custom Build - copy $(InputPath)
OutDir=.\Debug
InputPath=.\wwwinamp.js

"$(OutDir)/$(InputPath)" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "$(OutDir)/$(InputPath)"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\wwwinamp.nsi

!IF  "$(CFG)" == "wwwinamp - Win32 Release"

# Begin Custom Build - copy $(InputPath)
OutDir=.\Release
InputPath=.\wwwinamp.nsi

"$(OutDir)/$(InputPath)" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "$(OutDir)/$(InputPath)"

# End Custom Build

!ELSEIF  "$(CFG)" == "wwwinamp - Win32 Debug"

# Begin Custom Build - copy $(InputPath)
OutDir=.\Debug
InputPath=.\wwwinamp.nsi

"$(OutDir)/$(InputPath)" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "$(OutDir)/$(InputPath)"

# End Custom Build

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
