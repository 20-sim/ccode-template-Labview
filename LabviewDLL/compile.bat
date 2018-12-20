@echo off
IF NOT EXIST "%GENERATION_DIR%\*.vi" (
	echo "Copying Labview support files"
	xcopy /Y "%TEMPLATEDIR%\labview\*.*" "%GENERATION_DIR%\"
)
cd project
start /B cmd /C build.bat
cd ..