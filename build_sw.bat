
call variables.bat

git clone https://github.com/eidheim/Simple-Web-Server.git
mkdir Simple-Web-Server-build
cd Simple-Web-Server-build
cmake -G "Visual Studio 14 2015 Win64" ..\Simple-Web-Server 
cmake --build . --config Release
cd ..
