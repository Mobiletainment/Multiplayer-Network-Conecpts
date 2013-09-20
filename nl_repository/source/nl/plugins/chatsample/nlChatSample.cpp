//!---------------------------------------------------------------------------
//! \file "nlChatSample.cpp"
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
#include "nlChatSample.h"
#include "nlChatMacros.h"

namespace nl	{

	//-------------------------------------------------------------------------
	// ChatNetPackageDispatcher
	//-------------------------------------------------------------------------

	ChatNetPackageDispatcher::ChatNetPackageDispatcher()
		:_ctrlChatText(nullptr)
	{

	}

	ChatNetPackageDispatcher::~ChatNetPackageDispatcher() {
		setCtrChatText(nullptr);
	}

	void ChatNetPackageDispatcher::createUI( PeerNode* peerNode, CCControlBase* parentLayer )	{

		CCSize preferredSize(CCSizeMake(0,0));
		_ctrlChatText = ControlUtils::createValueBox(nullptr, preferredSize);
		_ctrlChatText->setText("\n");
		ControlUtils::addCtrlNode(_ctrlChatText, parentLayer);
	}

	void ChatNetPackageDispatcher::addChatMessage( const ChatMessage& newMessage )
	{
		_chatMessages.push_back(newMessage);
		size_t count(_chatMessages.size());

		std::ostringstream allMessages;

		for(size_t i(0); i < count; ++i)	{
			const ChatMessage& message(_chatMessages.elementAt(i));
			allMessages << message._sender.c_str() << " : " << message._content.c_str();
			if(i < count - 1)	{
				allMessages << std::endl;
			}
		}
		_ctrlChatText->setText(allMessages.str().c_str());

		CCControlBase* parentCtrl(dynamic_cast<CCControlBase*>(_ctrlChatText->getParent()));
		if(parentCtrl)	{
			parentCtrl->layoutChildren(ELayoutMode_topLeftDown);
		}
	}

	//-------------------------------------------------------------------------
	// ClientChatNetPackageDispatcher
	//-------------------------------------------------------------------------


	// TODO: @student : extend the protocol.
	//                  so the receiver can split the package into name and content
	void ClientChatNetPackageDispatcher::dispatchPacket(unsigned char packetIdentifier, NativePacket* nativePacket )
	{
		NetMessageIDTypes eNetMessageID(static_cast<NetMessageIDTypes>(packetIdentifier));
		const bool validMessageId((eNetMessageID > NETMSG_ID_START) && (eNetMessageID < NETMSG_ID_END));
		if(validMessageId == false)	{
			return;
		}

		switch(eNetMessageID)	{
		case NETMSG_ID_CHATLINE:
			{
				const char* message = (const char*)nativePacket->data;
				// skip the packet identifier
				message++;
				if(isEmptyString(message) == true)	{
					getPeer()->log(ELogType_Error, "received an empty chat message");
				}
				else	{
					// TODO: @student : split the packet ...
					ChatMessage chatMessage("---", message);
					addChatMessage(chatMessage);
				}
			}
			break;
		case NETMSG_ID_JSONOBJECT:
			{
				// TODO: @student : this might not be enough ...
				const char* message = (const char*)nativePacket->data;
				// skip the packet identifier
				message++;
				if(isEmptyString(message) == true)	{
					getPeer()->log(ELogType_Error, "received an empty chat message");
				}
				else	{
					SLAString json(message);
					getPeer()->log(ELogType_Info, "received json %s", json.c_str());
					CCDictionary* dictionary = CCJSONConverter::dictionaryFrom(json.c_str());
					getPeer()->log(ELogType_Info, dictionary);
					// TODO: @student : read the relevant dictionary members and pass them to the chat message
					ChatMessage chatMessage("---", json.c_str());
					addChatMessage(chatMessage);
				}
			}
			break;
		default:
			break;
		}
	}

	void ClientChatNetPackageDispatcher::createForPeerNode( PeerNode* peerNode, CCControlBase* parentLayer )	{
		ClientChatNetPackageDispatcher* netPackageDispatcher(ClientChatNetPackageDispatcher::create());
		peerNode->addNetPacketDispatcher(netPackageDispatcher);
		netPackageDispatcher->createUI(peerNode, parentLayer);
	}

	//-------------------------------------------------------------------------
	// ServerChatNetPackageDispatcher
	//-------------------------------------------------------------------------


	ServerChatNetPackageDispatcher::ServerChatNetPackageDispatcher()
	{
		_chatMessages.setCapacity(12);
	}

	// TODO: @student : extend the protocol.
	//                  so the receiver can split the package into name and content
	void ServerChatNetPackageDispatcher::dispatchPacket(unsigned char packetIdentifier, NativePacket* nativePacket )
	{
		NetMessageIDTypes eNetMessageID(static_cast<NetMessageIDTypes>(packetIdentifier));
		const bool validMessageId((eNetMessageID > NETMSG_ID_START) && (eNetMessageID < NETMSG_ID_END));
		if(validMessageId == false)	{
			return;
		}
		switch(eNetMessageID)	{
		case NETMSG_ID_CHATLINE:
			{
				const char* message = (const char*)nativePacket->data;
				// skip the packet identifier
				message++;
				if(isEmptyString(message) == true)	{
					getPeer()->log(ELogType_Error, "received an empty chat message");
				}
				else	{
					// TODO: split the packet ...
					ChatMessage chatMessage("---", message);
					addChatMessage(chatMessage);

					// now broadcast this message to everyone else connected to this peer
					// except for the sender
					getPeer()->accessRakNetPeer()->Send((const char*)nativePacket->data, nativePacket->length, 
						HIGH_PRIORITY, RELIABLE_ORDERED, 0, nativePacket->systemAddress, true);
				}
			}

			break;
		default:
			break;
		}
	}

	void ServerChatNetPackageDispatcher::createForPeerNode( PeerNode* peerNode, CCControlBase* parentLayer )	{
		ServerChatNetPackageDispatcher* netPackageDispatcher(ServerChatNetPackageDispatcher::create());
		peerNode->addNetPacketDispatcher(netPackageDispatcher);
		netPackageDispatcher->createUI(peerNode, parentLayer);
	}

	//-------------------------------------------------------------------------
	// AbstractChatNetPackageSender
	//-------------------------------------------------------------------------

	AbstractChatNetPackageSender::AbstractChatNetPackageSender()
		:_autoMessageTimer(0.0f)
		,_autoMessageInterval(5.0f)
		,_autoMessageIdx(0)
	{

	}

	AbstractChatNetPackageSender::~AbstractChatNetPackageSender()
	{

	}

	void AbstractChatNetPackageSender::serialize( float dt )
	{
		if(getPeer()->isConnected() == false)	{
			return;
		}

		if(getPeer()->getNumberOfConnections() == 0)	{
			return;
		}

		if(NetworkState::getBool(getPeer()->getPeerConfiguration(),NL_SERIALIZEKEY_PEER_SENDAUTOMESSAGES,false))	{
			_autoMessageTimer += dt;
			if(_autoMessageTimer > _autoMessageInterval)	{
				// TODO: @student : read the following comments
				// note: if nothing works start debugging here ...
				// getPeer()->log(ELogType_Message, "sending auto message");
				std::ostringstream autoMessage;
				autoMessage << "Hello - " << _autoMessageIdx;
				sendChatLine(autoMessage.str().c_str());
				_autoMessageTimer = 0.0f;
				++_autoMessageIdx;

				// reset the message content
				// otherwise the next auto message will not be send
				_sendMessage._content = "";
			}
		}
	}

	// TODO: @student : extend the protocol.
	//                  so the receiver can split the package into name and content
	void AbstractChatNetPackageSender::sendChatLine(const SLAString& text)	{
		// safety check ...
		if(text.length() > 2000)	{
			getPeer()->log(ELogType_Error, "ERROR text message is to long");
		}
		else if(text.length() == 0)	{
			return;
		}
		else if(_sendMessage._content.compare(text.c_str()) == 0)	{
			// prevent spam
			return;
		}

		_sendMessage._sender = _name;
		_sendMessage._content = text;

		const SLSize messageSize(2048);
		char message[messageSize];
		memset(message,0,sizeof(char) * messageSize);

		// the first byte carries the message id
		message[0] = (char)(NETMSG_ID_CHATLINE);

		strcat(message, _sendMessage._sender.c_str());
		strcat(message, " - ");
		strcat(message, _sendMessage._content.c_str());

		// Message now holds what we want to broadcast

		// message is the data to send
		// strlen(message) + 1 is to send the null terminator
		sendData(message, (const int) strlen(message) + 1);

		// TODO: @student : split into 2 functions and let the chat work with json.

		// note: this is just a little json how to ...
		if(getPeer()->isServer())	{
			if(NetworkState::getBool(getPeer()->getPeerConfiguration(),NL_SERIALIZEKEY_SERVER_SEND_JSON_AUTOMESSAGES,false))	{
				CCDictionary* messageDictionary(CCDictionary::create());
				CCDictionary* singleMessageDictionary(CCDictionary::create());
				messageDictionary->setObject(singleMessageDictionary, "message");
				singleMessageDictionary->setObject(CCString::create(text.c_str()),"content");
				singleMessageDictionary->setObject(CCString::create(_name.c_str()),"sender");
				CCString* json = CCJSONConverter::strFrom(messageDictionary);

				// zero out the data which have been send previously
				memset(message,0,sizeof(char) * messageSize);
				message[0] = (char)(NETMSG_ID_JSONOBJECT);
				strcat(message, json->getCString());
				sendData(message, (const int) strlen(message)+1);
			}
		}
	}

	//-------------------------------------------------------------------------
	// ClientChatNetPackageSender
	//-------------------------------------------------------------------------

	ClientChatNetPackageSender::ClientChatNetPackageSender()
		:_ctrlName(nullptr)
		,_ctrlPassword(nullptr)
		,_ctrlMessage(nullptr)
	{

		CCNotificationCenter* notificationCenter(CCNotificationCenter::sharedNotificationCenter());
		notificationCenter->addObserver(this, 
			callfuncO_selector(ClientChatNetPackageSender::onCtrlEditEnd), SL_NOTIFY_CTRL_EDIT_END, NULL );
	}

	ClientChatNetPackageSender::~ClientChatNetPackageSender() {
		CCNotificationCenter* notificationCenter(CCNotificationCenter::sharedNotificationCenter());
		notificationCenter->removeObserver(this, SL_NOTIFY_CTRL_EDIT_END);
	}

	void ClientChatNetPackageSender::onCtrlEditEnd(CCObject* sender)	{
		// note: the notification center spreads the function call
		//       to every registered observer
		//       so you have to explicitly check if 
		//       the sender is the object you want to deal with
		CCControlEditBox* editBox(dynamic_cast<CCControlEditBox*>(sender));
		if(editBox != nullptr)	{
			const char* editContent(editBox->getTextField()->getString());
			if(sl::isEmptyString(editContent) == false)	{
				if(editBox == _ctrlName)	{
					getPeer()->log(ELogType_Message, "%s name is:%s", __FUNCTION__, editContent);
					_name = editContent;
				}
				else if(editBox == _ctrlMessage)	{
					getPeer()->log(ELogType_Message, "%s message is:%s", __FUNCTION__, editContent);

					// we can now send this message ...
					SLAString message(editContent);
					this->sendChatLine(message);

					editBox->getTextField()->setString(nullptr);
				}
			}
		}
	}

	void ClientChatNetPackageSender::createClientUI( PeerNode* peerNode, CCControlBase* parentLayer )	{
		CCSize preferredSize(CCSizeMake(0,0));

		_ctrlName = ControlUtils::createEditBox("Name:", preferredSize);
		_ctrlPassword = ControlUtils::createEditBox("Password:", preferredSize);
		_ctrlMessage = nl::ControlUtils::createEditBox("Message:", preferredSize);

		ControlUtils::addCtrlNode(_ctrlName, parentLayer);
		ControlUtils::addCtrlNode(_ctrlPassword, parentLayer);
		ControlUtils::addCtrlNode(_ctrlMessage, parentLayer);
	}

	void ClientChatNetPackageSender::sendData( const char* data, int lenght )
	{
		const NetworkAddress& networkAddress(getPeer()->getNetworkAddress());
		RakNet::SystemAddress target;
		target.FromStringExplicitPort(networkAddress.getServerIp(),networkAddress.getServerPort());

		// HIGH_PRIORITY doesn't actually matter here because we don't use any other priority
		// RELIABLE_ORDERED means make sure the message arrives in the right order
		// We arbitrarily pick 0 for the ordering stream
		// target.FromStringExplicitPort created the address to send the data to
		// false means send the message to the given target address

		RakNet::RakPeerInterface* rakNetPeer(getPeer()->accessRakNetPeer());
		// note: better check if the peer is connected in the first place
		if(rakNetPeer != nullptr)	{
			rakNetPeer->Send(data, lenght, 
				HIGH_PRIORITY, RELIABLE_ORDERED, 0, target, false);
		}
	}


	void ClientChatNetPackageSender::createForPeerNode( PeerNode* peerNode, CCControlBase* parentLayer )	{
		ClientChatNetPackageSender* netPackageSender(ClientChatNetPackageSender::create());
		std::ostringstream senderName;
		senderName << "Client " << peerNode->peerIdx();
		netPackageSender->setName(senderName.str());
		peerNode->addNetPacketSender(netPackageSender);

		netPackageSender->createClientUI(peerNode, parentLayer);
	}

	//-------------------------------------------------------------------------
	// ServerChatNetPackageSender
	//-------------------------------------------------------------------------

	ServerChatNetPackageSender::ServerChatNetPackageSender() {

	}

	ServerChatNetPackageSender::~ServerChatNetPackageSender() {

	}

	void ServerChatNetPackageSender::createServerUI( PeerNode* peerNode, CCControlBase* parentLayer )	{
		CCSize preferredSize(CCSizeMake(0,0));
	}


	void ServerChatNetPackageSender::sendData(const char* data, int lenght )	{
		RakNet::SystemAddress target(RakNet::UNASSIGNED_SYSTEM_ADDRESS);

		// HIGH_PRIORITY doesn't actually matter here because we don't use any other priority
		// RELIABLE_ORDERED means make sure the message arrives in the right order
		// We arbitrarily pick 0 for the ordering stream
		// RakNet::UNASSIGNED_SYSTEM_ADDRESS means don't exclude anyone from the broadcast
		// true means broadcast the message to everyone connected

		RakNet::RakPeerInterface* rakNetPeer(getPeer()->accessRakNetPeer());
		rakNetPeer->Send(data, lenght, 
			HIGH_PRIORITY, RELIABLE_ORDERED, 0, target, true);
	}

	void ServerChatNetPackageSender::createForPeerNode( PeerNode* peerNode, CCControlBase* parentLayer )	{
		ServerChatNetPackageSender* netPackageSender(ServerChatNetPackageSender::create());
		std::ostringstream senderName;
		senderName << "Server " << peerNode->peerIdx();
		netPackageSender->setName(senderName.str());
		peerNode->addNetPacketSender(netPackageSender);

		netPackageSender->createServerUI(peerNode, parentLayer);
	}


	void ClientChatLogic::createForPeerNode( PeerNode* peerNode, CCControlBase* parentLayer )
	{
		ClientChatNetPackageSender::createForPeerNode(peerNode, parentLayer);
		ClientChatNetPackageDispatcher::createForPeerNode(peerNode, parentLayer);
	}


	void ServerChatLogic::createForPeerNode( PeerNode* peerNode, CCControlBase* parentLayer )
	{
		ServerChatNetPackageSender::createForPeerNode(peerNode, parentLayer);
		ServerChatNetPackageDispatcher::createForPeerNode(peerNode, parentLayer);
	}


	void PeerToPeerChatLogic::createForPeerNode( PeerNode* peerNode, CCControlBase* parentLayer )
	{
		// TODO: @student implement this
	}

}


