//!---------------------------------------------------------------------------
//! \file "nlGameContentReplicaManager.h"
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
#ifndef __NLGAMECONTENTREPLICAMANAGER_H__
#define __NLGAMECONTENTREPLICAMANAGER_H__


namespace sl	{
	class GameplayLayer;
}

namespace nl	{

	class GameContentReplicaManager;
	class TankVsTankGameplayLayer;

	class ReplicaManagerCreator : public IRakNetPluginCreator
	{
	public:
		ReplicaManagerCreator();

		virtual RakNet::PluginInterface2* createPlugin(Peer* parentPeer, ENetwokTopology topolgy);

		virtual void destroyPlugin(RakNet::PluginInterface2* plugin);

		SL_SYNTHESIZE(GameContentReplicaManager*, _replicaManager, ReplicaManager)
		SL_SYNTHESIZE(GameplayLayer*, _gameplayLayer, GameplayLayer)
	};


	class GameContentConnection : public Connection_RM3 {
	public:
		GameContentConnection(const SystemAddress &_systemAddress, RakNetGUID _guid);

		virtual ~GameContentConnection();

		// See documentation - Makes all messages between ID_REPLICA_MANAGER_DOWNLOAD_STARTED and
		// ID_REPLICA_MANAGER_DOWNLOAD_COMPLETE arrive in one tick

		bool QueryGroupDownloadMessages(void) const;

		virtual Replica3 *AllocReplica(RakNet::BitStream *allocationId, ReplicaManager3 *replicaManager3);
	protected:
	};




	class GameContentReplicaManager : public ReplicaManager
	{
		SL_DECLARE_BASE(ReplicaManager)
	public:
		GameContentReplicaManager(ENetwokTopology topology);
		virtual ~GameContentReplicaManager();

		virtual RakNet::Connection_RM3* AllocConnection(
			const RakNet::SystemAddress &systemAddress, 
			RakNet::RakNetGUID rakNetGUID) const;

		//! topology specific implementation of create replica
		virtual AbstractReplica* createClientReplica(const char* classId, CCDictionary* constructionDictionary) SL_OVERRIDE;
		//! topology specific implementation of create replica
		virtual AbstractReplica* createPeerToPeerReplica(const char* classId, CCDictionary* constructionDictionary) SL_OVERRIDE;
		//! topology specific implementation of create replica
		virtual AbstractReplica* createServerReplica(const char* classId, CCDictionary* constructionDictionary) SL_OVERRIDE;

		void readReplicas();
		void updateReplicas();
		void deleteReplicas();


		CC_SYNTHESIZE(GameplayLayer*, _gameplayLayer, GameplayLayer)

	protected:
		virtual RakNet::PluginReceiveResult OnReceive(RakNet::Packet *packet);
	private:
	};


}


#endif // __NLGAMECONTENTREPLICAMANAGER_H__
