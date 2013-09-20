project(slCocosUtil)

file (GLOB Lib_Src "*.cpp")
file (GLOB Lib_Hdrs "*.h")

file (GLOB slCocosControls_Src "controls/*.cpp")
file (GLOB slCocosControls_Hdrs "controls/*.h")

file (GLOB slCocosUI_Src "ui/*.cpp")
file (GLOB slCocosUI_Hdrs "ui/*.h")

file( GLOB_RECURSE slCocosJSon_Src 
	"json/*.c*"
)
file( GLOB_RECURSE slCocosJSon_Hdrs 
	"json/*.h*"
)

file( GLOB_RECURSE slCocosComponents_Src 
	"components/*.c*"
)
file( GLOB_RECURSE slCocosComponents_Hdrs 
	"components/*.h*"
)

file( GLOB_RECURSE slCocosLogic_Src 
	"logic/*.c*"
)
file( GLOB_RECURSE slCocosLogic_Hdrs 
	"logic/*.h*"
)

file( GLOB_RECURSE slCocosObjectstore_Src 
	"objectstore/*.c*"
)
file( GLOB_RECURSE slCocosObjectstore_Hdrs 
	"objectstore/*.h*"
)

file (GLOB slActor_Src "actor/*.cpp")
file (GLOB slActor_Hdrs "actor/*.h")


INCLUDE_DIRECTORIES(
	${RAKNET_INCLUDES}
	${COCOS2DX_INCLUDES}
	${POCO_INCLUDES}
	"${STATELESS_INCLUDE_DIRECTORIES}"
	"${CMAKE_CURRENT_SOURCE_DIR}"
)

LINK_DIRECTORIES(
	${COCOS2DX_LINK_DIRS}
	)
		
add_library(${PROJECT_NAME} SHARED 
	${Lib_Hdrs} ${Lib_Src} 
	${slDll_Hdrs} ${slDll_Src} 
	${slCocos2d_Hdrs} ${slCocos2d_Src}
	${slActor_Src} ${slActor_Hdrs}
	${slCocosControls_Src} ${slCocosControls_Hdrs}
	${slCocosUI_Src} ${slCocosUI_Hdrs}
	${slCocosJSon_Src} ${slCocosJSon_Hdrs}
	${slCocosComponents_Src} ${slCocosComponents_Hdrs}
	${slCocosLogic_Src} ${slCocosLogic_Hdrs}
	${slCocosObjectstore_Src} ${slCocosObjectstore_Hdrs}
)

# add POCO dependencies
# target_link_libraries(${PROJECT_NAME} ${POCO_PROJECTS})
add_pocouser_compiler_flags()
##add_definitions( -D_UNICODE)

# add cocos2d-x preprocessor macros
add_cocos2dx_user_compiler_flags()

# add cocos2d-x dependencies
target_link_libraries(${PROJECT_NAME} cocos2dx)
target_link_libraries(${PROJECT_NAME} slApplication)


# using precompiled headers
add_precompiledheader_flags(${PROJECT_NAME})

if (MSVC)
	set_source_files_properties(cJSON.c PROPERTIES COMPILE_FLAGS "/Ycstdafx.h")
endif()

source_group(Sources FILES ${Lib_Hdrs} ${Lib_Src})
source_group(Sources\\actor FILES ${slActor_Src} ${slActor_Hdrs})
source_group(Sources\\cocos2d FILES ${slCocos2d_Hdrs} ${slCocos2d_Src})
source_group(Sources\\platform\\win32 FILES ${slDll_Hdrs} ${slDll_Src})
source_group(Sources\\controls FILES ${slCocosControls_Hdrs} ${slCocosControls_Src})
source_group(Sources\\ui FILES ${slCocosUI_Hdrs} ${slCocosUI_Src})
source_group(Sources\\json FILES ${slCocosJSon_Hdrs} ${slCocosJSon_Src})
source_group(Sources\\components FILES ${slCocosComponents_Src} ${slCocosComponents_Hdrs})
source_group(Sources\\logic FILES ${slCocosLogic_Src} ${slCocosLogic_Hdrs})
source_group(Sources\\objectstore FILES ${slCocosObjectstore_Src} ${slCocosObjectstore_Hdrs})

#
# add build type postfixes
# 
SET_TARGETTYPEPROPERTIES(${PROJECT_NAME})

set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "external/stateless/lib" )

