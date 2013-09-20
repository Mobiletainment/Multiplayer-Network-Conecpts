//!---------------------------------------------------------------------------
//! \file "slPeerNodeControLayer.h"
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
#ifndef __SLPEERNODECONTROLLAYER_H__
#define __SLPEERNODECONTROLLAYER_H__


namespace sl	{

	class PeerNode;

	class slNetwork_API PeerNodeControlLayer
	{
	public:

		static void addPeerControls( CCLayer* layer, PeerNode* peerNode );
	};

}


#endif
