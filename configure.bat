call variables.bat
python waf configure^
  --boost-includes=%BOOST_ROOT%^
  --boost-libs=%BOOST_LIBRARYDIR%^
  --boost-mt^
  --opencv-lib=%CD%\opencv_build\install\x64\vc14\staticlib^
  --opencv-include=%CD%\opencv_build\install\include^
  --opencv-ver=300^
  %*
