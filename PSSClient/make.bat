@echo off

GOTO :build

:build

call ndk-build 
call adb push libs/armeabi-v7a/test-libstl /sdcard/ 
call adb shell cp /sdcard/test-libstl /data/local
call adb shell chmod +x /data/local/test-libstl
GOTO :end

:clean
IF EXIST build RMDIR /S /Q build
IF EXIST lua.exe DEL /F /Q lua.exe
GOTO :end

:end