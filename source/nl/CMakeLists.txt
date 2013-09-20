
file (GLOB nlCocos2d_Src "cocos2d/*.cpp")
file (GLOB nlCocos2d_Hdrs "cocos2d/*.h")

file (GLOB nlDll_Src "lib/core/platform/win32/*.cpp")
file (GLOB nlDll_Hdrs "lib/core/platform/win32/*.h")


## simply add directories
add_subdirectory(lib)
add_subdirectory(application)


## static libraries
set( nlStaticLib_INCLUDE_PATH 
	"${STATELESS_INCLUDE_DIRECTORIES}"
	"${CMAKE_CURRENT_SOURCE_DIR}/static_lib/shared"
)

set( nlStaticLibUser_INCLUDE_PATH 
	"${CMAKE_CURRENT_SOURCE_DIR}/static_lib"
)

add_subdirectory(static_lib/nl/plugin)

add_subdirectory(plugins)
