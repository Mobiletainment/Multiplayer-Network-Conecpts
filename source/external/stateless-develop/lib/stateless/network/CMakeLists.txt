project(slNetwork)

file (GLOB Lib_Src "*.cpp")
file (GLOB Lib_Hdrs "*.h")

file (GLOB CocosNet_Src "cocosnet/*.cpp")
file (GLOB CocosNet_Hdrs "cocosnet/*.h")

file (GLOB ReplicaNet_Src "replica/*.cpp")
file (GLOB ReplicaNet_Hdrs "replica/*.h")

file (GLOB Actor_Src "actor/*.cpp")
file (GLOB Actor_Hdrs "actor/*.h")

INCLUDE_DIRECTORIES(
	${RAKNET_INCLUDES}
	${COCOS2DX_INCLUDES}
	${POCO_INCLUDES}
	"${STATELESS_INCLUDE_DIRECTORIES}"
	${NWL_DIRECTORY}
	"${CMAKE_CURRENT_SOURCE_DIR}"
)

LINK_DIRECTORIES(
	${COCOS2DX_LINK_DIRS}
	)

# add shared library target	
add_library(${PROJECT_NAME} SHARED 
	${Lib_Hdrs} ${Lib_Src} 
	${slDll_Hdrs} ${slDll_Src} 
	${CocosNet_Hdrs} ${CocosNet_Src} 
	${Actor_Hdrs} ${Actor_Src} 
	${ReplicaNet_Hdrs} ${ReplicaNet_Src}
)

# define source groups for project file
source_group(Sources FILES ${Lib_Hdrs} ${Lib_Src})
source_group(Sources\\platform\\win32 FILES ${slDll_Hdrs} ${slDll_Src})
source_group(Sources\\cocosnet FILES ${CocosNet_Hdrs} ${CocosNet_Src})
source_group(Sources\\replica FILES ${ReplicaNet_Hdrs} ${ReplicaNet_Src})
source_group(Sources\\actor FILES ${Actor_Hdrs} ${Actor_Src})

#
# add dependencies and compiler settings
#

# add Stateless dependencies
target_link_libraries(${PROJECT_NAME} slCore)

# add POCO dependencies
# target_link_libraries(${PROJECT_NAME} ${POCO_PROJECTS})
add_pocouser_compiler_flags()
##add_definitions( -D_UNICODE)

# add RakNet dependencies
target_link_libraries(${PROJECT_NAME} RakNetDLL)

# add cocos2d-x preprocessor macros
add_cocos2dx_user_compiler_flags()

# add cocos2d-x dependencies
target_link_libraries(${PROJECT_NAME} cocos2dx)

# slApplication slCocosUtil slFramework
target_link_libraries(${PROJECT_NAME} ${SL_SHARED_LIBRARIES})
message(STATUS "linking with SL_SHARED_LIBRARIES: ${SL_SHARED_LIBRARIES}")


# using precompiled headers
add_precompiledheader_flags(${PROJECT_NAME})

#
# add build type postfixes
# 
SET_TARGETTYPEPROPERTIES(${PROJECT_NAME})

# determine the solution folder
set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "external/stateless/lib" )

