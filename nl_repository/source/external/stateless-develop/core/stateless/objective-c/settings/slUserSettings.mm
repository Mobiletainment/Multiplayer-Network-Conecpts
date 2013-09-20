//!---------------------------------------------------------------------------
//! \file "slUserSettings.mm"
//! \brief <brief-doc-todo>  
//! \details <details-doc-todo>
//! \author Cyrus Preuss
//!
//! This file is part of the Stateless Games SDK
//! Usage of the Stateless Games SDK is subject to an appropriate license agreement.
//!
//! Copyright (c) 2012-2013 Cyrus Preuss. All rights reserved.
//!----------------------------------------------------------------------------


/*
 *  SLUserSettings.h
 *
 *  Created by Cyrus Preuss on 02/02/08.
 *  Copyright 2008 Stateless Games. All rights reserved.
 *
 */

#import "SLUserSettings.h"




//------------------------------------------------------------------------------
SLAString SLUserSettings::getUserDefaultsStringForKey( const SLAString& key, const char* defaultValue )
{
	NSString* nsKey = [NSString stringWithUTF8String:key.c_str()];
	return getUserDefaultsValue_AString(nsKey, defaultValue);
}

//------------------------------------------------------------------------------
bool SLUserSettings::setUserDefaultsStringForKey( const SLAString& key, const SLAString& value, bool overWrite )
{
	NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
	NSString* nsKey = [NSString stringWithUTF8String:key.c_str()];
	bool storeValue(overWrite);
	if (overWrite == false) {
		// store only in case not present
		NSObject* value = [defaults objectForKey:nsKey];
		if (value == nil) {
			storeValue = true;
		}
	}
	if (storeValue) {
		NSString* nsValue = [NSString stringWithUTF8String:value.c_str()];
		[defaults setValue:nsValue forKey:nsKey];
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------
bool SLUserSettings::getUserDefaultsBoolForKey( const SLAString& key, bool defaultValue )
{
	NSString* nsKey = [NSString stringWithUTF8String:key.c_str()];
	return getUserDefaultsValue_bool(nsKey, defaultValue ? TRUE : FALSE);
}

//------------------------------------------------------------------------------
void SLUserSettings::setUserDefaultsBoolForKey( const SLAString& key, bool value )
{
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
	NSString* nsKey = [NSString stringWithUTF8String:key.c_str()];
	NSNumber* nsValue = [NSNumber numberWithBool:value ? TRUE : FALSE];
	[defaults setValue:nsValue forKey:nsKey];
}

//------------------------------------------------------------------------------
int SLUserSettings::getUserDefaultsIntegerForKey( const SLAString& key, int defaultValue )
{
	NSString* nsKey = [NSString stringWithUTF8String:key.c_str()];
	return getUserDefaultsValue_Int(nsKey, defaultValue);
}

//------------------------------------------------------------------------------
void SLUserSettings::setUserDefaultsIntegerForKey( const SLAString& key, int value )
{
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
	NSString* nsKey = [NSString stringWithUTF8String:key.c_str()];
	NSNumber* nsValue = [NSNumber numberWithInt:value];
	[defaults setValue:nsValue forKey:nsKey];
}

//------------------------------------------------------------------------------
float SLUserSettings::getUserDefaultsFloatForKey( const SLAString& key, float defaultValue )
{
	NSString* nsKey = [NSString stringWithUTF8String:key.c_str()];
	return getUserDefaultsValue_Float(nsKey, defaultValue);
}

//------------------------------------------------------------------------------
void SLUserSettings::setUserDefaultsFloatForKey( const SLAString& key, float value )
{
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
	NSString* nsKey = [NSString stringWithUTF8String:key.c_str()];
	NSNumber* nsValue = [NSNumber numberWithFloat:value];
	[defaults setValue:nsValue forKey:nsKey];
}

//------------------------------------------------------------------------------
double SLUserSettings::getUserDefaultsDoubleForKey( const SLAString& key, double defaultValue )
{
	NSString* nsKey = [NSString stringWithUTF8String:key.c_str()];
	return getUserDefaultsValue_Double(nsKey, defaultValue);
}

//------------------------------------------------------------------------------
bool SLUserSettings::setUserDefaultsDoubleForKey( const SLAString& key, double value, bool overWrite )
{
	NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
	NSString* nsKey = [NSString stringWithUTF8String:key.c_str()];
	bool storeValue(overWrite);
	if (overWrite == false) {
		// store only in case not present
		NSObject* value = [defaults objectForKey:nsKey];
		if (value == nil) {
			storeValue = true;
		}
	}
	if (storeValue) {
		NSNumber* nsValue = [NSNumber numberWithDouble:value];
		[defaults setValue:nsValue forKey:nsKey];
		return true;
	}
	return false;
}



//------------------------------------------------------------------------------
NSString* getUserDefaultsValue_String( NSString* key, NSString* defaultValue )
{
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
	NSString* valueString = [defaults objectForKey:key];
	if (valueString == nil) {
		if (defaultValue != nil) {
			return defaultValue;
		}
	}
	return valueString;
}

//------------------------------------------------------------------------------
SLAString getUserDefaultsValue_AString( NSString* key, const char* defaultValue )
{
	SLAString value;
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
	NSString* valueString = [defaults objectForKey:key];
	if (valueString == nil) {
		if ((defaultValue != NULL) && (defaultValue[0] != 0)) {
			value.assign(defaultValue);
		}
	}
	else	{
		value.assign([valueString UTF8String]);
	}
	return value;
}

//------------------------------------------------------------------------------
float getUserDefaultsValue_Float( NSString* key, float defaultValue )
{
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
	NSObject* value = [defaults objectForKey:key];
	if (value == nil) {
		return defaultValue;
	}
	return [defaults floatForKey:key];
}


//------------------------------------------------------------------------------
double getUserDefaultsValue_Double( NSString* key, double defaultValue )
{
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
	NSObject* value = [defaults objectForKey:key];
	if (value == nil) {
		return defaultValue;
	}
	return [defaults doubleForKey:key];
}


//------------------------------------------------------------------------------
BOOL getUserDefaultsValue_BOOL( NSString* key, BOOL defaultValue )
{
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
	NSObject* value = [defaults objectForKey:key];
	if (value == nil) {
		return defaultValue;
	}
	return [defaults boolForKey:key];
}

//------------------------------------------------------------------------------
bool getUserDefaultsValue_bool( NSString* key, BOOL defaultValue )
{
	return getUserDefaultsValue_BOOL(key, defaultValue) == YES ? true : false;
}

//------------------------------------------------------------------------------
int getUserDefaultsValue_Int( NSString* key, int defaultValue )
{
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
	NSObject* value = [defaults objectForKey:key];
	if (value == nil) {
		return defaultValue;
	}
	return [defaults integerForKey:key];
}

