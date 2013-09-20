//!---------------------------------------------------------------------------
//! \file "slAbstractReplica.h"
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
#ifndef __SLABSTRACTREPLICA_H__
#define __SLABSTRACTREPLICA_H__

#include <stateless/network/slNetworkMacros.h>
#include <stateless/network/slNetworkEnums.h>
#include <stateless/network/slNetworkAddress.h>

namespace RakNet	{
	class RakPeerInterface;
	class SocketLayer;
	struct SocketDescriptor;
	struct RakNetGUID;
}


#include "slReplicationRule.h"

namespace sl	{

	class Peer;
	class Component;

	class slNetwork_API AbstractReplica : public RakNet::Replica3
	{
		SL_DECLARE_BASE(RakNet::Replica3)
	public:
		AbstractReplica();
		~AbstractReplica();

		// Overloaded Replica3 functions

		virtual RakNet::RM3ConstructionState QueryConstruction(RakNet::Connection_RM3 *destinationConnection, RakNet::ReplicaManager3 *replicaManager3) SL_OVERRIDE;
		virtual bool QueryRemoteConstruction(RakNet::Connection_RM3 *sourceConnection) SL_OVERRIDE;
		virtual RakNet::RM3QuerySerializationResult QuerySerialization(RakNet::Connection_RM3 *destinationConnection)SL_OVERRIDE;
		virtual RakNet::RM3ActionOnPopConnection QueryActionOnPopConnection(RakNet::Connection_RM3 *droppedConnection) const SL_OVERRIDE;


		virtual void WriteAllocationID(RakNet::Connection_RM3 *destinationConnection, RakNet::BitStream *allocationIdBitstream) const SL_OVERRIDE;
		virtual void SerializeConstruction(RakNet::BitStream *constructionBitstream, RakNet::Connection_RM3 *destinationConnection) SL_OVERRIDE;
		virtual bool DeserializeConstruction(RakNet::BitStream *constructionBitstream, RakNet::Connection_RM3 *sourceConnection) SL_OVERRIDE;
		virtual void SerializeDestruction(RakNet::BitStream *destructionBitstream, RakNet::Connection_RM3 *destinationConnection) SL_OVERRIDE;
		virtual void PreDestruction(RakNet::Connection_RM3 *sourceConnection) SL_OVERRIDE;
		virtual bool DeserializeDestruction(RakNet::BitStream *destructionBitstream, RakNet::Connection_RM3 *sourceConnection) SL_OVERRIDE;

		virtual void OnUserReplicaPreSerializeTick(void) SL_OVERRIDE;
		virtual RakNet::RM3SerializationResult Serialize(RakNet::SerializeParameters *serializeParameters) SL_OVERRIDE;
		virtual void Deserialize(RakNet::DeserializeParameters *deserializeParameters) SL_OVERRIDE;

		virtual void OnPoppedConnection(RakNet::Connection_RM3 *droppedConnection) SL_OVERRIDE;

#if (RAKNET_VERSION_NUMBER_INT < 402)
		// see: http://www.jenkinssoftware.com/raknet/manual/revisionlog.html
		virtual void SerializeConstructionRequestAccepted(RakNet::BitStream *serializationBitstream, RakNet::Connection_RM3 *requestingConnection) SL_OVERRIDE;
		virtual void DeserializeConstructionRequestAccepted(RakNet::BitStream *serializationBitstream, RakNet::Connection_RM3 *acceptingConnection) SL_OVERRIDE;
		virtual void SerializeConstructionRequestRejected(RakNet::BitStream *serializationBitstream, RakNet::Connection_RM3 *requestingConnection) SL_OVERRIDE;
		virtual void DeserializeConstructionRequestRejected(RakNet::BitStream *serializationBitstream, RakNet::Connection_RM3 *rejectingConnection) SL_OVERRIDE;
#endif
		virtual void DeallocReplica(RakNet::Connection_RM3 *sourceConnection) SL_OVERRIDE;

		// note: this function appears pretty ineffective to me
		//       but for now I keep it as this is 'just' a demo
		const RakNet::RakString& getName(void) const;
		void setName(const char* name);
		// abstract
		SL_DECLARE_ABSTRACT_CLASSNAME

		// specific implementation

		virtual void onReplicaManagerDetach() SL_VIRTUAL_DECLARE;

		virtual void notifyReplicaOfMessageDeliveryStatus(RakNet::RakNetGUID guid, uint32_t receiptId, bool messageArrived) SL_VIRTUAL_DECLARE;
		virtual void log() SL_VIRTUAL_DECLARE;

		ENetwokTopology getTopology() const;

		virtual Component* getAbstractReplicaComponent() const SL_VIRTUAL_DECLARE;

		void printStringInBitstream(RakNet::BitStream *bs);

		SL_SYNTHESIZE(Peer*, _peer, Peer)
		SL_SYNTHESIZE(bool, _createdByReplicaManager, CreatedByReplicaManager)
		SL_SYNTHESIZE_RETAIN(ReplicationRule*, _replicationRule, ReplicationRule)
	protected:
		mutable RakNet::RakString _name;

	};

}


#endif
