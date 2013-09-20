# a dynamic plugin library
autogenerateProjectName(${CMAKE_CURRENT_SOURCE_DIR} "nl" LibraryName)
project(${LibraryName})

file (GLOB Lib_Src "*.cpp")
file (GLOB Lib_Hdrs "*.h")

file (GLOB Network_Src "network/*.cpp")
file (GLOB Network_Hdrs "network/*.h")

file (GLOB Components_Src "components/*.cpp")
file (GLOB Components_Hdrs "components/*.h")

INCLUDE_DIRECTORIES(
	${RAKNET_INCLUDES}
	${COCOS2DX_INCLUDES}
	${POCO_INCLUDES}
	${nlDllPlugin_INCLUDE_PATH}
	${nlStaticLibUser_INCLUDE_PATH}
	"${CMAKE_CURRENT_SOURCE_DIR}"
)

LINK_DIRECTORIES(
	${COCOS2DX_LINK_DIRS}
	)

add_library(${PROJECT_NAME} SHARED 
	${Lib_Hdrs} ${Lib_Src} 
	${Network_Hdrs} ${Network_Src}
	${Components_Hdrs} ${Components_Src}
	${nlDll_Hdrs} ${nlDll_Src}
	${nlDllPlugin_Hdrs} ${nlDllPlugin_Src}
)

# add POCO dependencies
# target_link_libraries(${PROJECT_NAME} ${POCO_PROJECTS})
add_pocouser_compiler_flags()

# add cocos2d-x preprocessor macros
add_cocos2dx_user_compiler_flags()

# add cocos2d-x dependencies
# target_link_libraries(${PROJECT_NAME} cocos2dx)

# add stateless games shared library dependencies
# target_link_libraries(${PROJECT_NAME} ${SL_SHARED_LIBRARIES})

# add stateless games static game library dependencies
# target_link_libraries(${PROJECT_NAME} slSimulation)
# target_link_libraries(${PROJECT_NAME} slCocosgame)

# add the plugin base implementation dependencies
target_link_libraries(${PROJECT_NAME} nlPlugin)


# using precompiled headers
add_custom_precompiledheader_flags(${PROJECT_NAME} ${nlDllPluginPrecompile_Src})

# define source groups for better readability of generated project files
source_group(Sources FILES 
	${Lib_Hdrs} ${Lib_Src}
)

source_group(Sources\\platform\\win32 FILES 
	${nlDll_Hdrs} ${nlDll_Src}
)

source_group(Sources\\shared FILES 
	${nlDllPlugin_Hdrs} ${nlDllPlugin_Src}
)

source_group(Sources\\network FILES 
	${Network_Hdrs} ${Network_Src}
)

source_group(Sources\\components FILES 
	${Components_Hdrs} ${Components_Src}
)

#
# add build type postfixes
# 
SET_TARGETTYPEPROPERTIES(${PROJECT_NAME})

# locate the project inside the solution
set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "${NL_FRAMEWORK_NAME}/plugins" )

