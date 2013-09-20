# a static library supporting 3D math
project(slSimulation)

file (GLOB Lib_Src "*.cpp")
file (GLOB Lib_Hdrs "*.h")

file (GLOB Opensteer_Src "${OPENSTEER_SOURCE_DIR}/src/*.cpp")
file (GLOB Opensteer_Hdrs "${OPENSTEER_SOURCE_DIR}/include/OpenSteer/*.h")

# file (GLOB OpensteerPlugin_Src "${OPENSTEER_SOURCE_DIR}/plugins/*.cpp")
# file (GLOB OpensteerPlugin_Hdrs "${OPENSTEER_SOURCE_DIR}/plugins/*.h")

file (GLOB EduNetMemory_Src "EduNetMemory/*.cpp")
file (GLOB EduNetMemory_Hdrs "EduNetMemory/*.h")

file (GLOB EduNetCore_Src "EduNetCore/*.cpp")
file (GLOB EduNetCore_Hdrs "EduNetCore/*.h")

file (GLOB OpensteerUtilities_Src "OpenSteerUtilities/*.cpp")
file (GLOB OpensteerUtilities_Hdrs "OpenSteerUtilities/*.h")

include_directories(
	${BULLET_PHYSICS_INCLUDE_DIR}
	${OPENSTEER_INCLUDE_DIR}
	${STATELESS_CORE_INCLUDE_DIRECTORIES}
	${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(${PROJECT_NAME} 
	${Lib_Hdrs} ${Lib_Src}
	${Opensteer_Hdrs} ${Opensteer_Src}
	${OpensteerPlugin_Hdrs} ${OpensteerPlugin_Src}
	${EduNetMemory_Hdrs} ${EduNetMemory_Src}
	${EduNetCore_Hdrs} ${EduNetCore_Src}
	${OpensteerUtilities_Hdrs} ${OpensteerUtilities_Src}
)

# add bullet dependencies
target_link_libraries(${PROJECT_NAME} LinearMath)

# target_link_libraries(${PROJECT_NAME} slCore)

# using precompiled headers
add_precompiledheader_flags(${PROJECT_NAME})


source_group(Sources FILES ${Lib_Hdrs} ${Lib_Src})
source_group(Sources\\opensteer FILES ${Opensteer_Hdrs} ${Opensteer_Src})
source_group(Sources\\opensteer\\plugins FILES ${OpensteerPlugin_Hdrs} ${OpensteerPlugin_Src})
source_group(Sources\\edunetmemory FILES ${EduNetMemory_Hdrs} ${EduNetMemory_Src})
source_group(Sources\\edunetcore FILES ${EduNetCore_Hdrs} ${EduNetCore_Src})
source_group(Sources\\opensteerutilities FILES ${OpensteerUtilities_Hdrs} ${OpensteerUtilities_Src})


#
# add build type postfixes
# 
SET_TARGETTYPEPROPERTIES(${PROJECT_NAME})

# determine the solution folder
set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "external/stateless/static_lib" )

