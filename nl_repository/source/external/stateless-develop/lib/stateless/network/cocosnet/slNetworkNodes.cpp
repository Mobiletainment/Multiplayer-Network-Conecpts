//!---------------------------------------------------------------------------
//! \file "slNetworkNodes.cpp"
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
#include "slNetworkNodes.h"
#include "slNetworkState.h"

using namespace RakNet;


namespace sl	{

	AbstractPeerNode::AbstractPeerNode()
		:_peer(nullptr)
	{

	}

	AbstractPeerNode::~AbstractPeerNode() {
		setPeer(nullptr);
	}


	AbstractNetPacketDispatcher::AbstractNetPacketDispatcher()
		:_peer(nullptr)
	{


	}
	AbstractNetPacketDispatcher::~AbstractNetPacketDispatcher() {
		setPeer(nullptr);
	}

	AbstractNetPacketSender::AbstractNetPacketSender()
		:_peer(nullptr)
	{
	}

	AbstractNetPacketSender::~AbstractNetPacketSender() {
		setPeer(nullptr);
	}


}
