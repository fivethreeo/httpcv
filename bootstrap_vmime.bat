@echo off
call variables.bat %~d0

git clone http://github.com/kisli/vmime.git
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



