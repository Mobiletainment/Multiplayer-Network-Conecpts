//!---------------------------------------------------------------------------
//! \file "nlChatSamplePluginContent.cpp"
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
#include "nlChatSamplePluginContent.h"
#include "nlChatSample.h"
#include "nlChatPeerOptionsUI.h"



namespace nl	{

	PluginLayerContent* createPluginLayerContent()	{
		return ChatSamplePluginContent::create();
	}

	ChatSamplePluginContent::ChatSamplePluginContent()
	{
		setNetworkArchitecture(ENetworkArchitecture_CLIENTSERVER);
		setLeftLayerClassName(CCString::create(CCControlColumn::staticClassName()));
		setRightLayerClassName(CCString::create(CCControlColumn::staticClassName()));
	}

	ChatSamplePluginContent::~ChatSamplePluginContent()
	{
	}

	bool ChatSamplePluginContent::init()
	{
		CCDictionary* dataSource(NetworkState::sharedInstance()->getSubDictionary(SL_SERIALIZEKEY_PLUGIN_GLOBAL_SETTINGS));

		// plugin information
		dataSource->setObject(CCString::create(
			"sample of a text-based chat client/server"
			),SL_SERIALIZEKEY_PLUGIN_INFO_BRIEF);

		dataSource->setObject(CCString::create(
			"This sample features UDP messages\n"
			"using the custom net message ids:\n"
			"\n"
			"NETMSG_ID_CHATLINE\n"
			"NETMSG_ID_JSONOBJECT\n"
			"\n"
			"\n"
			"Take a special look at the JSON serialization."
			),SL_SERIALIZEKEY_PLUGIN_INFO_DETAIL);

		// task information
		dataSource->setObject(CCString::create(
			"extend the client/server implementation\n"
			"- extend the protocol implementation to setup the chat message with sender and content\n"
			"- register client names on the server side\n"
			"- validate that the client has given the right password\n"
			"- never ever transmit or store plain text passwords\n"
			"- implement a peer to peer logic\n"
			"- what are the specific problems regarding a peer to peer implementation?"
			),SL_SERIALIZEKEY_PLUGIN_TASKS);

		dataSource->setObject(CCString::create(
			"- search for TODO: @student to find some entry points\n"
			"- read the related RakNet documentation\n"
			"- read and debug the code\n"
			"- check out the JSON serialization\n"
			"- invent new net message ids ?\n"
			"\n"
			"ask us if you do not understand ..."
			),SL_SERIALIZEKEY_PLUGIN_TASK_HINTS);


		return SLBaseClass::init();
	}

	void ChatSamplePluginContent::addRootMenuControls( CCArray* ctrlRowArray )
	{
		ctrlRowArray->addObject(createRootButton("Information", SL_CTRLID_INFO));
		ctrlRowArray->addObject(createRootButton("Client Server", SL_CTRLID_QUADLAYER_RECREATE_CLIENTSERVER));
		ctrlRowArray->addObject(createRootButton("Peer to Peer", SL_CTRLID_QUADLAYER_RECREATE_PEERTOPEER));
		ctrlRowArray->addObject(createRootButton("Tasks", SL_CTRLID_TASKS));
	}


	void ChatSamplePluginContent::addSubLayerContent( 
		SLSize idx, 
		CCControlBase* twinLayer, 
		CCControlBase* leftLayer, 
		CCControlBase* rightLayer )
	{
		switch(getNetworkArchitecture())	{
		case ENetworkArchitecture_PEERTOPEER:	
			{
				PeerNode* peerNode(nullptr);
				peerNode = PeerToPeerNode::create();
				PeerToPeerChatLogic::createForPeerNode(peerNode, rightLayer);

				// the twin layer is the master to add the peer node to
				// so the peerNode will be destroyed in conjunction
				// with it's 'master layer'
				twinLayer->addChild(peerNode);

				peerNode->createPeerUI(leftLayer);
				peerNode->addUI(ChatPeerOptionsUI::create());
			}
			break;
		case ENetworkArchitecture_CLIENTSERVER:	
			{
				PeerNode* peerNode(nullptr);
				if(idx == 0)	{
					peerNode = ServerPeerNode::create();
					ServerChatLogic::createForPeerNode(peerNode, rightLayer);
				}
				else	{
					peerNode = ClientPeerNode::create();
					ClientChatLogic::createForPeerNode(peerNode, rightLayer);
				}

				if(peerNode != nullptr)	{
					// the twin layer is the master to add the peer node to
					// so the peerNode will be destroyed in conjunction
					// with it's 'master layer'
					twinLayer->addChild(peerNode);

					peerNode->createPeerUI(leftLayer);
					peerNode->addUI(ChatPeerOptionsUI::create());
				}
			}
			break;
		}
	}

	void ChatSamplePluginContent::onUIAction( CCObject *sender, CCControlEvent controlEvent )
	{
		CCNode* senderNode(dynamic_cast<CCNode*>(sender));
		if(senderNode == nullptr)	{
			return;
		}
		CCControlQuadLayer* quadLayer(getQuadLayer());

		bool dispatched(true);
		// determine the sender tag to dispatch the action
		const int tag(senderNode->getTag());
		switch(tag)	{
		case SL_CTRLID_QUADLAYER_RECREATE_CLIENTSERVER:	
			{
				if(getNetworkArchitecture() != ENetworkArchitecture_CLIENTSERVER)	{
					setNetworkArchitecture(ENetworkArchitecture_CLIENTSERVER);
					recreateSubLayers(4);
				}
			}
			break;
		case SL_CTRLID_QUADLAYER_RECREATE_PEERTOPEER:	
			{
				if(getNetworkArchitecture() != ENetworkArchitecture_PEERTOPEER)	{
					setNetworkArchitecture(ENetworkArchitecture_PEERTOPEER);
					recreateSubLayers(4);
				}
			}
			break;
		default:	
			{
				dispatched = false;
			}
			break;
		}
		if(dispatched == false)	{
			SLBaseClass::onUIAction(sender, controlEvent);
		}
	}


}


