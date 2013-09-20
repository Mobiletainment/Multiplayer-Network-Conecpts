//!---------------------------------------------------------------------------
//! \file "CCControlTwinLayer.h"
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
#ifndef __CCCONTROLTWINLAYER_H__
#define __CCCONTROLTWINLAYER_H__

#include "CCControlBase.h"

namespace sl	{
	class slCocosUtil_API CCControlTwinLayer : public CCControlBase
	{
		SL_DECLARE_BASE(CCControlBase)
	public:

		SL_IMPLEMENT_CLASSNAME(CCControlTwinLayer)

		CCControlTwinLayer();

		virtual void needsLayout();

		// CCNode interface
		virtual void setContentSize(const CCSize& contentSize);

		virtual void addSubLayerContent();

		CCControlBase* getChildLayer( SLSize idx ) const;

		CC_SYNTHESIZE_RETAIN(CCString*, _leftLayerClassName, LeftLayerClassName)
		CC_SYNTHESIZE_RETAIN(CCString*, _rightLayerClassName, RightLayerClassName)
	protected:
		virtual ~CCControlTwinLayer();

	private:
		CCControlBase* _childLayer[2];

	};

	SL_INLINE
	CCControlBase* CCControlTwinLayer::getChildLayer( SLSize idx ) const	{
		return _childLayer[idx];
	}

}

#endif
