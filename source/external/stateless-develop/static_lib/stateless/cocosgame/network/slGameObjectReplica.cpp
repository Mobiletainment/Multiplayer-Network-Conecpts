//!---------------------------------------------------------------------------
//! \file "slGameObjectReplica.cpp"
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
#include "slGameObjectReplica.h"
#include "slReplicaComponent.h"

namespace sl	{

	GameObjectReplica::GameObjectReplica()
		:_replicaComponent(nullptr)
	{
	}

	GameObjectReplica::~GameObjectReplica()	{
		// note: this replica is not dynamically allocated
		//       so if we reach this point we explicitly set
		//       the replica manager to nullptr
		//       to prevent any damage from calling the base class destructor
		replicaManager = nullptr;
		// in addition we set the network id manager to null
		// see: NetworkIDObject::~NetworkIDObject
		SetNetworkIDManager(nullptr);

		if(false)
		{
			if(getPeer() != nullptr)	{
				getPeer()->log(ELogType_Message, "destructor: %s", getName().C_String());
			}
			else	{
				SL_PROCESS_APP()->log(ELogType_Message, "destructor: %s", getName().C_String());
			}
		}
	}

	bool GameObjectReplica::isValid() const
	{
		return (replicaManager != nullptr) && (getPeer() != nullptr);
	}


	ReplicaManager* GameObjectReplica::getReplicaManager() const	{
		// first we try to retrieve the replica manager directly from the replica it self
		// this might not always be successful therefor the fall back
		if(replicaManager != nullptr)	{
			return dynamic_cast<ReplicaManager*>(replicaManager);
		}
		if(getPeer() != nullptr)	{
			return getPeer()->getReplicaManager();
		}
		return nullptr;

	}

	void GameObjectReplica::setPeer(Peer* peer)	{
		SLBaseClass::setPeer(peer);
		getReplicationRule()->setTopology(peer != nullptr ? peer->getTopology() : SINGLEPLAYER);
		if(_replicaComponent != nullptr)	{
			_replicaComponent->setPeer(peer);
		}
	}

	RM3ConstructionState GameObjectReplica::QueryConstruction(RakNet::Connection_RM3 *destinationConnection, ReplicaManager3 *replicaManager3) {
		if(getReplicationRule()->isValid())	{
			return SLBaseClass::QueryConstruction(destinationConnection, replicaManager3);
		}
		return QueryConstruction_ServerConstruction(destinationConnection, getTopology() == SERVER);
	}

	bool GameObjectReplica::QueryRemoteConstruction(RakNet::Connection_RM3 *sourceConnection) {
		if(getReplicationRule()->isValid())	{
			return SLBaseClass::QueryRemoteConstruction(sourceConnection);
		}
		return QueryRemoteConstruction_ServerConstruction(sourceConnection, getTopology() == SERVER);
	}

	RM3QuerySerializationResult GameObjectReplica::QuerySerialization(RakNet::Connection_RM3 *destinationConnection) {
		if(getReplicationRule()->isValid())	{
			return SLBaseClass::QuerySerialization(destinationConnection);
		}
		return QuerySerialization_ServerSerializable(destinationConnection, getTopology() == SERVER);
	}

	RM3ActionOnPopConnection GameObjectReplica::QueryActionOnPopConnection(RakNet::Connection_RM3 *droppedConnection) const {
		if(getReplicationRule()->isValid())	{
			return SLBaseClass::QueryActionOnPopConnection(droppedConnection);
		}
		return QueryActionOnPopConnection_Server(droppedConnection);
	}

	void GameObjectReplica::DeallocReplica(RakNet::Connection_RM3 *sourceConnection) {
		// note: if the source connection is null then the replicamanager is Deallocating
		//       the replicas he's created

		// the default implementation calls delete this !!!
		// we do not want this
		// instead we remove the actor from the scene
		// so - do not call the base class
#if SL_CODERVERSION_CP
		if(getPeer() != nullptr)	{
			getPeer()->log(ELogType_Message, "DeallocReplica: %s", getName().C_String());
		}
		else	{
			SL_PROCESS_APP()->log(ELogType_Message, "DeallocReplica: %s", getName().C_String());
		}
#endif
		// the 'default' implementation 'delete this'
		// would finally lead to the destructor implementation
		// of replica3
		// which does a final call to replica manager Dereference
		// so we mimic this implementation here
		// and set the replica manager to null afterwards

		// this might actually destroy the whole object
		// so we need to do smth to stay alive
		// until we leave the function
		if(_replicaComponent != nullptr)	{
			GameActorComponentPtr replicaComponentPtr(_replicaComponent);
			{
				replicaComponentPtr->removeFromGameplayLayer();
				_replicaComponent = nullptr;

				if (replicaManager)	{
					replicaManager->Dereference(this);
					// mimic the network id object destructor code
					if(GetNetworkIDManager() != nullptr)	{
						GetNetworkIDManager()->StopTrackingNetworkIDObject(this);
					}
					replicaManager = nullptr;
				}

				setPeer(nullptr);
			}
		}
		else	{
			if (replicaManager)	{
				replicaManager->Dereference(this);
				replicaManager = nullptr;
			}

			setPeer(nullptr);
		}
	}

	void GameObjectReplica::SerializeConstruction(RakNet::BitStream *constructionBitstream, RakNet::Connection_RM3 *destinationConnection)	{
		if(getReplicaComponent() == nullptr)	{
			return;
		}
		getReplicaComponent()->serializeConstruction(constructionBitstream);
	}

	bool GameObjectReplica::DeserializeConstruction(RakNet::BitStream *constructionBitstream, RakNet::Connection_RM3 *sourceConnection) {
		if(getReplicaComponent() == nullptr)	{
			return false;
		}
		return getReplicaComponent()->deserializeConstruction(constructionBitstream);
	}

	void GameObjectReplica::SerializeDestruction(RakNet::BitStream *destructionBitstream, RakNet::Connection_RM3 *destinationConnection)	{
		if(getReplicaComponent() == nullptr)	{
			return;
		}
		getReplicaComponent()->serializeDestruction(destructionBitstream);
	}

	bool GameObjectReplica::DeserializeDestruction(RakNet::BitStream *destructionBitstream, RakNet::Connection_RM3 *sourceConnection) {
		if(getReplicaComponent() == nullptr)	{
			return true;
		}
		return getReplicaComponent()->deserializeDestruction(destructionBitstream);
	}

	/// Overloaded Replica3 function
	void GameObjectReplica::OnUserReplicaPreSerializeTick(void)	{
		if(getReplicaComponent() == nullptr)	{
			return;
		}
		return getReplicaComponent()->preSerialize();
	}

	RM3SerializationResult GameObjectReplica::Serialize(SerializeParameters *serializeParameters)	{
		if(getReplicaComponent() == nullptr)	{
			return RM3SR_DO_NOT_SERIALIZE;
		}
		// invalid replication rule
		// server to client serialization
		if ((getReplicationRule()->isValid() == false) && (getTopology() == CLIENT) )	{
			return RM3SR_DO_NOT_SERIALIZE;
		}
		return getReplicaComponent()->serialize(serializeParameters);
	}

	void GameObjectReplica::Deserialize(RakNet::DeserializeParameters *deserializeParameters) {
		if(getReplicaComponent() == nullptr)	{
			return;
		}
		getReplicaComponent()->deserialize(deserializeParameters);
	}

	void GameObjectReplica::log()	{
		SLBaseClass::log();
	}

	void GameObjectReplica::testOffline()
	{
		GameObjectReplica instance;
	}

	Component* GameObjectReplica::getAbstractReplicaComponent() const
	{
		return getReplicaComponent();
	}

}

