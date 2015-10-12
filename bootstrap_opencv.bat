
call variables.bat

git clone https://github.com/Itseez/opencv.git opencv_sources
mkdir opencv_build
cd opencv_build
cmake -G "Visual Studio 14 2015 Win64"^
 -DBUILD_SHARED_LIBS=OFF^
 ..\opencv_sources 
cmake --build . --config Release
cmake --build . --config Release --target INSTALL
cd ..