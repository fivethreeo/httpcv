@echo off
call variables.bat %~d0

git clone https://github.com/eidheim/Simple-Web-Server.git
rem mkdir Simple-Web-Server-build
rem cd Simple-Web-Server-build
rem cmake -G "Visual Studio 14 2015 Win64" ..\Simple-Web-Server 
rem cmake --build . --config Release
rem cd ..
