//!---------------------------------------------------------------------------
//! \file "slPeer.cpp"
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
#include "slPeer.h"

#include "replica/slReplicaManager.h"
#include "replica/slAbstractReplica.h"
#include "cocosnet/slNetworkState.h"


#define SL_ADVERTISE_NETWORK_MASK "255.255.255.255"

using namespace RakNet;

namespace sl	{
	ELogType logType = ELogType_Info;

	// note: this replica manager will most probably never work
	class DefaultReplicaManagerCreator : public IRakNetPluginCreator
	{
	public:
		virtual RakNet::PluginInterface2* createPlugin(Peer* parentPeer, ENetwokTopology topolgy)	{
			ReplicaManager* replicaManager = new ReplicaManager(topolgy);
			replicaManager->setPeer(parentPeer);
			return replicaManager;
		}

		virtual void destroyPlugin(RakNet::PluginInterface2* plugin)	{
			CC_SAFE_DELETE(plugin);
		}

		static IRakNetPluginCreator& sharedInstance()	{
			static DefaultReplicaManagerCreator pluginCreator;
			return pluginCreator;
		}
	};


	// unknown topology by default
	Peer::Peer()
		:_raknetPeer(nullptr)
		,_replicaManager(nullptr)
		,_topolgy(UNKNOWN)
		,_isConnected(false)
		,_tryToConnect(false)
		,_peerIdx(0)
		,_replicaManagerPluginCreator(&DefaultReplicaManagerCreator::sharedInstance())
	{
		_isPluginAttachedPacketLogger =
			_isPluginAttachedFullyConnectedMesh =
			_isPluginAttachedConnectionGraph2 =
			_isPluginAttachedRPC4 =
			_isPluginAttachedReplicaManager =
			false;

		_peerToPeerAdvertisementTicker.setAnimationInterval(3.0);

		unsigned char maxNetId(NETMSG_ID_END);
		if(maxNetId < NETMSG_ID_START)	{
			ProcessApplication::sharedInstance()->logger().error("WTF");
		}
	}

	Peer::~Peer()	{
		shutdown();
	}

	void Peer::log( ELogType type, const char* format, ... ) const	{
		CCString* tempString = new CCString();
		va_list ap;
		va_start(ap, format);
		bool initialized(tempString->initWithFormatAndValist(format, ap));
		va_end(ap);
		if(initialized)	{
			SLAString addressInfo;
			_netWorkAddress.getString(getTopology(),addressInfo);
			SL_PROCESS_APP()->log(type, isClient() ? EConsoleBackColor_Grey : EConsoleBackColor_Black, 
				isClient() ? 
				"Client-%d %s: %s" : isServer() ?
				"Server-%d %s: %s" : 
				"P2Peer-%d %s: %s", 
				_peerIdx,
				addressInfo.c_str(),
				tempString->getCString());
		}
		tempString->release();
	}

	void Peer::log( ELogType type, CCDictionary* dictionary ) const	{
		if(dictionary == nullptr)	{
			log(ELogType_Error, "log enmpty dictionary ?");
			return;
		}
		// Dump
		CCPrettyPrinter visitor(0);
		dictionary->acceptVisitor(visitor);
		log( type, "\n%s", visitor.getResult().c_str());
	}


	void Peer::connect()	{
		if(_isConnected)	{
			// in case the peer is connected nothing has to be done here
			return;
		}
		else	{
			// if the peer does not want to connect nothing has to be done here either
			if(_tryToConnect == false)	{
				return;
			}
		}

		if(_raknetPeer == nullptr)	{
			launch();
			if(_raknetPeer == nullptr)	{
				return;
			}
		}

		if(isServer())	{
			connectServer();
		}
		else if(isClient())	{
			connectClient();
		}
		else if(isPeerToPeer())	{
			connectPeerToPeer();
		}
	}

	bool Peer::isPortInUse( RakNet::SocketDescriptor &socketDescriptor ) const	{
		bool portInUse(false);
		portInUse = (RakNet::IRNS2_Berkley::IsPortInUse(socketDescriptor.port, 
			socketDescriptor.hostAddress, socketDescriptor.socketFamily, SOCK_DGRAM) == true);
		return portInUse;
	}

	PeerWrapper* Peer::createPeerWrapper() const	{
		PeerWrapper* peerWrapper(PeerWrapper::create());
		peerWrapper->setPeer(((Peer*)this));
		return peerWrapper;
	}

	void Peer::connectClient()	{
		if(_raknetPeer == nullptr)	{
			launch();
		}

		if(_raknetPeer == nullptr)	{
			return;
		}
		log( logType, "starting client ...");

		unsigned int maxIncomingConnections(32);
		unsigned int maxConnections(32);


		// Connecting the client is very simple.  0 means we don't care about
		// a connectionValidationInteger, and false for low priority threads
		RakNet::SocketDescriptor clientSocketDescriptor(_netWorkAddress.getClientPort(),0);
		_socketDescriptors[0] = clientSocketDescriptor;
		_socketDescriptors[0].socketFamily=AF_INET;

		while( isPortInUse(_socketDescriptors[0]) )	{
			++_socketDescriptors[0].port;
		}

		_netWorkAddress.setClientPort(_socketDescriptors[0].port);

		if(_raknetPeer->Startup(maxConnections, _socketDescriptors, 1) == RakNet::RAKNET_STARTED)	{
			_raknetPeer->SetMaximumIncomingConnections(maxIncomingConnections);
			_raknetPeer->SetOccasionalPing(true);
			_raknetPeer->SetUnreliableTimeout(1000);

			RakNet::PublicKey* publicKey(nullptr);

#if LIBCAT_SECURITY==1
			RakNet::PublicKey pk;
			publicKey = &pk
			char public_key[cat::EasyHandshake::PUBLIC_KEY_BYTES];
			FILE *fp = fopen("publicKey.dat","rb");
			fread(public_key,sizeof(public_key),1,fp);
			fclose(fp);
			pk.remoteServerPublicKey=public_key;
			pk.publicKeyMode=RakNet::PKM_USE_KNOWN_PUBLIC_KEY;
#else
#endif
			RakNet::ConnectionAttemptResult car = _raknetPeer->Connect(
				_netWorkAddress.getServerIp(),
				_netWorkAddress.getServerPort(), 
				_netWorkAddress.getSessionPassword(), 
				(int) strlen(_netWorkAddress.getSessionPassword()),
				publicKey);
			if(car == RakNet::CONNECTION_ATTEMPT_STARTED)	{
				_isConnected = true;
				_peerGUID = _raknetPeer->GetGuidFromSystemAddress(RakNet::UNASSIGNED_SYSTEM_ADDRESS);
				log( logType, "Client started with GUID(%s).", _peerGUID.ToString());

				// notify those who want to know
				Notification notification(SL_NOTIFY_PEERSTATE_IS_CONNECTED);
				notification.postNotification(createPeerWrapper());
			}
			else	{
				log( ELogType_Error, "Client failed to start.");
				RakAssert(car==RakNet::CONNECTION_ATTEMPT_STARTED);

				// notify those who want to know
				Notification notification(SL_NOTIFY_PEERSTATE_FAILED_TO_CONNECT);
				notification.postNotification(createPeerWrapper());

				shutdown();
			}
		}
	}

	void Peer::connectPeerToPeer( )	{
		log( logType, "starting peer to peer ...");
		RakNet::RakPeerInterface* peerToPeerPeer(_raknetPeer);

		peerToPeerPeer->SetIncomingPassword(_netWorkAddress.getSessionPassword(), 
			(int) strlen(_netWorkAddress.getSessionPassword()));
		peerToPeerPeer->SetTimeoutTime(30000,RakNet::UNASSIGNED_SYSTEM_ADDRESS);

#if LIBCAT_SECURITY==1
		cat::EasyHandshake handshake;
		char public_key[cat::EasyHandshake::PUBLIC_KEY_BYTES];
		char private_key[cat::EasyHandshake::PRIVATE_KEY_BYTES];
		handshake.GenerateServerKey(public_key, private_key);
		peerToPeerPeer->InitializeSecurity(public_key, private_key, false);
		FILE *fp = fopen("publicKey.dat","wb");
		fwrite(public_key,sizeof(public_key),1,fp);
		fclose(fp);
#endif

		const unsigned short peerToPeerPort(_netWorkAddress.getPeerToPeerPort());

		unsigned int maxIncomingConnections(32);
		unsigned int maxConnections(32);

		_socketDescriptors[0].port=peerToPeerPort;
		_socketDescriptors[0].socketFamily=AF_INET; // Test out IPV4
		_socketDescriptors[1].port=peerToPeerPort;
		_socketDescriptors[1].socketFamily=AF_INET6; // Test out IPV6

		while( isPortInUse(_socketDescriptors[0]) )	{
			++_socketDescriptors[0].port;
			++_socketDescriptors[1].port;
		}

#if 0
		// note: raknet will try to bind the descriptor
		//       so in case this fails the port is assumed to be used
		//       in consequence we can not use this routine for AF_INET6
		while( isPortInUse(socketDescriptors[1]) )	{
			++socketDescriptors[0].port;
			++socketDescriptors[1].port;
		}
#endif
		_netWorkAddress.setPeerToPeerPort(_socketDescriptors[1].port);

		// Starting the server is very simple.  maxConnections players allowed.
		// 0 means we don't care about a connectionValidationInteger, and false
		// for low priority threads
		// I am creating two socketDesciptors, to create two sockets. One using IPV6 and the other IPV4
		bool peerToPeerStarted(peerToPeerPeer->Startup(maxConnections, _socketDescriptors, _socketDescriptorCount ) == RakNet::RAKNET_STARTED);
		if (!peerToPeerStarted)	{
			log( logType, "Failed to start dual IPV4 and IPV6 ports. Trying IPV4 only.");

			// Try again, but leave out IPV6
			peerToPeerStarted = (peerToPeerPeer->Startup(maxConnections, _socketDescriptors, 1 ) == RakNet::RAKNET_STARTED);
			if (!peerToPeerStarted)
			{
				log( ELogType_Error, "P2P-Peer failed to start.");
			}
		}
		if (peerToPeerStarted)	{
			_peerGUID = _raknetPeer->GetGuidFromSystemAddress(RakNet::UNASSIGNED_SYSTEM_ADDRESS);
			peerToPeerPeer->SetMaximumIncomingConnections(maxIncomingConnections);
			peerToPeerPeer->SetOccasionalPing(true);
			peerToPeerPeer->SetUnreliableTimeout(1000);
			_isConnected = true;

			log( logType, "Peer to peer started with GUID(%s).", _peerGUID.ToString());

			// notify those who want to know
			Notification notification(SL_NOTIFY_PEERSTATE_IS_CONNECTED);
			notification.postNotification(createPeerWrapper());
		}
		else	{
			// notify those who want to know
			Notification notification(SL_NOTIFY_PEERSTATE_FAILED_TO_CONNECT);
			notification.postNotification(createPeerWrapper());
		}
	}

	void Peer::connectServer( )	{
		log( logType, "starting server ...");
		RakNet::RakPeerInterface* server(_raknetPeer);

		server->SetIncomingPassword(_netWorkAddress.getSessionPassword(), 
			(int) strlen(_netWorkAddress.getSessionPassword()));
		server->SetTimeoutTime(30000,RakNet::UNASSIGNED_SYSTEM_ADDRESS);

#if LIBCAT_SECURITY==1
		cat::EasyHandshake handshake;
		char public_key[cat::EasyHandshake::PUBLIC_KEY_BYTES];
		char private_key[cat::EasyHandshake::PRIVATE_KEY_BYTES];
		handshake.GenerateServerKey(public_key, private_key);
		server->InitializeSecurity(public_key, private_key, false);
		FILE *fp = fopen("publicKey.dat","wb");
		fwrite(public_key,sizeof(public_key),1,fp);
		fclose(fp);
#endif

		const unsigned short serverPort(_netWorkAddress.getServerPort());

		unsigned int maxIncomingConnections(32);
		unsigned int maxConnections(32);

		_socketDescriptors[0].port=serverPort;
		_socketDescriptors[0].socketFamily=AF_INET; // Test out IPV4
		_socketDescriptors[1].port=serverPort;
		_socketDescriptors[1].socketFamily=AF_INET6; // Test out IPV6

		while( isPortInUse(_socketDescriptors[0]) )	{
			++_socketDescriptors[0].port;
			++_socketDescriptors[1].port;
		}

#if 0
		// note: raknet will try to bind the descriptor
		//       so in case this fails the port is assumed to be used
		//       in consequence we can not use this routine for AF_INET6
		while( isPortInUse(socketDescriptors[1]) )	{
			++socketDescriptors[0].port;
			++socketDescriptors[1].port;
		}
#endif
		_netWorkAddress.setServerPort(_socketDescriptors[1].port);

		// Starting the server is very simple.  maxConnections players allowed.
		// 0 means we don't care about a connectionValidationInteger, and false
		// for low priority threads
		// I am creating two socketDesciptors, to create two sockets. One using IPV6 and the other IPV4
		bool serverStarted(server->Startup(maxConnections, _socketDescriptors, _socketDescriptorCount ) == RakNet::RAKNET_STARTED);
		if (!serverStarted)	{
			log( logType, "Failed to start dual IPV4 and IPV6 ports. Trying IPV4 only.");

			// Try again, but leave out IPV6
			serverStarted = (server->Startup(maxConnections, _socketDescriptors, 1 ) == RakNet::RAKNET_STARTED);
			if (!serverStarted)	{
				log( ELogType_Error, "Server failed to start.");
			}
			else	{
				// notify those who want to know
				Notification notification(SL_NOTIFY_PEERSTATE_IS_CONNECTED);
				notification.postNotification(createPeerWrapper());
			}
		}
		if (serverStarted)	{
			_peerGUID = _raknetPeer->GetGuidFromSystemAddress(RakNet::UNASSIGNED_SYSTEM_ADDRESS);
			server->SetMaximumIncomingConnections(maxIncomingConnections);
			server->SetOccasionalPing(true);
			server->SetUnreliableTimeout(1000);
			_isConnected = true;
			log( logType, "Server started with GUID(%s).", _peerGUID.ToString());
		}
		else	{
			// notify those who want to know
			Notification notification(SL_NOTIFY_PEERSTATE_FAILED_TO_CONNECT);
			notification.postNotification(createPeerWrapper());
		}
	}

	void Peer::launch()	{
		shutdown();

		_raknetPeer = RakPeerInterface::GetInstance();

		_raknetPeer->SetIncomingPassword(_netWorkAddress.getSessionPassword(), 
			(int) strlen(_netWorkAddress.getSessionPassword()));

		attachPlugins();
	}

	unsigned short Peer::getNumberOfConnections()	const {
		if(_raknetPeer != NULL)	{
			return _raknetPeer->NumberOfConnections();
		}
		return 0;
	}

	bool isPeerConnected(RakNet::RakPeerInterface* peer)	{
		if(peer != NULL)	{
			return 0 < peer->NumberOfConnections();
		}
		return false;
	}

	void closeOpenConnections( RakNet::RakPeerInterface* peer )	{
		if( peer != NULL )
		{
			DataStructures::List<SystemAddress> kAddresses;
			DataStructures::List<RakNetGUID> kGuids;

			peer->GetSystemList( kAddresses, kGuids );
			unsigned int usCount = kAddresses.Size();
			if( 0 < usCount )
			{
				for(unsigned int us = 0; us < usCount; ++us)
				{
					peer->CloseConnection(
						kAddresses[us], true );
				}

				while(isPeerConnected(peer))	{
					Poco::Thread::sleep( 100 );
				}
			}
		}
	}

	void Peer::shutdown()	{
		_isConnected = false;
		RakNet::RakPeerInterface* shutDownPeer(_raknetPeer);
		_raknetPeer = nullptr;

		if(shutDownPeer != nullptr)	{
			SLAString peerTypeInfo = isServer() ? "Server" : 
				isClient() ? "Client" : "P2Peer";

			SLAString peerLog(peerTypeInfo);
			peerLog += " shutting down ...";
			log( logType, peerLog.c_str());

			// notify those who want to know
			Notification notification(SL_NOTIFY_PEERSTATE_WILL_DISCONNECT);
			notification.postNotification(createPeerWrapper());

			// do not allow any further connections
			shutDownPeer->SetMaximumIncomingConnections( 0 );

			// detach all plugins before all connections will be closed
			// so we can notify connected peers about this
			detachPlugins(shutDownPeer);

			closeOpenConnections(shutDownPeer);

			shutDownPeer->Shutdown(300);

			RakNet::RakPeerInterface::DestroyInstance(shutDownPeer);

			peerLog = peerTypeInfo;
			peerLog += " shut down.";
			log( logType, peerLog.c_str());
		}
	}

	void Peer::updateNetworkSimulatorSetting(const NetworkSimulatorSetting& setting)	{
		if(&setting != &_networkSimulatorSetting)	{
			_networkSimulatorSetting = setting;
		}
		if(isConnected())	{
			if(_networkSimulatorSetting.enabled)	{
				_networkSimulatorSetting.packetloss = min(1.0f, max(0.0f, _networkSimulatorSetting.packetloss));
				_raknetPeer->ApplyNetworkSimulator(
					_networkSimulatorSetting.packetloss,
					_networkSimulatorSetting.minExtraPing,
					_networkSimulatorSetting.extraPingVariance
					);
			}
			else	{
				_raknetPeer->ApplyNetworkSimulator(0.0, 0, 0);
			}
		}
	}

	void Peer::updateNetwork(float dt, INetPacketDispatcher* packetDispatcher)	{
		if(isConnected())	{
#ifdef _DEBUG
			updateNetworkSimulatorSetting(_networkSimulatorSetting);
#endif
			if(isServer())	{
				updateServerNetwork(dt, packetDispatcher);
			}
			else if(isClient())	{
				updateClientNetwork(dt, packetDispatcher);
			}
			else if(isPeerToPeer())	{
				updatePeerToPeerNetwork(dt, packetDispatcher);
			}

			// replica manager garbage collection ...
			// TODO @CP : interval
			if(getReplicaManager())	{
				getReplicaManager()->collectGarbage();
			}
		}
		else	{
			// try to connect with given parameters
			connect();
		}
	}

	void Peer::updateClientNetwork(float dt, INetPacketDispatcher* packetDispatcher)	{
		bool shutdownPeer(false);
		bool updateStatus(false);

		RakNet::Packet* packet;
		for (packet=_raknetPeer->Receive(); packet; 
			_raknetPeer->DeallocatePacket(packet), packet=_raknetPeer->Receive())
		{
			// We got a packet, get the identifier with our handy function
			unsigned char packetIdentifier = getPacketIdentifier(packet);
			DefaultMessageIDTypes eMessageID = static_cast<DefaultMessageIDTypes>(packetIdentifier);
			NetMessageIDTypes eNetMessageID = static_cast<NetMessageIDTypes>(packetIdentifier);

			packetDispatcher->dispatchPacket(packetIdentifier, packet);

			if((eNetMessageID > NETMSG_ID_START) && (eNetMessageID < NETMSG_ID_END))	{
			}
			else	{
				const char* messageName(RakNet::PacketLogger::BaseIDTOString(packetIdentifier));
				// Check if this is a network message packet
				switch (packetIdentifier)
				{
				case ID_DISCONNECTION_NOTIFICATION:
					// Connection lost normally
					log( logType, "%s from %s with GUID %s", messageName, packet->systemAddress.ToString(true), packet->guid.ToString());
					// server has been shutdown disconnect ...
					if(packet->systemAddress.GetPort() == _netWorkAddress.getServerPort())	{
						shutdownPeer = true;
					}
					break;
				case ID_CONNECTION_BANNED: // Banned from this server
					log( logType, "%s from %s with GUID %s - We are banned from this server.", messageName, packet->systemAddress.ToString(true), packet->guid.ToString());
					shutdownPeer = true;
					break;			
				case ID_CONNECTION_ATTEMPT_FAILED:
					{
						// notify those who want to know
						Notification notification(SL_NOTIFY_PEERSTATE_CONNECTION_ATTEMPT_FAILED);
						notification.postNotification(createPeerWrapper());
						log( logType, "%s from %s with GUID %s", messageName, packet->systemAddress.ToString(true), packet->guid.ToString());
						shutdownPeer = true;
					}
					break;
				case ID_NO_FREE_INCOMING_CONNECTIONS:
					// Sorry, the server is full.  I don't do anything here but
					// A real app should tell the user
					log( logType, "%s from %s with GUID %s", messageName, packet->systemAddress.ToString(true), packet->guid.ToString());
					shutdownPeer = true;
					break;

				case ID_INVALID_PASSWORD:
					log( logType, "%s from %s with GUID %s", messageName, packet->systemAddress.ToString(true), packet->guid.ToString());
					shutdownPeer = true;
					break;

				case ID_CONNECTION_LOST:
					// perform the default behaviour
					// e.g. log and trigger notifications
					onReceivedPacket( packet);

					// Couldn't deliver a reliable packet
					// i.e. the other system was abnormally terminated
					// connection to server has been lost disconnect ...
					if(packet->systemAddress.GetPort() == _netWorkAddress.getServerPort())	{
						shutdownPeer = true;
					}
					break;

				default:
					onReceivedPacket(packet);
					break;
				}
			}
		}

		if(shutdownPeer == true)	{
			setTryConnect(false);
		}
	}


#if 0
	// list connections info
	const SLSize maxConnectionCount(64);
	unsigned short peerToPeerPorts[maxConnectionCount];

	SystemAddress remoteSystems[maxConnectionCount];
	unsigned short numberOfSystems = maxConnectionCount;
	_raknetPeer->GetConnectionList(remoteSystems, &numberOfSystems);
	for (unsigned int i(0); i < numberOfSystems; i++)	{
	}

	if(numberOfSystems >= _raknetPeer->GetMaximumIncomingConnections())	{
		// we have what we wanted
	}
	else	{
	}

#endif

	void Peer::updatePeerToPeerNetwork(float dt, INetPacketDispatcher* packetDispatcher)	{
		bool shutdownPeer(false);
		bool updateStatus(false);

		RakNet::Packet* packet;
		for (packet=_raknetPeer->Receive(); packet; 
			_raknetPeer->DeallocatePacket(packet), packet=_raknetPeer->Receive())
		{
			// We got a packet, get the identifier with our handy function
			unsigned char packetIdentifier = getPacketIdentifier(packet);
			DefaultMessageIDTypes eMessageID = static_cast<DefaultMessageIDTypes>(packetIdentifier);
			NetMessageIDTypes eNetMessageID = static_cast<NetMessageIDTypes>(packetIdentifier);

			packetDispatcher->dispatchPacket(packetIdentifier, packet);

			if((eNetMessageID > NETMSG_ID_START) && (eNetMessageID < NETMSG_ID_END))	{
			}
			else	{
				const char* messageName(RakNet::PacketLogger::BaseIDTOString(packetIdentifier));

				// Check if this is a network message packet
				switch (packetIdentifier)
				{
				case ID_DISCONNECTION_NOTIFICATION:
					// Connection lost normally
					log( logType, "%s from %s with GUID %s", messageName, packet->systemAddress.ToString(true), packet->guid.ToString());
					// another peer has been shutdown.
					break;
				case ID_CONNECTION_BANNED: // Banned from this server
					log( logType, "%s from %s with GUID %s - We are banned.", messageName, packet->systemAddress.ToString(true), packet->guid.ToString());
					shutdownPeer = true;
					break;			
				case ID_CONNECTION_ATTEMPT_FAILED:
					{
						// notify those who want to know
						Notification notification(SL_NOTIFY_PEERSTATE_CONNECTION_ATTEMPT_FAILED);
						notification.postNotification(createPeerWrapper());
						log( logType, "%s from %s with GUID %s", messageName, packet->systemAddress.ToString(true), packet->guid.ToString());
					}
					break;
				case ID_NO_FREE_INCOMING_CONNECTIONS:
					// Sorry, the network is full.
					// A real app should tell the user
					log( logType, "%s from %s with GUID %s", messageName, packet->systemAddress.ToString(true), packet->guid.ToString());
					shutdownPeer = true;
					break;

				case ID_INVALID_PASSWORD:
					log( logType, "%s from %s with GUID %s", messageName, packet->systemAddress.ToString(true), packet->guid.ToString());
					break;

				case ID_CONNECTION_LOST:

					// perform the default behaviour
					// e.g. log and trigger notifications
					onReceivedPacket( packet);

					break;

				default:
					onReceivedPacket(packet);
					break;
				}
			}
		}

		if(shutdownPeer == true)	{
			setTryConnect(false);
		}
		else	{
			advertiseSystem(dt);
		}
	}

	void Peer::updateServerNetwork(float dt, INetPacketDispatcher* packetDispatcher)	{
		RakNet::RakPeerInterface* server(_raknetPeer);
		RakNet::Packet* packet;
		for (packet=server->Receive(); packet; server->DeallocatePacket(packet), packet=server->Receive())
		{
			// We got a packet, get the identifier with our handy function
			unsigned char packetIdentifier = getPacketIdentifier(packet);
			DefaultMessageIDTypes eMessageID = static_cast<DefaultMessageIDTypes>(packetIdentifier);
			NetMessageIDTypes eNetMessageID = static_cast<NetMessageIDTypes>(packetIdentifier);

			packetDispatcher->dispatchPacket(packetIdentifier, packet);

			if((eNetMessageID > NETMSG_ID_START) && (eNetMessageID < NETMSG_ID_END))	{
			}
			else	{
				onReceivedPacket(packet);
			}
		}
	}

	void Peer::setIsServer( bool value )	{
		ENetwokTopology targetTopolgy = value ? SERVER : CLIENT;
		if(_topolgy != targetTopolgy)	{
			shutdown();
			_topolgy = targetTopolgy;
		}
	}

	bool Peer::isClient() const	{
		return (_topolgy == CLIENT);
	}

	bool Peer::isPeerToPeer() const	{
		return (_topolgy == P2P);
	}

	bool Peer::isServer() const	{
		return (_topolgy == SERVER);
	}

	void Peer::setServer()	{
		setIsServer(true);
	}

	void Peer::setClient()	{
		setIsServer(false);
	}

	void Peer::setPeerToPeer()	{
		ENetwokTopology targetTopolgy = P2P;
		if(_topolgy != targetTopolgy)	{
			shutdown();
			_topolgy = targetTopolgy;
		}
	}

	NetworkAddress& Peer::getNetworkAddress()	{
		return _netWorkAddress;
	}

	const NetworkAddress& Peer::getNetworkAddress() const	{
		return _netWorkAddress;
	}

	bool Peer::isConnected() const	{
		if(_raknetPeer == nullptr)	{
			// the peer is not connected
			// better safe then sorry
			return false;
		}

		return (_isConnected == true);
	}

	void Peer::logPeerInfo() const	{
		if(_raknetPeer != nullptr)	{
			DataStructures::List< RakNet::RakNetSocket2* > sockets;
			_raknetPeer->GetSockets(sockets);
			log( logType, "Socket addresses used by RakNet:");
			for (unsigned int i=0; i < sockets.Size(); i++)
			{
				log( logType, "%i. %s", i+1, sockets[i]->GetBoundAddress().ToString(true));
			}

			log( logType, "\nMy IP addresses:");
			for (unsigned int i=0; i < _raknetPeer->GetNumberOfAddresses(); i++)
			{
				log( logType, "%i. %s", i+1, _raknetPeer->GetLocalIP(i));
			}
		}
	}

	RakNet::RakPeerInterface* Peer::accessRakNetPeer() const	{
		return _raknetPeer;
	}

	void Peer::setTryConnect( bool value )	{
		if(value == _tryToConnect)	{
			return;
		}

		if(value == true)	{
			// read configuration before we try to connect

			CCDictionary* globalSetting(NetworkState::sharedInstance()->getSubDictionary(SL_SERIALIZEKEY_PLUGIN_GLOBAL_SETTINGS));


			// pass over the to be used network address
			// defined through the ui
			CCDictionary* peerSetting(getPeerConfiguration());
			if(peerSetting == nullptr)	{
				log(ELogType_Error,"failed to retrieve peer configuration");
				return;
			}

			NetworkAddress& networkAdress(getNetworkAddress());

			CCString* sessionPassword(dynamic_cast<CCString*>(globalSetting->objectForKey(SL_SERIALIZEKEY_PEER_SESSION_PASSWORD)));
			if(sessionPassword != nullptr)	{
				networkAdress.setSessionPassword(sessionPassword->getCString());
			}

			CCString* serverIp(dynamic_cast<CCString*>(peerSetting->objectForKey(SL_SERIALIZEKEY_PEER_SERVERIP)));
			if(serverIp != nullptr)	{
				networkAdress.setServerIp(serverIp->getCString());
			}
			CCInteger* clientPort(dynamic_cast<CCInteger*>(peerSetting->objectForKey(SL_SERIALIZEKEY_PEER_CLIENT_PORT)));
			if(clientPort != nullptr)	{
				networkAdress.setClientPort(clientPort->getValue());
			}
			CCInteger* peerToPeerPort(dynamic_cast<CCInteger*>(peerSetting->objectForKey(SL_SERIALIZEKEY_PEER_P2P_PORT)));
			if(peerToPeerPort != nullptr)	{
				networkAdress.setPeerToPeerPort(peerToPeerPort->getValue());
			}
			CCInteger* serverPort(dynamic_cast<CCInteger*>(peerSetting->objectForKey(SL_SERIALIZEKEY_PEER_SERVER_PORT)));
			if(serverPort != nullptr)	{
				networkAdress.setServerPort(serverPort->getValue());
			}
		}

		_tryToConnect = value;

		if(_tryToConnect == false)	{
			if(isConnected())	{
				shutdown();
			}
		}
	}

	const RakNet::RakNetGUID& Peer::getPeerGUID() const	{
		return _peerGUID;
	}

	void Peer::attachPlugins()	{
		if(_raknetPeer == nullptr)	{
			// the peer is not connected
			// better safe then sorry
			return;
		}

		if(NetworkState::getBool(getPeerConfiguration(),SL_SERIALIZEKEY_PEER_USE_PACKETLOGGER,false))	{
			log(ELogType_Message, "Attaching packetlogger plugin");
			_raknetPeer->AttachPlugin(&_packetLogger);
			_isPluginAttachedPacketLogger = true;
		}

		if(NetworkState::getBool(getPeerConfiguration(),SL_SERIALIZEKEY_PEER_USE_FULLYCONNECTEDMESH,false))	{
			log(ELogType_Message, "Attaching fully connected mesh plugin");
			_raknetPeer->AttachPlugin(&_fullyConnectedMeshPlugin);
			_fullyConnectedMeshPlugin.SetConnectOnNewRemoteConnection(true, _netWorkAddress.getSessionPassword());
			log(ELogType_Message, "Attaching connection graph plugin");
			_raknetPeer->AttachPlugin(&_connectionGraphPlugin);
			_isPluginAttachedFullyConnectedMesh = true;
			_isPluginAttachedConnectionGraph2 = true;
		}

		if(NetworkState::getBool(getPeerConfiguration(),SL_SERIALIZEKEY_PEER_USE_RPC4,false))	{
			log(ELogType_Message, "Attaching connection RPC4 plugin");
			_raknetPeer->AttachPlugin(&_rpc4Plugin);
			_isPluginAttachedRPC4 = true;
		}

		if(NetworkState::getBool(getPeerConfiguration(),SL_SERIALIZEKEY_PEER_USE_REPLICAMANAGER,false))	{
			log(ELogType_Message, "Attaching replica manager plugin");
			if(_replicaManagerPluginCreator != nullptr)	{
				_replicaManager = _replicaManagerPluginCreator->createPlugin(this, _topolgy);
			}
			else	{
				_replicaManager = new ReplicaManager(_topolgy);
			}
			if(_replicaManager != nullptr)	{
				_raknetPeer->AttachPlugin(_replicaManager);
				_isPluginAttachedReplicaManager = true;
			}
		}
	}

	void Peer::detachPlugins(RakNet::RakPeerInterface* peer)	{
		if(peer == nullptr)	{
			// the peer is not connected
			// better safe then sorry
			return;
		}

		if(_isPluginAttachedReplicaManager)		{
			peer->DetachPlugin(_replicaManager);
		}

		if(_replicaManagerPluginCreator != nullptr)	{
			_replicaManagerPluginCreator->destroyPlugin(_replicaManager);
			_replicaManager = nullptr;
		}
		else	{
			// delete the replica manager anyways
			CC_SAFE_DELETE(_replicaManager);
		}

		if(_isPluginAttachedRPC4)		{
			peer->DetachPlugin(&_rpc4Plugin);
		}
		if(_isPluginAttachedConnectionGraph2)		{
			peer->DetachPlugin(&_connectionGraphPlugin);
		}
		if(_isPluginAttachedFullyConnectedMesh)		{
			peer->DetachPlugin(&_fullyConnectedMeshPlugin);
		}
		if(_isPluginAttachedPacketLogger)	{
			peer->DetachPlugin(&_packetLogger);
		}

		// reset the attachment status booleans again
		_isPluginAttachedPacketLogger =
			_isPluginAttachedFullyConnectedMesh =
			_isPluginAttachedConnectionGraph2 =
			_isPluginAttachedRPC4 =
			_isPluginAttachedReplicaManager =
			false;
	}

	ReplicaManager* Peer::getReplicaManager() const	{
		// raknet has RTTI disabled by default
		// not good ...
//		return static_cast<ReplicaManager*>(_replicaManager);
		// now it does ...
		return dynamic_cast<ReplicaManager*>(_replicaManager);
	}


	void Peer::onReceivedPacket( RakNet::Packet* packet)	{
		RakNet::RakPeerInterface* peer(_raknetPeer);

		unsigned char packetIdentifier = getPacketIdentifier(packet);
		DefaultMessageIDTypes eMessageID = static_cast<DefaultMessageIDTypes>(packetIdentifier);
		NetMessageIDTypes eNetMessageID = static_cast<NetMessageIDTypes>(packetIdentifier);
		if((eNetMessageID > NETMSG_ID_START) && (eNetMessageID < NETMSG_ID_END))	{
		}
		else	{
			const char* messageName(RakNet::PacketLogger::BaseIDTOString(packetIdentifier));

			// Record the player ID of the client
			const SystemAddress& clientID = packet->systemAddress;
			// a raknet message
			switch(eMessageID)	{
			case ID_ALREADY_CONNECTED:
				// Connection lost normally
				log( logType, "ID_ALREADY_CONNECTED with guid %" PRINTF_64_BIT_MODIFIER "u", packet->guid);
				break;
			case ID_INCOMPATIBLE_PROTOCOL_VERSION:
				log( logType, "ID_INCOMPATIBLE_PROTOCOL_VERSION");
				break;
			case ID_REMOTE_DISCONNECTION_NOTIFICATION: 
				// Server telling the clients of another client disconnecting gracefully.  You can manually broadcast this in a peer to peer enviroment if you want.
				log( logType, "ID_REMOTE_DISCONNECTION_NOTIFICATION"); 
				break;
			case ID_REMOTE_CONNECTION_LOST: 
				// Server telling the clients of another client disconnecting forcefully.  You can manually broadcast this in a peer to peer enviroment if you want.
				log( logType, "ID_REMOTE_CONNECTION_LOST");
				break;
			case ID_REMOTE_NEW_INCOMING_CONNECTION: 
				// Server telling the clients of another client connecting.  You can manually broadcast this in a peer to peer enviroment if you want.
				log( logType, "ID_REMOTE_NEW_INCOMING_CONNECTION");
				break;

			case ID_NEW_INCOMING_CONNECTION:
				{
					// Somebody connected.  We have their IP now
					log( logType, "ID_NEW_INCOMING_CONNECTION from %s with GUID %s", packet->systemAddress.ToString(true), packet->guid.ToString());

					// TODO: list connection function
					log( logType, "Remote internal IDs:");
					for (int index=0; index < MAXIMUM_NUMBER_OF_INTERNAL_IDS; index++)	{
						RakNet::SystemAddress internalId = peer->GetInternalID(packet->systemAddress, index);
						if (internalId != RakNet::UNASSIGNED_SYSTEM_ADDRESS)	{
							log( logType, "%i. %s", index+1, internalId.ToString(true));
						}
					}

					Notification notification(SL_NOTIFY_NEW_INCOMMING_CONNECTION);
					PeerWrapper* peerWrapper(createPeerWrapper());
					peerWrapper->setSystemAddress(packet->systemAddress);
					peerWrapper->setGUID(packet->guid);
					notification.postNotification(peerWrapper);
				}

				break;
			case ID_CONNECTION_LOST:
				{
					// Couldn't deliver a reliable packet
					// i.e. the other system was abnormally terminated
					log( logType, "%s from %s with GUID %s", messageName, packet->systemAddress.ToString(true), packet->guid.ToString());

					Notification notification(SL_NOTIFY_CONNECTION_LOST);
					PeerWrapper* peerWrapper(createPeerWrapper());
					peerWrapper->setSystemAddress(packet->systemAddress);
					peerWrapper->setGUID(packet->guid);
					notification.postNotification(peerWrapper);
				}
				break;
			case ID_DISCONNECTION_NOTIFICATION:
				{
					// Connection lost normally
					log( logType, "%s from %s with GUID %s", messageName, packet->systemAddress.ToString(true), packet->guid.ToString());

					Notification notification(SL_NOTIFY_DISCONNECTION);
					PeerWrapper* peerWrapper(createPeerWrapper());
					peerWrapper->setSystemAddress(packet->systemAddress);
					peerWrapper->setGUID(packet->guid);
					notification.postNotification(peerWrapper);
				}
				break;
			case ID_CONNECTED_PING:
			case ID_UNCONNECTED_PING:
				log( logType, "%s from %s with GUID %s", messageName, packet->systemAddress.ToString(true), packet->guid.ToString());
				break;
			case ID_CONNECTED_PONG:
			case ID_UNCONNECTED_PONG:
				{
					log( logType, "%s from %s with GUID %s", messageName, packet->systemAddress.ToString(true), packet->guid.ToString());
					receivedPongPacket( packet );
				}
				break;
			case ID_CONNECTION_REQUEST_ACCEPTED:
				// This tells the client or the peer to peer connection they have connected
				log( logType, "%s from %s with GUID %s", messageName, packet->systemAddress.ToString(true), packet->guid.ToString());
				log( logType, "%s my external address is %s", messageName, _raknetPeer->GetExternalID(packet->systemAddress).ToString(true));
				break;
			case ID_ADVERTISE_SYSTEM:
				{
					// peer to peer connectivity
					// The first conditional is needed because ID_ADVERTISE_SYSTEM may be from a system we are connected to, 
					// but replying on a different address.
					// The second conditional is because AdvertiseSystem also sends to the loopback
					if (peer->GetSystemAddressFromGuid(packet->guid) == RakNet::UNASSIGNED_SYSTEM_ADDRESS &&
						peer->GetMyGUID() != packet->guid)	{

							RakNet::PublicKey* publicKey(nullptr);

#if LIBCAT_SECURITY==1
							RakNet::PublicKey pk;
							publicKey = &pk
								char public_key[cat::EasyHandshake::PUBLIC_KEY_BYTES];
							FILE *fp = fopen("publicKey.dat","rb");
							fread(public_key,sizeof(public_key),1,fp);
							fclose(fp);
							pk.remoteServerPublicKey = public_key;
							pk.publicKeyMode = RakNet::PKM_USE_KNOWN_PUBLIC_KEY;
#endif

						log( logType, "%s Connecting to %s", messageName, packet->systemAddress.ToString(true));
						RakNet::ConnectionAttemptResult car = peer->Connect(
							packet->systemAddress.ToString(false), 
							packet->systemAddress.GetPort(),
							_netWorkAddress.getSessionPassword(), 
							(int) strlen(_netWorkAddress.getSessionPassword()),
							publicKey);

						if(car == RakNet::CONNECTION_ATTEMPT_STARTED)	{
//							_peerGUID = _raknetPeer->GetGuidFromSystemAddress(RakNet::UNASSIGNED_SYSTEM_ADDRESS);
							log( logType, "Peer to peer connection established to %s.", packet->systemAddress.ToString(true));
						}
						else	{
							log( ELogType_Error, "failed to establish peer to peer connection.");
						}
					}
				}
				break;
			// replica related messages
			case ID_SND_RECEIPT_LOSS:
			case ID_SND_RECEIPT_ACKED:
				{
					if(_isPluginAttachedReplicaManager)	{
						log( logType, "%s from %s", messageName, packet->systemAddress.ToString(true));
						uint32_t msgNumber;
						memcpy(&msgNumber, packet->data+1, 4);

						RakNet::ReplicaManager3* replicaManager(dynamic_cast<RakNet::ReplicaManager3*>(_replicaManager));
						if(replicaManager != nullptr)	{
							DataStructures::List<Replica3*> replicaListOut;
							replicaManager->GetReplicasCreatedByMe(replicaListOut);
							for (unsigned int idx=0; idx < replicaListOut.Size(); idx++)
							{
								AbstractReplica* replica(dynamic_cast<AbstractReplica*>(replicaListOut[idx]));
								if(replica != nullptr)	{
									replica->notifyReplicaOfMessageDeliveryStatus(packet->guid,msgNumber, packet->data[0]==ID_SND_RECEIPT_ACKED);
								}
							}
						}
					}
				}
				break;
			case ID_REPLICA_MANAGER_CONSTRUCTION:	
			case ID_REPLICA_MANAGER_SCOPE_CHANGE:	
			case ID_REPLICA_MANAGER_SERIALIZE:	
			case ID_REPLICA_MANAGER_DOWNLOAD_STARTED:	
			case ID_REPLICA_MANAGER_DOWNLOAD_COMPLETE:	
				{
#if 0
					// hmm interesting ?
					log( logType, "%s from %s", messageName, packet->systemAddress.ToString(true));
#endif
				}
				break;

			default:
				log( logType, "%s from %s", messageName, packet->systemAddress.ToString(true));
				break;
			}
		}
	}

	//-----------------------------------------------------------------------------
	void Peer::receivedPongPacket( RakNet::Packet* packet )	{
#if 0
		if( true == WaitForPong() )
		{
			if( pPacket->systemAddress != m_pNetInterface->GetInternalID() )
			{
				log(logType, "Got pong from %s(%s) with time %i\n",
					pPacket->systemAddress.ToString(),
					pPacket->guid.ToString(),
					m_kPongEndTime - RakNet::GetTime() );

				NetworkAddress kAddress;
				kAddress.addressString = pPacket->systemAddress.ToString(false);
				kAddress.port = pPacket->systemAddress.port;

				ConnectToAddress( kAddress );
				m_iWaitForPongPort *= -1;
				m_bWaitForConnection = true;
			}
		}
#endif
	}

	// Copied from Multiplayer.cpp
	// If the first byte is ID_TIMESTAMP, then we want the 5th byte
	// Otherwise we want the 1st byte
	unsigned char Peer::getPacketIdentifier(RakNet::Packet *p)	{
		if (p==0)
			return 255;

		if ((unsigned char)p->data[0] == ID_TIMESTAMP)
		{
			RakAssert(p->length > sizeof(RakNet::MessageID) + sizeof(RakNet::Time));
			return (unsigned char) p->data[sizeof(RakNet::MessageID) + sizeof(RakNet::Time)];
		}
		else
			return (unsigned char) p->data[0];
	}

	void Peer::setConfigurationRootKey( const char* value )	{
		_configurationRootKey.assign(value);
	}

	const char* Peer::getConfigurationRootKey() const	{
		return _configurationRootKey.c_str();
	}

	CCDictionary* Peer::getPeerConfiguration() const	{
		NetworkState* networkState(NetworkState::sharedInstance());
		if(networkState == nullptr)	{
			SL_PROCESS_APP()->log(ELogType_Error,"failed to retrieve networkstate");
			return nullptr;
		}
		CCDictionary* rootDictionary(networkState->getRootDictionary());
		const char* peerDataKey(getConfigurationRootKey());
		CCDictionary* peerSetting(dynamic_cast<CCDictionary*>(rootDictionary->objectForKey(peerDataKey)));
		if(peerSetting == nullptr)	{
			peerSetting = CCDictionary::create();
			rootDictionary->setObject(peerSetting,peerDataKey);
		}
		return peerSetting;
	}

	void Peer::setPeerIdx(SLSize idx)	{
		_peerIdx = idx;
	}

	void Peer::advertiseSystem( float dt )
	{
		// peer to peer advertisement
		const unsigned short maximumIncomingConnections(_raknetPeer->GetMaximumIncomingConnections());
		if(getNumberOfConnections() < maximumIncomingConnections)	{
			// note: we do not need to advertise this system to systems we are already connected with
			//       but as we are spread over a couple of machines this would be a very complicated
			//       task
			//       so for our example we just accept 3 other connections
			//       TODO : option ...

			if(getNumberOfConnections() < 3)	{
				bool nextAdvertisement(false);
				_peerToPeerAdvertisementTicker.update(dt, nextAdvertisement);
				if(nextAdvertisement)	{

					if(NetworkState::getBool(getPeerConfiguration(),SL_SERIALIZEKEY_PEER_ADVERTISE_SYSTEM,false))	{
						log( logType, "advertising this system");
						// advertise to other systems
						// by default we start at peer to peer port 55000
						unsigned short peerToPeerPort(55000);

						for (unsigned short i(0); i < maximumIncomingConnections; ++i)	{
							if (_raknetPeer->GetInternalID(RakNet::UNASSIGNED_SYSTEM_ADDRESS,0).GetPort() != peerToPeerPort + i)	{
								_raknetPeer->AdvertiseSystem(SL_ADVERTISE_NETWORK_MASK, peerToPeerPort + i, 0, 0, 0);
								log( logType, "advertising this system");
							}
						}
					}
				}
			}
			else	{
				// we have what we wanted
			}
		}
	}

	CCString* Peer::getPacketInfoString(RakNet::Packet *packet, bool addGUID)	{
		const char* messageName(nullptr);
		unsigned char packetIdentifier = getPacketIdentifier(packet);
		DefaultMessageIDTypes eMessageID = static_cast<DefaultMessageIDTypes>(packetIdentifier);
		NetMessageIDTypes eNetMessageID = static_cast<NetMessageIDTypes>(packetIdentifier);
		if((eNetMessageID > NETMSG_ID_START) && (eNetMessageID < NETMSG_ID_END))	{
			messageName = "USERDEFINED_MESSAGE";
		}
		else	{
			messageName = RakNet::PacketLogger::BaseIDTOString(packetIdentifier);
		}
		if(addGUID)	{
			return CCString::createWithFormat( "%s from %s with GUID %s - %05d bytes - locally generated(%s)", 
				messageName, 
				packet->systemAddress.ToString(true), 
				packet->guid.ToString(),
				packet->length,
				packet->wasGeneratedLocally ? "TRUE" : "FALSE");
		}
		else	{
			return CCString::createWithFormat( "%s from %s - %05d bytes - locally generated(%s)", 
				messageName, 
				packet->systemAddress.ToString(true), 
				packet->length,
				packet->wasGeneratedLocally ? "TRUE" : "FALSE");
		}
	}


	//-------------------------------------------------------------------------
	// PeerWrapper
	//-------------------------------------------------------------------------

	PeerWrapper::PeerWrapper()
		:_peer(nullptr)
	{

	}

	PeerWrapper::~PeerWrapper()	{

	}

	bool PeerWrapper::init()	{
		return true;
	}

}

