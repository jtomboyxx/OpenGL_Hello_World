echo off

rem ========================================
rem === Local build environment setting  ===
rem ----------------------------------------
if "%NDKPATH%"=="" (
call cls

set WORKPATH=%~dp0%

call %WORKPATH%set_env.bat


)
rem ========================================


if exist %NDKPATH% goto FOUND

echo "ERROR!!! NDK PATH NOT FOUND => %NDKPATH%"
exit 

:FOUND
rem ========================================
rem === Check build directory            ===
rem ----------------------------------------
if exist %WORKPATH%build goto START
mkdir %WORKPATH%build
rem ========================================

:START

rem ========================================
rem === Remove objs directory            ===
rem ----------------------------------------
call del /s /q .\obj
rem ========================================

rem ========================================
rem === Just clean object without build  ===
rem ----------------------------------------
if "%1"=="clean" goto DONE
rem ========================================

rem ========================================
rem === Run NDK build                    ===
rem ----------------------------------------
call %NDKPATH%\ndk-build NDK_DEBUG=0 -j 4 
rem ========================================

call copy /Y .\obj\local\armeabi-v7a\* %WORKPATH%build

:DONE