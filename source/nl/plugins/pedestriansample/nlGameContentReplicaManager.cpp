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

#include "nlStudentReplicaComponent.h"

#include <stateless/cocosgame/slCocosGame.h>

#define NL_USE_STUDENT_REPLICA_COMPONENT 1
#if NL_USE_STUDENT_REPLICA_COMPONENT
#  define REPLICA_COMPONENT_CLASS StudentReplicaComponent
#else
#  define REPLICA_COMPONENT_CLASS ReplicaComponent
#endif

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

		ReplicaComponent* replicaComponent(nullptr);
		if (typeName == GameObjectReplica::staticClassName()) { 
			replicaComponent = REPLICA_COMPONENT_CLASS::create();

			// right now add the replica component to the replica manager itself
			// might be interesting for debugging, or dumping ...
			replicaManager->getRemoteComponents()->addComponent(replicaComponent);

			replica = replicaComponent->getReplica();
		}

		if(replica != nullptr)	{
			replica->setPeer(replicaManager->getPeer());

			// now let the replica component do the actor setup
			if(replicaComponent != nullptr)	{
				replicaComponent->setGameplayLayer(replicaManager->getGameplayLayer());
				replicaComponent->createActorNode(nullptr);
			}

			replicaManager->getPeer()->log(ELogType_Message, "allocated replica: %s", 
				replica->getName().C_String());
		}
		else	{
			replicaManager->getPeer()->log(ELogType_Error, "failed to allocate replica: %s", 
				typeName.C_String());
		}

		return replica;
	}

	//-------------------------------------------------------------------------
	// GameContentConnection
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

	AbstractReplica* GameContentReplicaManager::createClientReplica( const char* classId, CCDictionary* constructionDictionary )
	{
		AbstractReplica* replica(nullptr);
		RakNet::RakString typeName(classId);
		if (typeName == GameObjectReplica::staticClassName()) { 
			ReplicaComponent* replicaComponent(REPLICA_COMPONENT_CLASS::create());
			replicaComponent->setGameplayLayer(getGameplayLayer());
			replicaComponent->createActorNode(nullptr);
			replica = replicaComponent->getReplica();
		}
		return replica;
	}

	AbstractReplica* GameContentReplicaManager::createPeerToPeerReplica( const char* classId, CCDictionary* constructionDictionary )
	{
		AbstractReplica* replica(nullptr);
		RakNet::RakString typeName(classId);
		if (typeName == GameObjectReplica::staticClassName()) { 
			ReplicaComponent* replicaComponent(REPLICA_COMPONENT_CLASS::create());
			replicaComponent->setGameplayLayer(getGameplayLayer());
			replicaComponent->createActorNode(nullptr);

			replica = replicaComponent->getReplica();
		}
		return replica;
	}

	AbstractReplica* GameContentReplicaManager::createServerReplica( const char* classId, CCDictionary* constructionDictionary )
	{
		AbstractReplica* replica(nullptr);
		RakNet::RakString typeName(classId);
		if (typeName == GameObjectReplica::staticClassName()) { 
			ReplicaComponent* replicaComponent(REPLICA_COMPONENT_CLASS::create());
			replicaComponent->setGameplayLayer(getGameplayLayer());
			replicaComponent->createActorNode(nullptr);

			if(constructionDictionary == nullptr)	{
				constructionDictionary = CCDictionary::create();
			}
			int assetIdx(randomIntLowerUpper(0,3));
			SLAString textureName[4] =	{
				"bluetank.png",
				"redtank.png",
				"yellowtank.png",
				"greentank.png",
			};
			constructionDictionary->setObject(CCString::create(textureName[assetIdx].c_str()), "sprite");
			constructionDictionary->setObject(CCFloat::create(randomLowerUpper(-100.0f, 100.0f)), "x");
			constructionDictionary->setObject(CCFloat::create(randomLowerUpper(-100.0f, 100.0f)), "y");

			// note: the server creates the ActorSprite immediately
			//       while the client has to wait until
			//       StudentReplicaComponent::deserializeConstruction
			//       is called
			replicaComponent->createActorSprite(constructionDictionary);

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


