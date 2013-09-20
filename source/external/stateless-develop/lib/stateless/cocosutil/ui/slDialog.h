//!---------------------------------------------------------------------------
//! \file "slDialog.h"
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
#ifndef __SLDIALOG_H__
#define __SLDIALOG_H__

#include "../controls/CCControlBase.h"


namespace sl	{

	class CCControlColumn;
	class CCControlRow;
	class Dialog;

	class IDialogExtender	{

	public:
		virtual void extendDialog(Dialog* dialog) SL_ABSTRACT;

	protected:
		virtual ~IDialogExtender()	{

		}
	};

	//! @brief AbstractDialogExtenderNode declares a abstract definition
	//! of a dialog extension node
	class AbstractDialogExtenderNode : public CCNode, public IDialogExtender
	{
		SL_DECLARE_BASE(CCNode)
	public:

		virtual void configure() SL_ABSTRACT;

		virtual void onUIAction(CCObject *sender, CCControlEvent controlEvent) SL_ABSTRACT;
	};

	//! @brief Dialog implements a simple dialog
	//!
	class slCocosUtil_API Dialog : public CCControlBase	{
		SL_DECLARE_BASE(CCControlBase)
	public:
		CREATE_FUNC(Dialog)

		void centerOnScreen();

		//! shows this dialog at the center of the screen
		void show();

		virtual bool init();
#if 0
		virtual bool initWhithSubControl(CCControlBase* subControl);
#endif

		virtual void onAppResized(CCObject* app);

		virtual void onUIAction(CCObject *sender, CCControlEvent controlEvent);

		CCControlRow* createOkCancelBtns();
		CCControlRow* createOkBtn();

		//! determine the sender tag to dispatch the action
		static int getTag(CCObject* object);

		CC_SYNTHESIZE(CCControlColumn*, _ctrlColumn, CtrlColumn)
	protected:
		Dialog();
		virtual ~Dialog();
	};


}


#endif // __SLDIALOG_H__
