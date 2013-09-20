//!---------------------------------------------------------------------------
//! \file "nlPluginOptionsDialog.h"
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
#ifndef __NLPLUGINOPTIONSDIALOG_H__
#define __NLPLUGINOPTIONSDIALOG_H__

namespace nl	{

	class PluginLayerContent;

	class PluginOptionsDialog : public Dialog	{
		SL_DECLARE_BASE(Dialog)
	public:
		CREATE_FUNC(PluginOptionsDialog)

		virtual bool init() SL_OVERRIDE;
		virtual void onEnter() SL_OVERRIDE;
		virtual void onExit() SL_OVERRIDE;

		virtual void onUIAction(CCObject *sender, CCControlEvent controlEvent) SL_OVERRIDE;

		void onPhysicsFrameRateChangedNotification(CCObject* sender);

		SL_SYNTHESIZE_STATIC(PluginLayerContent*, _pluginLayerContent, PluginLayerContent)
		SL_SYNTHESIZE(CCControlEditBox*, _ctrlPhysicsFrameRate, PhysicsFrameRate)
	protected:
		PluginOptionsDialog();
		virtual ~PluginOptionsDialog();
	private:
		Notification _notificationPhysicsFrameRateChanged;

	};



}

#endif // __NLPLUGINOPTIONSDIALOG_H__
