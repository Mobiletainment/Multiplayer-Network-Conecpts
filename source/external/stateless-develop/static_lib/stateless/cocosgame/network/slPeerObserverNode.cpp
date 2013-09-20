//!---------------------------------------------------------------------------
//! \file "slPeerObserverNode.cpp"
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
#include "slPeerObserverNode.h"

namespace sl	{

	PeerObserverNode::PeerObserverNode()
		:_notificationConnectionLost(SL_NOTIFY_CONNECTION_LOST)
		,_notificationConnectionDisconnected(SL_NOTIFY_DISCONNECTION)
		,_notificationNewIncommingConnection(SL_NOTIFY_NEW_INCOMMING_CONNECTION)
		,_notificationPeerIsConnected(SL_NOTIFY_PEERSTATE_IS_CONNECTED)
		,_notificationPeerConnectionAttemptFailed(SL_NOTIFY_PEERSTATE_CONNECTION_ATTEMPT_FAILED)
		,_notificationPeerFailedToConnect(SL_NOTIFY_PEERSTATE_FAILED_TO_CONNECT)
		,_notificationPeerWillDisconnect(SL_NOTIFY_PEERSTATE_WILL_DISCONNECT)
		,_accumTime(0)
	{
	}

	PeerObserverNode::~PeerObserverNode()
	{
	}

	bool PeerObserverNode::init()	{
		bool initialized(SLBaseClass::init());

		// this leads to update being called once per frame
		// before the update of other objects
		// before actors being updated
		scheduleUpdateWithPriority(-1);

		return initialized;
	}

	void PeerObserverNode::update( float dt )	{
		SLBaseClass::update(dt);
		_accumTime += dt;
	}

	void PeerObserverNode::onEnter()	{
		SLBaseClass::onEnter();
		_notificationConnectionLost.addObserver(this, callfuncO_selector(PeerObserverNode::onConnectionLostNotification));
		_notificationConnectionDisconnected.addObserver(this, callfuncO_selector(PeerObserverNode::onConnectionDisconnectedNotification));
		_notificationNewIncommingConnection.addObserver(this, callfuncO_selector(PeerObserverNode::onNewIncommingConnectionNotification));
		_notificationPeerIsConnected.addObserver(this, callfuncO_selector(PeerObserverNode::onPeerIsConnectedNotification));
		_notificationPeerConnectionAttemptFailed.addObserver(this, callfuncO_selector(PeerObserverNode::onPeerConnectionAttemptFailedNotification));
		_notificationPeerFailedToConnect.addObserver(this, callfuncO_selector(PeerObserverNode::onPeerFailedToConnectNotification));
		_notificationPeerWillDisconnect.addObserver(this, callfuncO_selector(PeerObserverNode::onPeerWillDisconnectNotification));
	}

	void PeerObserverNode::onExit()	{
		_notificationConnectionLost.removeObserver();
		_notificationConnectionDisconnected.removeObserver();
		_notificationNewIncommingConnection.removeObserver();
		_notificationPeerIsConnected.removeObserver();
		_notificationPeerConnectionAttemptFailed.removeObserver();
		_notificationPeerFailedToConnect.removeObserver();
		_notificationPeerWillDisconnect.removeObserver();
		setPeer(nullptr);
		SLBaseClass::onExit();
	}

	void PeerObserverNode::onNewIncommingConnectionNotification(CCObject* peerWrapperObject)	{
		PeerWrapper* peerWrapper(dynamic_cast<PeerWrapper*>(peerWrapperObject));
		if(peerWrapper != nullptr)	{
			if(getPeer() == peerWrapper->getPeer())	{
				onNewIncommingConnection(peerWrapper);
			}
		}
	}

	void PeerObserverNode::onConnectionLostNotification(CCObject* peerWrapperObject)	{
		PeerWrapper* peerWrapper(dynamic_cast<PeerWrapper*>(peerWrapperObject));
		if(peerWrapper != nullptr)	{
			if(getPeer() == peerWrapper->getPeer())	{
				onConnectionLost(peerWrapper);
			}
		}
	}

	void PeerObserverNode::onConnectionDisconnectedNotification(CCObject* peerWrapperObject)	{
		PeerWrapper* peerWrapper(dynamic_cast<PeerWrapper*>(peerWrapperObject));
		if(peerWrapper != nullptr)	{
			if(getPeer() == peerWrapper->getPeer())	{
				onConnectionDisconnected(peerWrapper);
			}
		}
	}

	void PeerObserverNode::onPeerIsConnectedNotification(CCObject* peerWrapperObject)	{
		PeerWrapper* peerWrapper(dynamic_cast<PeerWrapper*>(peerWrapperObject));
		if(peerWrapper != nullptr)	{
			if(getPeer() == peerWrapper->getPeer())	{
				onPeerIsConnected(peerWrapper);
			}
		}
	}

	void PeerObserverNode::onPeerConnectionAttemptFailedNotification(CCObject* peerWrapperObject)	{
		PeerWrapper* peerWrapper(dynamic_cast<PeerWrapper*>(peerWrapperObject));
		if(peerWrapper != nullptr)	{
			if(getPeer() == peerWrapper->getPeer())	{
				onPeerConnectionAttemptFailed(peerWrapper);
			}
		}
	}

	void PeerObserverNode::onPeerFailedToConnectNotification(CCObject* peerWrapperObject)	{
		PeerWrapper* peerWrapper(dynamic_cast<PeerWrapper*>(peerWrapperObject));
		if(peerWrapper != nullptr)	{
			if(getPeer() == peerWrapper->getPeer())	{
				onPeerFailedToConnect(peerWrapper);
			}
		}
	}

	void PeerObserverNode::onPeerWillDisconnectNotification(CCObject* peerWrapperObject)	{
		PeerWrapper* peerWrapper(dynamic_cast<PeerWrapper*>(peerWrapperObject));
		if(peerWrapper != nullptr)	{
			if(getPeer() == peerWrapper->getPeer())	{
				onPeerWillDisconnect(peerWrapper);
			}
		}
	}

	void PeerObserverNode::onNewIncommingConnection(PeerWrapper* peerWrapper)	{
		getPeer()->log( ELogType_Info, "%s - received new incomming connection from %s with GUID %s", 
			getClassName(),
			peerWrapper->getSystemAddress().ToString(true), peerWrapper->getGUID().ToString());
	}

	void PeerObserverNode::onConnectionLost(PeerWrapper* peerWrapper)	{
		getPeer()->log( ELogType_Info, "%s - received lost connection from %s with GUID %s", 
			getClassName(),
			peerWrapper->getSystemAddress().ToString(true), peerWrapper->getGUID().ToString());
	}

	void PeerObserverNode::onConnectionDisconnected(PeerWrapper* peerWrapper)	{
		getPeer()->log( ELogType_Info, "%s - received dissonnected from %s with GUID %s", 
			getClassName(),
			peerWrapper->getSystemAddress().ToString(true), peerWrapper->getGUID().ToString());
	}

	void PeerObserverNode::onPeerIsConnected(PeerWrapper* peerWrapper)	{
		getPeer()->log(ELogType_Info, "%s - received peer is connected", getClassName());
	}

	void PeerObserverNode::onPeerConnectionAttemptFailed(PeerWrapper* peerWrapper)	{
		getPeer()->log(ELogType_Info, "%s - received peer connection attempt failed", getClassName());
	}

	void PeerObserverNode::onPeerFailedToConnect(PeerWrapper* peerWrapper)	{
		getPeer()->log(ELogType_Info, "%s - received peer failed to connected", getClassName());
	}

	void PeerObserverNode::onPeerWillDisconnect(PeerWrapper* peerWrapper)	{
		getPeer()->log(ELogType_Info, "%s - received peer will disconnect", getClassName());
	}

}


