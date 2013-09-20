# automatic project name generation
# extract the last component from CMAKE_CURRENT_SOURCE_DIR
string(REGEX MATCHALL "[a-zA-Z0-9]+\ |[a-zA-Z0-9]+$" PROJECT_DIR_NAME "${CMAKE_CURRENT_SOURCE_DIR}")
stringFirstCharUpper( ${PROJECT_DIR_NAME} U_PROJECT_DIR_NAME )

SET(PluginName "nl${U_PROJECT_DIR_NAME}")

message(STATUS "creating project for plugin: ${PluginName} -")

project(${PluginName})

file (GLOB App_Src "*.cpp")
file (GLOB App_Hdrs "*.h")

	file( GLOB_RECURSE App_Src 
"TestCpp/*.c*"
	)

	file( GLOB_RECURSE App_Hdrs 
"TestCpp/*.h*"
	)

INCLUDE_DIRECTORIES(
	${RAKNET_INCLUDES}
	${COCOS2DX_INCLUDES}
	${POCO_INCLUDES}
	${NWL_DIRECTORY}
	${nlStaticLibUser_INCLUDE_PATH}
)

LINK_DIRECTORIES(
	${COCOS2DX_LINK_DIRS}
	)

add_library(${PROJECT_NAME} SHARED ${App_Hdrs} ${App_Src})

# add POCO dependencies
target_link_libraries(${PROJECT_NAME} ${POCO_PROJECTS})
add_pocouser_compiler_flags()
##add_definitions( -D_UNICODE)

# add cocos2d-x preprocessor macros
add_cocos2dx_user_compiler_flags()

#################### Compiler Flags #####################
# -MD    Minimal Rebuild
# -RTC1  Both basic runtime checks
# -MD[d] Multithreaded [debug] DLL
# -Zi    Program Database
# -ZI    Program Database for Edit & Continue
# -WX    General warning Level X
# -wdX   Disable specific warning X
# -wnX   Set warning level of specific warning X to level n

# warning C4251: 'xxx' : class 'yyy' needs to have dll-interface to be used by clients of class 'zzz'
ADD_DEFINITIONS("-wd4251")

# warning C4244: 'initializing' : conversion from 'float' to 'int', possible loss of data
ADD_DEFINITIONS("-wd4244")

# warning C4800: 'unsigned int' : forcing value to bool 'true' or 'false' (performance warning)
ADD_DEFINITIONS("-wd4800")

ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)

# add cocos2d-x dependencies
target_link_libraries(${PROJECT_NAME} cocos2dx)
target_link_libraries(${PROJECT_NAME} slFramework)


# TODO use macro
## ADD_MSVC_PRECOMPILED_HEADER("pch/stdafx.h" "pch/stdafx.cpp" ${App_Src})
if (MSVC)
#   set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "/Yustdafx.h")
#   set_source_files_properties(stdafx.cpp PROPERTIES COMPILE_FLAGS "/Ycstdafx.h")
endif(MSVC)

source_group(Sources FILES ${App_Hdrs} ${App_Src})

#
# add build type postfixes
# 
SET_TARGETTYPEPROPERTIES(${PROJECT_NAME})

set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "${NL_FRAMEWORK_NAME}/plugins/cocos2d" )

