//!---------------------------------------------------------------------------
//! \file "nlSampleReplicaManager.cpp"
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
#include "nlSampleReplicaManager.h"
#include "nlSampleReplica.h"

namespace nl	{

	//-------------------------------------------------------------------------
	// ClientCreatible_ClientSerialized
	//-------------------------------------------------------------------------
	class ClientCreatible_ClientSerialized : public SampleReplica {
	public:
		ClientCreatible_ClientSerialized::~ClientCreatible_ClientSerialized()	{
		}

		SL_IMPLEMENT_CLASSNAME(ClientCreatible_ClientSerialized)

		virtual RM3SerializationResult Serialize(SerializeParameters *serializeParameters)
		{
			return SampleReplica::Serialize(serializeParameters);
		}

		virtual RM3ConstructionState QueryConstruction(RakNet::Connection_RM3 *destinationConnection, ReplicaManager3 *replicaManager3) {
			return QueryConstruction_ClientConstruction(destinationConnection,getTopology()!=CLIENT);
		}

		virtual bool QueryRemoteConstruction(RakNet::Connection_RM3 *sourceConnection) {
			return QueryRemoteConstruction_ClientConstruction(sourceConnection,getTopology()!=CLIENT);
		}

		virtual RM3QuerySerializationResult QuerySerialization(RakNet::Connection_RM3 *destinationConnection) {
			return QuerySerialization_ClientSerializable(destinationConnection,getTopology()!=CLIENT);
		}
		virtual RM3ActionOnPopConnection QueryActionOnPopConnection(RakNet::Connection_RM3 *droppedConnection) const {
			return QueryActionOnPopConnection_Client(droppedConnection);
		}
	};

	//-------------------------------------------------------------------------
	// ClientCreatible_ServerSerialized
	//-------------------------------------------------------------------------
	class ClientCreatible_ServerSerialized : public SampleReplica {
	public:
		ClientCreatible_ServerSerialized::~ClientCreatible_ServerSerialized()	{
		}

		SL_IMPLEMENT_CLASSNAME(ClientCreatible_ServerSerialized)

		virtual RM3SerializationResult Serialize(SerializeParameters *serializeParameters)
		{
			if (getTopology()==CLIENT)	{
				return RM3SR_DO_NOT_SERIALIZE;
			}
			return SampleReplica::Serialize(serializeParameters);
		}
		virtual RM3ConstructionState QueryConstruction(RakNet::Connection_RM3 *destinationConnection, ReplicaManager3 *replicaManager3) {
			return QueryConstruction_ClientConstruction(destinationConnection,getTopology()!=CLIENT);
		}
		virtual bool QueryRemoteConstruction(RakNet::Connection_RM3 *sourceConnection) {
			return QueryRemoteConstruction_ClientConstruction(sourceConnection,getTopology()!=CLIENT);
		}
		virtual RM3QuerySerializationResult QuerySerialization(RakNet::Connection_RM3 *destinationConnection) {
			return QuerySerialization_ServerSerializable(destinationConnection,getTopology()!=CLIENT);
		}
		virtual RM3ActionOnPopConnection QueryActionOnPopConnection(RakNet::Connection_RM3 *droppedConnection) const {
			return QueryActionOnPopConnection_Client(droppedConnection);
		}
	};

	// note: this might be a very very bad idea !!!
	//       think about the use case
	//-------------------------------------------------------------------------
	// ServerCreated_ClientSerialized
	//-------------------------------------------------------------------------
	class ServerCreated_ClientSerialized : public SampleReplica {
	public:
		ServerCreated_ClientSerialized::~ServerCreated_ClientSerialized()	{
		}

		SL_IMPLEMENT_CLASSNAME(ServerCreated_ClientSerialized)

		virtual RM3SerializationResult Serialize(SerializeParameters *serializeParameters)
		{
			return SampleReplica::Serialize(serializeParameters);
		}

		virtual RM3ConstructionState QueryConstruction(RakNet::Connection_RM3 *destinationConnection, ReplicaManager3 *replicaManager3) {
			return QueryConstruction_ServerConstruction(destinationConnection,getTopology()!=CLIENT);
		}

		virtual bool QueryRemoteConstruction(RakNet::Connection_RM3 *sourceConnection) {
			return QueryRemoteConstruction_ServerConstruction(sourceConnection,getTopology()!=CLIENT);
		}

		virtual RM3QuerySerializationResult QuerySerialization(RakNet::Connection_RM3 *destinationConnection) {
			if(getTopology()==CLIENT)	{
				return RakNet::RM3QSR_CALL_SERIALIZE;
			}
			return QuerySerialization_ClientSerializable(destinationConnection,getTopology()!=CLIENT);
		}
		virtual RM3ActionOnPopConnection QueryActionOnPopConnection(RakNet::Connection_RM3 *droppedConnection) const {
			return QueryActionOnPopConnection_Server(droppedConnection);
		}
	};

	//-------------------------------------------------------------------------
	// ServerCreated_ServerSerialized
	//-------------------------------------------------------------------------
	class ServerCreated_ServerSerialized : public SampleReplica {
	public:
		ServerCreated_ServerSerialized::~ServerCreated_ServerSerialized()	{
		}

		SL_IMPLEMENT_CLASSNAME(ServerCreated_ServerSerialized)

		virtual RM3SerializationResult Serialize(SerializeParameters *serializeParameters)
		{
			if (getTopology()==CLIENT)	{
				return RM3SR_DO_NOT_SERIALIZE;
			}
			return SampleReplica::Serialize(serializeParameters);
		}

		virtual RM3ConstructionState QueryConstruction(RakNet::Connection_RM3 *destinationConnection, ReplicaManager3 *replicaManager3) {
			return QueryConstruction_ServerConstruction(destinationConnection,getTopology()!=CLIENT);
		}

		virtual bool QueryRemoteConstruction(RakNet::Connection_RM3 *sourceConnection) {
			return QueryRemoteConstruction_ServerConstruction(sourceConnection,getTopology()!=CLIENT);
		}

		virtual RM3QuerySerializationResult QuerySerialization(RakNet::Connection_RM3 *destinationConnection) {
			return QuerySerialization_ServerSerializable(destinationConnection,getTopology()!=CLIENT);
		}

		virtual RM3ActionOnPopConnection QueryActionOnPopConnection(RakNet::Connection_RM3 *droppedConnection) const {
			return QueryActionOnPopConnection_Server(droppedConnection);
		}
	};

	//-------------------------------------------------------------------------
	// P2PReplica
	//-------------------------------------------------------------------------
	class P2PReplica : public SampleReplica {
	public:
		P2PReplica::~P2PReplica()	{
		}

		SL_IMPLEMENT_CLASSNAME(P2PReplica)

		virtual RM3SerializationResult Serialize(SerializeParameters *serializeParameters)
		{
			if (getTopology()==CLIENT)	{
				return RM3SR_DO_NOT_SERIALIZE;
			}
			return SampleReplica::Serialize(serializeParameters);
		}

		virtual RM3ConstructionState QueryConstruction(RakNet::Connection_RM3 *destinationConnection, ReplicaManager3 *replicaManager3) {
			return QueryConstruction_PeerToPeer(destinationConnection);
		}

		virtual bool QueryRemoteConstruction(RakNet::Connection_RM3 *sourceConnection) {
			return QueryRemoteConstruction_PeerToPeer(sourceConnection);
		}

		virtual RM3QuerySerializationResult QuerySerialization(RakNet::Connection_RM3 *destinationConnection) {
			return QuerySerialization_PeerToPeer(destinationConnection);
		}

		virtual RM3ActionOnPopConnection QueryActionOnPopConnection(RakNet::Connection_RM3 *droppedConnection) const {
			return QueryActionOnPopConnection_PeerToPeer(droppedConnection);
		}
	};

	//-------------------------------------------------------------------------
	// SampleConnection
	//-------------------------------------------------------------------------
	class SampleConnection : public Connection_RM3 {
	public:
		SampleConnection(const SystemAddress &_systemAddress, RakNetGUID _guid) : Connection_RM3(_systemAddress, _guid) 
		{

		}

		virtual ~SampleConnection() 
		{

		}

		// See documentation - Makes all messages between ID_REPLICA_MANAGER_DOWNLOAD_STARTED and
		// ID_REPLICA_MANAGER_DOWNLOAD_COMPLETE arrive in one tick

		bool QueryGroupDownloadMessages(void) const {return true;}

		virtual Replica3 *AllocReplica(RakNet::BitStream *allocationId, ReplicaManager3 *replicaManager3)
		{
			AbstractReplica* replica(nullptr);
			RakNet::RakString typeName;
			allocationId->Read(typeName);

			if (typeName == "ClientCreatible_ClientSerialized") { 
				replica = new ClientCreatible_ClientSerialized(); 
			}
			else if (typeName == "ServerCreated_ClientSerialized") { 
				replica = new ServerCreated_ClientSerialized(); 
			}
			else if (typeName == "ClientCreatible_ServerSerialized") { 
				replica = new ClientCreatible_ServerSerialized(); 
			}
			else if (typeName == "ServerCreated_ServerSerialized") { 
				replica = new ServerCreated_ServerSerialized(); 
			}
			else if (typeName == "P2PReplica") { 
				replica = new P2PReplica;
			}

			ReplicaManager* plugin(dynamic_cast<ReplicaManager*>(replicaManager3));
			if(replica != nullptr)	{
				replica->setPeer(plugin->getPeer());
				plugin->getPeer()->log(ELogType_Message, "allocated replica: %s", 
					replica->getName().C_String());
			}
			else	{
				plugin->getPeer()->log(ELogType_Error, "failed to allocate replica: %s", 
					typeName.C_String());
			}
			
			return replica;
		}
	protected:
	};

	//-------------------------------------------------------------------------
	// SampleReplicaManager
	//-------------------------------------------------------------------------
	SampleReplicaManager::SampleReplicaManager(ENetwokTopology topology)
		:SLBaseClass(topology)
	{
	}

	SampleReplicaManager::~SampleReplicaManager()	{
	}

	RakNet::Connection_RM3* SampleReplicaManager::AllocConnection(
		const RakNet::SystemAddress &systemAddress, 
		RakNet::RakNetGUID rakNetGUID) const {
		return new SampleConnection(systemAddress,rakNetGUID);
	}

	RakNet::PluginReceiveResult SampleReplicaManager::OnReceive( RakNet::Packet *packet )	{
		RakNet::PluginReceiveResult result(SLBaseClass::OnReceive(packet));
		getPeer()->log(ELogType_Message, "ReplicaManager received : %s", Peer::getPacketInfoString(packet)->getCString());
		return result;
	}

	void SampleReplicaManager::createReplicas()	{
		AbstractReplica* replica;
		switch(getPeer()->getTopology())	{
		case(CLIENT):
			{
				replica = new ClientCreatible_ClientSerialized();
				replica->setPeer(getPeer());
				referenceReplica(replica);

				replica = new ClientCreatible_ServerSerialized();
				replica->setPeer(getPeer());
				referenceReplica(replica);
			}
			break;
		case(SERVER):
			{
				replica = new ServerCreated_ClientSerialized();
				replica->setPeer(getPeer());
				referenceReplica(replica);

				replica = new ServerCreated_ServerSerialized();
				replica->setPeer(getPeer());
				referenceReplica(replica);
			}
			break;
		case(P2P):
			{
				replica = new P2PReplica();
				replica->setPeer(getPeer());
				referenceReplica(replica);
			}
			break;
		}
	}

	void SampleReplicaManager::readReplicas()	{

	}

	void SampleReplicaManager::updateReplicas()	{

		unsigned replicaCount(GetReplicaCount());
		for (unsigned int idx(0); idx < replicaCount; ++idx)	{
			SampleReplica* replica(dynamic_cast<SampleReplica*>(GetReplicaAtIndex(idx)));
			if(replica != nullptr)	{
				RakNet::RakString typeName(replica->getName());

				if (typeName=="ClientCreatible_ClientSerialized") {
					if(getTopology() == CLIENT && replica->getCreatedByReplicaManager())	{
						replica->randomizeVariables();
					}
				}
				else if (typeName=="ClientCreatible_ServerSerialized") { 
					if(getTopology() == SERVER)	{
						replica->randomizeVariables();
					}
				}

				else if (typeName=="ServerCreated_ClientSerialized") { 
					if(getTopology() == CLIENT)	{
						replica->randomizeVariables();
					}
				}
				else if (typeName=="ServerCreated_ServerSerialized") { 
					if(getTopology() == SERVER)	{
						replica->randomizeVariables();
					}
				}
				else if (typeName=="P2PReplica") { 
					if(getTopology() == P2P)	{
						replica->randomizeVariables();
					}
				}
			}
		}
	}

	void SampleReplicaManager::deleteReplicas()	{
		deleteCreatedReplicas(false);
	}


}

