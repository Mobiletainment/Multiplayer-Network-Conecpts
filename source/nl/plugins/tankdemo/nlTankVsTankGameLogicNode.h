//!---------------------------------------------------------------------------
//! \file "nlTankVsTankGameLogicNode.h"
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
#ifndef __NLTANKVSTANKEGAMELOGICNODE_H__
#define __NLTANKVSTANKEGAMELOGICNODE_H__

#include <stateless/cocosgame/slCocosGame.h>

namespace nl	{



	class TankVsTankGameLogicNode : public PeerObserverNode	{
		SL_DECLARE_BASE(PeerObserverNode)
	public:
		CREATE_FUNC(TankVsTankGameLogicNode)

		SL_IMPLEMENT_CLASSNAME(TankVsTankGameLogicNode)

		virtual bool init() SL_OVERRIDE;

		virtual void update(float dt) SL_OVERRIDE;

		virtual void onEnter() SL_OVERRIDE;
		virtual void onExit() SL_OVERRIDE;

		// peer notifications
		virtual void onNewIncommingConnection(PeerWrapper* peerWrapper) SL_OVERRIDE;
		virtual void onConnectionLost(PeerWrapper* peerWrapperObject) SL_OVERRIDE;
		virtual void onConnectionDisconnected(PeerWrapper* peerWrapperObject) SL_OVERRIDE;

		virtual void onPeerIsConnected(PeerWrapper* peerWrapper) SL_OVERRIDE;
		virtual void onPeerConnectionAttemptFailed(PeerWrapper* peerWrapper) SL_OVERRIDE;
		virtual void onPeerFailedToConnect(PeerWrapper* peerWrapper) SL_OVERRIDE;
		virtual void onPeerWillDisconnect(PeerWrapper* peerWrapper) SL_OVERRIDE;

		static CCDictionary* createTankCreationDictionary();

		static 	CCDictionary* createTankCreationDictionary( 
			const char* textureName, 
			const CCPoint& position );


		SL_SYNTHESIZE(GameplayLayer*, _gameplayLayer, GameplayLayer);
	protected:
		TankVsTankGameLogicNode();
		virtual ~TankVsTankGameLogicNode();

		DataStructures::List<Replica3*> _destructionReplicaList;


	private:

		static void TankVsTankGameLogicNode::createTankProjectileCreationDictionary( 
			CCDictionary* parentDictionary);	

		static 	CCDictionary* createTankCreationDictionary( 
			SLSize idx ); 

	};



}


#endif // __NLTANKVSTANKEGAMELOGICNODE_H__