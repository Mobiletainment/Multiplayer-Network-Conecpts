//!---------------------------------------------------------------------------
//! \file "nlGameContentReplicaManager.cpp"
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
#include "nlGameContentReplicaManager.h"

#include "nlGameStateReplicaComponent.h"
#include "nlPlayerReplicaComponent.h"
#include "nlLocalPlayerReplicaComponent.h"
#include "nlTankPlayerReplicaComponent.h"
#include "nlTankReplicaComponent.h"
#include "nlTankProjectileReplicaComponent.h"
#include "components/slTankWeaponComponent.h"

#include <stateless/cocosgame/slCocosGame.h>

#include "slCompressed.h"
namespace nl	{

	//-------------------------------------------------------------------------
	// ReplicaManagerCreator
	//-------------------------------------------------------------------------

	ReplicaManagerCreator::ReplicaManagerCreator()
		:_replicaManager(nullptr)
		,_gameplayLayer(nullptr)
	{

	}

	RakNet::PluginInterface2* ReplicaManagerCreator::createPlugin(Peer* parentPeer, ENetwokTopology topolgy)	{
		_replicaManager = new GameContentReplicaManager(topolgy);
		_replicaManager->setGameplayLayer(_gameplayLayer);
		_replicaManager->setPeer(parentPeer);
		return _replicaManager;
	}

	void ReplicaManagerCreator::destroyPlugin(RakNet::PluginInterface2* plugin)	{
		delete plugin;
		_replicaManager = nullptr;
	}

	//-------------------------------------------------------------------------
	// GameContentConnection
	//-------------------------------------------------------------------------

	GameContentConnection::GameContentConnection(const SystemAddress &_systemAddress, RakNetGUID _guid) : Connection_RM3(_systemAddress, _guid) 
	{

	}

	GameContentConnection::~GameContentConnection() 
	{

	}

	bool GameContentConnection::QueryGroupDownloadMessages(void) const {
		// See documentation - Makes all messages between ID_REPLICA_MANAGER_DOWNLOAD_STARTED and
		// ID_REPLICA_MANAGER_DOWNLOAD_COMPLETE arrive in one tick
		return true;
	}

	Replica3* GameContentConnection::AllocReplica(RakNet::BitStream *allocationId, ReplicaManager3 *replicaManager3)
	{
		AbstractReplica* replica(nullptr);
		RakNet::RakString typeName;
		allocationId->Read(typeName);

		GameContentReplicaManager* replicaManager(dynamic_cast<GameContentReplicaManager*>(replicaManager3));
#if 0
		replicaManager->getPeer()->log(ELogType_Message, "allocate replica: %s ...", typeName.C_String());
#endif
		ReplicaComponent* replicaComponent(nullptr);

		// Done: @David : create related replica components
		if(typeName == nlGameStateReplicaComponent::staticClassName())
		{
			replicaComponent = nlGameStateReplicaComponent::create();
			replica = replicaComponent->getReplica();
		}		

		if (typeName == LocalPlayerReplicaComponent::staticClassName())
		{
			replicaComponent = LocalPlayerReplicaComponent::create();
			replica = replicaComponent->getReplica();
		}
		else if (typeName == TankPlayerReplicaComponent::staticClassName())
		{
			replicaComponent = TankPlayerReplicaComponent::create();
			replica = replicaComponent->getReplica();
		}
		else if (typeName == TankReplicaComponent::staticClassName())
		{
			replicaComponent = TankReplicaComponent::create();
			replica = replicaComponent->getReplica();
		}
		else if (typeName == TankProjectileReplicaComponent::staticClassName())
		{
			replicaComponent = TankProjectileReplicaComponent::create();
			replica = replicaComponent->getReplica();
		}


		if(replicaComponent != nullptr)	{
			// right now add the replica component to the replica manager itself
			// might be interesting for debugging, or dumping ...
			replicaManager->getRemoteComponents()->addComponent(replicaComponent);
		}

		// now let the replica component do the actor setup
		if(replica != nullptr)	{

			if(replica->getName() != typeName)	{
				replicaManager->getPeer()->log(ELogType_Error, "allocated replica: %s instead of %s", 
					replica->getName().C_String(), typeName.C_String());
			}

			replica->setPeer(replicaManager->getPeer());

			// now let the replica component do the actor setup
			if(replicaComponent != nullptr)	{
				replicaComponent->setGameplayLayer(replicaManager->getGameplayLayer());
				replicaComponent->createActorNode(nullptr);
			}

#if 0
			replicaManager->getPeer()->log(ELogType_Message, "allocated replica: %s", 
				replica->getName().C_String());
#endif
		}
		else	{
			replicaManager->getPeer()->log(ELogType_Error, "failed to allocate replica: %s", 
				typeName.C_String());
		}

		return replica;
	}

	//-------------------------------------------------------------------------
	// GameContentReplicaManager
	//-------------------------------------------------------------------------

	GameContentReplicaManager::GameContentReplicaManager(ENetwokTopology topology)
		:SLBaseClass(topology)
		,_gameplayLayer(nullptr)
	{
		_allocatedReplicas = false;
	}

	GameContentReplicaManager::~GameContentReplicaManager()	{
	}

	RakNet::Connection_RM3* GameContentReplicaManager::AllocConnection(
		const RakNet::SystemAddress &systemAddress, 
		RakNet::RakNetGUID rakNetGUID) const {
		return new GameContentConnection(systemAddress,rakNetGUID);
	}

	RakNet::PluginReceiveResult GameContentReplicaManager::OnReceive( RakNet::Packet *packet )	{
		RakNet::PluginReceiveResult result(SLBaseClass::OnReceive(packet));
		return result;
	}

	AbstractReplica* GameContentReplicaManager::createClientReplica( 
		const char* classId, CCDictionary* constructionDictionary )
	{
		AbstractReplica* replica(nullptr);
		RakNet::RakString typeName(classId);
		ReplicaComponent* replicaComponent(nullptr);
	
		// DONE: By @David : create related replica component(s)
		if (typeName == LocalPlayerReplicaComponent::staticClassName())	{
			replicaComponent = LocalPlayerReplicaComponent::create();
		}
		else if (typeName == TankProjectileReplicaComponent::staticClassName()) { 
			replicaComponent = TankProjectileReplicaComponent::create();
		}

		if(replicaComponent != nullptr)	{
			replicaComponent->setGameplayLayer(getGameplayLayer());
			replicaComponent->createActorNode(nullptr);
			replica = replicaComponent->getReplica();
		}
		return replica;
	}

	AbstractReplica* GameContentReplicaManager::createPeerToPeerReplica( 
		const char* classId, CCDictionary* constructionDictionary )
	{
		AbstractReplica* replica(nullptr);
		// TODO @student : implement peer to peer architecture
		return replica;
	}

	AbstractReplica* GameContentReplicaManager::createServerReplica( 
		const char* classId, CCDictionary* constructionDictionary )
	{
		AbstractReplica* replica(nullptr);
		ReplicaComponent* replicaComponent(nullptr);

		RakNet::RakString typeName(classId);

		// DONE: By @David : create related replica component(s)
		/*
		... insert missing code
		*/
		if (typeName == TankPlayerReplicaComponent::staticClassName())	{
			
			replicaComponent = TankPlayerReplicaComponent::create();
		}
		else if(typeName == nlGameStateReplicaComponent::staticClassName())
		{
			replicaComponent = nlGameStateReplicaComponent::create();
		}


		else if (typeName == TankProjectileReplicaComponent::staticClassName()) { 
			replicaComponent = TankProjectileReplicaComponent::create();
		}
		else if (typeName == TankReplicaComponent::staticClassName()) { 
			replicaComponent = TankReplicaComponent::create();

		}

		if(replicaComponent != nullptr)	{

			replicaComponent->setGameplayLayer(getGameplayLayer());
			replicaComponent->createActorNode(nullptr);

			// special case for tanks
			// tanks need weapons ...
			if (typeName == TankReplicaComponent::staticClassName()) {
				TankWeaponComponent* weaponComponent(TankWeaponComponent::create());
				weaponComponent->addToActorNode(replicaComponent->getActorNode());
			}

			// create an actor sprite if needed information are provided
			if(constructionDictionary != nullptr)	{
				// note: the server creates the ActorSprite immediately
				//       while the client has to wait until
				//       GameReplicaComponent::deserializeConstruction
				//       is called
				replicaComponent->createActorSprite(constructionDictionary);
			}

			replica = replicaComponent->getReplica();
		}

		return replica;
	}

	void GameContentReplicaManager::readReplicas()	{

	}

	void GameContentReplicaManager::updateReplicas()	{

	}

	void GameContentReplicaManager::deleteReplicas()	{
		deleteCreatedReplicas(false);
	}


}


