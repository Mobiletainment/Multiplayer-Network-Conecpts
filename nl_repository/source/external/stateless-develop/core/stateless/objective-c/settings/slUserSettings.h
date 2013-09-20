#pragma once
#ifndef __SLUSERSETTINGS_H__
#define __SLUSERSETTINGS_H__

//!---------------------------------------------------------------------------
//! \file "slUserSettings.h"
//! \brief <brief-doc-todo>  
//! \details <details-doc-todo>
//! \author Cyrus Preuss
//!
//! This file is part of the Stateless Games SDK
//! Usage of the Stateless Games SDK is subject to an appropriate license agreement.
//!
//! Copyright (c) 2008-2013 Cyrus Preuss. All rights reserved.
//!----------------------------------------------------------------------------


#include "slMacros.h"
#include "slCoreTypes.h"
#include "slSTL.h"

#ifdef __OBJC__
	NSString* getUserDefaultsValue_String( NSString* key, NSString* defaultValue );
	SLAString getUserDefaultsValue_AString( NSString* key, const char* defaultValue );
	float getUserDefaultsValue_Float( NSString* key, float defaultValue );
	double getUserDefaultsValue_Double( NSString* key, double defaultValue );
	BOOL getUserDefaultsValue_BOOL( NSString* key, BOOL defaultValue );
	int getUserDefaultsValue_Int( NSString* key, int defaultValue );
	bool getUserDefaultsValue_bool( NSString* key, BOOL defaultValue );
#else
#endif


//------------------------------------------------------------------------------
/**
 @brief    A class used to set or retrieve user settings on a key value base.
 */
class SLUserSettings
{
public:

	static SLAString getUserDefaultsStringForKey( const SLAString& key, const char* defaultValue = "" );
	static bool setUserDefaultsStringForKey( const SLAString& key, const SLAString& value, bool overWrite = true );

	static bool getUserDefaultsBoolForKey( const SLAString& key, bool defaultValue = false );
	static void setUserDefaultsBoolForKey( const SLAString& key, bool value );

	static int getUserDefaultsIntegerForKey( const SLAString& key, int defaultValue = 0 );
	static void setUserDefaultsIntegerForKey( const SLAString& key, int value );

	static float getUserDefaultsFloatForKey( const SLAString& key, float defaultValue = 0 );
	static void setUserDefaultsFloatForKey( const SLAString& key, float value );

	static double getUserDefaultsDoubleForKey( const SLAString& key, double defaultValue = 0 );
	static bool setUserDefaultsDoubleForKey( const SLAString& key, double value, bool overWrite = true );
};

#endif /* defined(__SLUSERSETTINGS_H__) */
