project(ExtOpensteer)

INCLUDE_DIRECTORIES(${EDUNET_GAMES_SOURCE_DIR}/Source/opensteer/include
)

SET(OPENTSTEER_LIB_SRC
 AABBox.cpp
 Annotation.cpp
 GlobalData.cpp
 GlobalSelection.cpp
 Obstacle.cpp
 OldPathway.cpp
 Path.cpp
 Plugin.cpp
 PluginRegistry.cpp
 PolylineSegmentedPath.cpp
 PolylineSegmentedPathwaySegmentRadii.cpp
 PolylineSegmentedPathwaySingleRadius.cpp
 SegmentedPath.cpp
 SegmentedPathway.cpp
 SimpleVehicle.cpp
 TerrainRayTest.cpp
 Vec3.cpp
 Vec3Utilities.cpp
 Color.cpp
 lq.c
 Clock.cpp
 Camera.cpp
 Pathway.cpp
 Entity.cpp
 SimplePlayer.cpp
 Renderer.cpp
)

#SET(OPENTSTEER_LIB_HDRS
# ../include/OpenSteer/*.h
# )
file (GLOB OPENTSTEER_LIB_HDRS "${${PROJECT_NAME}_SOURCE_DIR}/../include/OpenSteer/*.h")


#set compiler options to link the lib into a shared lib
IF (NOT WIN32)
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
   SET(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
ENDIF (NOT WIN32)

add_library(${PROJECT_NAME} STATIC ${OPENTSTEER_LIB_HDRS} ${OPENTSTEER_LIB_SRC})

source_group( source FILES ${OPENTSTEER_LIB_HDRS} ${OPENTSTEER_LIB_SRC} )

set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "edunet/external/libraries" )

set_target_properties( ${PROJECT_NAME} PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY ${EDUNET_COMMON_LIB_OUTPUT_DIRECTORY} )

set_target_properties( ${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "_Debug" )
set_target_properties( ${PROJECT_NAME} PROPERTIES RELEASE_POSTFIX "_Release" )
set_target_properties( ${PROJECT_NAME} PROPERTIES MINSIZEREL_POSTFIX "_MinsizeRel" )
set_target_properties( ${PROJECT_NAME} PROPERTIES RELWITHDEBINFO_POSTFIX "_RelWithDebugInfo" )
	
	
# APPLE
if ( WIN32 )
   message(STATUS "${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR}")
   message(STATUS "${PROJECT_NAME} builds to ${LIBRARY_OUTPUT_PATH} ${CMAKE_CFG_INTDIR}")
else(UNIX)

endif()

