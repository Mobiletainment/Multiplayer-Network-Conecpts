//!---------------------------------------------------------------------------
//! \file "nlTankVsTankPluginContent.h"
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
#ifndef __NLTANKVSTANKPLUGINCONTENT_H__
#define __NLTANKVSTANKPLUGINCONTENT_H__

namespace sl	{
	class GameplayLayer;
	class GameActorNode;
}

namespace nl	{

	class TankVsTankGameplayLayer;

	//! @brief TankVsTankPluginContent features a replica based multi player game implementation
	//!
	class TankVsTankPluginContent : public PluginLayerContent	{
		SL_DECLARE_BASE(PluginLayerContent)
	public:
		TankVsTankPluginContent();

		CREATE_FUNC(TankVsTankPluginContent)

		SL_IMPLEMENT_CLASSNAME(TankVsTankPluginContent)

		virtual bool init();

		virtual void addRootMenuControls(CCArray* ctrlRowArray);

		virtual void addSubLayerContent(
			SLSize idx, 
			CCControlBase* twinLayer, 
			CCControlBase* leftLayer, 
			CCControlBase* rightLayer);

		virtual void onUIAction(CCObject *sender, CCControlEvent controlEvent);

		TankVsTankGameplayLayer* addGameplayLayer(CCLayer* parentLayer);
		void addGameContent( SLSize idx, PeerNode* peerNode, CCControlBase* parentLayer );
		void addGameContentUI( SLSize idx, PeerNode* peerNode, TankVsTankGameplayLayer* parentLayer );

	protected:
		~TankVsTankPluginContent();
	private:

		GameActorNode* addActorNode( 
			CCControlBase* parentLayer, 
			const char* textureName, 
			const CCPoint& position );

		void addSinglePlayerGameContent( CCControlBase* parentLayer );


	};


}


#endif // __NLPEDESTRIANSAMPLEPLUGINCONTENT_H__
