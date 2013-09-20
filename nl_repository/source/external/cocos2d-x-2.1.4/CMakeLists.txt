cmake_minimum_required(VERSION 2.8.10)

## this will add the project Box2D
add_subdirectory(external)

project(cocos2dx)

set(COCOS_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cocos2dx)
set(COCOS_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

INCLUDE_DIRECTORIES("${COCOS_SOURCE_DIR}"
					"${COCOS_SOURCE_DIR}/include" 
					"${COCOS_SOURCE_DIR}/kazmath/include"
					"${COCOS_ROOT_DIR}/extensions"
					"${COCOS_ROOT_DIR}/external/libwebsockets/win32/include"
					"${COCOS_ROOT_DIR}/external/libwebsockets/win32/include/win32helpers"
					"${COCOS_ROOT_DIR}/external/sqlite3/include"
					"${COCOS_ROOT_DIR}/CocosDenshion/include"
					"${COCOS_SOURCE_DIR}/platform/third_party/win32"
					"${BOX2D_INCLUDES}"
					)
						
file( GLOB COCOS2DX_SRCS 
	"${COCOS_SOURCE_DIR}/*.c*" 
	"${COCOS_SOURCE_DIR}/actions/*.c*"
	"${COCOS_SOURCE_DIR}/base_nodes/*.c*"		
	"${COCOS_SOURCE_DIR}/cocoa/*.c*"
	"${COCOS_SOURCE_DIR}/draw_nodes/*.c*"
	"${COCOS_SOURCE_DIR}/effects/*.c*"
	"${COCOS_SOURCE_DIR}/kazmath/src/*.c*"
	"${COCOS_SOURCE_DIR}/kazmath/src/GL/*.c*"
	"${COCOS_SOURCE_DIR}/keypad_dispatcher/*.c*"
	"${COCOS_SOURCE_DIR}/label_nodes/*.c*"
	"${COCOS_SOURCE_DIR}/layers_scenes_transitions_nodes/*.c*"
	"${COCOS_SOURCE_DIR}/menu_nodes/*.c*"
	"${COCOS_SOURCE_DIR}/misc_nodes/*.c*"
	"${COCOS_SOURCE_DIR}/particle_nodes/*.c*"
	"${COCOS_SOURCE_DIR}/platform/*.c*"
	"${COCOS_SOURCE_DIR}/script_support/*.c*"
	"${COCOS_SOURCE_DIR}/shaders/*.c*"
	"${COCOS_SOURCE_DIR}/sprite_nodes/*.c*"
	"${COCOS_SOURCE_DIR}/support/*.c*"
	"${COCOS_SOURCE_DIR}/support/component/*.c*"
	"${COCOS_SOURCE_DIR}/support/data_support/*.c*"
	"${COCOS_SOURCE_DIR}/support/image_support/*.c*"
	"${COCOS_SOURCE_DIR}/support/tinyxml2/*.c*"
	"${COCOS_SOURCE_DIR}/support/user_default/*.c*"
	"${COCOS_SOURCE_DIR}/support/zip_support/*.c*" #?todo
	"${COCOS_SOURCE_DIR}/text_input_node/*.c*"
	"${COCOS_SOURCE_DIR}/textures/*.c*"
	"${COCOS_SOURCE_DIR}/tileMap_parallax_nodes/*.c*"
	"${COCOS_SOURCE_DIR}/touch_dispatcher/*.c*"
)

file( GLOB COCOS2DX_HDRS
	"${COCOS_SOURCE_DIR}/*.h*" 
	"${COCOS_SOURCE_DIR}/include/*.h*" 
	"${COCOS_SOURCE_DIR}/actions/*.h*"
	"${COCOS_SOURCE_DIR}/base_nodes/*.h*"		
	"${COCOS_SOURCE_DIR}/cocoa/*.h*"
	"${COCOS_SOURCE_DIR}/draw_nodes/*.h*"
	"${COCOS_SOURCE_DIR}/effects/*.h*"
	"${COCOS_SOURCE_DIR}/kazmath/src/*.h*"
	"${COCOS_SOURCE_DIR}/kazmath/src/GL/*.h*"
	"${COCOS_SOURCE_DIR}/keypad_dispatcher/*.h*"
	"${COCOS_SOURCE_DIR}/label_nodes/*.h*"
	"${COCOS_SOURCE_DIR}/layers_scenes_transitions_nodes/*.h*"
	"${COCOS_SOURCE_DIR}/menu_nodes/*.h*"
	"${COCOS_SOURCE_DIR}/misc_nodes/*.h*"
	"${COCOS_SOURCE_DIR}/particle_nodes/*.h*"
	"${COCOS_SOURCE_DIR}/platform/*.h*"
	"${COCOS_SOURCE_DIR}/script_support/*.h*"
	"${COCOS_SOURCE_DIR}/shaders/*.h*"
	"${COCOS_SOURCE_DIR}/sprite_nodes/*.h*"
	"${COCOS_SOURCE_DIR}/support/*.h*"
	"${COCOS_SOURCE_DIR}/support/component/*.h*"
	"${COCOS_SOURCE_DIR}/support/data_support/*.h*"
	"${COCOS_SOURCE_DIR}/support/image_support/*.h*"
	"${COCOS_SOURCE_DIR}/support/tinyxml2/*.h*"
	"${COCOS_SOURCE_DIR}/support/user_default/*.h*"
	"${COCOS_SOURCE_DIR}/support/zip_support/*.h*" #?todo
	"${COCOS_SOURCE_DIR}/text_input_node/*.h*"
	"${COCOS_SOURCE_DIR}/textures/*.h*"
	"${COCOS_SOURCE_DIR}/tileMap_parallax_nodes/*.h*"
	"${COCOS_SOURCE_DIR}/touch_dispatcher/*.h*"
) 

file( GLOB_RECURSE COCOS_EXTENSION_SRCS 
	"${COCOS_ROOT_DIR}/extensions/AssetsManager/*.c*"
	"${COCOS_ROOT_DIR}/extensions/CCArmature/*.c*"
	"${COCOS_ROOT_DIR}/extensions/CCBReader/*.c*"
	"${COCOS_ROOT_DIR}/extensions/Components/*.c*"
	"${COCOS_ROOT_DIR}/extensions/GUI/*.c*"
	"${COCOS_ROOT_DIR}/extensions/LocalStorage/*.c*"
	"${COCOS_ROOT_DIR}/extensions/network/*.c*"
	"${COCOS_ROOT_DIR}/extensions/physics_nodes/*.c*"
	"${COCOS_ROOT_DIR}/extensions/proj.win32/*.c*"
)
				
IF(ANDROID)
ELSEIF(WIN32)
	set( PREBUILT_DIR "${COCOS_SOURCE_DIR}/platform/third_party/win32" )
	add_definitions (
		-DUNICODE 
		-D_UNICODE 
		-D_USRDLL 
		-DCOCOS2DXWIN32_EXPORTS 
		-DGL_GLEXT_PROTOTYPES 
		-DCC_ENABLE_BOX2D_INTEGRATION=1
	)

	# CocosDenshion
	add_definitions (-D_EXPORT_DLL_)
	
	
	IF(MSVC)
		#SET( CMAKE_DEBUG_POSTFIX "d" )
		add_definitions(-D_CRT_SECURE_NO_WARNINGS 
						-D_SCL_SECURE_NO_WARNINGS 
						-wd4251
						-wd4244
						-wd4334
						-wd4005)
    ENDIF(MSVC)

	if(NOT MSVC)
		set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS}" )
		set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive" )
	endif()
  
	INCLUDE_DIRECTORIES(
		${COCOS_SOURCE_DIR}/platform/win32 
		${COCOS_SOURCE_DIR}/platform/third_party/win32/curl 
		${COCOS_SOURCE_DIR}/platform/third_party/win32/iconv 
		${COCOS_SOURCE_DIR}/platform/third_party/win32/libjpeg 
		${COCOS_SOURCE_DIR}/platform/third_party/win32/libpng 
		${COCOS_SOURCE_DIR}/platform/third_party/win32/libtiff 
		${COCOS_SOURCE_DIR}/platform/third_party/win32/libwebp 
		${COCOS_SOURCE_DIR}/platform/third_party/win32/OGLES 
		${COCOS_SOURCE_DIR}/platform/third_party/win32/pthread 
		${COCOS_SOURCE_DIR}/platform/third_party/win32/zlib 
		${CMAKE_INSTALL_PREFIX}/include
	)
	
	LINK_DIRECTORIES(
		${COCOS_SOURCE_DIR}/platform/third_party/win32/libraries
		${COCOS_ROOT_DIR}/external/libwebsockets/win32/lib
		${COCOS_ROOT_DIR}/external/sqlite3/libraries/win32
	)

	set( EXTERNAL_LIBS 
		opengl32 
		glew32 
		libpng 		 
		ws2_32 
	)
	
	file( GLOB PLATFORM_SRCS 
		"${COCOS_SOURCE_DIR}/platform/win32/*.c*" 
	)

	file( GLOB COCOSDENSHION_SRCS 
		"${COCOS_ROOT_DIR}/CocosDenshion/win32/*.c*"
	) 
	
	add_library(cocos2dx SHARED 
		${COCOS2DX_SRCS} ${COCOS2DX_HDRS} 
		${COCOSDENSHION_SRCS} 
		${COCOS_EXTENSION_SRCS} 
		${PLATFORM_SRCS} 
	)
	
	source_group(Sources\\cocos2dx FILES ${COCOS2DX_SRCS} ${COCOS2DX_HDRS})
	source_group(Sources\\cocosdension FILES ${COCOSDENSHION_SRCS})
	source_group(Sources\\extensions FILES ${COCOS_EXTENSION_SRCS})
	source_group(Sources\\platform FILES ${PLATFORM_SRCS})

ENDIF()

add_cocos2dx_build_compiler_flags(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}  ${EXTERNAL_LIBS})
target_link_libraries(${PROJECT_NAME} Box2D)

SET_TARGETTYPEPROPERTIES(${PROJECT_NAME})

set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "external/cocos2d" )

#set_target_properties(glew PROPERTIES PUBLIC_HEADER "${API}")

set(BINDIR bin)
set(BINCOMPONENT runtime)
set(LIBDIR lib)
set(LIBCOMPONENT dev)
set(INCLUDEDIR include)
set(INCLUDECOMPONENT dev)

install(TARGETS cocos2dx
	RUNTIME DESTINATION ${BINDIR} COMPONENT ${BINCOMPONENT}
	LIBRARY DESTINATION ${LIBDIR} COMPONENT ${LIBCOMPONENT}
	ARCHIVE DESTINATION ${LIBDIR} COMPONENT ${LIBCOMPONENT})

#specific for cocos2dx
install(DIRECTORY ${COCOS_SOURCE_DIR}/
    DESTINATION ${INCLUDEDIR} COMPONENT ${INCLUDECOMPONENT}
    FILES_MATCHING
    PATTERN "*.h"
    PATTERN ".git*" EXCLUDE # we dont want a repo created now do we?
    PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
)
