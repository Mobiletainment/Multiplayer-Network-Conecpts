//!---------------------------------------------------------------------------
//! \file "slNetworkMacros.h"
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
#ifndef __SLNETWORKMACROS_H__
#define __SLNETWORKMACROS_H__

#define SL_NOTIFY_PEERSTATE_CHANGED "SL_NOTIFY_PEERSTATE_CHANGED"
#define SL_NOTIFY_PEERCONNECTION_CHANGED "SL_NOTIFY_PEERCONNECTION_CHANGED"

#define SL_NOTIFY_PEERSTATE_IS_CONNECTED "SL_NOTIFY_PEERSTATE_IS_CONNECTED"
#define SL_NOTIFY_PEERSTATE_FAILED_TO_CONNECT "SL_NOTIFY_PEERSTATE_FAILED_TO_CONNECT"
#define SL_NOTIFY_PEERSTATE_WILL_DISCONNECT "SL_NOTIFY_PEERSTATE_WILL_DISCONNECT"
#define SL_NOTIFY_PEERSTATE_CONNECTION_ATTEMPT_FAILED "SL_NOTIFY_PEERSTATE_CONNECTION_ATTEMPT_FAILED"

#define SL_NOTIFY_NEW_INCOMMING_CONNECTION "SL_NOTIFY_NEW_INCOMMING_CONNECTION"

#define SL_NOTIFY_CONNECTION_LOST "SL_NOTIFY_CONNECTION_LOST"
#define SL_NOTIFY_DISCONNECTION "SL_NOTIFY_DISCONNECTION"

// some actor sample related ctrl id's
#define SL_CTRLID_ADD_ACTORNODE 2000

// peer base commands
#define SL_CTRLID_PEER_CONNECT 3000
#define SL_CTRLID_PEER_OPTIONS 3001


// some replica ctrl id's used inside the sample code
#define SL_CTRLID_REPLICA_CREATE 3002
#define SL_CTRLID_REPLICA_READ 3003
#define SL_CTRLID_REPLICA_UPDATE 3004
#define SL_CTRLID_REPLICA_DELETE 3005

#define SL_CTRLID_REPLICA_LIST 3006


#define SL_SERIALIZEKEY_PEER_ADVERTISE_SYSTEM "advertise_system"

#define SL_SERIALIZEKEY_PEER_CLIENT_PORT "clientport"
#define SL_SERIALIZEKEY_PEER_P2P_PORT "peertopeerport"
#define SL_SERIALIZEKEY_PEER_SERVER_PORT "serverport"
#define SL_SERIALIZEKEY_PEER_SERVERIP "serverip"
#define SL_SERIALIZEKEY_PEER_LOCALIP "localip"
#define SL_SERIALIZEKEY_PEER_CONNECTIONS "connections"

#define SL_SERIALIZEKEY_PEER_USE_FULLYCONNECTEDMESH "use_extension_fullyconnected_mesh"
#define SL_SERIALIZEKEY_PEER_USE_PACKETLOGGER "use_extension_packetlogger"
#define SL_SERIALIZEKEY_PEER_USE_REPLICAMANAGER "use_extension_replicamanager"
#define SL_SERIALIZEKEY_PEER_USE_RPC4 "use_extension_rpc4"

#define SL_SERIALIZEKEY_PEER_SESSION_PASSWORD "session_password"

#define SL_SERIALIZEKEY_PEER_NETWORKSIMULATOR_ENABLE "netsimulator_enable"
#define SL_SERIALIZEKEY_PEER_NETWORKSIMULATOR_PACKETLOSS "netsimulator_packet_loss"
#define SL_SERIALIZEKEY_PEER_NETWORKSIMULATOR_MIN_EXTRA_PING "netsimulator_min_extra_ping"
#define SL_SERIALIZEKEY_PEER_NETWORKSIMULATOR_EXTRA_PING_VARIANCE "netsimulator_extra_ping packet_variance"


#define SL_SERIALIZEKEY_PLUGIN_GLOBAL_SETTINGS "plugin_global_settings"
#define SL_SERIALIZEKEY_PLUGIN_INFO_BRIEF "plugin_info_brief"
#define SL_SERIALIZEKEY_PLUGIN_INFO_DETAIL "plugin_info_detail"

#define SL_SERIALIZEKEY_PLUGIN_TASKS "plugin_tasks"
#define SL_SERIALIZEKEY_PLUGIN_TASK_HINTS "plugin_task_hints"




#endif // __SLNETWORKMACROS_H__
