//!---------------------------------------------------------------------------
//! \file "nlOpenSteerSamplePluginContent.h"
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
#ifndef __NLOPENSTEERSAMPLEPLUGINCONTENT_H__
#define __NLOPENSTEERSAMPLEPLUGINCONTENT_H__


namespace sl	{
	class GameplayLayer;
}

namespace nl	{


	// a plugin specific implementation
	class OpenSteerSamplePluginContent : public PluginLayerContent	{
		SL_DECLARE_BASE(PluginLayerContent)
	public:
		OpenSteerSamplePluginContent();

		CREATE_FUNC(OpenSteerSamplePluginContent)

		SL_IMPLEMENT_CLASSNAME(OpenSteerSamplePluginContent)

		virtual bool init();

		virtual void addRootMenuControls(CCArray* ctrlRowArray);

		virtual void addSubLayerContent(
			SLSize idx, 
			CCControlBase* twinLayer, 
			CCControlBase* leftLayer, 
			CCControlBase* rightLayer);

		virtual void onUIAction(CCObject *sender, CCControlEvent controlEvent);

		GameplayLayer* addGameplayLayer(CCLayer* parentLayer);
		void addGameContent( CCControlBase* parentLayer );
		void addActorNode( CCControlBase* parentLayer );

	private:

	};


}


#endif // __NLOPENSTEERSAMPLEPLUGINCONTENT_H__
