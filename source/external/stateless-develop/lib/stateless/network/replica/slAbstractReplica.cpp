//!---------------------------------------------------------------------------
//! \file "slAbstractReplica.cpp"
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
#include "slAbstractReplica.h"

#include "RakString.h"
#include "Rand.h"

#include <stateless/network/slPeer.h>

namespace sl	{


	AbstractReplica::AbstractReplica()
		:_peer(nullptr)
		,_replicationRule(nullptr)
		,_createdByReplicaManager(false)
	{
		setReplicationRule(ReplicationRule::create());
	}

	AbstractReplica::~AbstractReplica()
	{
		setReplicationRule(nullptr);
#if 0
		if(getName() != "TankProjectileReplicaComponent")	
		{
			SL_PROCESS_APP()->log(ELogType_Message, "AbstractReplica::~AbstractReplica() name(%s)", getName().C_String());
		}
#endif
	}

	void AbstractReplica::setName(const char* name)	{
		_name = name;
	}

	const RakNet::RakString& AbstractReplica::getName(void) const {
		if(_name.GetLength() == 0)	{
			_name = getClassName();
		}
		return _name;
	}

	//-------------------------------------------------------------------------
	// the core of replication - replication rules
	//-------------------------------------------------------------------------

	RakNet::RM3ConstructionState AbstractReplica::QueryConstruction(
		RakNet::Connection_RM3 *destinationConnection, RakNet::ReplicaManager3 *replicaManager3) {
		if(_replicationRule->isValid())	{
			return _replicationRule->queryConstruction(destinationConnection, replicaManager3);
		}
		return RakNet::RM3CS_NO_ACTION;
	}

	bool AbstractReplica::QueryRemoteConstruction(RakNet::Connection_RM3 *sourceConnection) {
		if(_replicationRule->isValid())	{
			return _replicationRule->queryRemoteConstruction(sourceConnection);
		}
		return true;
	}

	RakNet::RM3QuerySerializationResult AbstractReplica::QuerySerialization(RakNet::Connection_RM3 *destinationConnection) {
		if(_replicationRule->isValid())	{
			return _replicationRule->querySerialization(destinationConnection);
		}
		return RakNet::RM3QSR_NEVER_CALL_SERIALIZE;
	}

	RakNet::RM3ActionOnPopConnection AbstractReplica::QueryActionOnPopConnection(RakNet::Connection_RM3 *droppedConnection) const {
		if(_replicationRule->isValid())	{
			return _replicationRule->queryActionOnPopConnection(droppedConnection);
		}
		return RakNet::RM3AOPC_DO_NOTHING;
	}

	//-------------------------------------------------------------------------
	// essential for development
	//-------------------------------------------------------------------------
	void AbstractReplica::log()	{
		if(getPeer() != nullptr)	{
			getPeer()->log(ELogType_Message, "replica: created(%s) %s", 
				getCreatedByReplicaManager() ? "local" : "remote",
				getName().C_String());
			getPeer()->log(ELogType_Message, "         WasReferenced: %s", WasReferenced() ? "true" : "false");
		}
		else	{
			SL_PROCESS_APP()->log(ELogType_Message, "replica: created(%s) %s", 
				getCreatedByReplicaManager() ? "local" : "remote",
				getName().C_String());
			SL_PROCESS_APP()->log(ELogType_Message, "         WasReferenced: %s", WasReferenced() ? "true" : "false");
		}
	}

	//-------------------------------------------------------------------------
	// serialization
	//-------------------------------------------------------------------------
	void AbstractReplica::WriteAllocationID(RakNet::Connection_RM3 *destinationConnection, RakNet::BitStream *allocationIdBitstream) const {
		allocationIdBitstream->Write(getName());
	}

	void AbstractReplica::SerializeConstruction(RakNet::BitStream *constructionBitstream, RakNet::Connection_RM3 *destinationConnection)	{
		// you can pass information regarding the object construction here
		constructionBitstream->Write(getName() + RakNet::RakString(" SerializeConstruction"));
	}

	void AbstractReplica::PreDestruction(RakNet::Connection_RM3 *sourceConnection) {
		// TODO @CP : check this out ...
#if SL_CODERVERSION_CP
		(void) sourceConnection;
		if(getPeer() != nullptr)	{
			getPeer()->log(ELogType_Message, "PreDestruction replica: %s", getName().C_String());
			getPeer()->log(ELogType_Message, "PreDestruction         WasReferenced: %s", WasReferenced() ? "true" : "false");
		}
		else	{
			SL_PROCESS_APP()->log(ELogType_Message, "PreDestruction replica: %s", getName().C_String());
			SL_PROCESS_APP()->log(ELogType_Message, "PreDestruction         WasReferenced: %s", WasReferenced() ? "true" : "false");
		}
#endif
	}

	bool AbstractReplica::DeserializeConstruction(RakNet::BitStream *constructionBitstream, RakNet::Connection_RM3 *sourceConnection) {
		// and read it on the other side to do special setups
		printStringInBitstream(constructionBitstream);
		return true;
	}

	void AbstractReplica::SerializeDestruction(RakNet::BitStream *destructionBitstream, RakNet::Connection_RM3 *destinationConnection)	{
		// you can pass information regarding the object destruction here
		destructionBitstream->Write(getName() + RakNet::RakString(" SerializeDestruction"));
	}

	bool AbstractReplica::DeserializeDestruction(RakNet::BitStream *destructionBitstream, RakNet::Connection_RM3 *sourceConnection) {
		// and read it on the other side ...
		printStringInBitstream(destructionBitstream);
		return true;
	}

	/// Overloaded Replica3 function
	void AbstractReplica::OnUserReplicaPreSerializeTick(void)	{
	}

	RakNet::RM3SerializationResult AbstractReplica::Serialize(RakNet::SerializeParameters *serializeParameters)	{
		// This return type makes is to ReplicaManager3 itself does not do a memory compare. 
		// we entirely control serialization ourselves here.
		// Use RM3SR_SERIALIZED_ALWAYS instead of RM3SR_SERIALIZED_ALWAYS_IDENTICALLY to support sending different data to different system, which is needed when using unreliable and dirty variable resends
		return RakNet::RM3SR_SERIALIZED_ALWAYS;
	}

	void AbstractReplica::Deserialize(RakNet::DeserializeParameters *deserializeParameters) {
		// add log
	}

	void AbstractReplica::OnPoppedConnection(RakNet::Connection_RM3 *droppedConnection)	{
		// Same as in SerializeDestruction(), no longer track this system
		if(getPeer() != nullptr)	{
			getPeer()->log(ELogType_Message, "OnPoppedConnection for replica: %s", getName().C_String());
		}
		else	{
			SL_PROCESS_APP()->log(ELogType_Message, "OnPoppedConnection for replica: %s", getName().C_String());
		}
	}

#if (RAKNET_VERSION_NUMBER_INT < 402)
	void AbstractReplica::SerializeConstructionRequestAccepted(RakNet::BitStream *serializationBitstream, RakNet::Connection_RM3 *requestingConnection)	{
		serializationBitstream->Write(getName() + RakNet::RakString(" SerializeConstructionRequestAccepted"));
	}
	void AbstractReplica::DeserializeConstructionRequestAccepted(RakNet::BitStream *serializationBitstream, RakNet::Connection_RM3 *acceptingConnection) {
		printStringInBitstream(serializationBitstream);
	}
	void AbstractReplica::SerializeConstructionRequestRejected(RakNet::BitStream *serializationBitstream, RakNet::Connection_RM3 *requestingConnection)	{
		serializationBitstream->Write(getName() + RakNet::RakString(" SerializeConstructionRequestRejected"));
	}
	void AbstractReplica::DeserializeConstructionRequestRejected(RakNet::BitStream *serializationBitstream, RakNet::Connection_RM3 *rejectingConnection) {
		printStringInBitstream(serializationBitstream);
	}
#endif

	//-------------------------------------------------------------------------
	// allocation
	//-------------------------------------------------------------------------
	void AbstractReplica::DeallocReplica(RakNet::Connection_RM3 *sourceConnection) {
		// note: if the source connection is null then the replicamanager is Deallocating
		//       the replicas he's created

		// important overwrite in case you handle deletion differently
		getPeer()->log(ELogType_Message, "DeallocReplica: %s with 'delete this'", 
			getName().C_String());
		delete this;
	}

	//-------------------------------------------------------------------------
	// specials
	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	// implement in derivates
	//-------------------------------------------------------------------------
	void AbstractReplica::onReplicaManagerDetach()	{

	}

	void AbstractReplica::notifyReplicaOfMessageDeliveryStatus(RakNet::RakNetGUID guid, uint32_t receiptId, bool messageArrived)
	{
		// When using UNRELIABLE_WITH_ACK_RECEIPT, the system tracks which variables were updated with which sends
		// So it is then necessary to inform the system of messages arriving or lost
		// Lost messages will flag each variable sent in that update as dirty, 
		// meaning the next Serialize() call will resend them with the current values
	}

	ENetwokTopology AbstractReplica::getTopology() const	{
		if(_peer)	{
			return _peer->getTopology();
		}
		// no peer assigned must be a single player replica
		return SINGLEPLAYER;
	}

	void AbstractReplica::printStringInBitstream(RakNet::BitStream *bs)	{
		// you did forget to assign a peer to this replica
		if (bs->GetNumberOfBitsUsed()==0)
			return;

		if(getPeer() != nullptr)	{
			RakNet::RakString rakString;
			bs->Read(rakString);
			getPeer()->log(ELogType_Message, "Receive: %s", rakString.C_String());
		}
	}

	Component* AbstractReplica::getAbstractReplicaComponent() const SL_VIRTUAL_DECLARE
	{
		return nullptr;
	}

}

