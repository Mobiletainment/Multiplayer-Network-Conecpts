//!---------------------------------------------------------------------------
//! \file "slDictionary.h"
//! \brief <brief-doc-todo>  
//! \details <details-doc-todo>
//! \author Cyrus Preuss
//!
//! This file is part of the Stateless Games SDK
//! Usage of the Stateless Games SDK is subject to an appropriate license agreement.
//!
//! Copyright (c) 2012-2013 Cyrus Preuss. All rights reserved.
//!----------------------------------------------------------------------------


#pragma once
#ifndef __SLDICTIONARY_H__
#define __SLDICTIONARY_H__

#include "../slAbstractNode.h"

namespace sl	{

	class slCocosUtil_API Dictionary : public AbstractNode	{
		SL_DECLARE_BASE(AbstractNode)
	public:

		static CCDictionary* copyDictionary(CCDictionary* source);

		static void setObjectIfNotExist(CCDictionary* target, CCObject* pObject, intptr_t key);
		static void setObjectIfNotExist(CCDictionary* target, CCObject* pObject, const char* key);
		static void setObjectIfNotExist(CCDictionary* target, const char* value, const char* key);

		static const char* getString( CCDictionary* dictionary, const char* key, const char* defaultValue = "" );
		static void setString( CCDictionary* dictionary, const char* key, const char* value );

		static bool getBool( CCDictionary* dictionary, const char* key, bool defaultValue = false );
		static void setBool( CCDictionary* dictionary, const char* key, bool value );

		static int getInteger( CCDictionary* dictionary, const char* key, int defaultValue = 0 );
		static void setInteger( CCDictionary* dictionary, const char* key, int value );

		static float getFloat( CCDictionary* dictionary, const char* key, float defaultValue = 0 );
		static void setFloat( CCDictionary* dictionary, const char* key, float value );

		static double getDouble( CCDictionary* dictionary, const char* key, double defaultValue = 0 );
		static void setDouble( CCDictionary* dictionary, const char* key, double value );

		static CCDictionary* getDictionary( CCDictionary* dictionary, const char* key );

	protected:
		Dictionary();
		~Dictionary();
	};

}

#endif
