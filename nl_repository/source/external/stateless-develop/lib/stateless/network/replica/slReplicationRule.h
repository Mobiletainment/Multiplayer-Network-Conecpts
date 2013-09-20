//!---------------------------------------------------------------------------
//! \file "slReplicationRule.h"
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
#ifndef __SLREPLICATIONRULE_H__
#define __SLREPLICATIONRULE_H__

#include "../slNetworkEnums.h"

namespace sl	{

	class ReplicaComponent;

	class slNetwork_API ReplicationRule : public CCObject
	{
	public:
		CREATE_FUNC(ReplicationRule)

		virtual bool init();

		virtual RakNet::RM3ConstructionState queryConstruction(
			RakNet::Connection_RM3 *destinationConnection, 
			RakNet::ReplicaManager3 *replicaManager3)  SL_VIRTUAL_DECLARE;

		virtual bool queryRemoteConstruction(RakNet::Connection_RM3 *sourceConnection)  SL_VIRTUAL_DECLARE;

		virtual RakNet::RM3QuerySerializationResult querySerialization(
			RakNet::Connection_RM3 *destinationConnection)  SL_VIRTUAL_DECLARE;

		virtual RakNet::RM3ActionOnPopConnection queryActionOnPopConnection(
			RakNet::Connection_RM3 *droppedConnection) const  SL_VIRTUAL_DECLARE;

		SL_SYNTHESIZE_IS(bool, _isValid, Valid, IsValid)
		SL_SYNTHESIZE(ENetwokTopology, _networkTopology, Topology)
	protected:
		ReplicationRule();
		virtual ~ReplicationRule();

	private:

	};


}


#endif
