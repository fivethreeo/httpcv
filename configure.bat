
mkdir build
cd build

call ..\variables.bat

cmake -G "Visual Studio 14 2015 Win64"^
 -DVMIME_ROOT=%VMIME_ROOT%^
 -DOPENCV_DIR="%OPENCV_DIR%"^
 ..\

cd ..