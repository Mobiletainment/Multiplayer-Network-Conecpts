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
#include "network/nlTankPlayerReplicaComponent.h"
#include "network/nlTankReplicaComponent.h"
#include "network/nlLocalPlayerReplicaComponent.h"

namespace nl	{

	static size_t PLAYER_LIMIT = 4;

	class TankVsTankGameLogicNode : public PeerObserverNode
	{
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

		// new by David
		template <class T>
		T* getComponentFromActorNode(ActorNode *actorNode)
		{
			// find the TankPlayerReplica component by iterating over all components of the GameActor until we find it
			ComponentArray* components(actorNode->getComponents());
			SLSize idx(0);
			IComponent* component(components->componentAt(idx));
			while(component != nullptr)
			{
				T* replicaComponent(dynamic_cast<T*>(component));

				if(replicaComponent != nullptr)
				{
					return replicaComponent;
				}
				++idx;
				component = components->componentAt(idx);
			}

			return nullptr;
		}

		TankReplicaComponent* getTankReplicaComponentFromActorNode(ActorNode *actorNode);


		static CCDictionary* createTankCreationDictionary();

		static 	CCDictionary* createTankCreationDictionary( 
			const char* textureName, 
			const CCPoint& position );


		SL_SYNTHESIZE(GameplayLayer*, _gameplayLayer, GameplayLayer);
	
	protected:
		TankVsTankGameLogicNode();
		virtual ~TankVsTankGameLogicNode();

		DataStructures::List<Replica3*> _destructionReplicaList;
		//SL_SYNTHESIZE(CCArray*, _spectators, Spectators);
		
		CCArray *_spectators; //this list provides the order of which spectator becomes active next
		unsigned int _totalPlayers;

	private:

		static void TankVsTankGameLogicNode::createTankProjectileCreationDictionary( 
			CCDictionary* parentDictionary);	

		static 	CCDictionary* createTankCreationDictionary( 
			SLSize idx ); 

	};



}


#endif // __NLTANKVSTANKEGAMELOGICNODE_H__
