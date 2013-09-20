#pragma once
#ifndef __SLNETWORK_H__
#define __SLNETWORK_H__
//!---------------------------------------------------------------------------
//! \file "slNetwork.h"
//! \brief <brief-doc-todo>  
//! \details <details-doc-todo>
//! \author Cyrus Preuss
//!
//! This file is part of the Stateless Games SDK
//! Usage of the Stateless Games SDK is subject to an appropriate license agreement.
//!
//! Copyright (c) 2012-2013 Cyrus Preuss. All rights reserved.
//!----------------------------------------------------------------------------

#include "slNetworkAPI.h"

#include "slRaknet.h"

// allow namespace sl usage of
// - namespace sltl
// - namespace RakNet
namespace sl	{
	using namespace sltl;
	using namespace RakNet;
}

#include "slNetworkMacros.h"
#include "slNetworkEnums.h"
#include "slNetworkInterfaces.h"

#include "slNetworkAddress.h"
#include "slPeer.h"

#include <stateless/cocos2d/slCocosIncludes.h>

#include "cocosnet/slPeerNode.h"
#include "cocosnet/slClientPeerNode.h"
#include "cocosnet/slPeerToPeerNode.h"
#include "cocosnet/slServerPeerNode.h"
#include "cocosnet/slNetworkState.h"

#include "replica/slReplicationRule.h"
#include "replica/slAbstractReplica.h"
#include "replica/slReplicaManager.h"


// some macros



//-----------------------------------------------------------------------------

#endif  // __SLNETWORK_H__
