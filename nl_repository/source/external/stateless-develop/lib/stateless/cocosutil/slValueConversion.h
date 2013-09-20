//!---------------------------------------------------------------------------
//! \file "slValueConversion.h"
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
#ifndef __SLVALUECONVERSION_H__
#define __SLVALUECONVERSION_H__

namespace sl	{

	class slCocosUtil_API ValueConversion	{
	public:
		static 	cocos2d::CCString* getStringForObject( cocos2d::CCObject* object );
		static 	cocos2d::CCObject* setValueFromString( cocos2d::CCString* strValue, cocos2d::CCObject* object );

	};

}

#endif
