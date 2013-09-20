//!---------------------------------------------------------------------------
//! \file "slReplicaManager.cpp"
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
#include "slReplicaManager.h"
#include "slAbstractReplica.h"

#include <stateless/network/slPeer.h>


using namespace RakNet;

namespace sl	{
	ReplicaManager::ReplicaManager(ENetwokTopology topology)
		:_peer(nullptr)
		,_allocatedReplicas(true)
	{
		SetDefaultPacketReliability(UNRELIABLE);
		SetDefaultPacketPriority(LOW_PRIORITY);
		SetNetworkIDManager(&_netWorkIDManager);

#if SL_HAVE_COMPONENT_BACKUP
		_components = ComponentArray::create(); _components->retain();
		_remoteComponents = ComponentArray::create(); _remoteComponents->retain();
#endif
	}

	ReplicaManager::~ReplicaManager()	{

#if SL_HAVE_COMPONENT_BACKUP
		_components->clear();
		_remoteComponents->clear();
		CC_SAFE_RELEASE(_components);
		CC_SAFE_RELEASE(_remoteComponents);
#endif

		SetNetworkIDManager(nullptr);
	}

	void ReplicaManager::Update(void) {
		// note: debug entry point
		SLBaseClass::Update();
	}

	void ReplicaManager::OnDetach(void)	{
		SetDefaultPacketReliability(RELIABLE_ORDERED);
		SetDefaultPacketPriority(LOW_PRIORITY);

		deleteCreatedReplicas(true);

		// TODO: something with other replicas ?

		// notes: when a system is actively disconnecting from another peer
		//        the replica manager will be detached
		//        meaning that replicas created by the other peer will still survive
		//        up to this time
		//        so we need to manually remove this objects as they might for example
		//        remain inside the scene
		//        the chance to receive controlling network messages is pretty low at this time

		// so we try to get rid of those replicas too
		deleteRemoteReplicas(true);

		// finally the replica manager should be clear. no ?
		const unsigned replicaCount(GetReplicaCount());
		if(replicaCount > 0)	{
			// check for any other missing objects
			getPeer()->log(ELogType_Warning, "you need to handle some leftovers (%d) replicas leaking ...",
				replicaCount);
		}

		SLBaseClass::OnDetach();
	}

	RakNet::Connection_RM3* ReplicaManager::AllocConnection(const RakNet::SystemAddress &systemAddress, RakNet::RakNetGUID rakNetGUID) const {
		// note: you need to implement this
		assert(false);
		return nullptr;
	}

	void ReplicaManager::DeallocConnection(RakNet::Connection_RM3 *connection) const {
		delete connection;
	}

	RakNet::PluginReceiveResult ReplicaManager::OnReceive( RakNet::Packet *packet )	{
		// right now just a hook for debugging
		RakNet::PluginReceiveResult result(SLBaseClass::OnReceive(packet));
		return result;
	}

	ENetwokTopology ReplicaManager::getTopology() const	{
		if(_peer != nullptr)	{
			return _peer->getTopology();
		}
		return UNKNOWN;
	}

	void ReplicaManager::referenceReplica( AbstractReplica* replica )	{
		assert(replica != nullptr);
		Reference(replica);
		replica->setCreatedByReplicaManager(true);
	}

	void ReplicaManager::deleteCreatedReplicas(bool isDetaching)	{
		// now we try to delete everything we've created
		DataStructures::List<Replica3*> replicaListOut;
		// The reason for DeallocReplica is that 
		// we don't track which objects have and have not been allocated at the peer level. 
		// So DeallocReplica will call delete on every object in the returned list, 
		// which is every object that the peer has created. Another way to put it is
		// 	A. Send a packet to tell other systems to delete these objects
		// 	B. Delete these objects on my own system
		// note: if the application will crash other peers will need to validate their
		//       replicas from time to time !

		GetReplicasCreatedByMe(replicaListOut);
		BroadcastDestructionList(replicaListOut, RakNet::UNASSIGNED_SYSTEM_ADDRESS);

		const unsigned ownReplicaCount(replicaListOut.Size());

		for (unsigned int i=0; i < replicaListOut.Size(); i++)	{
			AbstractReplica* replica(dynamic_cast<AbstractReplica*>(replicaListOut[i]));
			if(replica != nullptr)	{
				if(isDetaching)	{
					replica->onReplicaManagerDetach();
				}
			}

			// the order matters 
//			Dereference(replicaListOut[i]);

			// this is actually the same as if the replica manager would call
			// DeallocReplica on all those replicas
			// missing feature in RakNet ?
			// so it is up to the implementor of the replica to do the right thing
			
			// DeallocReplica might lead to replica destruction via delete
			replicaListOut[i]->DeallocReplica(nullptr);
		}
	}

	void ReplicaManager::deleteRemoteReplicas(bool isDetaching)	{
		// notes: when a system is actively disconnecting from another peer
		//        the replica manager will be detached
		//        meaning that replicas created by the other peer will still survive
		//        up to this time
		//        so we need to manually remove this objects as they might for example
		//        remain inside the scene
		//        the chance to receive controlling network messages is pretty low at this time
	//	if(isDetaching)	
		{
			// double check for left overs
			DataStructures::List<Replica3*> replicaListOut;
			GetReplicasCreatedByMe(replicaListOut);
			const unsigned ownReplicaCount(replicaListOut.Size());
			if(ownReplicaCount > 0)	{
				getPeer()->log(ELogType_Warning, "deleteRemoteReplicas detected %d own replicas. there should be none at this point",
					ownReplicaCount);
			}

			const unsigned replicaCount(GetReplicaCount());
			if(replicaCount > ownReplicaCount)	{
				getPeer()->log(ELogType_Message, "deleteRemoteReplicas detected %d replicas.",
					replicaCount);
				DataStructures::List<Replica3*> replicaListAll;
				GetReferencedReplicaList(replicaListAll);

				getPeer()->log(ELogType_Message, "deleteRemoteReplicas got %d referenced replicas.",
					replicaListAll.Size());

				for (unsigned int i=0; i < replicaListAll.Size(); i++)	{
					AbstractReplica* replica(dynamic_cast<AbstractReplica*>(replicaListAll[i]));
					if(replica != nullptr)	{
						if(isDetaching)	{
							replica->onReplicaManagerDetach();
						}
					}
					
					// the order matters 
//					Dereference(replicaListAll[i]);

					// this is actually the same as if the replica manager would call
					// DeallocReplica on all those replicas
					// missing feature in RakNet ?
					// so it is up to the implementor of the replica to do the right thing

					// DeallocReplica might lead to replica destruction via delete
					replicaListAll[i]->DeallocReplica(nullptr);
				}
			}
		}
	}


	AbstractReplica* ReplicaManager::createReplica( const char* classId, CCDictionary* constructionDictionary )
	{
		AbstractReplica* replica(nullptr);
		switch(getPeer()->getTopology())	{
		case(CLIENT):
			replica = createClientReplica(classId, constructionDictionary);
			break;
		case(SERVER):
			replica = createServerReplica(classId, constructionDictionary);
			break;
		case(P2P):
			replica = createPeerToPeerReplica(classId, constructionDictionary);
			break;
		}
		if(replica != nullptr)	{
			replica->setPeer(getPeer());
			referenceReplica(replica);

			// do this once the replica has been referenced 
			// so the component will be valid at this time
			if(replica->getAbstractReplicaComponent() != nullptr)	{
				// right now add the replica component to the replica manager itself
				// note: the only purpose here is to support debugging
				getAuthorityComponents()->addComponent(replica->getAbstractReplicaComponent());
			}
		}
		else	{
			getPeer()->log(ELogType_Error, "failed to create replica with classId %s", classId);
		}
		return replica;
	}

	AbstractReplica* ReplicaManager::createClientReplica( const char* classId, CCDictionary* constructionDictionary )
	{
		AbstractReplica* replica(nullptr);
		return replica;
	}

	AbstractReplica* ReplicaManager::createPeerToPeerReplica( const char* classId, CCDictionary* constructionDictionary )
	{
		AbstractReplica* replica(nullptr);
		return replica;
	}

	AbstractReplica* ReplicaManager::createServerReplica( const char* classId, CCDictionary* constructionDictionary )
	{
		AbstractReplica* replica(nullptr);
		return replica;
	}

	void ReplicaManager::listReplicas()	{
		unsigned replicaCount(GetReplicaCount());
		_peer->log(ELogType_Message, ">> listing %d replicas", replicaCount);
		for (unsigned int idx(0); idx < replicaCount; ++idx)	{
			AbstractReplica* replica(dynamic_cast<AbstractReplica*>(GetReplicaAtIndex(idx)));
			if(replica != nullptr)	{
				replica->log();
			}
		}
		_peer->log(ELogType_Message, ">> list end");
	}

	RakNet::Replica3* ReplicaManager::getReplicaByNetworkID( RakNet::NetworkID networkId, RakNet::WorldId worldId /*= 0*/ )
	{
		return 	GetReplicaByNetworkID(networkId, worldId);
	}

	ComponentArray* ReplicaManager::getRemoteComponents()
	{
#if SL_HAVE_COMPONENT_BACKUP
		return _remoteComponents;
#else
		return nullptr;
#endif
	}

	ComponentArray* ReplicaManager::getAuthorityComponents()
	{
#if SL_HAVE_COMPONENT_BACKUP
		return _components;
#else
		return nullptr;
#endif
	}

	void ReplicaManager::collectGarbage()
	{
		{
			ComponentArray* invalidComponents(ComponentArray::create());
			ComponentArray* components(getRemoteComponents());
			SLSize idx(0);
			IComponent* component(components->componentAt(idx));
			while(component != nullptr)	{
				if(component->isValid() == false)	{
					invalidComponents->addComponent(component);
				}
				++idx;
				component = components->componentAt(idx);
			}
			idx = 0;
			component = invalidComponents->componentAt(idx);
			while(component != nullptr)	{
				components->removeComponent(component);
				++idx;
				component = invalidComponents->componentAt(idx);
			}
		}
		{
			ComponentArray* invalidComponents(ComponentArray::create());
			ComponentArray* components(getAuthorityComponents());
			SLSize idx(0);
			IComponent* component(components->componentAt(idx));
			while(component != nullptr)	{
				if(component->isValid() == false)	{
					invalidComponents->addComponent(component);
				}
				++idx;
				component = components->componentAt(idx);
			}
			idx = 0;
			component = invalidComponents->componentAt(idx);
			while(component != nullptr)	{
				components->removeComponent(component);
				++idx;
				component = invalidComponents->componentAt(idx);
			}
		}
	}

	void ReplicaManager::purgeGarbage()
	{
		_components->clear();
		_remoteComponents->clear();
	}


}

