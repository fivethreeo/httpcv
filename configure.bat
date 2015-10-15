@echo off

set "PREFIXDRIVE=%~d0"

set "BOOST_ROOT=%PREFIXDRIVE%\boost_1_59_0"
set "BOOST_LIBRARYDIR=%PREFIXDRIVE%\boost_1_59_0\lib64-msvc-14.0"
set "ICU_ROOT=%PREFIXDRIVE%\icu"
set "OPENCV_DIR=%PREFIXDRIVE%\opencv"
set "VMIME_ROOT=%PREFIXDRIVE%\vmime"

if "%1" equ "bootstrap" (
    
    git clone https://github.com/Itseez/opencv.git opencv_sources
    mkdir opencv_build
    cd opencv_build
    cmake -G "Visual Studio 14 2015 Win64"^
     -DBUILD_SHARED_LIBS=OFF^
     -DCMAKE_INSTALL_PREFIX=%OPENCV_DIR%^
     ..\opencv_sources
    cmake --build . --config Release --target INSTALL
    cd ..

    git clone http://github.com/kisli/vmime.git
    cd vmime
    git apply --reject --whitespace=fix ..\vmime_msvc_mt.diff
    cd ..
    mkdir vmime-build
    cd vmime-build
    cmake -G "Visual Studio 14 2015 Win64"^
     -DVMIME_BUILD_SHARED_LIBRARY=OFF^
     -DVMIME_BUILD_STATIC_LIBRARY=ON^
     -DVMIME_HAVE_SASL_SUPPORT=OFF^
     -DVMIME_HAVE_TLS_SUPPORT=OFF^
     -DVMIME_HAVE_MESSAGING_PROTO_SENDMAIL=OFF^
     -DVMIME_BUILD_SAMPLES=OFF^
     -DVMIME_CHARSETCONV_LIB=icu^
     -DICU_ROOT=%ICU_ROOT%^
     -DCMAKE_INSTALL_PREFIX=%VMIME_ROOT%^
     ..\vmime
    cmake --build . --config Release --target INSTALL
    cd ..
    
    git clone https://github.com/eidheim/Simple-Web-Server.git
    rem mkdir Simple-Web-Server-build
    rem cd Simple-Web-Server-build
    rem cmake -G "Visual Studio 14 2015 Win64" ..\Simple-Web-Server 
    rem cmake --build . --config Release
    rem cd ..
)


if "%1" neq "bootstrap" (
        
    mkdir build
    cd build
    cmake -G "Visual Studio 14 2015 Win64"^
     -DVMIME_ROOT=%VMIME_ROOT%^
     -DICU_ROOT=%ICU_ROOT%^
     -DOPENCV_DIR="%OPENCV_DIR%"^
     ..\
    
    cd ..
)