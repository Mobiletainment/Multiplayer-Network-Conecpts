//!---------------------------------------------------------------------------
//! \file "slClientPeerNode.cpp"
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
#include "slClientPeerNode.h"


using namespace RakNet;

namespace sl	{

	InstanceCount ClientPeerNode::s_instanceCount;

	ClientPeerNode::ClientPeerNode()	{
		_clientIdx = getInstanceCount().constructor();
		_peerInstance.setClient();
		_peerInstance.setPeerIdx(_clientIdx);
		_peerInstance.log(ELogType_Message, "ClientPeerNode created");
	}

	ClientPeerNode::~ClientPeerNode()	{
		getInstanceCount().destructor();
		getInstanceCount().resetIfAllInstancesAreDestroyed();
		_peerInstance.log(ELogType_Message, "ClientPeerNode destroyed");
	}

	void ClientPeerNode::updateNetwork(float dt)	{
		SLBaseClass::updateNetwork(dt);
	}

	bool ClientPeerNode::init()	{
		bool initialized(SLBaseClass::init());
		if(initialized)	{
		}	
		return initialized;
	}

	void ClientPeerNode::update( float dt )	{
		SLBaseClass::update(dt);
	}

	SLSize ClientPeerNode::peerIdx() const	{
		return clientIdx();
	}


}

