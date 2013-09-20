//!---------------------------------------------------------------------------
//! \file "slPeerNode.h"
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
#ifndef __SLPEERNODE_H__
#define __SLPEERNODE_H__

#include "slNetworkNodes.h"
#include <stateless/network/slPeer.h>

namespace sl	{
	class CCControlBase;
}

namespace sl	{
	class PeerNode;

	class slNetwork_API AbstractPeerUI : public CCComponent	{
		SL_DECLARE_BASE(CCComponent)
	public:
		virtual void configure() SL_ABSTRACT;

		virtual void onUIAction(CCObject *sender, CCControlEvent controlEvent) SL_ABSTRACT;

		PeerNode* getPeerNode() const;

		virtual void onEnter()	{
			SLBaseClass::onEnter();
			configure();
		}

	};

	class slNetwork_API PeerNode : public AbstractNetPacketDispatcher
	{
		SL_DECLARE_BASE(AbstractNetPacketDispatcher)
	public:
		void onPeerStateChanged(CCObject* sender);

		virtual void onUIAction(CCObject *sender, CCControlEvent controlEvent) SL_VIRTUAL_DECLARE;
		virtual void onConnectAction(CCObject *sender, CCControlEvent controlEvent) SL_VIRTUAL_DECLARE;

		virtual void updateNetwork(float dt) SL_VIRTUAL_DECLARE;

		// no create function as this object is still abstract
		// CREATE_FUNC(PeerNode)

		// interface CCNode

		virtual bool init() SL_OVERRIDE;

		virtual void update(float dt) SL_OVERRIDE;

		virtual void onEnter() SL_OVERRIDE;
		virtual void onExit() SL_OVERRIDE;

		// interface IPacketDispatcher
		virtual void dispatchPacket(unsigned char packetIdentifier, NativePacket* nativePacket) SL_OVERRIDE;

		// own interface
		virtual SLSize peerIdx() const SL_ABSTRACT;

		// additional implementation
		void createPeerUI(CCLayer* parentLayer);

		void addNetPacketDispatcher(CCObject* packetDispatcher);
		void addNetPacketSender(CCObject* netPacketSender);
		void addUI(CCObject* uiInterface);

		CCControlBase* createPeerInfoControl();

		CC_SYNTHESIZE_RETAIN(CCControlButton*, _btnConnect, BtnConnect)
		CC_SYNTHESIZE(CCControlBase*, _ctrlServerPort, CtrlServerPort)
		CC_SYNTHESIZE(CCControlBase*, _ctrlClientPort, CtrlClientPort)
		CC_SYNTHESIZE(CCControlBase*, _ctrlPeerToPeerPort, CtrlPeerToPeerPort)
		CC_SYNTHESIZE(CCControlBase*, _ctrlConnections, CtrlConnections)
		CC_SYNTHESIZE(CCControlBase*, _ctrlLocalIP, CtrlLocalIP)


		static void shutDownAllPeers();
		static SLSize getPeerNodesCount();
		static const AbstractNodeArray& getAllPeerNodes();
	protected:
		PeerNode();
		virtual ~PeerNode();

		virtual InstanceCount& getInstanceCount() const SL_ABSTRACT;


		Peer _peerInstance;
		unsigned short _numberOfConnections;


		CCArray* _netPacketDispatchers;
		CCArray* _netPacketSenders;
		CCArray* _uiInterfaces;

	private:

		static AbstractNodeArray _allPeerNodes;

	};

}


#endif
