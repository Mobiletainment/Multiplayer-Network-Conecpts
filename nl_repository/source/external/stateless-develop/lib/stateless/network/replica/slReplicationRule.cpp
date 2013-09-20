//!---------------------------------------------------------------------------
//! \file "slReplicationRule.cpp"
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
#include "slReplicationRule.h"

namespace sl	{

	// TODO: NICE TO HAVE add verbose mode
	//		_replica->getPeer()->log(ELogType_Status, "queryRemoteConstruction");

	ReplicationRule::ReplicationRule()
		:_isValid(false)
		,_networkTopology(SINGLEPLAYER)
	{

	}

	ReplicationRule::~ReplicationRule()	{

	}

	bool ReplicationRule::init()	{
		return true;
	}

	RakNet::RM3ConstructionState ReplicationRule::queryConstruction(RakNet::Connection_RM3 *destinationConnection, RakNet::ReplicaManager3 *replicaManager3)
	{
/*
			/// \brief Return codes for Connection_RM3::GetConstructionState() and Replica3::QueryConstruction()
			/// \details Indicates what state the object should be in for the remote system
			/// \ingroup REPLICA_MANAGER_GROUP3
			enum RM3ConstructionState
			{
				/// This object should exist on the remote system. Send a construction message if necessary
				/// If the NetworkID is already in use, it will not do anything
				/// If it is not in use, it will create the object, and then call DeserializeConstruction
				RM3CS_SEND_CONSTRUCTION,

				/// This object should exist on the remote system.
				/// The other system already has the object, and the object will never be deleted.
				/// This is true of objects that are loaded with the level, for example.
				/// Treat it as if it existed, without sending a construction message.
				/// Will call Serialize() and SerializeConstructionExisting() to the object on the remote system
				RM3CS_ALREADY_EXISTS_REMOTELY,

				/// Same as RM3CS_ALREADY_EXISTS_REMOTELY but does not call SerializeConstructionExisting()
				RM3CS_ALREADY_EXISTS_REMOTELY_DO_NOT_CONSTRUCT,

				/// This object will never be sent to the target system
				/// This object will never be serialized from this system to the target system
				RM3CS_NEVER_CONSTRUCT,

				/// Don't do anything this tick. Will query again next tick
				RM3CS_NO_ACTION,

				/// Max enum
				RM3CS_MAX,
			};
*/
		return RakNet::RM3CS_NO_ACTION;
	}

	bool ReplicationRule::queryRemoteConstruction(RakNet::Connection_RM3 *sourceConnection)	{
		return false;
	}

	RakNet::RM3QuerySerializationResult ReplicationRule::querySerialization(RakNet::Connection_RM3 *destinationConnection)	{
/*
			/// First pass at topology to see if an object should be serialized
			/// \ingroup REPLICA_MANAGER_GROUP3
			enum RM3QuerySerializationResult
			{
				/// Call Serialize() to see if this object should be serializable for this connection
				RM3QSR_CALL_SERIALIZE,
				/// Do not call Serialize() this tick to see if this object should be serializable for this connection
				RM3QSR_DO_NOT_CALL_SERIALIZE,
				/// Never call Serialize() for this object and connection. This system will not serialize this object for this topology
				RM3QSR_NEVER_CALL_SERIALIZE,
				/// Max enum
				RM3QSR_MAX,
			};
*/
			return RakNet::RM3QSR_NEVER_CALL_SERIALIZE;
	}

	RakNet::RM3ActionOnPopConnection ReplicationRule::queryActionOnPopConnection(RakNet::Connection_RM3 *droppedConnection) const	{
/*
/// \ingroup REPLICA_MANAGER_GROUP3
enum RM3ActionOnPopConnection
{
RM3AOPC_DO_NOTHING,
RM3AOPC_DELETE_REPLICA,
RM3AOPC_DELETE_REPLICA_AND_BROADCAST_DESTRUCTION,
RM3AOPC_MAX,
};
*/
		return RakNet::RM3AOPC_DO_NOTHING;
	}



}

