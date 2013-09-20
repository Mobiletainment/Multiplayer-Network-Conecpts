//!---------------------------------------------------------------------------
//! \file "nlChatPeerOptionsUI.cpp"
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
#include "nlChatPeerOptionsUI.h"

#include "nlChatMacros.h"

namespace nl	{


	ChatPeerOptionsUI::~ChatPeerOptionsUI()
	{

	}

	void ChatPeerOptionsUI::configure()
	{
		Peer* peer(getPeerNode()->getPeer());
		CCDictionary* peerSetting(peer->getPeerConfiguration());
		NetworkState* networkState(NetworkState::sharedInstance());
		networkState->setObjectIfNotExist(peerSetting, CCBool::create(true),NL_SERIALIZEKEY_PEER_SENDAUTOMESSAGES);

		if(peer->isServer())	{
			networkState->setObjectIfNotExist(peerSetting, CCBool::create(false),NL_SERIALIZEKEY_SERVER_SEND_JSON_AUTOMESSAGES);
		}
	}

	void ChatPeerOptionsUI::extendDialog( Dialog* dialog )
	{
		CCControlColumn* ctrlColumn(dialog->getCtrlColumn());
		if(ctrlColumn != nullptr)	{
			CCSize preferredSize(CCSizeMake(0,0));

			CCControlColumn* ctrlChatPeerOptionsContainer(ControlUtils::createControlColumn(nullptr,preferredSize));

			CCControlCheckBox* ctrlCheckBox;
			ctrlCheckBox = nl::ControlUtils::createCheckBox("Send auto messages", preferredSize);
			ctrlCheckBox->setValueKey(NL_SERIALIZEKEY_PEER_SENDAUTOMESSAGES);
			ctrlChatPeerOptionsContainer->addNode(ctrlCheckBox);

			Peer* peer(getPeerNode()->getPeer());
			if(peer->isServer())	{
				ctrlChatPeerOptionsContainer->setPreferredSize(preferredSize);
				ctrlCheckBox = nl::ControlUtils::createCheckBox("Send JSON auto messages", preferredSize);
				ctrlCheckBox->setValueKey(NL_SERIALIZEKEY_SERVER_SEND_JSON_AUTOMESSAGES);
				ctrlChatPeerOptionsContainer->addNode(ctrlCheckBox);
			}

			ctrlColumn->setPreferredSize(preferredSize);
			ctrlColumn->addNodeAt(ctrlChatPeerOptionsContainer, 1);

			ctrlChatPeerOptionsContainer->redoLayout();
		}
	}

	void ChatPeerOptionsUI::onUIAction( CCObject *sender, CCControlEvent controlEvent )
	{
		PeerNode* peerNode(this->getPeerNode());

		// determine the sender tag to dispatch the action
		const int tag(Dialog::getTag(sender));
		if(tag == SL_CTRLID_PEER_CONNECT)	{
		}
		else if(tag == SL_CTRLID_PEER_OPTIONS)	{
			PeerOptionsDialog* dialog(PeerOptionsDialog::createForPeerNode(peerNode, this));
			if(dialog != nullptr)	{
				dialog->show();
			}
		}
	}

}


