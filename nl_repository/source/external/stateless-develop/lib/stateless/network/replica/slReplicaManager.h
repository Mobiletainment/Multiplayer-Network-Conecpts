//!---------------------------------------------------------------------------
//! \file "slReplicaManager.h"
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
#ifndef __SLREPLICAMANAGER_H__
#define __SLREPLICAMANAGER_H__

#include <stateless/network/slNetworkMacros.h>
#include <stateless/network/slNetworkEnums.h>
#include <stateless/network/slNetworkAddress.h>

// to prevent cocos garbage collection from doing bad things
#define SL_HAVE_COMPONENT_BACKUP 1

namespace RakNet	{
	class RakPeerInterface;
	class SocketLayer;
	struct SocketDescriptor;
	struct RakNetGUID;
}

namespace sl	{

	class Peer;
	class AbstractReplica;

	//! @brief ReplicaManager a master object helping to keep your network game alive
	//!
	//! you will derive from ReplicaManager to control what will happen regarding
	//! replica objects in your game
	class slNetwork_API ReplicaManager : public RakNet::ReplicaManager3
	{
		SL_DECLARE_BASE(RakNet::ReplicaManager3)
	public:
		ReplicaManager(ENetwokTopology topology);
		virtual ~ReplicaManager();

		// Update is called every update cycle 
		//! @brief see RakNet::ReplicaManager3 for details
		virtual void Update(void) SL_OVERRIDE;

		//! @brief see RakNet::ReplicaManager3 for details
		virtual RakNet::Connection_RM3* AllocConnection(const RakNet::SystemAddress &systemAddress, RakNet::RakNetGUID rakNetGUID) const SL_OVERRIDE;

		//! @brief see RakNet::ReplicaManager3 for details
		virtual void DeallocConnection(RakNet::Connection_RM3 *connection) const SL_OVERRIDE;

		//! @brief see RakNet::ReplicaManager3 for details
		//!
		//! take a closer look at the implementation
		virtual void OnDetach(void) SL_OVERRIDE;

		//! create a replica and reference it within the replica manager
		virtual AbstractReplica* createReplica(const char* classId, CCDictionary* constructionDictionary) SL_VIRTUAL_DECLARE;

		//! topology specific implementation of create replica
		virtual AbstractReplica* createClientReplica(const char* classId, CCDictionary* constructionDictionary) SL_VIRTUAL_DECLARE;
		//! topology specific implementation of create replica
		virtual AbstractReplica* createPeerToPeerReplica(const char* classId, CCDictionary* constructionDictionary) SL_VIRTUAL_DECLARE;
		//! topology specific implementation of create replica
		virtual AbstractReplica* createServerReplica(const char* classId, CCDictionary* constructionDictionary) SL_VIRTUAL_DECLARE;

		//! list all replicas the replica manager knows about
		void listReplicas();

		//! retrieves a replica with a given networkId
		RakNet::Replica3* getReplicaByNetworkID(RakNet::NetworkID networkId, RakNet::WorldId worldId = 0);

		//! tries to delete all 'self' created replicas
		void deleteCreatedReplicas(bool isDetaching);

		//! tries to delete all 'remote' via connection alloc replica created replicas
		void deleteRemoteReplicas(bool isDetaching);

		//! function to call instead of Reference
		void referenceReplica(AbstractReplica* replica);

		//! retrieve the network topology
		ENetwokTopology getTopology() const;

		void collectGarbage();
		void purgeGarbage();

		ComponentArray* getRemoteComponents();
		ComponentArray* getAuthorityComponents();

		//! the peer which has created this replica manager
		SL_SYNTHESIZE(Peer*, _peer, Peer)

	protected:
		virtual RakNet::PluginReceiveResult OnReceive(RakNet::Packet *packet);

		//! the network id manager for this replica manager
		RakNet::NetworkIDManager _netWorkIDManager;

		//! indicates if the replicas have been dynamically allocated
		bool _allocatedReplicas;
	private:
#if SL_HAVE_COMPONENT_BACKUP
		ComponentArray* _components;
		ComponentArray* _remoteComponents;
#endif

	};

#if 0 // maybe later
	SL_INLINE
	ComponentArray* ReplicaManager::getAuthorityComponents()
	{
#if SL_HAVE_COMPONENT_BACKUP
		return _remoteComponents;
#else
		return nullptr;
#endif
	}

	SL_INLINE
	ComponentArray* ReplicaManager::getRemoteComponents()
	{
#if SL_HAVE_COMPONENT_BACKUP
		return _remoteComponents;
#else
		return nullptr;
#endif
	}
#endif

}


#endif
