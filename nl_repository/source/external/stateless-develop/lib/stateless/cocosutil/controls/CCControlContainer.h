//!---------------------------------------------------------------------------
//! \file "CCControlContainer.h"
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
#ifndef __CCCONTROLBASECONTAINER_H__
#define __CCCONTROLBASECONTAINER_H__

#include "CCControlBase.h"

namespace sl	{

	class slCocosUtil_API CCControlContainer : public CCControlBase
	{
		SL_DECLARE_BASE(CCControlBase)
	public:
		SL_IMPLEMENT_CLASSNAME(CCControlContainer)

		void addNode(CCNode* node);
		void addNodeAt(CCNode* node, unsigned int  idx);
		void addNodes(CCArray* nodes);

		// a hack actually
		virtual void redoLayout() SL_OVERRIDE;

		virtual bool initFromNodes(
			CCScale9Sprite* backgroundSprite,
			CCArray* nodes,
			const CCSize& preferredSize);

		// touch events
		virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
		virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

		virtual void serialize( CCDictionary* target );
		virtual void deserialize( CCDictionary* source );

		CC_SYNTHESIZE_RETAIN(CCArray*, _nodes, Nodes)
	protected:
		CCControlContainer();
		~CCControlContainer();

	};
}

#endif
