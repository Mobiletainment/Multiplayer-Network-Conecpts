//!---------------------------------------------------------------------------
//! \file "slPeerToPeerNode.cpp"
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
#include "slPeerToPeerNode.h"


using namespace RakNet;

namespace sl	{

	InstanceCount PeerToPeerNode::s_instanceCount;

	PeerToPeerNode::PeerToPeerNode()	{
		_peerToPeerIdx = getInstanceCount().constructor();
		_peerInstance.setPeerToPeer();
		_peerInstance.setPeerIdx(_peerToPeerIdx);
		_peerInstance.log(ELogType_Message, "PeerToPeerNode created");
	}

	PeerToPeerNode::~PeerToPeerNode()	{
		getInstanceCount().destructor();
		getInstanceCount().resetIfAllInstancesAreDestroyed();
		_peerInstance.log(ELogType_Message, "PeerToPeerNode destroyed");
	}

	void PeerToPeerNode::updateNetwork(float dt)	{
		SLBaseClass::updateNetwork(dt);
	}

	bool PeerToPeerNode::init()	{
		bool initialized(SLBaseClass::init());
		if(initialized)	{
		}	
		return initialized;
	}

	void PeerToPeerNode::update( float dt )	{
		SLBaseClass::update(dt);
	}

	SLSize PeerToPeerNode::peerIdx() const	{
		return peerToPeerIdx();
	}


}

