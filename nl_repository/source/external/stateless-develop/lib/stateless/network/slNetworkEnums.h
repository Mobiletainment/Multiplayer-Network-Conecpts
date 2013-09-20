//!---------------------------------------------------------------------------
//! \file "slNetworkEnums.h"
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
#ifndef __SLNETWORKENUMS_H__
#define __SLNETWORKENUMS_H__


#include "MessageIdentifiers.h"

namespace sl	{

	enum NetMessageIDTypes
	{
		NETMSG_ID_START = ID_USER_PACKET_ENUM,
		NETMSG_ID_CHATLINE,
		NETMSG_ID_JSONOBJECT,
		NETMSG_ID_BINARY,
		NETMSG_ID_END
	};

	enum ENetwokTopology
	{
		UNKNOWN,
		SINGLEPLAYER,
		CLIENT,
		SERVER,
		P2P
	};

	enum ENetworkArchitecture
	{
		ENetworkArchitecture_UNKNOWN,
		ENetworkArchitecture_SINGLEPLAYER,
		ENetworkArchitecture_CLIENTSERVER,
		ENetworkArchitecture_PEERTOPEER
	};

}


#endif // __SLNETWORKENUMS_H__
