//!---------------------------------------------------------------------------
//! \file "nlReplicationRules.h"
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
#ifndef __SLREPLICATIONRULES_H__
#define __SLREPLICATIONRULES_H__


namespace nl	{

	class ServerAuthorityReplicationRule : public ReplicationRule	{
	public:
		ServerAuthorityReplicationRule()	{
			setIsValid(true);
		}

		CREATE_FUNC(ServerAuthorityReplicationRule)

		virtual RakNet::RM3ConstructionState queryConstruction(
			RakNet::Connection_RM3 *destinationConnection, 
			RakNet::ReplicaManager3 *replicaManager3)  SL_OVERRIDE;

		virtual bool queryRemoteConstruction(RakNet::Connection_RM3 *sourceConnection) SL_OVERRIDE;

		virtual RakNet::RM3QuerySerializationResult querySerialization(
			RakNet::Connection_RM3 *destinationConnection) SL_OVERRIDE;

		virtual RakNet::RM3ActionOnPopConnection queryActionOnPopConnection(
			RakNet::Connection_RM3 *droppedConnection) const SL_OVERRIDE;

		AbstractReplica* _replica;
	};


	class ClientToServerReplicationRule : public ReplicationRule	{
	public:
		ClientToServerReplicationRule()	{
			setIsValid(true);
		}

		CREATE_FUNC(ClientToServerReplicationRule)

		virtual RakNet::RM3ConstructionState queryConstruction(
			RakNet::Connection_RM3 *destinationConnection, 
			RakNet::ReplicaManager3 *replicaManager3)  SL_OVERRIDE;

		virtual bool queryRemoteConstruction(RakNet::Connection_RM3 *sourceConnection) SL_OVERRIDE;

		virtual RakNet::RM3QuerySerializationResult querySerialization(
			RakNet::Connection_RM3 *destinationConnection) SL_OVERRIDE;

		virtual RakNet::RM3ActionOnPopConnection queryActionOnPopConnection(
			RakNet::Connection_RM3 *droppedConnection) const SL_OVERRIDE;

		AbstractReplica* _replica;
	};

}


#endif // __SLREPLICATIONRULES_H__
