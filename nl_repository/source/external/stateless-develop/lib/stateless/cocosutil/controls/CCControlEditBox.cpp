//!---------------------------------------------------------------------------
//! \file "CCControlEditBox.cpp"
//! \brief <brief-doc-todo>  
//! \details <details-doc-todo>
//! \author Cyrus Preuss
//!
//! This file is part of the Stateless Games SDK
//! Usage of the Stateless Games SDK is subject to an appropriate license agreement.
//!
//! Copyright (c) 2012-2013 Cyrus Preuss. All rights reserved.
//!----------------------------------------------------------------------------


#include "stdafx.h"
#include "CCControlEditBox.h"
#include "GUI/CCControlExtension/CCScale9Sprite.h"
#include "../slValueConversion.h"
#include "../slCocosUtilMacros.h"

namespace sl	{
	CCControlEditBox::CCControlEditBox()
		:m_label(nullptr)
		,_textField(nullptr)
		,_textCursor(nullptr)
	{
		_textFieldObserver._editBox = this;
	}

	CCControlEditBox::~CCControlEditBox()
	{
		setTextField(nullptr);
		setTextCursor(nullptr);
	}

	CCControlEditBox* CCControlEditBox::editBoxFromSprites(
		CCScale9Sprite* backgroundSprite,
		CCNode* label,
		const CCSize& preferredSize
		)
	{
		CCControlEditBox *pRet = new CCControlEditBox();
		pRet->initFromSprites(backgroundSprite, label, preferredSize);
		pRet->autorelease();
		return pRet;
	}

	bool CCControlEditBox::initFromSprites(
		CCScale9Sprite* backgroundSprite,
		CCNode* label,
		const CCSize& preferredSize
	)
	{
		if (!SLBaseClass::initWithBackGroundSprite(backgroundSprite))
			return false;

		setPreferredSize(preferredSize);

		setOpacity(255);

		if (label) {
			m_label=label;
			addChild(m_label, 2);
		}

		setTouchEnabled(true);

		CCTextFieldTTF * textField = CCTextFieldEx::textFieldWithPlaceHolder("<input>",
			SL_UI_TEXTFIELD_FONTNAME,
			SL_UI_TEXTFIELD_FONTSIZE);

//		textField->setColor(ccc3(0, 255, 0));


		addChild(textField, 1);

		textField->setHorizontalAlignment(kCCTextAlignmentLeft);
		// to match the text alignment
		textField->setAnchorPoint(ccp(0,0.5f)); 

		textField->setDelegate(&_textFieldObserver);

		setTextField(textField);


		setTextCursor(CCSprite::create("controls/green_edit_cursor.png"));
		getTextCursor()->setVisible(false);

		CCSize s(getTextCursor()->getContentSize());
		getTextCursor()->setContentSize(s);

		addChild(getTextCursor(),2);


		needsLayout();

		/*
		addTargetWithActionForControlEvents(this, cccontrol_selector(CCControlEditBox::saveValue), CCControlEventValueChanged);
		*/
		return true;
	}


	void CCControlEditBox::needsLayout()	{
		if (_textField == nullptr) {
			return;
		}
		if (m_label == nullptr) {
			return;
		}
		if (getBackGroundSprite() == nullptr) {
			return;
		}

		float lblWidth = m_label ? m_label->getContentSize().width : 0;
		float inputWidth = _textField ? _textField->getContentSize().width : 0;
		float lblHeight = m_label ? m_label->getContentSize().height : 0;
		float inputHeight = _textField ? _textField->getContentSize().height : 0;
		float padding=10;
		float width =
			m_marginH + 
			padding + lblWidth + 
			padding + inputWidth + 
			m_marginH;

		float height = m_marginV + 
			std::max(inputHeight, lblHeight)+m_marginV;

		CCSize preferredSize(getPreferredSize());
		if(width != preferredSize.width && preferredSize.width != 0)	{
			width = preferredSize.width;
		}
		CCSize size = CCSizeMake(width, height);
		setPreferredSize(size);

		SLBaseClass::needsLayout();

		float y = height * 0.5f;
		if (m_label) {
			m_label->setPosition(ccp( m_marginH + (m_label->getContentSize().width * 0.5), y ));
		}

		CCPoint textPosition(m_label->getPosition());
		textPosition.x += m_label->getContentSize().width * 0.5f + padding;
		_textField->setPosition( ccp( textPosition.x, y ));
	}

	bool CCControlEditBox::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)	{
		if (!isTouchInside(pTouch) || !isEnabled())
		{
			return false;
		}

		sendActionsForControlEvents(CCControlEventTouchDown);
		return true;
	}

	void CCControlEditBox::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)	{    
		if (isTouchInside(pTouch))
		{
			_textField->attachWithIME();
		}	
	}

	void CCControlEditBox::serialize( CCDictionary* target )	{
		if( canSerializeTo(target) == false)	{
			return;
		}
		SLBaseClass::serialize(target);
		CCObject* value(getCtrlValue());
		CCObject* newValue(ValueConversion::setValueFromString(CCString::create(_textField->getString()),value));
		if(newValue != value)	{
			target->setObject(newValue,getValueKey());
			// now we reserialize as the value might get validated ...
			deserialize(target);
		}
	}

	void CCControlEditBox::deserialize( CCDictionary* source )	{
		if( canDeserializeFrom(source) == false)	{
			return;
		}
		SLBaseClass::deserialize(source);
		CCObject* value(getCtrlValue());

		CCString* strValue(ValueConversion::getStringForObject(value));
		_textField->setString(strValue->getCString());

		if(getCtrlLayoutFlags().hasFlag(ECtrlLayoutFlag_ResetPreferredSizeOnSerialize))	{
			CCSize preferredSize(CCSizeMake(0,0));
			setPreferredSize(preferredSize);
			needsLayout();
		}
	}

	CCTextFieldEx * CCTextFieldEx::textFieldWithPlaceHolder( const char *placeholder, const char *fontName, float fontSize )
	{
		CCTextFieldEx *pRet = new CCTextFieldEx();
		if(pRet && pRet->initWithString("", fontName, fontSize))
		{
			pRet->autorelease();
			if (placeholder)
			{
				pRet->setPlaceHolder(placeholder);
			}
			return pRet;
		}
		CC_SAFE_DELETE(pRet);

		return nullptr;
	}

	void CCTextFieldEx::didAttachWithIME()
	{

	}

	void CCTextFieldEx::didDetachWithIME()
	{

	}


	bool CCTextFieldObserver::onDraw( CCTextFieldTTF * sender )	{
		CCPoint p(sender->getPosition());
		p.x += _editBox->getTextCursor()->getContentSize().width * 0.5f;

		if(isEmptyString(sender->getString()) == false)	{
			CCSize s(sender->getContentSize());
			p.x += s.width;
		}

		_editBox->getTextCursor()->setPosition(p);
		return false;
	}

	bool CCTextFieldObserver::onTextFieldAttachWithIME( CCTextFieldTTF * sender )	{
		CCSprite* textCursor(_editBox->getTextCursor());

		CCActionInterval* action(CCBlink::create(5, 10));
		CCSequence* blinkSequence( CCSequence::create( action, NULL));
		textCursor->runAction(CCRepeatForever::create(blinkSequence));

		return false;
	}

	bool CCTextFieldObserver::onTextFieldDetachWithIME( CCTextFieldTTF * sender )	{
		CCSprite* textCursor(_editBox->getTextCursor());
		textCursor->stopAllActions();
		textCursor->setVisible(false);

		_editBox->serialize(_editBox->getDataSource());

		CCNotificationCenter* notificationCenter(CCNotificationCenter::sharedNotificationCenter());
		notificationCenter->postNotification(SL_NOTIFY_CTRL_EDIT_END, _editBox);	

		return false;
	}

}


