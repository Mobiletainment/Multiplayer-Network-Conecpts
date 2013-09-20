//!---------------------------------------------------------------------------
//! \file "ControlFactory.h"
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
#ifndef __SLCONTROLFACTORY_H__
#define __SLCONTROLFACTORY_H__

#include "CCControlBase.h"


typedef  sl::CCControlBase* (slFunc_createControl)( 
	const char* className, 
	const CCSize& preferredSize,
	CCScale9Sprite* backgroundSprite
);

namespace sl	{
	class slCocosUtil_API ControlFactory
	{
	public:
		static CCControlBase* createControl(
			const char* className, 
			const CCSize& preferredSize,
			CCScale9Sprite* backgroundSprite = nullptr);

		static void removeCreateControlCallbacks();
		static void addCreateControlCallBack( 
			const char* className, slFunc_createControl* function );
	protected:

	private:
	};
}

#endif
