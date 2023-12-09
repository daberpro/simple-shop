##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            =web-tokoh
ConfigurationName      =Debug
WorkspaceConfiguration =Debug
WorkspacePath          =G:\c++\web-tokoh
ProjectPath            =G:\c++\web-tokoh\web-tokoh
IntermediateDirectory  =$(ConfigurationName)
OutDir                 =$(IntermediateDirectory)
CurrentFileName        =
CurrentFilePath        =
CurrentFileFullPath    =
User                   =Daber
Date                   =09/12/2023
CodeLitePath           ="C:\Program Files\CodeLite"
LinkerName             =link.exe /nologo
SharedObjectLinkerName =link.exe /nologo /DLL
ObjectSuffix           =.obj
DependSuffix           =
PreprocessSuffix       =.i
DebugSwitch            =/Zi 
IncludeSwitch          =/I
LibrarySwitch          = 
OutputSwitch           =/OUT:
LibraryPathSwitch      =/LIBPATH:
PreprocessorSwitch     =/D
SourceSwitch           =
OutputDirectory        =G:\c++\web-tokoh\$(WorkspaceConfiguration)
OutputFile             =..\$(WorkspaceConfiguration)\$(ProjectName).exe
Preprocessors          =
ObjectSwitch           =/Fo
ArchiveOutputSwitch    =/OUT:
PreprocessOnlySwitch   =/P /Fi:
ObjectsFileList        ="web-tokoh.txt"
MakeDirCommand         =mkdir -p
RcCmpOptions           = 
RcCompilerName         =rc.exe /nologo
LinkOptions            =  /DEBUG
PCHCreateFlags         =
PCHUseFlags            =
IncludePath            = $(IncludeSwitch)"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.37.32822\include" $(IncludeSwitch)"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.37.32822\ATLMFC\include" $(IncludeSwitch)"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\VS\include" $(IncludeSwitch)"C:\Program Files (x86)\Windows Kits\10\include\10.0.22621.0\ucrt" $(IncludeSwitch)"C:\Program Files (x86)\Windows Kits\10\\include\10.0.22621.0\\um" $(IncludeSwitch)"C:\Program Files (x86)\Windows Kits\10\\include\10.0.22621.0\\shared" $(IncludeSwitch)"C:\Program Files (x86)\Windows Kits\10\\include\10.0.22621.0\\winrt" $(IncludeSwitch)"C:\Program Files (x86)\Windows Kits\10\\include\10.0.22621.0\\cppwinrt" $(IncludeSwitch)"C:\Program Files (x86)\Windows Kits\NETFXSDK\4.8\include\um"  $(IncludeSwitch). $(IncludeSwitch)$(ProjectPath)\Dependencies\nlohmann $(IncludeSwitch)$(ProjectPath)\Dependencies\httplib\include $(IncludeSwitch)$(ProjectPath)\Dependencies\mysql-connector-c++-8.2.0-winx64\include 
RcIncludePath          = 
Libs                   = $(LibrarySwitch)libcrypto.lib $(LibrarySwitch)libssl.lib $(LibrarySwitch)mysqlcppconn-static.lib $(LibrarySwitch)mysqlcppconn.lib $(LibrarySwitch)mysqlcppconn8-static.lib $(LibrarySwitch)mysqlcppconn8.lib 
ArLibs                 =  "liblibcrypto.lib" "liblibssl.lib" "mysqlcppconn-static.lib" "mysqlcppconn.lib" "mysqlcppconn8-static.lib" "mysqlcppconn8.lib" 
LibPath                =$(LibraryPathSwitch)"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.37.32822\ATLMFC\lib\x64" $(LibraryPathSwitch)"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.37.32822\lib\x64" $(LibraryPathSwitch)"C:\Program Files (x86)\Windows Kits\NETFXSDK\4.8\lib\um\x64" $(LibraryPathSwitch)"C:\Program Files (x86)\Windows Kits\10\lib\10.0.22621.0\ucrt\x64" $(LibraryPathSwitch)"C:\Program Files (x86)\Windows Kits\10\\lib\10.0.22621.0\\um\x64"  $(LibraryPathSwitch). $(LibraryPathSwitch)$(ProjectPath)\Dependencies\mysql-connector-c++-8.2.0-winx64\lib64\vs14 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overridden using an environment variable
##
AR       = lib.exe /nologo
CXX      = cl.exe /nologo /TP /FC
CC       = cl.exe /nologo /TC /FC
CXXFLAGS =  /std:c++20 /EHsc /MT $(Preprocessors)
CFLAGS   =   $(Preprocessors)
ASFLAGS  = 
AS       = ml.exe /nologo


##
## User defined environment variables
##
CodeLiteDir=C:\Program Files\CodeLite
MYSQL="C:\Program Files\MySQL\MySQL Server 8.0\lib"
MYSQL_CONNECTOR="C:\Program Files\MySQL\MySQL Connector C++ 8.2\include\mysql"
MYSQL_CONNECTOR_LIB="C:\Program Files\MySQL\MySQL Connector C++ 8.2\lib64\vs14"
MYSQL_CONNECTOR_LIB_ADDITION="C:\Program Files\MySQL\MySQL Connector C++ 8.2\lib64"
MARIADB_INCLUDE="C:/Program Files/MariaDB/MariaDB C++ Connector 64-bit/include/mariadb"
MARIADB_LIB="C:/Program Files/MariaDB/MariaDB C++ Connector 64-bit"
MARIADB_LIB2="C:\Program Files\MariaDB\MariaDB C++ Connector 64-bit\plugin"
BOOST="C:\boost-c++\boost_1_83_0\build"
HTTPLIB="C:/Program Files (x86)/httplib/include/"
WIN10ADDITION="C:\Program Files (x86)\Windows Kits\10\Include\10.0.22621.0\shared"
WIN10LIB="C:\Program Files (x86)\Windows Kits\10\Lib\10.0.22621.0\um\x64"
WIN10HEADER="C:\Program Files (x86)\Windows Kits\10\Include\10.0.22621.0\um"


##
## Object Targets Lists 
##
Objects0=$(IntermediateDirectory)\main.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)\.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)\.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)
.PHONY: clean PrePreBuild PreBuild MakeIntermediateDirs all PostBuild 

MakeIntermediateDirs:
	@$(MakeDirCommand) "$(IntermediateDirectory)"
	@$(MakeDirCommand) "$(OutputDirectory)"


$(IntermediateDirectory)\.d:
	@echo Creating Intermediate Directory
	@$(MakeDirCommand) "$(IntermediateDirectory)"
	@echo Intermediate directories created
PreBuild:


##
## Objects
##
$(IntermediateDirectory)\main.cpp$(ObjectSuffix): main.cpp 
	$(CXX) $(PCHUseFlags) $(SourceSwitch) "G:\c++\web-tokoh\web-tokoh\main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)\main.cpp$(PreprocessSuffix): main.cpp
	$(CXX) $(CXXFLAGS) $(PCHUseFlags) $(IncludePath) $(PreprocessOnlySwitch) $(IntermediateDirectory)\main.cpp$(PreprocessSuffix) "main.cpp"

##
## Clean
##
clean:
	@if exist "$(IntermediateDirectory)" rmdir /S /Q "$(IntermediateDirectory)"


