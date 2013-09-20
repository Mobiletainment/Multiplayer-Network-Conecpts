//!---------------------------------------------------------------------------
//! \file "slDialog.cpp"
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
#include "slDialog.h"
#include "../controls/CCControlRow.h"
#include "../slControlUtils.h"
#include "../slVisibleRect.h"



namespace sl	{


	Dialog::Dialog()
		:_ctrlColumn(nullptr)
	{
		registerResizeObserver();
	}

	Dialog::~Dialog()
	{
		_ctrlColumn = nullptr;
		deregisterResizeObserver();
	}

#if 0
	bool Dialog::initWhithSubControl(CCControlBase* subControl)	{
		return true;
	}
#endif

	bool Dialog::init()	{
		CCSize ctrlSize(CCSizeMake(200,200));

		// first create the ok cancel buttons
		CCControlRow* okCancelBtns = createOkCancelBtns();
		ctrlSize.width = okCancelBtns->getContentSize().width;
		ctrlSize.height = 200;

		setPreferredSize(ctrlSize);
//		bool initialized = initWithBackGroundSprite("controls/button.png");
		bool initialized = SLBaseClass::init();

		addChild(okCancelBtns);

		return initialized;
	}

	void Dialog::onUIAction( CCObject *sender, CCControlEvent controlEvent )
	{
		bool doRemoveFromParent(false);
		// determine the sender tag to dispatch the action
		const int tag(Dialog::getTag(sender));
		switch(tag)	{
		case SL_CTRLID_OK:	{
			serializeAll(getDataSource());
			doRemoveFromParent = true;
							}
			break;
		case SL_CTRLID_CANCEL:
			doRemoveFromParent = true;
			break;
		}

		if(doRemoveFromParent)	{
			removeFromParent();
		}
	}

	CCControlRow* Dialog::createOkBtn()	{
		CCSize btnContainerSize(CCSizeMake(0,0));
		CCArray* columnCtrls = CCArray::create();

		CCControlButton* ctrlBtn;
		ctrlBtn = sl::ControlUtils::createButton("OK");
		ctrlBtn->setTag(SL_CTRLID_OK);
		ctrlBtn->addTargetWithActionForControlEvents(
			this, 
			cccontrol_selector(Dialog::onUIAction), 
			CCControlEventTouchUpInside);
		columnCtrls->addObject(ctrlBtn);

		CCControlRow* btnContainer = ControlUtils::createControlRow(columnCtrls,btnContainerSize);
		return btnContainer;
	}

	CCControlRow* Dialog::createOkCancelBtns()	{
		CCSize btnContainerSize(CCSizeMake(0,0));
		CCArray* columnCtrls = CCArray::create();

		CCControlButton* ctrlBtn;
		ctrlBtn = sl::ControlUtils::createButton("OK");
		ctrlBtn->setTag(SL_CTRLID_OK);
		ctrlBtn->addTargetWithActionForControlEvents(
			this, 
			cccontrol_selector(Dialog::onUIAction), 
			CCControlEventTouchUpInside);
		columnCtrls->addObject(ctrlBtn);

		ctrlBtn = sl::ControlUtils::createButton("Cancel");
		ctrlBtn->setTag(SL_CTRLID_CANCEL);
		ctrlBtn->addTargetWithActionForControlEvents(
			this, 
			cccontrol_selector(Dialog::onUIAction), 
			CCControlEventTouchUpInside);
		columnCtrls->addObject(ctrlBtn);

		CCControlRow* btnContainer = ControlUtils::createControlRow(columnCtrls,btnContainerSize);
		return btnContainer;
	}

	void Dialog::centerOnScreen()	{
		CCPoint center(sl::VisibleRect::center());
		center.x -= getContentSize().width * 0.5f;
		center.y -= getContentSize().height * 0.5f;
		setPosition(center);
	}

	void Dialog::onAppResized( CCObject* app )	{
		centerOnScreen();
	}

	void Dialog::show()
	{
		// determine the main layer
		// and add the dialog there
		CCDirector* director(CCDirector::sharedDirector());
		CCScene* currentScene(director->getRunningScene());
		if(currentScene != nullptr)	{
			Dialog* modalLayer(this);
			if(modalLayer != nullptr)	{
				modalLayer->centerOnScreen();
				currentScene->addChild(modalLayer, 100);
			}
		}
	}

	int Dialog::getTag( CCObject* object )
	{
		CCNode* node(dynamic_cast<CCNode*>(object));
		if(node == nullptr)	{
			SL_PROCESS_APP()->log(ELogType_Error, "%s CCObject is not a node", __FUNCTION__);
			return -1;
		}
		return node->getTag();
	}

}


