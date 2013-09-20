
project(OpenSteerDemo)

IF(WIN32)
	file (GLOB App_Resources "${OPENSTEER_SOURCE_DIR}/win32/*.*")
ENDIF(WIN32)


file (GLOB App_Src "${OPENSTEER_SOURCE_DIR}/demo/*.cpp")
file (GLOB App_Hdrs "${OPENSTEER_SOURCE_DIR}/demo/*.h")

file (GLOB App_PrecompiledSrc "${OPENSTEER_SOURCE_DIR}/demo/stdafx.cpp")

file (GLOB Opensteer_Src "${OPENSTEER_SOURCE_DIR}/src/*.cpp")
file (GLOB Opensteer_Hdrs "${OPENSTEER_SOURCE_DIR}/include/OpenSteer/*.h")

file (GLOB OpensteerPlugin_Src "${OPENSTEER_SOURCE_DIR}/plugins/*.cpp")
file (GLOB OpensteerPlugin_Hdrs "${OPENSTEER_SOURCE_DIR}/plugins/*.h")


include_directories(
	${BULLET_PHYSICS_INCLUDE_DIR}
	${OPENSTEER_INCLUDE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/demo
)

link_directories(
	${COCOS2DX_LINK_DIRS}
)

add_executable(${PROJECT_NAME} 
	${App_Hdrs} ${App_Src}
	${App_Resources}
	${Opensteer_Hdrs} ${Opensteer_Src}
	${OpensteerPlugin_Hdrs} ${OpensteerPlugin_Src}
)

ADD_DEFINITIONS (-DOPENSTEER_Z_ISUP=0)
ADD_DEFINITIONS (-DOS_HAVE_GLUT=1)

# add bullet dependencies
target_link_libraries(${PROJECT_NAME} LinearMath)

# determine the solution folder
set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "external/stateless/external/opensteer" )

# using precompiled headers
add_custom_precompiledheader_flags(${PROJECT_NAME} ${App_PrecompiledSrc})
#add_precompiledheader_flags(${PROJECT_NAME})

source_group(Resources FILES ${App_Resources})
source_group(Demo FILES ${App_Src} ${App_Hdrs})
source_group(Sources FILES ${Opensteer_Src} ${Opensteer_Hdrs})
source_group(Plugins FILES ${OpensteerPlugin_Src} ${OpensteerPlugin_Hdrs})


#
# add build type postfixes
# 
SET_TARGETTYPEPROPERTIES(${PROJECT_NAME})
