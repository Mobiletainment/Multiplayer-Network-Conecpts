//!---------------------------------------------------------------------------
//! \file "nlTankVsTankClientLogicNode.cpp"
//! \brief <brief-doc-todo>  
//! \details <details-doc-todo>
//! \author Cyrus Preuss
//!
//! This file is part of the Stateless Games SDK
//! Usage of the Stateless Games SDK is subject to an appropriate license agreement.
//!
//! Copyright (c) 2012-2013 Cyrus Preuss. All rights reserved.
//!----------------------------------------------------------------------------


#include "stdafx.h"
#include "nlTankVsTankClientLogicNode.h"

#include "network/nlGameContentReplicaManager.h"
#include "network/nlLocalPlayerReplicaComponent.h"

namespace nl	{

	TankVsTankClientLogicNode::TankVsTankClientLogicNode()
		:_gameplayLayer(nullptr)
	{
	}

	TankVsTankClientLogicNode::~TankVsTankClientLogicNode()
	{
	}

	bool TankVsTankClientLogicNode::init()	{
		bool initialized(SLBaseClass::init());


		return initialized;
	}

	void TankVsTankClientLogicNode::update( float dt )	{
		SLBaseClass::update(dt);

		SLTimeInterval accumTime(getAccumTime());
	}

	void TankVsTankClientLogicNode::onEnter()	{
		SLBaseClass::onEnter();
	}

	void TankVsTankClientLogicNode::onExit()	{
		setGameplayLayer(nullptr);
		//setLocalPlayerReplicaComponent(nullptr);
		SLBaseClass::onExit();
	}

	void TankVsTankClientLogicNode::onNewIncommingConnection(PeerWrapper* peerWrapper)	{
		getPeer()->log( ELogType_Info, "%s - received new incomming connection from %s with GUID %s", 
			getClassName(),
			peerWrapper->getSystemAddress().ToString(true), peerWrapper->getGUID().ToString());
			
	}

	void TankVsTankClientLogicNode::onConnectionLost(PeerWrapper* peerWrapper)	{
		getPeer()->log( ELogType_Info, "%s - received lost connection from %s with GUID %s", 
			getClassName(),
			peerWrapper->getSystemAddress().ToString(true), peerWrapper->getGUID().ToString());

		//setLocalPlayerReplicaComponent(nullptr);
	}

	void TankVsTankClientLogicNode::onConnectionDisconnected(PeerWrapper* peerWrapper)	{
		getPeer()->log( ELogType_Info, "%s - received dissonnected from %s with GUID %s", 
			getClassName(),
			peerWrapper->getSystemAddress().ToString(true), peerWrapper->getGUID().ToString());
	}

	void TankVsTankClientLogicNode::onPeerIsConnected(PeerWrapper* peerWrapper)	{
		getPeer()->log(ELogType_Info, "%s - received peer is connected", getClassName());

		// TODO @student : 1. create a replica for the local player
		// TODO @student : this is your entry point to let it all work

		getPeer()->log(ELogType_Info, "%s - creating local player replica ...", getClassName());

		

		// we need the replica manager here
		ReplicaManager* replicaManager(getPeer()->getReplicaManager());
		if(replicaManager != nullptr)	{
			// to create the local player replica
			AbstractReplica* localPlayerReplica(replicaManager->createReplica(LocalPlayerReplicaComponent::staticClassName(), nullptr));
			if(localPlayerReplica != nullptr)	{
				//TODO: @David we need the local player replica as we need to update it's update tick value once we've received the tankReplicaObject
				GameObjectReplica *gameObjectReplica(dynamic_cast<GameObjectReplica*>(localPlayerReplica));
				LocalPlayerReplicaComponent *replicaComponent(dynamic_cast<LocalPlayerReplicaComponent*>(gameObjectReplica->getReplicaComponent()));
				//setLocalPlayerReplicaComponent(replicaComponent);
				// now we might want to set a little data to this replica ...
				// like a name etc.
				// or provide a construction dictionary
				
			}
		}

		
	}

	void TankVsTankClientLogicNode::onPeerConnectionAttemptFailed(PeerWrapper* peerWrapper)	{
		getPeer()->log(ELogType_Info, "%s - received peer connection attempt failed", getClassName());
	}

	void TankVsTankClientLogicNode::onPeerFailedToConnect(PeerWrapper* peerWrapper)	{
		getPeer()->log(ELogType_Info, "%s - received peer failed to connected", getClassName());
	}

	void TankVsTankClientLogicNode::onPeerWillDisconnect(PeerWrapper* peerWrapper)	{
		getPeer()->log(ELogType_Info, "%s - received peer will disconnect", getClassName());
	}


}


