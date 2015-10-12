
project (httpcv)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -Wall")
cmake_minimum_required (VERSION 2.8)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")


set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    ON)

#Only tested with versions 1.55 and 1.56
find_package(Boost 1.59.0 COMPONENTS regex system thread chrono coroutine context filesystem date_time REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

find_package(OPENCV REQUIRED)

find_package(Vmime REQUIRED)
include_directories(${VMIME_INCLUDE_DIR})

if(MSVC)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    foreach(flag_var
            CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
            CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO
            CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
            CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
      if(${flag_var} MATCHES "/MD")
        string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
      endif()
      if(${flag_var} MATCHES "/MDd")
        string(REGEX REPLACE "/MDd" "/MTd" ${flag_var} "${${flag_var}}")
      endif()
    endforeach(flag_var)
endif()

include_directories(.)
add_executable(greenshift greenshift.cpp)
target_link_libraries(greenshift ${Boost_LIBRARIES})
target_link_libraries(greenshift ${VMIME_LIBRARIES})
target_link_libraries(greenshift ${OpenCV_LIBS} )
target_link_libraries(greenshift ${CMAKE_THREAD_LIBS_INIT})

set_target_properties(greenshift PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
set_target_properties(greenshift PROPERTIES COMPILE_DEFINITIONS_DEBUG "_WINDOWS")
set_target_properties(greenshift PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
set_target_properties(greenshift PROPERTIES COMPILE_DEFINITIONS_RELEASE "_WINDOWS")
