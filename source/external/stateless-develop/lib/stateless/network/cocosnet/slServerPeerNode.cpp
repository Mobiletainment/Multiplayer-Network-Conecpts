//!---------------------------------------------------------------------------
//! \file "slServerPeerNode.cpp"
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
#include "slServerPeerNode.h"


using namespace RakNet;

namespace sl	{

	InstanceCount ServerPeerNode::s_instanceCount;

	ServerPeerNode::ServerPeerNode()	{
		_serverIdx = getInstanceCount().constructor();
		_peerInstance.setServer();
		_peerInstance.setPeerIdx(_serverIdx);
		_peerInstance.log(ELogType_Message, "ServerPeerNode created");
	}

	ServerPeerNode::~ServerPeerNode()	{
		getInstanceCount().destructor();
		getInstanceCount().resetIfAllInstancesAreDestroyed();
		_peerInstance.log(ELogType_Message, "ServerPeerNode destroyed");
	}

	void ServerPeerNode::updateNetwork(float dt)	{
		SLBaseClass::updateNetwork(dt);
	}

	bool ServerPeerNode::init()	{
		bool initialized(SLBaseClass::init());
		if(initialized)	{
		}		
		return initialized;
	}

	void ServerPeerNode::update( float dt )	{
		SLBaseClass::update(dt);
	}

	SLSize ServerPeerNode::peerIdx() const	{
		return serverIdx();
	}


}

