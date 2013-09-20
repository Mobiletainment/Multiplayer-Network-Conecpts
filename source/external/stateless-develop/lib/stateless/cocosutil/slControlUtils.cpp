//!---------------------------------------------------------------------------
//! \file "slControlUtils.cpp"
//! \brief <brief-doc-todo>  
//! \details <details-doc-todo>
//! \author Cyrus Preuss
//!
//! This file is part of the Stateless Games SDK
//! Usage of the Stateless Games SDK is subject to an appropriate license agreement.
//!
//! Copyright (c) 2012-2013 Cyrus Preuss. All rights reserved.
//!----------------------------------------------------------------------------


#include "stdAfx.h"

#include "slControlUtils.h"

namespace sl	{


	CCControlButton *ControlUtils::createButton(const char * title)
	{
		CCScale9Sprite *backgroundButton(CCScale9Sprite::create("controls/button.png"));
		CCScale9Sprite *backgroundHighlightedButton(CCScale9Sprite::create("controls/buttonHighlighted.png"));

		CCLabelTTF *titleButton = CCLabelTTF::create(title, NL_UI_CTRL_FONTNAME, NL_UI_LBL_FONTSIZE);
		titleButton->setColor(ccc3(0, 255, 0));

		CCControlButton *button = CCControlButton::create(titleButton, backgroundButton);
		button->setBackgroundSpriteForState(backgroundHighlightedButton, CCControlStateHighlighted);
		button->setTitleColorForState(ccWHITE, CCControlStateHighlighted);

		return button;
	}
	

	CCControlCheckBox* ControlUtils::createCheckBox(
		const char* checkedIconSpriteName,
		const char* uncheckedIconSpriteName,
		CCScale9Sprite* backgroundSprite,
		const char* title, const CCSize& preferredSize
		)
	{
		CCSprite *checkedSprite = CCSprite::create(checkedIconSpriteName);
		CCSprite *uncheckedSprite = CCSprite::create(uncheckedIconSpriteName);
		CCNode* label(createLabel(title, kCCTextAlignmentLeft));
		CCControlCheckBox* checkbox = CCControlCheckBox::checkBoxFromSprites(checkedSprite, 
			uncheckedSprite, 
			backgroundSprite, 
			label, preferredSize);
		return checkbox;
	}

	CCControlCheckBox* ControlUtils::createCheckBox( const char* title, 
		const CCSize& preferredSize )
	{
		return ControlUtils::createCheckBox(
			"controls/ctrl_check_box_checked.png",
			"controls/ctrl_check_box_unchecked.png",
			ControlUtils::createOpaqueCtrlBackground(),
			title, preferredSize
			);
	}

	CCControlEditBox* ControlUtils::createEditBox( 
		const char* title, const CCSize& preferredSize )
	{
		CCNode* label(createLabel(title));
		return CCControlEditBox::editBoxFromSprites(
			ControlUtils::createOpaqueCtrlBackground(),
			label,
			preferredSize
			);
	}

	CCControlBase* ControlUtils::createBaseControl( const CCSize& ctrlSize, CCScale9Sprite* backgroundSprite )
	{
		CCControlBase* ctrl = new CCControlBase();
		ctrl->autorelease();
		ctrl->setPreferredSize(ctrlSize);
		if(backgroundSprite == nullptr)	{
			ctrl->initWithBackGroundSprite("controls/button.png");
		}
		else	{
			ctrl->initWithBackGroundSprite(backgroundSprite);
		}
		return ctrl;
	}

	CCNode* ControlUtils::createLabel( const char* title, CCTextAlignment alignment )
	{
		CCNode* label(nullptr);
		if (isEmptyString(title) == false) {
			CCLabelTTF *titleLbl = CCLabelTTF::create(title, NL_UI_LBL_FONTNAME, NL_UI_LBL_FONTSIZE);
			titleLbl->setColor(NL_UI_LBL_COLOR);
			titleLbl->setHorizontalAlignment(alignment);
			label = titleLbl;
		}
		return label;
	}

	CCControlTwinLayer* ControlUtils::createTwinControl( const CCSize& ctrlSize,
		CCString* leftLayerClassName, CCString* rightLayerClassName)
	{
		CCControlTwinLayer* ctrl(new CCControlTwinLayer());
		ctrl->setLeftLayerClassName(leftLayerClassName);
		ctrl->setRightLayerClassName(rightLayerClassName);
		ctrl->autorelease();
		ctrl->setPreferredSize(ctrlSize);
		ctrl->initWithBackGroundSprite((CCScale9Sprite*)nullptr);
		return ctrl;
	}

	CCControlQuadLayer* ControlUtils::createQuadControl( 
		const CCSize& ctrlSize, SLSize numSublayers,
		CCString* leftLayerClassName, CCString* rightLayerClassName
)
	{
		CCControlQuadLayer* ctrl(new CCControlQuadLayer());
		ctrl->setLeftLayerClassName(leftLayerClassName);
		ctrl->setRightLayerClassName(rightLayerClassName);
		ctrl->setNumSubLayers(numSublayers);
		ctrl->autorelease();
		ctrl->setPreferredSize(ctrlSize);
		ctrl->initWithBackGroundSprite(ControlUtils::createMainViewBackground());
		return ctrl;
	}

	CCControlValueBox* ControlUtils::createValueBox( 
		const char* title, 
		const CCSize& preferredSize,
		const char* text )
	{
		CCNode* label(nullptr);

		if(isEmptyString(title) == false)	{
			label = createLabel(title);
		}

		return CCControlValueBox::valueBoxFromSprites(
			ControlUtils::createOpaqueCtrlBackground(),
			label,
			preferredSize,
			text
			);
	}

	CCScale9Sprite* ControlUtils::createSubViewBackground()
	{
		CCScale9Sprite* backgroundSprite(CCScale9Sprite::create("views/subview.png"));
		backgroundSprite->setInsetLeft(8);
		backgroundSprite->setInsetTop(8);
		backgroundSprite->setInsetRight(8);
		backgroundSprite->setInsetBottom(8);
		return backgroundSprite;
	}

	CCScale9Sprite* ControlUtils::createCtrlBackground()
	{
		CCScale9Sprite* backgroundSprite(CCScale9Sprite::create("controls/ctrl_frame_37x37.png"));
		backgroundSprite->setInsetLeft(8);
		backgroundSprite->setInsetTop(8);
		backgroundSprite->setInsetRight(8);
		backgroundSprite->setInsetBottom(8);
		return backgroundSprite;
	}

	CCScale9Sprite* ControlUtils::createOpaqueCtrlBackground()
	{
		CCScale9Sprite* backgroundSprite(CCScale9Sprite::create("controls/ctrl_background_37x37.png"));
		backgroundSprite->setInsetLeft(8);
		backgroundSprite->setInsetTop(8);
		backgroundSprite->setInsetRight(8);
		backgroundSprite->setInsetBottom(8);
		return backgroundSprite;
	}


	CCScale9Sprite* ControlUtils::createMainViewBackground()
	{
		CCScale9Sprite* backgroundSprite(CCScale9Sprite::create("views/lecture_background_01.png"));
		backgroundSprite->setInsetLeft(8);
		backgroundSprite->setInsetTop(8);
		backgroundSprite->setInsetRight(8);
		backgroundSprite->setInsetBottom(8);
		return backgroundSprite;
	}


	CCControlRow* ControlUtils::createControlRow( CCArray* nodes, const CCSize& preferredSize )
	{
		return CCControlRow::controlRowFromNodes(
			ControlUtils::createCtrlBackground(),
			nodes, preferredSize);
	}

	CCControlColumn* ControlUtils::createControlColumn( CCArray* nodes, const CCSize& preferredSize )
	{
		return CCControlColumn::controlColumnFromNodes(
			ControlUtils::createCtrlBackground(),
			nodes, preferredSize);
	}

	void ControlUtils::addCtrlNode(CCNode* node, CCNode* toLayer)	{
		CCControlContainer* ctrlContainer(dynamic_cast<CCControlContainer*>(toLayer));
		if(ctrlContainer)	{
			ctrlContainer->addNode(node);
		}
		else	{
			toLayer->addChild(node);
		}
	}



}
