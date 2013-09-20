project(slApplication)

file (GLOB Lib_Src "*.cpp")
file (GLOB Lib_Hdrs "*.h")

INCLUDE_DIRECTORIES(
##	${RAKNET_INCLUDES}
	${COCOS2DX_INCLUDES}
	${POCO_INCLUDES}
	"${STATELESS_INCLUDE_DIRECTORIES}"
)

LINK_DIRECTORIES(
	${COCOS2DX_LINK_DIRS}
	)

add_library(${PROJECT_NAME} SHARED 
	${Lib_Hdrs} ${Lib_Src} 
	${slDll_Hdrs} ${slDll_Src}
)

# add Stateless dependencies
target_link_libraries(${PROJECT_NAME} slCore)

# add POCO dependencies
target_link_libraries(${PROJECT_NAME} ${POCO_PROJECTS})
add_pocouser_compiler_flags()
##add_definitions( -D_UNICODE)

# add cocos2d-x preprocessor macros
add_cocos2dx_user_compiler_flags()
	
# add cocos2d-x dependencies
target_link_libraries(${PROJECT_NAME} cocos2dx)

# using precompiled headers
add_precompiledheader_flags(${PROJECT_NAME})

source_group(Sources FILES ${Lib_Hdrs} ${Lib_Src})
source_group(Sources\\platform\\win32 FILES ${slDll_Hdrs} ${slDll_Src})

#
# add build type postfixes
# 
SET_TARGETTYPEPROPERTIES(${PROJECT_NAME})

# determine the solution folder
set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "external/stateless/lib" )

