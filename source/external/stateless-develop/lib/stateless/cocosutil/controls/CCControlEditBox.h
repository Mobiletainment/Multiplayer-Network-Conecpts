//!---------------------------------------------------------------------------
//! \file "CCControlEditBox.h"
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
#ifndef __CCCONTROL_EDITBOX_H__
#define __CCCONTROL_EDITBOX_H__

#include "CCControlBase.h"
#include "text_input_node/CCTextFieldTTF.h"

namespace sl {

	class CCControlEditBox;
	class slCocosUtil_API CCTextFieldObserver : public CCObject, public CCTextFieldDelegate
	{
	public:


		bool init() {return true;}
		CREATE_FUNC(CCTextFieldObserver)
		/**
		@brief    If the sender doesn't want to draw, return true.
		*/
		virtual bool onDraw(CCTextFieldTTF * sender);

		/**
		@brief    If the sender doesn't want to attach to the IME, return true;
		*/
		virtual bool onTextFieldAttachWithIME(CCTextFieldTTF * sender);

		/**
		@brief    If the sender doesn't want to detach from the IME, return true;
		*/
		virtual bool onTextFieldDetachWithIME(CCTextFieldTTF * sender);

		CCControlEditBox* _editBox;
	};


	class slCocosUtil_API CCTextFieldEx : public cocos2d::CCTextFieldTTF	
	{

	public:
		static CCTextFieldEx * textFieldWithPlaceHolder(const char *placeholder, 
			const char *fontName, float fontSize);

		/**
		@brief    When the delegate detaches from the IME, this method is called by CCIMEDispatcher.
		*/
		virtual void didAttachWithIME();

		/**
		@brief    When the delegate detaches from the IME, this method is called by CCIMEDispatcher.
		*/
		virtual void didDetachWithIME();

	};

	class slCocosUtil_API CCControlEditBox : public CCControlBase
	{
		SL_DECLARE_BASE(CCControlBase)
	public:

		SL_IMPLEMENT_CLASSNAME(CCControlEditBox)

		static CCControlEditBox* editBoxFromSprites(
			CCScale9Sprite* backgroundSprite,
			CCNode* label,
			const CCSize& preferredSize
			);

		virtual bool initFromSprites(
			CCScale9Sprite* backgroundSprite,
			CCNode* label,
			const CCSize& preferredSize);

		//touch events
		virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
		virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

		CC_SYNTHESIZE(CCTextFieldTTF*, _textField, TextField)
		CC_SYNTHESIZE(CCSprite*, _textCursor, TextCursor)


		virtual void serialize( CCDictionary* target );
		virtual void deserialize( CCDictionary* source );


	protected:
		CCTextFieldObserver _textFieldObserver;
		CCNode* m_label;

	protected:
		CCControlEditBox();
		~CCControlEditBox();
		virtual void needsLayout();

	private:
		bool m_defaultSetting;
	};
}

#endif
