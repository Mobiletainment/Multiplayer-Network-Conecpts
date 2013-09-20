project(EduNetMemory)

INCLUDE_DIRECTORIES( ${EDUNET_GAMES_SOURCE_DIR}/Source/src )

IF (ET_USE_MMGR_FLUID)
	message(STATUS "Use Fluid memory manager")

	SET( EduNetMemoryFuildSrc
	Fluid/mmgr.cpp
	)
	SET( EduNetMemoryFuildHdr
	Fluid/mmgr.h
	Fluid/nommgr.h
	)
ENDIF(ET_USE_MMGR_FLUID)

SET( EduNetMemorySrc
EduNetMemory.cpp
)
SET( EduNetMemoryHdr
EduNetMemory.h
)

#set compiler options to link the lib into a shared lib
IF (NOT WIN32)
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
ENDIF (NOT WIN32)

add_library(${PROJECT_NAME} STATIC
	${EduNetMemoryHdr} ${EduNetMemorySrc} ${EduNetMemoryFuildSrc} ${EduNetMemoryFuildHdr})

source_group( source FILES ${EduNetMemoryHdr} ${EduNetMemorySrc} )
source_group( fluid FILES ${EduNetMemoryFuildSrc} ${EduNetMemoryFuildHdr} )

set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "edunet/libraries" )

set_target_properties( ${PROJECT_NAME} PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY ${EDUNET_COMMON_LIB_OUTPUT_DIRECTORY} )
	
set_target_properties( ${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "_Debug" )
set_target_properties( ${PROJECT_NAME} PROPERTIES RELEASE_POSTFIX "_Release" )
set_target_properties( ${PROJECT_NAME} PROPERTIES MINSIZEREL_POSTFIX "_MinsizeRel" )
set_target_properties( ${PROJECT_NAME} PROPERTIES RELWITHDEBINFO_POSTFIX "_RelWithDebugInfo" )
	