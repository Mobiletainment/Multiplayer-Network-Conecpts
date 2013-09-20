project(slCore)

file (GLOB_RECURSE Lib_Src "*.cpp")
file (GLOB_RECURSE Lib_Hdrs "*.h")

INCLUDE_DIRECTORIES(
	${STATELESS_CORE_INCLUDE_DIRECTORIES}
	"${CMAKE_CURRENT_SOURCE_DIR}"
	"${CMAKE_CURRENT_SOURCE_DIR}/universal/core"
)

source_group(Sources FILES ${Lib_Hdrs} ${Lib_Src})

add_library(${PROJECT_NAME} ${Lib_Hdrs} ${Lib_Src})

#
# add build type postfixes
# 
SET_TARGETTYPEPROPERTIES(${PROJECT_NAME})

set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "external/stateless/static_lib/core" )
