######################################################################
# set project target properties
macro(SET_TARGETTYPEPROPERTIES projectname)
	message(STATUS "- ${projectname} -")
	set_target_properties( ${projectname} PROPERTIES RELEASE_POSTFIX "_Release" )
	set_target_properties( ${projectname} PROPERTIES DEBUG_POSTFIX "_Debug" )
	set_target_properties( ${projectname} PROPERTIES MINSIZEREL_POSTFIX "_MinsizeRel" )
	set_target_properties( ${projectname} PROPERTIES RELWITHDEBINFO_POSTFIX "_RelWithDebugInfo" )
endmacro(SET_TARGETTYPEPROPERTIES projectname)


macro(add_cocos2dx_user_compiler_flags)
	if(COCOS2DX_LINK_ICONV)
		add_definitions(-DCC_ENABLE_ICONV_INTEGRATION=1)
	else()
		add_definitions(-DCC_ENABLE_ICONV_INTEGRATION=0)
	endif()
	
	if(COCOS2DX_LINK_LIBCURL)
		add_definitions(-DCC_ENABLE_LIBCURL_INTEGRATION=1)
	else()
		add_definitions(-DCC_ENABLE_LIBCURL_INTEGRATION=0)
	endif()
	
	if(COCOS2DX_LINK_LIBJPEG)
		add_definitions(-DCC_ENABLE_LIBJPEG_INTEGRATION=1)
	else()
		add_definitions(-DCC_ENABLE_LIBJPEG_INTEGRATION=0)
	endif()
		
	if(COCOS2DX_LINK_LIBTIFF)
		add_definitions(-DCC_ENABLE_LIBTIFF_INTEGRATION=1)
	else()
		add_definitions(-DCC_ENABLE_LIBTIFF_INTEGRATION=0)
	endif()
	
	if(COCOS2DX_LINK_PTHREAD)
		add_definitions(-DCC_ENABLE_PTHREAD_INTEGRATION=1)
	else()
		add_definitions(-DCC_ENABLE_PTHREAD_INTEGRATION=0)
	endif()
	
	if(COCOS2DX_LINK_SQLITE)
		add_definitions(-DCC_ENABLE_SQLITE_INTEGRATION=1)
	else()
		add_definitions(-DCC_ENABLE_SQLITE_INTEGRATION=0)
	endif()

	if(COCOS2DX_LINK_WEBP)
		add_definitions(-DCC_ENABLE_WEBP_INTEGRATION=1)
	else()
		add_definitions(-DCC_ENABLE_WEBP_INTEGRATION=0)
	endif()
	
	if(COCOS2DX_LINK_WEBSOCKETS)
		add_definitions(-DCC_ENABLE_WEBSOCKETS_INTEGRATION=1)
	else()
		add_definitions(-DCC_ENABLE_WEBSOCKETS_INTEGRATION=0)
	endif()
	
	if(COCOS2DX_LINK_ZLIB)
		add_definitions(-DCC_ENABLE_ZLIB_INTEGRATION=1)
	else()
		add_definitions(-DCC_ENABLE_ZLIB_INTEGRATION=0)
	endif()

	if(COCOS2DX_LINK_AUDIO)
		add_definitions(-DCC_ENABLE_AUDIO_INTEGRATION=1)
	else()
		add_definitions(-DCC_ENABLE_AUDIO_INTEGRATION=0)
	endif()
	
	add_definitions(-DCC_ENABLE_BOX2D_INTEGRATION=1)
	add_definitions(-DCOCOS2D_DEBUG=1)
	
endmacro()

macro(add_cocos2dx_build_compiler_flags projectname)
	add_cocos2dx_user_compiler_flags()
	
	if(COCOS2DX_LINK_ICONV)
	endif()
	
	if(COCOS2DX_LINK_LIBCURL)
		target_link_libraries(${projectname} libcurl_imp) 
	endif()
	
	if(COCOS2DX_LINK_LIBJPEG)
		target_link_libraries(${projectname} libjpeg)
	endif()
			
	if(COCOS2DX_LINK_LIBTIFF)
		target_link_libraries(${projectname} libtiff)
	endif()
	
	if(COCOS2DX_LINK_PTHREAD)
		target_link_libraries(${projectname} pthreadVCE2)
	endif()
	
	if(COCOS2DX_LINK_SQLITE)
		target_link_libraries(${projectname} sqlite3)
	endif()
	
	if(COCOS2DX_LINK_WEBSOCKETS)
		target_link_libraries(${projectname} libwebp)
	endif()
	
	if(COCOS2DX_LINK_WEBSOCKETS)
		target_link_libraries(${projectname} websockets)
	endif()
	
	if(COCOS2DX_LINK_ZLIB)
		target_link_libraries(${projectname} libzlib)
	endif()
	
	if(COCOS2DX_LINK_AUDIO)
		target_link_libraries(${projectname} Winmm)
	endif()
	
endmacro()

