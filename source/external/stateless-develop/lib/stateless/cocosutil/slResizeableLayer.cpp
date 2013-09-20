//!---------------------------------------------------------------------------
//! \file "slResizeableLayer.cpp"
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
#include "slResizeableLayer.h"
#include "slVisibleRect.h"


namespace sl	{
	AbstractResizableLayer::AbstractResizableLayer()	{
		CCNotificationCenter* notificationCenter(CCNotificationCenter::sharedNotificationCenter());
		notificationCenter->addObserver(this, callfuncO_selector(ResizableLayer::onAppResized), SL_NOTIFY_APPRESIZED, NULL );
	}

	AbstractResizableLayer::~AbstractResizableLayer()	{
		CCNotificationCenter* notificationCenter(CCNotificationCenter::sharedNotificationCenter());
		notificationCenter->removeObserver(this, SL_NOTIFY_APPRESIZED);
	}

	ResizableLayer::ResizableLayer()
	{

	}

	ResizableLayer::~ResizableLayer()
	{

	}

	void ResizableLayer::onAppResized( CCObject* app )
	{
		CCNode* parent(getParent());
		if(dynamic_cast<CCScene*>(parent) != nullptr)	{
			CCRect visibleRect(VisibleRect::getVisibleRect());
			parent->setContentSize(visibleRect.size);
			setContentSize(visibleRect.size);
		}
	}

}


