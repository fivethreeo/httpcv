rem @echo off
call variables.bat %~d0

mkdir build
cd build
cmake -G "Visual Studio 14 2015 Win64"^
 -DVMIME_ROOT=%VMIME_ROOT%^
 -DICU_ROOT=%ICU_ROOT%^
 -DOPENCV_DIR="%OPENCV_DIR%"^
 ..\

cd ..