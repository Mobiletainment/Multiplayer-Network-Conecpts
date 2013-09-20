//!---------------------------------------------------------------------------
//! \file "slNotification.cpp"
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

#include "slNotification.h"

namespace sl	{

	Notification::Notification(const char *name)
		:_registeredObserver(false)
		,_target(nullptr)
		,_selector(nullptr)
		,_name(name)
		,_obj(nullptr)
	{

	}

	Notification::~Notification()
	{
		removeObserver();
	}

	void Notification::addObserver( 
		CCObject *target, 
		SEL_CallFuncO selector, 
		CCObject *obj ) const
	{
		if(_registeredObserver)	{
			removeObserver();
		}

		CCNotificationCenter::sharedNotificationCenter()->addObserver(
			target, selector, _name, obj);
		_registeredObserver = true;
		_target = target;
		_selector = selector;
		_obj = obj;
	}

	void Notification::removeObserver() const
	{
		if(_registeredObserver)	{
			CCNotificationCenter::sharedNotificationCenter()->removeObserver(_target, _name);
			_registeredObserver = false;
			_target = nullptr;
			_selector = nullptr;
			_obj = nullptr;
		}
	}

	void Notification::postNotification(CCObject *object) const	{
		CCNotificationCenter::sharedNotificationCenter()->postNotification(_name, object);
	}

}


