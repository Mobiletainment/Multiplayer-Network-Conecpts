//!---------------------------------------------------------------------------
//! \file "nlChatSample.h"
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
#ifndef __NLCHATSAMPLE_H__
#define __NLCHATSAMPLE_H__


namespace nl	{


	//! @brief ChatMessage represents one single chat message
	//!
	struct ChatMessage
	{
	public:
		ChatMessage()	{

		}

		ChatMessage(const SLAString& sender, const SLAString& content):
		  _sender(sender),_content(content)
		  {
		  }
		  SLAString _sender;
		  SLAString _content;
		  //<type> timeStamp;
	};

	typedef CircularVector<ChatMessage, 7> ChatMessages;

	//! @brief ChatNetPackageDispatcher is the object responsible
	//!        connect received chat messages with the UI
	//!
	class ChatNetPackageDispatcher : public AbstractNetPacketDispatcher	{
	public:
		void createUI( PeerNode* peerNode, CCControlBase* parentLayer );

		CC_SYNTHESIZE(CCControlValueBox*, _ctrlChatText, CtrChatText)

	protected:
		ChatNetPackageDispatcher();
		virtual ~ChatNetPackageDispatcher();

		void addChatMessage(const ChatMessage& message);

		ChatMessages _chatMessages;
	};

	//! @brief ClientChatNetPackageDispatcher is the object responsible
	//!        to read and dispatch incoming messages on the client side.
	//!
	//! ClientChatNetPackageDispatcher needs to implement a meaningful version
	//! of dispatchPacket
	//!
	class ClientChatNetPackageDispatcher : public ChatNetPackageDispatcher	{
	public:
		static void createForPeerNode( PeerNode* peerNode, CCControlBase* parentLayer );

		// interface INetPacketDispatcher
		virtual void dispatchPacket(unsigned char packetIdentifier, NativePacket* nativePacket);

	protected:
		ClientChatNetPackageDispatcher() {

		};
		virtual ~ClientChatNetPackageDispatcher() {

		};
	private:
		CREATE_FUNC(ClientChatNetPackageDispatcher)
	};


	//! @brief ServerChatNetPackageDispatcher is the object responsible
	//!        to read and dispatch incoming messages on the server side.
	//!
	//! ServerChatNetPackageDispatcher needs to implement a meaningful version
	//! of dispatchPacket
	//!
	class ServerChatNetPackageDispatcher : public ChatNetPackageDispatcher	{
	public:

		static void createForPeerNode( PeerNode* peerNode, CCControlBase* parentLayer );

		// interface INetPacketDispatcher
		virtual void dispatchPacket(unsigned char packetIdentifier, NativePacket* nativePacket);

	protected:
		ServerChatNetPackageDispatcher();
		virtual ~ServerChatNetPackageDispatcher() {

		};
	private:
		CREATE_FUNC(ServerChatNetPackageDispatcher)
	};




	//! @brief AbstractChatNetPackageSender is an abstract implementation
	//!        supporting the sending of chat messages
	//!
	//! client and server derivations need to be implemented
	//!
	class AbstractChatNetPackageSender : public AbstractNetPacketSender	{
	public:

		//! serialize is sending a default message for demo purposes
		virtual void serialize(float dt);

		CC_SYNTHESIZE(SLAString, _name, Name)
		CC_SYNTHESIZE(ChatNetPackageDispatcher*, _chatNetPackageDispatcher, ChatNetPackageDispatcher)
	protected:
		AbstractChatNetPackageSender();
		virtual ~AbstractChatNetPackageSender();

		//! send a chat line
		void sendChatLine(const SLAString& text);

	private:
		ChatMessage _sendMessage;
		float _autoMessageTimer;
		float _autoMessageInterval;
		SLSize _autoMessageIdx;

	};

	//! @brief ClientChatNetPackageSender is the client side specific implementation
	//!        supporting the sending of chat messages
	//!
	//! this class is also featuring a simple user interface
	//!
	class ClientChatNetPackageSender : public AbstractChatNetPackageSender	{
	public:

		static void createForPeerNode( PeerNode* peerNode, CCControlBase* parentLayer );

		virtual void onCtrlEditEnd(CCObject* sender);

		void onUIAction( CCObject *sender, CCControlEvent controlEvent );


		CC_SYNTHESIZE(CCControlEditBox*, _ctrlName, CtrlName)
		CC_SYNTHESIZE(CCControlEditBox*, _ctrlPassword, CtrlPassword)
		CC_SYNTHESIZE(CCControlEditBox*, _ctrlMessage, CtrlMessage)

	protected:
		ClientChatNetPackageSender();
		virtual ~ClientChatNetPackageSender();

		void createClientUI( PeerNode* peerNode, CCControlBase* parentLayer );

		virtual void sendData(const char* data, int lenght);
	private:
		CREATE_FUNC(ClientChatNetPackageSender)
	};


	//! @brief ServerChatNetPackageSender is the client side specific implementation
	//!        supporting the sending of chat messages
	//!
	//! this class is also featuring a simple user interface
	//!
	class ServerChatNetPackageSender : public AbstractChatNetPackageSender	{
	public:

		static void createForPeerNode( PeerNode* peerNode, CCControlBase* parentLayer  );

	protected:
		ServerChatNetPackageSender();
		virtual ~ServerChatNetPackageSender();

		void createServerUI( PeerNode* peerNode, CCControlBase* parentLayer );

		virtual void sendData(const char* data, int lenght);
	private:
		CREATE_FUNC(ServerChatNetPackageSender)

	};


	class ClientChatLogic : public AbstractNode	{
	public:
		static void createForPeerNode( PeerNode* peerNode, CCControlBase* parentLayer );
	};

	class ServerChatLogic : public AbstractNode	{
	public:
		static void createForPeerNode( PeerNode* peerNode, CCControlBase* parentLayer );
	};

	class PeerToPeerChatLogic : public AbstractNode	{
	public:
		static void createForPeerNode( PeerNode* peerNode, CCControlBase* parentLayer );
	};
}


#endif // __NLCHATSAMPLE_H__
