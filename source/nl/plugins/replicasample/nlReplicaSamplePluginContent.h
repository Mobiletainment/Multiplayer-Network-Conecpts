//!---------------------------------------------------------------------------
//! \file "nlReplicaSamplePluginContent.h"
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
#ifndef __NLREPLICASAMPLEPLUGINCONTENT_H__
#define __NLREPLICASAMPLEPLUGINCONTENT_H__


namespace nl	{

	class SampleReplicaManager;

	class ReplicaManagerCreator : public IRakNetPluginCreator
	{
	public:
		ReplicaManagerCreator();

		virtual RakNet::PluginInterface2* createPlugin(Peer* parentPeer, ENetwokTopology topolgy);

		virtual void destroyPlugin(RakNet::PluginInterface2* plugin);

		SL_SYNTHESIZE(SampleReplicaManager*, _replicaManager, ReplicaManager)
	};


	class ReplicaManagerUIActionDispatcher : public CCNode	{
		SL_DECLARE_BASE(CCNode)
	public:
		CREATE_FUNC(ReplicaManagerUIActionDispatcher)

		virtual bool init();

		void onUIAction( CCObject *sender, CCControlEvent controlEvent );

		ReplicaManagerCreator _replicaManagerCreator;
	};


	// a plugin specific implementation
	class ReplicaSamplePluginContent : public PluginLayerContent	{
		SL_DECLARE_BASE(PluginLayerContent)
	public:
		ReplicaSamplePluginContent();

		CREATE_FUNC(ReplicaSamplePluginContent)

		SL_IMPLEMENT_CLASSNAME(ReplicaSamplePluginContent)

		virtual bool init();

		virtual void addRootMenuControls(CCArray* ctrlRowArray);

		virtual void addSubLayerContent(
			SLSize idx, 
			CCControlBase* twinLayer, 
			CCControlBase* leftLayer, 
			CCControlBase* rightLayer);

		virtual void onUIAction(CCObject *sender, CCControlEvent controlEvent);

		void addGameContent( SLSize idx, PeerNode* peerNode, CCControlBase* parentLayer );

		SL_SYNTHESIZE(bool, _isClientServerMode, ClientServerMode)
	private:

	};

}


#endif // __NLREPLICASAMPLEPLUGINCONTENT_H__
