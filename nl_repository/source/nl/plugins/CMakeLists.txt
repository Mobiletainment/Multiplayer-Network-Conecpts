

file (GLOB nlDllPluginPrecompile_Src "shared/stdafx.cpp")
file (GLOB nlDllPlugin_Src "shared/*.cpp")
file (GLOB nlDllPlugin_Hdrs "shared/*.h")

set( nlDllPlugin_INCLUDE_PATH 
	"${STATELESS_INCLUDE_DIRECTORIES}"
	"${CMAKE_CURRENT_SOURCE_DIR}/shared"
)

## simply add directories

## this sample will not work without a full blown coccos compilation
if(COCOS2DX_LINK_WEBSOCKETS)
	add_subdirectory(cocossamples)
endif()

add_subdirectory(cocosdemo3d)

add_subdirectory(chatsample)
add_subdirectory(quadlayersample)
add_subdirectory(opensteersample)
add_subdirectory(replicasample)
