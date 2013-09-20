//!---------------------------------------------------------------------------
//! \file "slUserSettings.cpp"
//! \brief <brief-doc-todo>  
//! \details <details-doc-todo>
//! \author Cyrus Preuss
//!
//! This file is part of the Stateless Games SDK
//! Usage of the Stateless Games SDK is subject to an appropriate license agreement.
//!
//! Copyright (c) 2008-2013 Cyrus Preuss. All rights reserved.
//!----------------------------------------------------------------------------


#include "slUserSettings.h"


#ifdef WIN32

SLAString SLUserSettings::getUserDefaultsStringForKey( const SLAString& key, const char* defaultValue )
{
	return SLAString(defaultValue);
}
bool SLUserSettings::setUserDefaultsStringForKey( const SLAString& key, const SLAString& value, bool overWrite )
{
	return false;
}

bool SLUserSettings::getUserDefaultsBoolForKey( const SLAString& key, bool defaultValue )
{
	return defaultValue;
}

void SLUserSettings::setUserDefaultsBoolForKey( const SLAString& key, bool value )
{

}

int SLUserSettings::getUserDefaultsIntegerForKey( const SLAString& key, int defaultValue )
{
	return defaultValue;
}

void SLUserSettings::setUserDefaultsIntegerForKey( const SLAString& key, int value )
{

}

float SLUserSettings::getUserDefaultsFloatForKey( const SLAString& key, float defaultValue )
{
	return defaultValue;
}

void SLUserSettings::setUserDefaultsFloatForKey( const SLAString& key, float value )
{

}

double SLUserSettings::getUserDefaultsDoubleForKey( const SLAString& key, double defaultValue )
{
	return defaultValue;
}

bool SLUserSettings::setUserDefaultsDoubleForKey( const SLAString& key, double value, bool overWrite )
{
	return false;
}

#endif
