project(nlLauncher)

file (GLOB App_Src "*.cpp")
file (GLOB App_Hdrs "*.h")

IF(WIN32)
	file (GLOB App_Resources "resources/*.*")
ENDIF(WIN32)

INCLUDE_DIRECTORIES(
	${RAKNET_INCLUDES}
	${COCOS2DX_INCLUDES}
	${POCO_INCLUDES}
	"${STATELESS_INCLUDE_DIRECTORIES}"
	${NWL_DIRECTORY}
)

LINK_DIRECTORIES(
	${COCOS2DX_LINK_DIRS}
)


add_executable(${PROJECT_NAME} WIN32  ${App_Hdrs} ${App_Src} ${App_Resources})

# add POCO dependencies
target_link_libraries(${PROJECT_NAME} ${POCO_PROJECTS})
add_pocouser_compiler_flags()
##add_definitions( -D_UNICODE)

target_link_libraries(${PROJECT_NAME} ${SL_SHARED_LIBRARIES})

# using precompiled headers
add_precompiledheader_flags(${PROJECT_NAME})

source_group(Resources FILES ${App_Resources})
source_group(Sources FILES ${App_Hdrs} ${App_Src})

#
# add build type postfixes
# 
SET_TARGETTYPEPROPERTIES(${PROJECT_NAME})

set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "${NL_FRAMEWORK_NAME}/application" )

