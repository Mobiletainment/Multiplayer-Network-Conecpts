//!---------------------------------------------------------------------------
//! \file "CCControlQuadLayer.h"
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
#ifndef __CCCONTROLQUADLAYER_H__
#define __CCCONTROLQUADLAYER_H__

#include "CCControlBase.h"

namespace sl	{
	class slCocosUtil_API CCControlQuadLayer : public CCControlBase
	{
		SL_DECLARE_BASE(CCControlBase)
	public:

		SL_IMPLEMENT_CLASSNAME(CCControlQuadLayer)

		CCControlQuadLayer();

		virtual void needsLayout();

		// CCNode interface
		virtual void setContentSize(const CCSize& contentSize);

		// custom content interface
		virtual void addSubLayerContent();

		CCControlBase* getSubLayer( SLSize idx ) const;

		void recreate( SLSize numSublayers );

		CC_SYNTHESIZE(SLSize, _numSubLayers, NumSubLayers)

		CC_SYNTHESIZE_RETAIN(CCString*, _leftLayerClassName, LeftLayerClassName)
		CC_SYNTHESIZE_RETAIN(CCString*, _rightLayerClassName, RightLayerClassName)
	protected:
		virtual ~CCControlQuadLayer();

	private:
		CCControlBase* _subLayer[4];

	};

	SL_INLINE
	CCControlBase* CCControlQuadLayer::getSubLayer( SLSize idx ) const	{
		return _subLayer[idx];
	}
	
}

#endif
