//!---------------------------------------------------------------------------
//! \file "slReplicaComponent.cpp"
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
#include "slReplicaComponent.h"

#include "actor/slGameActorNode.h"
#include "actor/slActorSprite.h"

namespace sl	{


	ReplicaComponent::ReplicaComponent()
		:_constructionDictionary(nullptr)
	{
		_replica.setReplicaComponent(this);
		_serializeLogTick.setAnimationFrequency(1);
	}

	ReplicaComponent::~ReplicaComponent()
	{
		setConstructionDictionary(nullptr);
	}

	bool ReplicaComponent::isValid() const
	{
		return _replica.isValid();
	}

	void ReplicaComponent::handleDestroyedActorNode()	{
		bool isDestroyed(false);
		ActorNode* actorNode(getActorNode());
		if(actorNode != nullptr)	{
			isDestroyed = actorNode->isDestroyed();
		}
		if(isDestroyed)	{
			// we want to stay alive until this action is performed
			// so we smart pointer ourself
			ComponentPtr myself(this);
			{
				ReplicaManager* replicaManager(getReplicaManager());
				if(replicaManager != nullptr)	{

					// the order matters 

					// TODO: @CP validate with PreDestruction RakNet implementation
					//           would the DeallocReplica implementation
					//           be a better place for PreDestruction ?

					// 1. broadcast the replica destruction
					getReplica()->BroadcastDestruction();
					// 2. call PreDestruction on this replica
					getReplica()->PreDestruction(nullptr);
					// 3. call DeallocReplica on this replica
					//    take a look at GameObjectReplica,
					//    to see what will happen now
					getReplica()->DeallocReplica(nullptr);
				}
				else	{
					removeFromGameplayLayer();
				}
			}
		}
	}

	void ReplicaComponent::preUpdate( float delta ) {
		SLBaseClass::preUpdate(delta);
		_serializeLogTick.accumulate(delta);
	}

	void ReplicaComponent::postUpdate( float delta )	{
		SLBaseClass::postUpdate(delta);
	}


	void ReplicaComponent::preSerialize()
	{

	}

	void ReplicaComponent::serializeConstruction(RakNet::BitStream *constructionBitstream)	{

	}

	bool ReplicaComponent::deserializeConstruction(RakNet::BitStream *constructionBitstream)	{
		return true;
	}

	void ReplicaComponent::serializeDestruction(RakNet::BitStream *destructionBitstream)	{

	}

	bool ReplicaComponent::deserializeDestruction(RakNet::BitStream *destructionBitstream)	{
		return true;
	}


	RakNet::RM3SerializationResult ReplicaComponent::serialize(RakNet::SerializeParameters *serializeParameters)
	{
		// This return type makes is to ReplicaManager3 itself does not do a memory compare. 
		// we entirely control serialization ourselves here.
		// Use RM3SR_SERIALIZED_ALWAYS instead of RM3SR_SERIALIZED_ALWAYS_IDENTICALLY 
		// to support sending different data to different system, which is needed when using unreliable and dirty variable resends
		return RakNet::RM3SR_SERIALIZED_ALWAYS;
	}

	void ReplicaComponent::deserialize(RakNet::DeserializeParameters *deserializeParameters)
	{
		// in case the replica is deserialized
		// it should be pretty obvious that the hosted simulation object
		// is a remote object
		// sooo ...
		// in case not done already
		ActorSprite* actorSprite(getActorSprite());
		if(actorSprite != nullptr)	{
			AbstractVehicle* vehicle(actorSprite->getVehicle());
			if(vehicle != nullptr)	{
				vehicle->setIsRemoteObject(true);
			}

			if(actorSprite->isVisible() == false)	{
				actorSprite->setVisible(true);
			}
		}
	}

	void ReplicaComponent::createActorNode( CCDictionary* parameters /*= nullptr*/ )
	{
		// just call the base class
		// your debugging entry
		SLBaseClass::createActorNode(parameters);
	}

	void ReplicaComponent::createActorSprite( CCDictionary* parameters /*= nullptr*/ )
	{
		setConstructionDictionary(parameters);
		// just call the base class
		// your debugging entry
		SLBaseClass::createActorSprite(parameters);
	}

	void ReplicaComponent::removeFromGameplayLayer()
	{
		// just call the base class
		// your debugging entry
		SLBaseClass::removeFromGameplayLayer();
	}

	sl::ENetwokTopology ReplicaComponent::getTopology() const
	{
		return _replica.getTopology();
	}

	void ReplicaComponent::setPeer(Peer* peer)	{

	}

	ReplicaManager* ReplicaComponent::getReplicaManager() const
	{
		return _replica.getReplicaManager();
	}



}


