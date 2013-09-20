project(OpenSteerDemo)

INCLUDE_DIRECTORIES( ${EDUNET_GAMES_SOURCE_DIR}/Source/opensteer/include )
SET(opensteerDemoSrcs
Soccer.cpp
PedestriansWalkingAnEight.cpp
Pedestrian.cpp
OneTurning.cpp
MultiplePursuit.cpp
MapDrive.cpp
LowSpeedTurn.cpp
CaptureTheFlag.cpp
Boids.cpp
../src/main.cpp
../src/Draw.cpp
../src/OpenSteerDemo.cpp
)

IF(WIN32)
	SET(opensteerDemoSrcs ${opensteerDemoSrcs}
	../win32/resource.h
	../win32/SteerTest.rc
	../win32/OpenSteerDemo.ico)
ENDIF(WIN32)

if ( WIN32 )
	
	#SET(GLUT_glut_LIBRARY ${CMAKE_BINARY_DIR}/bin)
	#SET(GLUT_LIBRARIES glut32.lib)
else(UNIX)
     
endif()


LINK_LIBRARIES(
 ExtOpensteer  ${GLUT_glut_LIBRARY} 
${OPENGL_gl_LIBRARY} 
${OPENGL_glu_LIBRARY} ExtIProf
)

add_executable(${PROJECT_NAME} ${opensteerDemoSrcs})

set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "edunet/applications" )

set_target_properties( ${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "_Debug" )
set_target_properties( ${PROJECT_NAME} PROPERTIES RELEASE_POSTFIX "_Release" )
set_target_properties( ${PROJECT_NAME} PROPERTIES MINSIZEREL_POSTFIX "_MinsizeRel" )
set_target_properties( ${PROJECT_NAME} PROPERTIES RELWITHDEBINFO_POSTFIX "_RelWithDebugInfo" )
