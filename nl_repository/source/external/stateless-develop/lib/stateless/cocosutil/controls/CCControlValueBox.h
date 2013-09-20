//!---------------------------------------------------------------------------
//! \file "CCControlValueBox.h"
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
#ifndef __CCCONTROL_VALUEBOX_H__
#define __CCCONTROL_VALUEBOX_H__

#include "CCControlBase.h"

namespace sl	{
	class slCocosUtil_API CCControlValueBox : public CCControlBase
	{
		SL_DECLARE_BASE(CCControlBase)
	public:

		SL_IMPLEMENT_CLASSNAME(CCControlValueBox)

		static CCControlValueBox* valueBoxFromSprites(
			CCScale9Sprite* backgroundSprite,
			CCNode* label,
			const CCSize& preferredSize, const char* text
			);

		virtual bool initFromSprites(
			CCScale9Sprite* backgroundSprite,
			CCNode* label,
			const CCSize& preferredSize, const char* text);

		//touch events
		virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
		virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

		CCControlValueBox();

		CC_SYNTHESIZE(CCNode*, _textField, TextField)

		virtual void serialize( CCDictionary* target );
		virtual void deserialize( CCDictionary* source );

		void setText(const char* text);

	protected:
		CCNode* m_label;

	protected:
		~CCControlValueBox();
		virtual void needsLayout();

	private:
	};
}

#endif
