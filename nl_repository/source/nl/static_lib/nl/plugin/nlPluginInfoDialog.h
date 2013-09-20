//!---------------------------------------------------------------------------
//! \file "nlPluginInfoDialog.h"
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
#ifndef __NLPLUGININFODIALOG_H__
#define __NLPLUGININFODIALOG_H__

namespace nl	{


	class PluginInfoDialog : public Dialog	{
		SL_DECLARE_BASE(Dialog)
	public:
		CREATE_FUNC(PluginInfoDialog)

		virtual bool init();

		virtual void onUIAction(CCObject *sender, CCControlEvent controlEvent);

	protected:
		PluginInfoDialog();
		virtual ~PluginInfoDialog();
	};



}

#endif // __NLPLUGININFODIALOG_H__
