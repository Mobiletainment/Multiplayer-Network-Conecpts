#
# POCO Framework include directories
# 
set(POCO_INCLUDES 
	${CMAKE_CURRENT_SOURCE_DIR}/external/poco-develop/Foundation/include
	${CMAKE_CURRENT_SOURCE_DIR}/external/poco-develop/JSON/include
	${CMAKE_CURRENT_SOURCE_DIR}/external/poco-develop/Net/include
	${CMAKE_CURRENT_SOURCE_DIR}/external/poco-develop/Util/include
	${CMAKE_CURRENT_SOURCE_DIR}/external/poco-develop/XML/include
	${CMAKE_CURRENT_SOURCE_DIR}/external/poco-develop/Zip/include
)

#
# POCO Framework project names
# 
set(POCO_PROJECTS 
	PocoFoundation
	PocoJSON
	PocoNet
	PocoUtil
	PocoXML
	PocoZip
)

#
# BOX2D include directories
# 
set(BOX2D_INCLUDES 
	${CMAKE_CURRENT_SOURCE_DIR}/external/cocos2d-x-2.1.4/external
)

#
# chipmunk include directories
# 
set(CHIPMUNK_INCLUDES 
	${CMAKE_CURRENT_SOURCE_DIR}/external/cocos2d-x-2.1.4/external/chipmunk/include/chipmunk
)

#
# COCOS2DX directories
# 
set(COCOS2DX_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/cocos2d-x-2.1.4)
set(COCOS2DX_SOURCE_DIR ${COCOS2DX_DIR}/cocos2dx)
set(COCOS2DX_ROOT_DIR ${COCOS2DX_DIR})

set(COCOS2DX_INCLUDES 
	${COCOS2DX_SOURCE_DIR}
	${COCOS2DX_SOURCE_DIR}/include
	${COCOS2DX_ROOT_DIR}/CocosDenshion/include
	${COCOS2DX_SOURCE_DIR}/platform/win32
	${COCOS2DX_SOURCE_DIR}/kazmath/include
	${COCOS2DX_SOURCE_DIR}/platform/third_party/win32/curl 
	${COCOS2DX_SOURCE_DIR}/platform/third_party/win32/iconv 
	${COCOS2DX_SOURCE_DIR}/platform/third_party/win32/libjpeg 
	${COCOS2DX_SOURCE_DIR}/platform/third_party/win32/libpng 
	${COCOS2DX_SOURCE_DIR}/platform/third_party/win32/libtiff 
	${COCOS2DX_SOURCE_DIR}/platform/third_party/win32/libwebp 
	${COCOS2DX_SOURCE_DIR}/platform/third_party/win32/OGLES 
	${COCOS2DX_SOURCE_DIR}/platform/third_party/win32/pthread 
	${COCOS2DX_SOURCE_DIR}/platform/third_party/win32/zlib 
	${COCOS2DX_SOURCE_DIR}/platform/third_party/win32 
	${COCOS2DX_DIR}/extensions
	${COCOS2DX_DIR}/extensions/network
	${COCOS2DX_DIR}/external/libwebsockets/win32/include
	${BOX2D_INCLUDES}
	${CHIPMUNK_INCLUDES}
)

set(COCOS2DX_LINK_DIRS
	${COCOS2DX_SOURCE_DIR}/platform/third_party/win32/libraries
	${COCOS2DX_DIR}/external/libwebsockets/win32/lib
	${COCOS2DX_DIR}/external/sqlite3/libraries/win32
)

#
# RakNet directories
# 
set(RAKNET_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/RakNet_PC-4.08)
set(RAKNET_INCLUDES ${RAKNET_DIRECTORY}/Source)


#
# Network lesson directories
# 
set(NWL_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

set(NL_MACRO_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules)

## set a shared libraries variable for projects dependencies
set(SL_SHARED_LIBRARIES 
	slApplication slCocosUtil slFramework
)

## simply add directories
add_subdirectory(external)


#
# define macro include path 
# 
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules/")

include("${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules/nlMacros.cmake")

#
# Stateless include directories
# 

# stateless simulation depends on bullet
# define needed settings to use the sources
set(BULLET_PHYSICS_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/stateless-develop/external/bullet)
set(BULLET_PHYSICS_INCLUDE_DIR ${BULLET_PHYSICS_SOURCE_DIR}/src)
set(BULLET_VERSION 3.0)

# stateless simulation depends on OpenSteer
# define needed settings to use the sources
set(OPENSTEER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/stateless-develop/external/opensteer)
set(OPENSTEER_INCLUDE_DIR ${OPENSTEER_SOURCE_DIR}/include)

set(STATELESS_INCLUDE_DIRECTORIES
	${BULLET_PHYSICS_INCLUDE_DIR}
	${OPENSTEER_INCLUDE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/external/stateless-develop/core
	${CMAKE_CURRENT_SOURCE_DIR}/external/stateless-develop/static_lib
	${CMAKE_CURRENT_SOURCE_DIR}/external/stateless-develop/static_lib/stateless/simulation
	${CMAKE_CURRENT_SOURCE_DIR}/external/stateless-develop/lib
)

set(STATELESS_INCLUDES 
	${STATELESS_INCLUDE_DIRECTORIES}
)




# the network lessons subfolder
add_subdirectory(nl)
