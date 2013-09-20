//!---------------------------------------------------------------------------
//! \file "slPeerToPeerNode.h"
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
#ifndef __SLPEERTOPEERNODE_H__
#define __SLPEERTOPEERNODE_H__

#include "slPeerNode.h"

namespace sl	{

	class slNetwork_API PeerToPeerNode : public PeerNode
	{
		SL_DECLARE_BASE(PeerNode)
	public:
		virtual void updateNetwork(float dt);

		// CCNode declarations
		CREATE_FUNC(PeerToPeerNode)

		virtual bool init();

		virtual void update(float dt);

		virtual SLSize peerIdx() const;

		SLSize peerToPeerIdx() const;

	protected:
		PeerToPeerNode();
		virtual ~PeerToPeerNode();

		virtual InstanceCount& getInstanceCount() const;

	private:
		SLSize _peerToPeerIdx;

		static InstanceCount s_instanceCount;
	};

	SL_INLINE
	SLSize PeerToPeerNode::peerToPeerIdx() const	{
		return _peerToPeerIdx;
	}

	SL_INLINE
	InstanceCount& PeerToPeerNode::getInstanceCount() const	{
		return s_instanceCount;
	}

}


#endif
