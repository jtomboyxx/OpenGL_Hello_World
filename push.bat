rem ========================================
rem === root platform                    ===
rem ----------------------------------------
adb root
adb remount

rem ========================================
rem === Local environment setting        ===
rem ----------------------------------------
if "%WORKPATH%"=="" (
call cls
set WORKPATH=%~dp0%
)

rem ========================================
rem === push data                        ===
rem ----------------------------------------
adb push %WORKPATH%libs/armeabi-v7a/OpenGL_Hello_World /data
adb shell "chmod 777 /data/OpenGL_Hello_World"

REM pause