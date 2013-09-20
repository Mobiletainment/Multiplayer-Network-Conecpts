//!---------------------------------------------------------------------------
//! \file "slPeerNodeControLayer.cpp"
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
#include "slPeerNodeControLayer.h"

#include "slPeerNode.h"

namespace sl	{

	void PeerNodeControlLayer::addPeerControls( CCLayer* layer, PeerNode* peerNode )
	{
		const Peer& peer(*peerNode->getPeer());

		CCDictionary* peerSetting(peer.getPeerConfiguration());
		if(peerSetting == nullptr)	{
			SL_PROCESS_APP()->log(ELogType_Error,"failed to retrieve peer configuration");
			return;
		}

		CCSize preferredSize(CCSizeMake(layer->getContentSize().width - 20,0));
		CCControlBase* ctrlContainer;
		CCArray* subCtrls;



		subCtrls = CCArray::create();
		subCtrls->addObject(peerNode->createPeerInfoControl());

		{
			CCSize btnContainerSize(CCSizeMake(0,0));
			CCArray* columnCtrls = CCArray::create();

			CCControlButton* ctrlBtn;
			ctrlBtn = ControlUtils::createButton("Options");
			ctrlBtn->setTag(SL_CTRLID_PEER_OPTIONS);
			ctrlBtn->addTargetWithActionForControlEvents(
				peerNode, 
				cccontrol_selector(PeerNode::onUIAction), 
				CCControlEventTouchUpInside);
			columnCtrls->addObject(ctrlBtn);

			if(peer.isClient())	{
				ctrlBtn = ControlUtils::createButton("Connect");
			}
			else	{
				ctrlBtn = ControlUtils::createButton("Start");
			}
			ctrlBtn->setTag(SL_CTRLID_PEER_CONNECT);

			// add two targets for this button
			// one general action
			ctrlBtn->addTargetWithActionForControlEvents(
				peerNode, 
				cccontrol_selector(PeerNode::onUIAction), 
				CCControlEventTouchUpInside);

			// the specific connect action
			ctrlBtn->addTargetWithActionForControlEvents(
				peerNode, 
				cccontrol_selector(PeerNode::onConnectAction), 
				CCControlEventTouchUpInside);
			columnCtrls->addObject(ctrlBtn);

			CCControlColumn* btnContainer = ControlUtils::createControlColumn(columnCtrls,btnContainerSize);
			subCtrls->addObject(btnContainer);
		}


		ctrlContainer = ControlUtils::createControlRow(subCtrls, preferredSize);
		ctrlContainer->getCtrlFlags().removeFlag(ECtrlFlag_UseScissorTest);

		ControlUtils::addCtrlNode(ctrlContainer, layer);


		if(peer.isServer())	{
			ctrlContainer = ControlUtils::createEditBox("Port:", preferredSize);
			ControlUtils::addCtrlNode(ctrlContainer, layer);

			ctrlContainer->setValueKey(SL_SERIALIZEKEY_PEER_SERVER_PORT);
			ctrlContainer->deserialize(peerSetting);

			peerNode->setCtrlServerPort(ctrlContainer);
		}
		else if(peer.isClient())	{
			ctrlContainer = ControlUtils::createEditBox("Svr-IP:", preferredSize);
			ControlUtils::addCtrlNode(ctrlContainer, layer);
			ctrlContainer->setValueKey(SL_SERIALIZEKEY_PEER_SERVERIP);
			ctrlContainer->deserialize(peerSetting);

			ctrlContainer = ControlUtils::createEditBox("Svr-Port:", preferredSize);
			ControlUtils::addCtrlNode(ctrlContainer, layer);
			ctrlContainer->setValueKey(SL_SERIALIZEKEY_PEER_SERVER_PORT);
			ctrlContainer->deserialize(peerSetting);

			peerNode->setCtrlServerPort(ctrlContainer);

			ctrlContainer = ControlUtils::createEditBox("Port:", preferredSize);
			ControlUtils::addCtrlNode(ctrlContainer, layer);
			ctrlContainer->setValueKey(SL_SERIALIZEKEY_PEER_CLIENT_PORT);
			ctrlContainer->deserialize(peerSetting);

			peerNode->setCtrlClientPort(ctrlContainer);
		}
		else if(peer.isPeerToPeer())	{
			ctrlContainer = ControlUtils::createEditBox("Port:", preferredSize);
			ControlUtils::addCtrlNode(ctrlContainer, layer);
			ctrlContainer->setValueKey(SL_SERIALIZEKEY_PEER_P2P_PORT);
			ctrlContainer->deserialize(peerSetting);

			CCControlCheckBox* ctrlCheckBox;

			ctrlCheckBox = ControlUtils::createCheckBox("Advertise System", preferredSize);
			ctrlCheckBox->setValueKey(SL_SERIALIZEKEY_PEER_ADVERTISE_SYSTEM);
			ctrlCheckBox->deserialize(peerSetting);
			ControlUtils::addCtrlNode(ctrlCheckBox, layer);


			peerNode->setCtrlPeerToPeerPort(ctrlContainer);
		}

		// common controls for client and server

		ctrlContainer = ControlUtils::createValueBox("Connections:", preferredSize);

		ControlUtils::addCtrlNode(ctrlContainer, layer);

		ctrlContainer->setValueKey(SL_SERIALIZEKEY_PEER_CONNECTIONS);
		ctrlContainer->deserialize(peerSetting);

		peerNode->setCtrlConnections(ctrlContainer);

		ctrlContainer = ControlUtils::createValueBox("Local-IP:", preferredSize);

		ControlUtils::addCtrlNode(ctrlContainer, layer);

		ctrlContainer->setValueKey(SL_SERIALIZEKEY_PEER_LOCALIP);
		ctrlContainer->deserialize(peerSetting);

		peerNode->setCtrlLocalIP(ctrlContainer);
	}

}
