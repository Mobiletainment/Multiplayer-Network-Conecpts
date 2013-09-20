//!---------------------------------------------------------------------------
//! \file "CCControlCheckBox.cpp"
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
#include "CCControlCheckBox.h"
#include "../slValueConversion.h"

namespace sl	{


	//------------------------------------------------------------------------------
	CCControlCheckBox::CCControlCheckBox()
		:m_checkedSprite(NULL)
		,m_uncheckedSprite(NULL)
		,m_label(NULL)
		,m_checked(false)
	{
	}

	//------------------------------------------------------------------------------
	CCControlCheckBox::~CCControlCheckBox()
	{
	}


	//------------------------------------------------------------------------------
	CCControlCheckBox* CCControlCheckBox::checkBoxFromSprites(CCSprite* checkedSprite,
															  CCSprite* uncheckedSprite,
															  CCScale9Sprite* backgroundSprite,
															  CCNode* label,
															  const CCSize& preferredSize
															  )
	{
		CCControlCheckBox *pRet = new CCControlCheckBox();
		pRet->initFromSprites(checkedSprite, uncheckedSprite, backgroundSprite, label, preferredSize);
		pRet->autorelease();
		return pRet;
	}

	//------------------------------------------------------------------------------
	bool CCControlCheckBox::initFromSprites(CCSprite* checkedSprite,
											CCSprite* uncheckedSprite,
											CCScale9Sprite* backgroundSprite,
											CCNode* label,
											const CCSize& preferredSize
											)
	{
		if (!SLBaseClass::initWithBackGroundSprite(backgroundSprite))
			return false;

		setPreferredSize(preferredSize);

		m_checkedSprite = checkedSprite;
		m_uncheckedSprite = uncheckedSprite;
		addChild(m_checkedSprite, 1);
		addChild(m_uncheckedSprite, 1);
		if (label) {
			m_label=label;
			addChild(m_label, 2);
		}

		setTouchEnabled(true);

		needsLayout();

		addTargetWithActionForControlEvents(this, 
			cccontrol_selector(CCControlCheckBox::saveValue), 
			CCControlEventValueChanged);

		setIsChecked(m_checked);

		return true;
	}

	//------------------------------------------------------------------------------
	void CCControlCheckBox::setIsChecked(bool val)
	{
		bool previousValue(m_checked);
		m_checked=val;
		if (m_checked)
		{
			m_checkedSprite->setVisible(true);
			m_uncheckedSprite->setVisible(false);
		}
		else
		{
			m_checkedSprite->setVisible(false);
			m_uncheckedSprite->setVisible(true);
		}
		if(previousValue != m_checked)	{
			sendActionsForControlEvents(CCControlEventValueChanged);
		}
	}

	//------------------------------------------------------------------------------
	void CCControlCheckBox::saveValue(CCObject* o, CCControlEvent e)
	{
		serialize(getDataSource());
	}

	//------------------------------------------------------------------------------
	void CCControlCheckBox::needsLayout()
	{
		if (m_checkedSprite == nullptr) {
			return;
		}
		if (m_uncheckedSprite == nullptr) {
			return;
		}
		if (m_label == nullptr) {
			return;
		}
		if (getBackGroundSprite() == nullptr) {
			return;
		}

		float lblWidth = m_label ? m_label->getContentSize().width : 0;
		float inputWidth = m_uncheckedSprite ? m_uncheckedSprite->getContentSize().width : 0;
		float lblHeight = m_label ? m_label->getContentSize().height : 0;
		float inputHeight = m_uncheckedSprite ? m_uncheckedSprite->getContentSize().height : 0;
		float padding=10;
		float width =
			m_marginH + 
			padding + lblWidth + 
			padding + inputWidth + 
			m_marginH;

		float height= m_marginV + 
			std::max(inputHeight, lblHeight)+m_marginV;

		CCSize preferredSize(getPreferredSize());
		if(width != preferredSize.width && preferredSize.width != 0)	{
			width = preferredSize.width;
		}
		CCSize size = CCSizeMake(width, height);
		setPreferredSize(size);

		SLBaseClass::needsLayout();

		float y = height * 0.5f;
		if (m_uncheckedSprite) {
			m_uncheckedSprite->setPosition(ccp( m_marginH + (m_uncheckedSprite->getContentSize().width * 0.5), y ));
		}
		if (m_checkedSprite) {
			m_checkedSprite->setPosition(ccp( m_marginH + (m_checkedSprite->getContentSize().width * 0.5), y ));
		}

		CCPoint textPosition(m_checkedSprite->getPosition());
		textPosition.x += m_checkedSprite->getContentSize().width * 0.5f + padding + lblWidth * 0.5f;
		m_label->setPosition( ccp( textPosition.x, y ));
	}

	//------------------------------------------------------------------------------
	bool CCControlCheckBox::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
	{
		if (!isTouchInside(pTouch) || !isEnabled())
		{
			return false;
		}
    	
		sendActionsForControlEvents(CCControlEventTouchDown);
		return true;
	}

	//------------------------------------------------------------------------------
	void CCControlCheckBox::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
	{    
		if (isTouchInside(pTouch))
		{
			setIsChecked(!m_checked);
		}	
	}

	void CCControlCheckBox::serialize( CCDictionary* target )
	{
		if( canSerializeTo(target) == false)	{
			return;
		}

		CCObject* value(getCtrlValue());

		if(value == nullptr)	{
			value = CCBool::create(m_checked);
		}
		CCObject* newValue(ValueConversion::setValueFromString(CCString::create(m_checked ? "true" : "false"),value));
		if(newValue != value)	{
			target->setObject(newValue, getValueKey());
			// now we reserialize as the value might get validated ...
			deserialize(target);
		}
	}

	void CCControlCheckBox::deserialize( CCDictionary* source )
	{
		if( canDeserializeFrom(source) == false)	{
			return;
		}
		SLBaseClass::deserialize(source);
		CCObject* value(getCtrlValue());
		CCBool* boolValue(dynamic_cast<CCBool*>(value));
		if(boolValue != nullptr)	{
			setIsChecked(boolValue->getValue());
		}
	}


}


