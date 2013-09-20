set(LIBNAME "PocoZip")

aux_source_directory(src SRCS)

if (NOT POCO_STATIC)
  add_definitions(-DZip_EXPORTS)
endif (NOT POCO_STATIC)

add_library( ${LIBNAME} ${LIB_MODE} ${SRCS} )
set_target_properties( ${LIBNAME} 
	PROPERTIES
	VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION} )
target_link_libraries( ${LIBNAME} PocoUtil PocoXML PocoFoundation)

set_target_properties( ${LIBNAME} PROPERTIES DEBUG_POSTFIX "_Debug" )
set_target_properties( ${LIBNAME} PROPERTIES RELEASE_POSTFIX "_Release" )
set_target_properties( ${LIBNAME} PROPERTIES MINSIZEREL_POSTFIX "_MinsizeRel" )
set_target_properties( ${LIBNAME} PROPERTIES RELWITHDEBINFO_POSTFIX "_RelWithDebugInfo" )

set_property( TARGET ${LIBNAME} PROPERTY FOLDER "external/poco" )

install(
    DIRECTORY include/Poco
    DESTINATION include
    PATTERN ".svn" EXCLUDE
    )
    
install(
    TARGETS ${LIBNAME}
    DESTINATION lib
    )

if (ENABLE_TESTS)
  add_subdirectory(samples)
  add_subdirectory(testsuite)
endif ()

