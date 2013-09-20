//!---------------------------------------------------------------------------
//! \file "CCControlCheckBox.h"
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
#ifndef __CCCONTROL_CHECKBOX_H__
#define __CCCONTROL_CHECKBOX_H__

#include "CCControlBase.h"


namespace sl	{
	class slCocosUtil_API CCControlCheckBox : public CCControlBase
	{
		SL_DECLARE_BASE(CCControlBase)
	public:

		SL_IMPLEMENT_CLASSNAME(CCControlCheckBox)

		CC_SYNTHESIZE_READONLY(bool, m_checked, IsChecked);
		virtual void setIsChecked(bool val);

	public:
		static CCControlCheckBox* checkBoxFromSprites(
			CCSprite* checkedSprite,
			CCSprite* uncheckedSprite,
			CCScale9Sprite* backgroundSprite,
			CCNode* label,
			const CCSize& preferredSize
			);

		virtual bool initFromSprites(
			CCSprite* checkedSprite,
			CCSprite* uncheckedSprite,
			CCScale9Sprite* backgroundSprite,
			CCNode* label,
			const CCSize& preferredSize);

		virtual void saveValue(CCObject* o, CCControlEvent e);

		// touch events
		virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
		virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

		virtual void serialize( CCDictionary* target );
		virtual void deserialize( CCDictionary* source );


	protected:
		CCControlCheckBox();
		~CCControlCheckBox();
		virtual void needsLayout();

		CCSprite* m_checkedSprite, *m_uncheckedSprite;
		CCNode* m_label;

	private:
	};
}



#endif
