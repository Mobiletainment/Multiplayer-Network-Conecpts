


macro(ADD_MSVC_PRECOMPILED_HEADER PrecompiledHeader PrecompiledSource SourcesVar)
  IF(MSVC)
    GET_FILENAME_COMPONENT(PrecompiledBasename ${PrecompiledHeader} NAME_WE)
    SET(PrecompiledBinary "$(IntDir)/${PrecompiledBasename}.pch")
    SET(Sources ${SourcesVar})

    SET_SOURCE_FILES_PROPERTIES(${PrecompiledSource}
                                PROPERTIES COMPILE_FLAGS "/Yc\"${PrecompiledHeader}\" /Fp\"${PrecompiledBinary}\""
                                           OBJECT_OUTPUTS "${PrecompiledBinary}")
    SET_SOURCE_FILES_PROPERTIES(${Sources}
                                PROPERTIES COMPILE_FLAGS "/Yu\"${PrecompiledHeader}\" /FI\"${PrecompiledHeader}\" /Fp\"${PrecompiledBinary}\""
                                           OBJECT_DEPENDS "${PrecompiledBinary}")  
    # Add precompiled header to SourcesVar
    LIST(APPEND ${SourcesVar} ${PrecompiledSource})
    LIST(APPEND ${SourcesVar} ${PrecompiledHeader})
  ENDIF(MSVC)
endmacro(ADD_MSVC_PRECOMPILED_HEADER PrecompiledHeader PrecompiledSource SourcesVar)


macro(add_pocouser_compiler_flags)
	add_definitions( -DPOCO_NO_AUTOMATIC_LIBS)
#	add_definitions( -DPOCO_WIN32_UTF8)
#	add_definitions( -DPOCO_NO_WSTRING)
#	add_definitions( -D_UNICODE)
endmacro(add_pocouser_compiler_flags)


macro(add_precompiledheader_flags projectname)
if (MSVC)
   set_target_properties(${projectname} PROPERTIES COMPILE_FLAGS "/Yustdafx.h")
   set_source_files_properties(stdafx.cpp PROPERTIES COMPILE_FLAGS "/Ycstdafx.h")
endif(MSVC)
endmacro(add_precompiledheader_flags projectname)

macro(add_custom_precompiledheader_flags projectname precompile_cpp)
if (MSVC)
   set_target_properties(${projectname} PROPERTIES COMPILE_FLAGS "/Yustdafx.h")
   set_source_files_properties(${precompile_cpp} PROPERTIES COMPILE_FLAGS "/Ycstdafx.h")
endif(MSVC)
endmacro(add_custom_precompiledheader_flags projectname precompile_cpp)

function(stringFirstCharUpper input_var result)
	string(SUBSTRING ${input_var} 0 1 FIRST_LETTER)
	string(TOUPPER ${FIRST_LETTER} FIRST_LETTER)
	string(REGEX REPLACE "^.(.*)" "${FIRST_LETTER}\\1" PROTO_NAME_CAP "${input_var}")
	set (${result} ${PROTO_NAME_CAP} PARENT_SCOPE)
#  foreach(f ${ARGN})
    # do something interesting with 'f' -> other optional arg list
#    message("f='${f}' arg1='${arg1}' arg2='${arg2}'")
#  endforeach()
#  message("f='${f}' arg1='${arg1}' arg2='${arg2}'")  
#  message("ARGV0='${ARGV0}' ARGV1='${ARGV1}' ARGV2='${ARGV2}'")
#  set (${result} ${arg1}_TOTO PARENT_SCOPE)
endfunction()

## usage 
## autogenerateProjectName(${CMAKE_CURRENT_SOURCE_DIR} "sl" PROJECT_NAME)
function(autogenerateProjectName currentSourceDir prefix result)
	# automatic project name generation
	# extract the last component from CMAKE_CURRENT_SOURCE_DIR
	string(REGEX MATCHALL "[a-zA-Z0-9]+\ |[a-zA-Z0-9]+$" PROJECT_DIR_NAME "${currentSourceDir}")
	stringFirstCharUpper( ${PROJECT_DIR_NAME} U_PROJECT_DIR_NAME )

	SET(${result} "${prefix}${U_PROJECT_DIR_NAME}" PARENT_SCOPE)

	message(STATUS "creating project: ${prefix}${U_PROJECT_DIR_NAME}")
endfunction()

