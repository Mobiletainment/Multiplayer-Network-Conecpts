//!---------------------------------------------------------------------------
//! \file "CCControlValueBox.cpp"
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
#include "CCControlValueBox.h"
#include "GUI/CCControlExtension/CCScale9Sprite.h"
#include "../slValueConversion.h"
#include "../slControlUtils.h"
#include "label_nodes/CCLabelTTF.h"

namespace sl	{
	CCControlValueBox::CCControlValueBox()
		:m_label(nullptr)
		,_textField(nullptr)
	{
	}

	CCControlValueBox::~CCControlValueBox()
	{
		setTextField(nullptr);
	}

	CCControlValueBox* CCControlValueBox::valueBoxFromSprites(
		CCScale9Sprite* backgroundSprite,
		CCNode* label,
		const CCSize& preferredSize, 
		const char* text
		)
	{
		CCControlValueBox *pRet = new CCControlValueBox();
		pRet->initFromSprites(backgroundSprite, label, preferredSize, text);
		pRet->autorelease();
		return pRet;
	}

	bool CCControlValueBox::initFromSprites(
		CCScale9Sprite* backgroundSprite,
		CCNode* label,
		const CCSize& preferredSize,
		const char* text
	)
	{
		if (!SLBaseClass::initWithBackGroundSprite(backgroundSprite))
			return false;

		setPreferredSize(preferredSize);

		if (label) {
			m_label=label;
			addChild(m_label, 2);
		}

		setTouchEnabled(true);

		const char* labelText(isEmptyString(text) ? "value" : text);

		CCNode * textField = ControlUtils::createLabel(labelText, kCCTextAlignmentLeft);

		addChild(textField, 1);

		// to match the text alignment
		textField->setAnchorPoint(ccp(0,0.5f)); 

		setTextField(textField);

		needsLayout();

		/*
		addTargetWithActionForControlEvents(this, cccontrol_selector(CCControlValueBox::saveValue), CCControlEventValueChanged);
		*/
		return true;
	}


	void CCControlValueBox::needsLayout()
	{
		if (_textField == nullptr) {
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
			std::max(inputHeight, lblHeight) + m_marginV;

		CCSize preferredSize(getPreferredSize());
		if(width != preferredSize.width && preferredSize.width != 0)	{
			width = preferredSize.width;
		}
		CCSize size = CCSizeMake(width, height);
		setPreferredSize(size);

		SLBaseClass::needsLayout();

		float y = height - ( m_marginV + lblHeight * 0.5f);
		if (m_label) {
			m_label->setPosition(ccp( m_marginH + (m_label->getContentSize().width * 0.5), y ));
		}

		y = height * 0.5f;
		if (m_label) {
			CCPoint textPosition(m_label->getPosition());
			textPosition.x += m_label->getContentSize().width * 0.5f + padding;
			_textField->setPosition( ccp( textPosition.x, y ));
		}
		else	{
			CCPoint textPosition(ccp( m_marginH, y ));
			_textField->setPosition( textPosition );
		}
	}

	bool CCControlValueBox::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
	{
		if (!isTouchInside(pTouch) || !isEnabled())
		{
			return false;
		}

		sendActionsForControlEvents(CCControlEventTouchDown);
		return true;
	}

	void CCControlValueBox::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
	{    
		if (isTouchInside(pTouch))
		{
		}	
	}

	void CCControlValueBox::serialize( CCDictionary* target )
	{
		if( canSerializeTo(target) == false)	{
			return;
		}
		SLBaseClass::serialize(target);
		CCObject* value(getCtrlValue());

		CCLabelTTF *titleLbl(dynamic_cast<CCLabelTTF*>(_textField));

		CCObject* newValue(ValueConversion::setValueFromString(CCString::create(titleLbl->getString()),value));
		if(newValue != value)	{
			target->setObject(newValue,getValueKey());
			// now we reserialize as the value might get validated ...
			deserialize(target);
		}
	}

	void CCControlValueBox::deserialize( CCDictionary* source )
	{
		if( canDeserializeFrom(source) == false)	{
			return;
		}
		SLBaseClass::deserialize(source);
		CCObject* value(getCtrlValue());

		CCString* strValue(ValueConversion::getStringForObject(value));
		CCLabelTTF *titleLbl(dynamic_cast<CCLabelTTF*>(_textField));
		titleLbl->setString(strValue->getCString());

		if(getCtrlLayoutFlags().hasFlag(ECtrlLayoutFlag_ResetPreferredSizeOnSerialize))	{
			CCSize preferredSize(CCSizeMake(0,0));
			setPreferredSize(preferredSize);
		}
		needsLayout();
	}

	void CCControlValueBox::setText( const char* text )	{
		CCLabelTTF *titleLbl(dynamic_cast<CCLabelTTF*>(_textField));
		if(isEmptyString(text))	{
			titleLbl->setString("");
		}
		else	{
			titleLbl->setString(text);
		}
		needsLayout();
	}


}


