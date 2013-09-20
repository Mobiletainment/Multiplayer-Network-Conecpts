# a static library supporting top level game objects
# at a later point this might become a shared library

# automatic project name generation
autogenerateProjectName(${CMAKE_CURRENT_SOURCE_DIR} "sl" LibraryName)
project(${LibraryName})


file (GLOB Lib_Src "*.cpp")
file (GLOB Lib_Hdrs "*.h")

file (GLOB Actor_Src "actor/*.cpp")
file (GLOB Actor_Hdrs "actor/*.h")

file (GLOB Network_Src "network/*.cpp")
file (GLOB Network_Hdrs "network/*.h")

file (GLOB Vehicle_Src "vehicle/*.cpp")
file (GLOB Vehicle_Hdrs "vehicle/*.h")


include_directories(
	${RAKNET_INCLUDES}
	${COCOS2DX_INCLUDES}
	${POCO_INCLUDES}
	${STATELESS_INCLUDE_DIRECTORIES}
	${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(${PROJECT_NAME} 
	${Lib_Hdrs} ${Lib_Src}
	${Actor_Hdrs} ${Actor_Src}
	${Network_Hdrs} ${Network_Src}
	${Vehicle_Hdrs} ${Vehicle_Src}
)

# add dependencies
# target_link_libraries(${PROJECT_NAME} slCore)
# target_link_libraries(${PROJECT_NAME} ${SL_SHARED_LIBRARIES})

# we want to develop network games so we add this dependency too
# target_link_libraries(${PROJECT_NAME} slNetwork)
# target_link_libraries(${PROJECT_NAME} slSimulation)

# slApplication is dependant on poco
# we do not want poco to use the auto lib feature
add_pocouser_compiler_flags()

# add cocos2d-x preprocessor macros
add_cocos2dx_user_compiler_flags()

# using precompiled headers
add_precompiledheader_flags(${PROJECT_NAME})


# define source groups for project file
source_group(Sources FILES 
	${Lib_Hdrs} ${Lib_Src}
)

source_group(Sources\\actor FILES 
	${Actor_Hdrs} ${Actor_Src}
)

source_group(Sources\\network FILES 
	${Network_Hdrs} ${Network_Src}
)

source_group(Sources\\vehicle FILES 
	${Vehicle_Hdrs} ${Vehicle_Src}
)



#
# add build type postfixes
# 
SET_TARGETTYPEPROPERTIES(${PROJECT_NAME})

# determine the solution folder
set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "external/stateless/static_lib" )

