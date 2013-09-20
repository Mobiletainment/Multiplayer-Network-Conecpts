
# automatic project name generation
# extract the last component from CMAKE_CURRENT_SOURCE_DIR
string(REGEX MATCHALL "[a-zA-Z0-9]+\ |[a-zA-Z0-9]+$" PROJECT_DIR_NAME "${CMAKE_CURRENT_SOURCE_DIR}")
stringFirstCharUpper( ${PROJECT_DIR_NAME} U_PROJECT_DIR_NAME )

SET(PluginName "nl${U_PROJECT_DIR_NAME}")

message(STATUS "creating project for plugin: ${PluginName}")

project(${PluginName})

file (GLOB Lib_Src "*.cpp")
file (GLOB Lib_Hdrs "*.h")

INCLUDE_DIRECTORIES(
	${RAKNET_INCLUDES}
	${COCOS2DX_INCLUDES}
	${POCO_INCLUDES}
	${nlStaticLib_INCLUDE_PATH}
)

LINK_DIRECTORIES(
	${COCOS2DX_LINK_DIRS}
	)

add_library(${PROJECT_NAME} 
	${Lib_Hdrs} ${Lib_Src} 
)

# add POCO dependencies
# target_link_libraries(${PROJECT_NAME} ${POCO_PROJECTS})
add_pocouser_compiler_flags()

# add cocos2d-x preprocessor macros
add_cocos2dx_user_compiler_flags()

# add cocos2d-x dependencies
target_link_libraries(${PROJECT_NAME} cocos2dx)

# add stateless dependencies
target_link_libraries(${PROJECT_NAME} ${SL_SHARED_LIBRARIES})

target_link_libraries(${PROJECT_NAME} slSimulation)
target_link_libraries(${PROJECT_NAME} slCocosgame)
target_link_libraries(${PROJECT_NAME} slNetwork)

# using precompiled headers
add_custom_precompiledheader_flags(${PROJECT_NAME} ${Lib_Src})

source_group(Sources FILES ${Lib_Hdrs} ${Lib_Src})

#
# add build type postfixes
# 
SET_TARGETTYPEPROPERTIES(${PROJECT_NAME})

set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "${NL_FRAMEWORK_NAME}/plugins/static_lib" )

