@ECHO OFF
rem ----Usage----
rem build [clean|noclean]
rem vs2008 for compiling with visual studio 2008
rem clean to force a full rebuild
rem noclean to force a build without clean
rem noprompt to avoid all prompts
CLS
COLOR 1B
TITLE 20-sim DLL for Labview build script
rem -------------------------------------------------------------
rem Config
rem If you get an error that Visual studio was not found, SET your path for VSNET main executable.
rem -------------------------------------------------------------
rem	CONFIG START
SET CURPATH=%CD%
SET comp=vs2008
SET promptlevel=prompt
SET exitcode=0
SET buildmode=clean
SET DLL=lv20simmodel.dll
FOR %%b in (%1, %2, %3, %4, %5) DO (
	IF %%b==vs2008 SET comp=vs2008
	IF %%b==vs2010 SET comp=vs2010
	IF %%b==clean SET buildmode=clean
	IF %%b==noclean SET buildmode=noclean
	IF %%b==noprompt SET promptlevel=noprompt
)

SET buildconfig=Release

ECHO ------------------------------------------------------------
ECHO 20-sim DLL for Labview
ECHO ------------------------------------------------------------
ECHO Searching for Visual C++ compiler...

rem Search for VS 2008 or VS 2008 Express
set NET="%ProgramFiles%\Microsoft Visual Studio 9.0\Common7\IDE\VCExpress.exe"

IF %comp%==vs2008 (
	IF "%VS90COMNTOOLS%"=="" (
		set NET="%ProgramFiles%\Microsoft Visual Studio 9.0\Common7\IDE\VCExpress.exe"
		ECHO Found Visual C++ Express 2008
	) ELSE IF EXIST "%VS90COMNTOOLS%\..\IDE\VCExpress.exe" (
		set NET="%VS90COMNTOOLS%\..\IDE\VCExpress.exe"
		ECHO Found Visual C++ Express 2008
	) ELSE IF EXIST "%VS90COMNTOOLS%\..\IDE\devenv.exe" (
		set NET="%VS90COMNTOOLS%\..\IDE\devenv.exe"
		ECHO Found Visual C++ 2008
	)
	set PROJ_DIR=VS2008
)

rem Seach for VS 2010 / VS 2010 Express
IF %comp%==vs2010 (
	IF "%VS100COMNTOOLS%"=="" (
		set NET="%ProgramFiles%\Microsoft Visual Studio 10.0\Common7\IDE\VCExpress.exe"
		ECHO Found Visual C++ Express 2010
	) ELSE IF EXIST "%VS100COMNTOOLS%\..\IDE\VCExpress.exe" (
		set NET="%VS100COMNTOOLS%\..\IDE\VCExpress.exe"
		ECHO Found Visual C++ Express 2010
	) ELSE IF EXIST "%VS100COMNTOOLS%\..\IDE\devenv.exe" (
		set NET="%VS100COMNTOOLS%\..\IDE\devenv.exe"
		ECHO Found Visual C++ 2010
	)
	set PROJ_DIR=VS2010
)

IF NOT EXIST %NET% (
	set DIETEXT="Visual C++ 2008 (Express) was not found."
	goto DIE
)

set OPTS_DLL="%PROJ_DIR%\lv20simmodel.sln" /build "%buildconfig%"
set CLEAN_DLL="%PROJ_DIR%\lv20simmodel.sln" /clean "%buildconfig%"

rem ECHO %PROJ_DIR%
rem ECHO %OPTS_DLL%
rem ECHO %CLEAN_DLL%

ECHO ------------------------------------------------------------
rem	CONFIG END
rem -------------------------------------------------------------

:DLL_COMPILE
  IF EXIST %PROJ_DIR%\%buildconfig%\buildlog.html del %PROJ_DIR%\%buildconfig%\buildlog.html /q
  IF %buildmode%==clean goto COMPILE_DLL
  IF %buildmode%==noclean goto COMPILE_NO_CLEAN_DLL
  rem ---------------------------------------------
  rem	check for existing dll
  rem ---------------------------------------------
  
  IF EXIST ..\%DLL% (
    echo "Found existing DLL %DLL%"
    goto DLL_EXIST
  )
  goto COMPILE_DLL

:DLL_EXIST
  IF %promptlevel%==noprompt goto COMPILE_DLL
  ECHO Found a previous compiled WIN32 DLL.
  ECHO [1] a NEW DLL will be compiled.
  ECHO [2] existing DLL will be updated (quick mode compile).
  ECHO ------------------------------------------------------------
  set /P COMPILE_ANSWER=Compile a new EXE? [1/2]:
  if /I %COMPILE_ANSWER% EQU 1 goto COMPILE_DLL
  if /I %COMPILE_ANSWER% EQU 2 goto COMPILE_NO_CLEAN_DLL
  
:COMPILE_DLL
  ECHO Cleaning Solution...
  %NET% %CLEAN_DLL%
  ECHO Compiling 20-sim Labview DLL for submodel "%SUBMODEL_NAME%"
  %NET% %OPTS_DLL%
  IF NOT EXIST ..\%DLL% (
  	set DIETEXT="lv20simmodel.dll failed to build!  See ..\%PROJ_DIR%\%buildconfig%\BuildLog.htm for details."
  	goto DIE
  )
  ECHO Done.
  ECHO ------------------------------------------------------------
  set buildmode=clean
  GOTO MAKE_BUILD_DLL
  
:COMPILE_NO_CLEAN_DLL
  ECHO Compiling 20-sim Labview DLL for submodel "%SUBMODEL_NAME%"
  %NET% %OPTS_DLL%
  IF NOT EXIST ..\%DLL% (
  	set DIETEXT="lv20simmodel.dll failed to build!  See ..\%PROJ_DIR%\%buildconfig%\BuildLog.htm for details."
  	goto DIE
  )
  ECHO Done.
  ECHO ------------------------------------------------------------
  GOTO MAKE_BUILD_DLL

:MAKE_BUILD_DLL
  cd ..
  ECHO Your Labview DLL is ready and can be found in:
  ECHO   %CD%
  ECHO The name of your DLL is:
  ECHO   %DLL%
  ECHO See the Labview VI "test_20simModel.vi" for the usage.
  ECHO.
  TYPE inputs_and_outputs.txt
  ECHO ------------------------------------------------------------
  cd %CURPATH%
  GOTO END
  
:DIE
  set DIETEXT=Error: %DIETEXT%
  echo %DIETEXT%
  SET exitcode=1
  ECHO ------------------------------------------------------------

:END
  IF %promptlevel% NEQ noprompt (
  ECHO Press any key to exit...
  pause > NUL
  )
  EXIT /B %exitcode%
