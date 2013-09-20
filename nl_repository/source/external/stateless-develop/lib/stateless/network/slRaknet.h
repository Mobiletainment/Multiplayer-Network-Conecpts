#pragma once
#ifndef __SLRAKNET_H__
#define __SLRAKNET_H__

//!---------------------------------------------------------------------------
//! \file "slRaknet.h"
//! \brief <brief-doc-todo>  
//! \details <details-doc-todo>
//! \author Cyrus Preuss
//!
//! This file is part of the Stateless Games SDK
//! Usage of the Stateless Games SDK is subject to an appropriate license agreement.
//!
//! Copyright (c) 2012-2013 Cyrus Preuss. All rights reserved.
//!----------------------------------------------------------------------------


#include "RakNetVersion.h"

#include "MessageIdentifiers.h"
#include "RakPeerInterface.h"
#include "RakNetStatistics.h"
#include "RakNetTypes.h"
#include "BitStream.h"

// raknet plugins

#include "PacketLogger.h"
#include "FullyConnectedMesh2.h"
#include "ConnectionGraph2.h"

#include "RPC4Plugin.h"

#include "ReplicaManager3.h"
#include "NetworkIDManager.h"
#include "VariableDeltaSerializer.h"


#if LIBCAT_SECURITY==1
#include "SecureHandshake.h" // Include header for secure handshake
#endif

#include "SocketLayer.h"
#include "RakNetSocket2.h"

#include "RakSleep.h"
#include "RakString.h"

//-----------------------------------------------------------------------------

#endif  // __SLRAKNET_H__
