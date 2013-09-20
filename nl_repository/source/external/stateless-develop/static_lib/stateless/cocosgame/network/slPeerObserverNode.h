//!---------------------------------------------------------------------------
//! \file "slPeerObserverNode.h"
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
#ifndef __SLPEEROBSERVERNODE_H__
#define __SLPEEROBSERVERNODE_H__


namespace sl	{



	//! @brief PeerObserverNode a node receiving significant network events
	//!
	//! you will derive from PeerObserverNode to control network gamelogic
	//! based on the connection status see TankVsTankClientLogicNode for example
	class slCocosGame_API PeerObserverNode : public AbstractPeerNode	{
		SL_DECLARE_BASE(AbstractPeerNode)
	public:
		CREATE_FUNC(PeerObserverNode)

		SL_IMPLEMENT_CLASSNAME(PeerObserverNode)

		virtual bool init() SL_OVERRIDE;

		virtual void update(float dt) SL_OVERRIDE;

		virtual void onEnter() SL_OVERRIDE;
		virtual void onExit() SL_OVERRIDE;

		// peer notifications
		virtual void onNewIncommingConnectionNotification(CCObject* peerWrapperObject) SL_VIRTUAL_DECLARE;
		virtual void onConnectionLostNotification(CCObject* peerWrapperObject) SL_VIRTUAL_DECLARE;
		virtual void onConnectionDisconnectedNotification(CCObject* peerWrapperObject) SL_VIRTUAL_DECLARE;

		virtual void onPeerIsConnectedNotification(CCObject* peerWrapperObject) SL_VIRTUAL_DECLARE;
		virtual void onPeerConnectionAttemptFailedNotification(CCObject* peerWrapperObject) SL_VIRTUAL_DECLARE;
		virtual void onPeerFailedToConnectNotification(CCObject* peerWrapperObject) SL_VIRTUAL_DECLARE;
		virtual void onPeerWillDisconnectNotification(CCObject* peerWrapperObject) SL_VIRTUAL_DECLARE;

		virtual void onNewIncommingConnection(PeerWrapper* peerWrapper) SL_VIRTUAL_DECLARE;
		virtual void onConnectionLost(PeerWrapper* peerWrapperObject) SL_VIRTUAL_DECLARE;
		virtual void onConnectionDisconnected(PeerWrapper* peerWrapperObject) SL_VIRTUAL_DECLARE;

		virtual void onPeerIsConnected(PeerWrapper* peerWrapper) SL_VIRTUAL_DECLARE;
		virtual void onPeerConnectionAttemptFailed(PeerWrapper* peerWrapper) SL_VIRTUAL_DECLARE;
		virtual void onPeerFailedToConnect(PeerWrapper* peerWrapper) SL_VIRTUAL_DECLARE;
		virtual void onPeerWillDisconnect(PeerWrapper* peerWrapper) SL_VIRTUAL_DECLARE;

		SL_SYNTHESIZE_GET(SLTimeInterval, _accumTime, AccumTime);
	protected:
		PeerObserverNode();
		virtual ~PeerObserverNode();

	private:
		Notification _notificationConnectionLost;
		Notification _notificationConnectionDisconnected;

		Notification _notificationNewIncommingConnection;
		Notification _notificationPeerIsConnected;
		Notification _notificationPeerConnectionAttemptFailed;
		Notification _notificationPeerFailedToConnect;
		Notification _notificationPeerWillDisconnect;
	};



}


#endif // __SLPEEROBSERVERNODE_H__
