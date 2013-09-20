cmake_minimum_required(VERSION 2.8)

#
# Organize projects into folders
# 
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#
# declare root project
# 
project(NetWorkLessons)

#
# cocos2dx add root options
# 
option(COCOS2DX_LINK_ICONV      "Link iconv.dll to cocos2d-x" OFF)
option(COCOS2DX_LINK_LIBCURL    "Link libcurl.dll to cocos2d-x" OFF)
option(COCOS2DX_LINK_LIBJPEG    "Link libjpeg.lib to cocos2d-x" OFF)
option(COCOS2DX_LINK_LIBTIFF    "Link libtiff.dll to cocos2d-x" OFF)
option(COCOS2DX_LINK_PTHREAD    "Link pthreadVCE2.dll to cocos2d-x" OFF)
option(COCOS2DX_LINK_SQLITE     "Link sqlite3.dll to cocos2d-x" OFF)
option(COCOS2DX_LINK_WEBP       "Link webp image decoding to cocos2d-x" OFF)
option(COCOS2DX_LINK_WEBSOCKETS "Link websockets.dll to cocos2d-x" OFF)
option(COCOS2DX_LINK_ZLIB       "Link zlib1.dll to cocos2d-x" ON)
option(COCOS2DX_LINK_AUDIO      "Link audio libs to cocos2d-x" OFF)

#
# raknet add root options
# 

set(GLOBAL_CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules")

include("${GLOBAL_CMAKE_MODULE_PATH}/slGlobalMacros.cmake")

#
# set project version
# 
set(NetWorkLessons_Version 0.05)
#SET(CMake_VERSION_MAJOR 0)
#SET(CMake_VERSION_MINOR 5)
#SET(CMake_VERSION_PATCH 0)

set(NL_FRAMEWORK_NAME nl)

#
# determine the output directories at a convenient place
# 

set(NL_BINARY_ROOT_DIR ${CMAKE_BINARY_DIR}/../distribute)
set(NL_BINARY_OUTPUT_DIR ${NL_BINARY_ROOT_DIR}/bin)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${NL_BINARY_ROOT_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${NL_BINARY_ROOT_DIR}/lib)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${NL_BINARY_OUTPUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${NL_BINARY_OUTPUT_DIR} )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${NL_BINARY_OUTPUT_DIR} )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${NL_BINARY_OUTPUT_DIR} )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${NL_BINARY_OUTPUT_DIR})


#
# add projects ... 
# 
add_subdirectory(source)
