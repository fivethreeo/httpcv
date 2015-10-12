HttpCV
======

A opencv/httpserver webapp.

Make sure you have `visual studio community`_, `cmake`_, `boost libs/headers`_, python_ and git installed.

Install Cygwin and select packages 'make', 'dos2unix' and 'binutils'. p7zip, md5sum, patch

set PATH=%PATH%;C:\Cygwin64\bin
dos2unix *
dos2unix -f configure
"C:\Program Files\Microsoft Visual Studio 14.0\VC\x86_64vcvars.bat"
bash runConfigureICU Cygwin/MSVC -prefix=/cygdrive/c/icu/dist -enable-static -disable-shared
make && make install

Open a cmd shell in this dir and run

.. code-block:: bash

    bootstrap.bat

    configure.bat

    build.bat

    run.bat

.. _`visual studio community`: https://www.visualstudio.com/
.. _`cmake`: https://cmake.org/
.. _`boost libs/headers`: http://sourceforge.net/projects/boost/files/boost-binaries/1.59.0/boost_1_59_0-msvc-14.0-64.exe/download
.. _python: https://www.python.org/downloads/release/python-278/