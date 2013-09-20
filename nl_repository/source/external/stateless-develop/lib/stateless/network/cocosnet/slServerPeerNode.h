//!---------------------------------------------------------------------------
//! \file "slServerPeerNode.h"
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
#ifndef __SLSERVERPEERNODE_H__
#define __SLSERVERPEERNODE_H__

#include "slPeerNode.h"

namespace sl	{

	class slNetwork_API ServerPeerNode : public PeerNode
	{
		SL_DECLARE_BASE(PeerNode)
	public:
		virtual void updateNetwork(float dt);

		// CCNode declarations
		CREATE_FUNC(ServerPeerNode)

		virtual bool init();

		virtual void update(float dt);

		virtual SLSize peerIdx() const;

		SLSize serverIdx() const;

	protected:
		ServerPeerNode();
		virtual ~ServerPeerNode();

		virtual InstanceCount& getInstanceCount() const;

	private:
		SLSize _serverIdx;
		static InstanceCount s_instanceCount;
	};

	SL_INLINE
	SLSize ServerPeerNode::serverIdx() const	{
		return _serverIdx;
	}

	SL_INLINE
	InstanceCount& ServerPeerNode::getInstanceCount() const	{
		return s_instanceCount;
	}

}


#endif
