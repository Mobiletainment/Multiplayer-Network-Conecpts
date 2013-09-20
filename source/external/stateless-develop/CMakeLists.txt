# stateless static and shared library projects

# stateless simulation depends on bullet
# define needed settings to use the sources
set(BULLET_PHYSICS_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/bullet)
set(BULLET_PHYSICS_INCLUDE_DIR ${BULLET_PHYSICS_SOURCE_DIR}/src)
set(BULLET_VERSION 3.0)

# stateless simulation depends on OpenSteer
# define needed settings to use the sources
set(OPENSTEER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/opensteer)
set(OPENSTEER_INCLUDE_DIR ${OPENSTEER_SOURCE_DIR}/include)


set(INSTALL_LIBS 0)

# this defines the basic LinearMath project
add_subdirectory(${BULLET_PHYSICS_SOURCE_DIR}/src/LinearMath)

# cmake target modification for NetworkLessons
SET_TARGETTYPEPROPERTIES(LinearMath)

# for our solution add LinearMath to externals ...
set_property( TARGET LinearMath PROPERTY FOLDER "external/stateless/external/bullet" )


set(SL_SUBDIRECTORY_NAME stateless)


# one core root include directory
set(STATELESS_CORE_INCLUDE_DIRECTORIES
	${CMAKE_CURRENT_SOURCE_DIR}/core
)

# some standard include directory
set(STATELESS_INCLUDE_DIRECTORIES
	${BULLET_PHYSICS_INCLUDE_DIR}
	${OPENSTEER_INCLUDE_DIR}
	${STATELESS_CORE_INCLUDE_DIRECTORIES}
	${CMAKE_CURRENT_SOURCE_DIR}/static_lib
	${CMAKE_CURRENT_SOURCE_DIR}/lib
	# note: this is actually a hack to include EduNetSources
	#       would be nice to remove this at a later point
	${CMAKE_CURRENT_SOURCE_DIR}/static_lib/stateless/simulation
)

# this defines the stateless core project
add_subdirectory(external/opensteer)


# this defines the stateless core project
add_subdirectory(core/${SL_SUBDIRECTORY_NAME})

# this defines the basic simulation project
add_subdirectory(static_lib/${SL_SUBDIRECTORY_NAME}/simulation)

# library projects
add_subdirectory(lib)

# other projects dependant on those projects
add_subdirectory(static_lib/${SL_SUBDIRECTORY_NAME}/cocosgame)



