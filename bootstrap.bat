python get_waf.py

call variables.bat

git clone https://github.com/cpp-netlib/cpp-netlib.git
cd cpp-netlib
git submodule init
git submodule update
cd ..
mkdir cpp-netlib-build
cd cpp-netlib-build
cmake ..\cpp-netlib -G "Visual Studio 14 2015 Win64"
cmake --build . --config Release
cd ..

git clone https://github.com/Itseez/opencv.git opencv_sources
mkdir opencv_build
cd opencv_build
cmake ..\opencv_sources -G "Visual Studio 14 2015 Win64" -DBUILD_SHARED_LIBS=OFF
cmake --build . --config Release
cmake --build . --config Release --target INSTALL
cd ..