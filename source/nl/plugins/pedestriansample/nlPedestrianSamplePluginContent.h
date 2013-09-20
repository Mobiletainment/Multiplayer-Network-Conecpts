//!---------------------------------------------------------------------------
//! \file "nlPedestrianSamplePluginContent.h"
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
#ifndef __NLPEDESTRIANSAMPLEPLUGINCONTENT_H__
#define __NLPEDESTRIANSAMPLEPLUGINCONTENT_H__

namespace sl	{
	class GameplayLayer;
}

namespace nl	{


	//! @brief PedestrianSamplePluginContent features a replica based simulation implementation
	//!
	class PedestrianSamplePluginContent : public PluginLayerContent	{
		SL_DECLARE_BASE(PluginLayerContent)
	public:
		PedestrianSamplePluginContent();

		CREATE_FUNC(PedestrianSamplePluginContent)

		SL_IMPLEMENT_CLASSNAME(PedestrianSamplePluginContent)

		virtual bool init();

		virtual void addRootMenuControls(CCArray* ctrlRowArray);

		virtual void addSubLayerContent(
			SLSize idx, 
			CCControlBase* twinLayer, 
			CCControlBase* leftLayer, 
			CCControlBase* rightLayer);

		virtual void onUIAction(CCObject *sender, CCControlEvent controlEvent);

		GameplayLayer* addGameplayLayer(CCLayer* parentLayer);
		void addGameContent( SLSize idx, PeerNode* peerNode, CCControlBase* parentLayer );
		void addGameContentUI( SLSize idx, PeerNode* peerNode, GameplayLayer* parentLayer );

	private:

	};


}


#endif // __NLPEDESTRIANSAMPLEPLUGINCONTENT_H__
