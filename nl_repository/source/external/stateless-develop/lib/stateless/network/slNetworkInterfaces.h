#pragma once
#ifndef __SLNETWORKINTERFACES_H__
#define __SLNETWORKINTERFACES_H__

//!---------------------------------------------------------------------------
//! \file "slNetworkInterfaces.h"
//! \brief <brief-doc-todo>  
//! \details <details-doc-todo>
//! \author Cyrus Preuss
//!
//! This file is part of the Stateless Games SDK
//! Usage of the Stateless Games SDK is subject to an appropriate license agreement.
//!
//! Copyright (c) 2012-2013 Cyrus Preuss. All rights reserved.
//!----------------------------------------------------------------------------


#include "slNetworkEnums.h"

namespace RakNet	{
	struct Packet;
	class PluginInterface2;
}

// TODO: add packet ID to function signature

namespace sl	{

	typedef RakNet::Packet NativePacket;

	class Peer;
/*
	enum ENetPacketDispatchStatus	{
		ENetPacketDispatchStatus_unknown,
		ENetPacketDispatchStatus_dispatched,
	};
*/
	class IPeerExtension	{
	public:
		virtual void setPeer(Peer* peer) SL_ABSTRACT;
		virtual Peer* getPeer( void ) const SL_ABSTRACT;
	protected:
		virtual ~IPeerExtension()	{

		}
	};

	class INetPacketDispatcher: public IPeerExtension	{
		SL_DECLARE_BASE(IPeerExtension)
	public:
		virtual void dispatchPacket(unsigned char packetIdentifier, NativePacket* nativePacket) SL_ABSTRACT;
	protected:
		virtual ~INetPacketDispatcher()	{

		}
	};

	class INetPacketSender: public IPeerExtension	{
		SL_DECLARE_BASE(IPeerExtension)
	public:
		virtual void serialize(float dt) SL_ABSTRACT;
		virtual void sendData(const char* data, int lenght) SL_ABSTRACT;
// not sure about this one at the moment
//		virtual void sendPacket(unsigned char packetIdentifier, NativePacket* nativePacket) SL_ABSTRACT;
	protected:
		virtual ~INetPacketSender()	{

		}

	};

	class IRakNetPluginCreator	{
	public:
		virtual RakNet::PluginInterface2* createPlugin(Peer* parentPeer, ENetwokTopology topolgy) SL_ABSTRACT;
		virtual void destroyPlugin(RakNet::PluginInterface2* plugin) SL_ABSTRACT;
	protected:
		virtual ~IRakNetPluginCreator()	{

		}
	};

}

#endif  // __SLNETWORKINTERFACES_H__
