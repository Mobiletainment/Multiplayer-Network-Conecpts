//!---------------------------------------------------------------------------
//! \file "slNotification.h"
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
#ifndef __SLNOTIFICATION_H__
#define __SLNOTIFICATION_H__

namespace sl	{
	class slCocosUtil_API Notification
	{
	public:
		Notification(const char *name);
		virtual ~Notification();

		void addObserver(
			CCObject *target, 
			SEL_CallFuncO selector, 
			CCObject *obj = nullptr) const;

		void removeObserver() const;

		void postNotification(CCObject *object) const;

// TODO: sendNotification
//		void sendNotification(CCObject *object, float delay) const;
	private:
		const char* _name;
		mutable bool _registeredObserver;
		mutable CCObject* _target;
		mutable SEL_CallFuncO _selector;
		mutable CCObject *_obj;
	};
}


#endif // __SLNOTIFICATION_H__
