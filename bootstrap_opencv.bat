@echo off
call variables.bat %~d0
git clone https://github.com/Itseez/opencv.git opencv_sources
mkdir opencv_build
cd opencv_build
cmake -G "Visual Studio 14 2015 Win64"^
 -DBUILD_SHARED_LIBS=OFF^
 -DCMAKE_INSTALL_PREFIX=%OPENCV_DIR%^
 ..\opencv_sources
cmake --build . --config Release --target INSTALL
cd ..