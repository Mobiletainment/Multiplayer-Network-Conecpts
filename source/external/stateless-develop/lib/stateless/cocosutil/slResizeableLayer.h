//!---------------------------------------------------------------------------
//! \file "slResizeableLayer.h"
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
#ifndef __SLRESIZEABLELAYER_H__
#define __SLRESIZEABLELAYER_H__

namespace sl	{
	class slCocosUtil_API AbstractResizableLayer : public cocos2d::CCLayer
	{
		SL_DECLARE_BASE(cocos2d::CCLayer)

		virtual void onAppResized(CCObject* app) SL_ABSTRACT;
	protected:
		AbstractResizableLayer();
		virtual ~AbstractResizableLayer();

	};

	class slCocosUtil_API ResizableLayer : public AbstractResizableLayer
	{
		SL_DECLARE_BASE(AbstractResizableLayer)

		virtual void onAppResized(CCObject* app);
	protected:
		ResizableLayer();
		virtual ~ResizableLayer();

	};
}


#endif // __SLRESIZEABLELAYER_H__
