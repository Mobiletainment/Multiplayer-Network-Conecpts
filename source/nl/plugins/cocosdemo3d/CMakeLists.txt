# automatic project name generation
# extract the last component from CMAKE_CURRENT_SOURCE_DIR
string(REGEX MATCHALL "[a-zA-Z0-9]+\ |[a-zA-Z0-9]+$" PROJECT_DIR_NAME "${CMAKE_CURRENT_SOURCE_DIR}")
stringFirstCharUpper( ${PROJECT_DIR_NAME} U_PROJECT_DIR_NAME )

SET(PluginName "nl${U_PROJECT_DIR_NAME}")

message(STATUS "creating project for plugin: ${PluginName} -")

project(${PluginName})

file (GLOB App_Src "*.cpp")
file (GLOB App_Hdrs "*.h")

file (GLOB Dll_Src "Cocosx_Demo3D/proj.win32.dll/*.cpp")
file (GLOB Dll_Hdrs "Cocosx_Demo3D/proj.win32.dll/*.h")


	file( GLOB_RECURSE Demo_Src 
"Cocosx_Demo3D/Classes/*.c*"
	)

	file( GLOB_RECURSE Demo_Hdrs 
"Cocosx_Demo3D/Classes/*.h*"
	)

INCLUDE_DIRECTORIES(
	${RAKNET_INCLUDES}
	${COCOS2DX_INCLUDES}
	${POCO_INCLUDES}
	${NWL_DIRECTORY}
	${CMAKE_CURRENT_SOURCE_DIR}
	${nlStaticLibUser_INCLUDE_PATH}
)

LINK_DIRECTORIES(
	${COCOS2DX_LINK_DIRS}
	)

add_library(${PROJECT_NAME} SHARED ${App_Hdrs} ${App_Src} ${Dll_Src} ${Dll_Hdrs} ${Demo_Src} ${Demo_Hdrs})

# add POCO dependencies
# target_link_libraries(${PROJECT_NAME} ${POCO_PROJECTS})
add_pocouser_compiler_flags()
##add_definitions( -D_UNICODE)

# add cocos2d-x preprocessor macros
add_cocos2dx_user_compiler_flags()

# add cocos2d-x dependencies
target_link_libraries(${PROJECT_NAME} cocos2dx)
target_link_libraries(${PROJECT_NAME} slFramework)

source_group(Sources FILES ${App_Hdrs} ${App_Src} ${Dll_Src} ${Dll_Hdrs} ${Demo_Src} ${Demo_Hdrs})

#
# add build type postfixes
# 
SET_TARGETTYPEPROPERTIES(${PROJECT_NAME})

set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "${NL_FRAMEWORK_NAME}/plugins/cocos2d" )

