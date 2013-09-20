//!---------------------------------------------------------------------------
//! \file "slPeer.h"
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
#ifndef __SLPEER_H__
#define __SLPEER_H__

#include "slNetworkMacros.h"
#include "slNetworkEnums.h"
#include "slNetworkInterfaces.h"
#include "slNetworkAddress.h"

// forwarding some stuff to keep the dependencies where they belong

namespace cocos2d	{
	class CCDictionary;
	class CCString;
}

namespace RakNet	{
	class RakPeerInterface;
	class SocketLayer;
	struct SocketDescriptor;
	struct RakNetGUID;
}

namespace sl	{

	class PeerWrapper;
	class ReplicaManager;

	typedef struct NetworkSimulatorSetting_t
	{
	public:
		NetworkSimulatorSetting_t()
			:enabled(false),
			packetloss(0.0f),
			minExtraPing(0),
			extraPingVariance(0){}
		bool enabled;
		float packetloss;
		int minExtraPing;
		int extraPingVariance;
	}	NetworkSimulatorSetting;

	class slNetwork_API Peer
	{
	public:
		Peer();
		virtual ~Peer();

		void shutdown();
		void updateNetwork(float dt, INetPacketDispatcher* packetDispatcher);

		void setClient();
		void setPeerToPeer();
		void setServer();

		bool isClient() const;
		bool isPeerToPeer() const;
		bool isServer() const;
		bool isConnected() const;

		void setTryConnect(bool value);

		void log( ELogType type, const char* format, ... ) const;
		void log( ELogType type, CCDictionary* dictionary ) const;

		NetworkAddress& getNetworkAddress();
		const NetworkAddress& getNetworkAddress() const;

		RakNet::RakPeerInterface* accessRakNetPeer() const;
		const RakNet::RakNetGUID& getPeerGUID() const;

		unsigned short getNumberOfConnections()	const;

		void setConfigurationRootKey( const char* value);
		const char* getConfigurationRootKey() const;
		cocos2d::CCDictionary* getPeerConfiguration() const;

		void setPeerIdx(SLSize idx);

		// functions to access raknet plugins
		ReplicaManager* getReplicaManager() const;

		void updateNetworkSimulatorSetting(const NetworkSimulatorSetting& setting);

		static unsigned char getPacketIdentifier(RakNet::Packet *p);
		static CCString* getPacketInfoString(RakNet::Packet *packet, bool addGUID = false);

		SL_SYNTHESIZE(IRakNetPluginCreator*, _replicaManagerPluginCreator, ReplicaManagerCreator)
		SL_SYNTHESIZE_READONLY(ENetwokTopology, _topolgy, Topology)

	protected:
		virtual void onReceivedPacket(RakNet::Packet* packet);

		virtual void updateClientNetwork(float dt, INetPacketDispatcher* packetDispatcher);
		virtual void updatePeerToPeerNetwork(float dt, INetPacketDispatcher* packetDispatcher);
		virtual void updateServerNetwork(float dt, INetPacketDispatcher* packetDispatcher);

		void advertiseSystem( float dt );

		PeerWrapper* createPeerWrapper() const;

		RakNet::RakPeerInterface* _raknetPeer;

	private:
		void connect();
		void launch();

		void attachPlugins();
		void detachPlugins(RakNet::RakPeerInterface* peer);

		void connectClient();
		void connectPeerToPeer();
		void connectServer();

		void receivedPongPacket( RakNet::Packet* packet );

		bool isPortInUse( RakNet::SocketDescriptor &socketDescriptor ) const;

		void setIsServer( bool value );

		void logPeerInfo() const;

		NetworkAddress _netWorkAddress;

		enum{
			_socketDescriptorCount = 2
		};

		NetworkSimulatorSetting _networkSimulatorSetting;
		RakNet::SocketDescriptor _socketDescriptors[_socketDescriptorCount];
		RakNet::RakNetGUID _peerGUID;

		// raknet plugins
		RakNet::PacketLogger _packetLogger;
		RakNet::FullyConnectedMesh2 _fullyConnectedMeshPlugin;
		RakNet::ConnectionGraph2 _connectionGraphPlugin;

		RakNet::RPC4 _rpc4Plugin;

		// using a pointer here as we reallocate the network id manager for each peer
		RakNet::PluginInterface2* _replicaManager;

		sl::AnimationTick _peerToPeerAdvertisementTicker;

		SLAString _configurationRootKey;
		SLSize _peerIdx;

		bool _isConnected;
		bool _tryToConnect;

		bool _isPluginAttachedPacketLogger;
		bool _isPluginAttachedFullyConnectedMesh;
		bool _isPluginAttachedConnectionGraph2;
		bool _isPluginAttachedRPC4;
		bool _isPluginAttachedReplicaManager;
	};



	class slNetwork_API PeerWrapper : public CCObject	{
		SL_DECLARE_BASE(CCObject)
	public:
		CREATE_FUNC(PeerWrapper)

		CC_SYNTHESIZE(Peer*, _peer, Peer)
		CC_SYNTHESIZE(RakNet::SystemAddress, _systemAddress, SystemAddress)
		CC_SYNTHESIZE(RakNet::RakNetGUID, _guid, GUID)

	protected:
		PeerWrapper();
		virtual ~PeerWrapper();


		virtual bool init() SL_VIRTUAL_DECLARE;


	};

}


#endif
