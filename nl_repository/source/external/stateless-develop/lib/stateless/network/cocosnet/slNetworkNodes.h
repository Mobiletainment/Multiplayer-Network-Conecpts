//!---------------------------------------------------------------------------
//! \file "slNetworkNodes.h"
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
#ifndef __SLNETWORKNODES_H__
#define __SLNETWORKNODES_H__

#include <stateless/cocosutil/slAbstractNode.h>

#include <stateless/network/slNetworkMacros.h>
#include <stateless/network/slNetworkEnums.h>
#include <stateless/network/slNetworkInterfaces.h>

namespace sl	{
	class CCControlBase;
}

namespace sl	{

	class slNetwork_API AbstractPeerNode : public AbstractNode, public IPeerExtension
	{
	public:
		SL_SYNTHESIZE(Peer*, _peer, Peer);
	protected:
		AbstractPeerNode();
		virtual ~AbstractPeerNode();
	};

	class slNetwork_API AbstractNetPacketDispatcher : public AbstractNode, public INetPacketDispatcher
	{
	public:
		SL_SYNTHESIZE(Peer*, _peer, Peer);
	protected:
		AbstractNetPacketDispatcher();
		virtual ~AbstractNetPacketDispatcher();
	};

	class slNetwork_API AbstractNetPacketSender : public AbstractNode, public INetPacketSender
	{
	public:
		SL_SYNTHESIZE(Peer*, _peer, Peer);
	protected:
		AbstractNetPacketSender();
		virtual ~AbstractNetPacketSender();
	};


}


#endif
