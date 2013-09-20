//!---------------------------------------------------------------------------
//! \file "nlChatSamplePluginContent.h"
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
#ifndef __NLCHATSAMPLEPLUGINCONTENT_H__
#define __NLCHATSAMPLEPLUGINCONTENT_H__

namespace nl	{

	//! @brief ChatSamplePluginContent features a simple chat implementation
	//!
	class ChatSamplePluginContent : public PluginLayerContent	{
		SL_DECLARE_BASE(PluginLayerContent)
	public:
		ChatSamplePluginContent();
		virtual ~ChatSamplePluginContent();

		CREATE_FUNC(ChatSamplePluginContent)

		SL_IMPLEMENT_CLASSNAME(ChatSamplePluginContent)

		virtual bool init();

		virtual void addRootMenuControls(CCArray* ctrlRowArray);

		virtual void addSubLayerContent(
			SLSize idx, 
			CCControlBase* twinLayer, 
			CCControlBase* leftLayer, 
			CCControlBase* rightLayer);

		virtual void onUIAction(CCObject *sender, CCControlEvent controlEvent);

	};

}


#endif // __NLCHATSAMPLEPLUGINCONTENT_H__
