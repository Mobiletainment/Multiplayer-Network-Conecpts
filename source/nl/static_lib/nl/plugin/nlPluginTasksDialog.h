//!---------------------------------------------------------------------------
//! \file "nlPluginTasksDialog.h"
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
#ifndef __NLPLUGINTASKSDIALOG_H__
#define __NLPLUGINTASKSDIALOG_H__

namespace nl	{


	class PluginTasksDialog : public Dialog	{
		SL_DECLARE_BASE(Dialog)
	public:
		CREATE_FUNC(PluginTasksDialog)

		virtual bool init();

		virtual void onUIAction(CCObject *sender, CCControlEvent controlEvent);

	protected:
		PluginTasksDialog();
		virtual ~PluginTasksDialog();
	};



}

#endif // __NLPLUGINTASKSDIALOG_H__
