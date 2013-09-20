//!---------------------------------------------------------------------------
//! \file "slClientPeerNode.h"
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
#ifndef __SLCLIENTPEERNODE_H__
#define __SLCLIENTPEERNODE_H__

#include "slPeerNode.h"

namespace sl	{

	class slNetwork_API ClientPeerNode : public PeerNode
	{
		SL_DECLARE_BASE(PeerNode)
	public:
		virtual void updateNetwork(float dt);

		// CCNode declarations
		CREATE_FUNC(ClientPeerNode)

		virtual bool init();

		virtual void update(float dt);

		virtual SLSize peerIdx() const;

		SLSize clientIdx() const;

	protected:
		ClientPeerNode();
		virtual ~ClientPeerNode();

		virtual InstanceCount& getInstanceCount() const;


	private:
		SLSize _clientIdx;

		static InstanceCount s_instanceCount;
	};

	SL_INLINE
	SLSize ClientPeerNode::clientIdx() const	{
		return _clientIdx;
	}

	SL_INLINE
	InstanceCount& ClientPeerNode::getInstanceCount() const	{
		return s_instanceCount;
	}

}


#endif
