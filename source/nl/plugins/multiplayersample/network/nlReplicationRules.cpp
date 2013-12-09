//!---------------------------------------------------------------------------
//! \file "nlReplicationRules.cpp"
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
#include "nlReplicationRules.h"

#include "nlGameContentReplicaManager.h"
#include "nlTankProjectileReplicaComponent.h"

namespace nl	{

	// TODO @student : implement appropriate replication rules

	//-------------------------------------------------------------------------
	// ServerAuthorityReplicationRule
	//-------------------------------------------------------------------------
	// TODO @student : 1.2
	RakNet::RM3ConstructionState ServerAuthorityReplicationRule::queryConstruction(
		RakNet::Connection_RM3 *destinationConnection, 
		RakNet::ReplicaManager3 *replicaManager3)	{


		if(_replica->getPeer() == nullptr)	{
			// where does this replica come from ???
			// is you discover this log put breakpoint here
			GameContentReplicaManager* replicaManager(dynamic_cast<GameContentReplicaManager*>(replicaManager3));
			if(replicaManager != nullptr)	{
				replicaManager->getPeer()->log(ELogType_Error, "Replica is already out of 'scope' queryConstruction for %s for %s with GUID %s",
					_replica->getName().C_String(), 
					destinationConnection->GetSystemAddress().ToString(true), 
					destinationConnection->GetRakNetGUID().ToString());
			}
			return RM3CS_NEVER_CONSTRUCT;
		}
// TODO @student : activate for debugging
#if 0
		_replica->getPeer()->log(ELogType_Status, "queryConstruction for %s for %s with GUID %s",
				_replica->getName().C_String(), 
				destinationConnection->GetSystemAddress().ToString(true), 
				destinationConnection->GetRakNetGUID().ToString());
#endif

		// DONE: @David : implement the right return value
		//return SLBaseClass::queryConstruction(destinationConnection, replicaManager3);
		return _replica->QueryConstruction_ServerConstruction(destinationConnection, getTopology() == SERVER );
	}

	bool ServerAuthorityReplicationRule::queryRemoteConstruction(RakNet::Connection_RM3 *sourceConnection)	{
		// DONE: @student : implement the right return value
//		return SLBaseClass::queryRemoteConstruction(sourceConnection);
		return _replica->QueryRemoteConstruction_ServerConstruction(sourceConnection, getTopology() == SERVER );
	}

	RakNet::RM3QuerySerializationResult ServerAuthorityReplicationRule::querySerialization(
		RakNet::Connection_RM3 *destinationConnection)	{
		// DONE @student : implement the right return value
//		return SLBaseClass::querySerialization(destinationConnection);
		return _replica->QuerySerialization_ServerSerializable(destinationConnection, getTopology() == SERVER );
	}

	RakNet::RM3ActionOnPopConnection ServerAuthorityReplicationRule::queryActionOnPopConnection(
		RakNet::Connection_RM3 *droppedConnection) const	{
/*
/// Default: If we are a client, and the connection is lost, delete the server's objects
virtual RM3ActionOnPopConnection QueryActionOnPopConnection_Client(RakNet::Connection_RM3 *droppedConnection) const;
/// Default: If we are a server, and the connection is lost, delete the client's objects and broadcast the destruction
virtual RM3ActionOnPopConnection QueryActionOnPopConnection_Server(RakNet::Connection_RM3 *droppedConnection) const;
/// Default: If we are a peer, and the connection is lost, delete the peer's objects
virtual RM3ActionOnPopConnection QueryActionOnPopConnection_PeerToPeer(RakNet::Connection_RM3 *droppedConnection) const;
*/
		// TODO @student : implement the right return value
//		return SLBaseClass::queryActionOnPopConnection(droppedConnection);
		return _replica->QueryActionOnPopConnection_Server(droppedConnection);
	}

	//-------------------------------------------------------------------------
	// ClientToServerReplicationRule
	//-------------------------------------------------------------------------
	// DONE @student : 1.1
	RakNet::RM3ConstructionState ClientToServerReplicationRule::queryConstruction(
		RakNet::Connection_RM3 *destinationConnection, 
		RakNet::ReplicaManager3 *replicaManager3)	{
		// DONE @student : implement the right return value
//		return SLBaseClass::queryConstruction(destinationConnection, replicaManager3);
			if(getTopology() == CLIENT)	{
				return RakNet::RM3CS_SEND_CONSTRUCTION;
			}
			else	{
				return RakNet::RM3CS_NEVER_CONSTRUCT;
			}
	}

	bool ClientToServerReplicationRule::queryRemoteConstruction(RakNet::Connection_RM3 *sourceConnection)	{
//		return SLBaseClass::queryRemoteConstruction(sourceConnection);
		if(_replica->getCreatedByReplicaManager() || (getTopology() == SERVER))	{
			return true;
		}
		return false;
	}

	RakNet::RM3QuerySerializationResult ClientToServerReplicationRule::querySerialization(
		RakNet::Connection_RM3 *destinationConnection)	{
		// DONE @student : implement the right return value
//		return SLBaseClass::querySerialization(destinationConnection);
			if(getTopology() == CLIENT)	{
				return RakNet::RM3QSR_CALL_SERIALIZE;
			}
			else	{
				return RakNet::RM3QSR_NEVER_CALL_SERIALIZE;
			}
	}

	RakNet::RM3ActionOnPopConnection ClientToServerReplicationRule::queryActionOnPopConnection(
		RakNet::Connection_RM3 *droppedConnection) const	{
		// TODO @student : implement the right return value
//		return SLBaseClass::queryActionOnPopConnection(droppedConnection);
			return _replica->QueryActionOnPopConnection_Client(droppedConnection);
	}

}


