//!---------------------------------------------------------------------------
//! \file "nlPeerOptionsDialog.cpp"
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
#include "nlPeerOptionsDialog.h"
#include "nlPeerOptionsUI.h"



namespace nl	{

	//! @brief NetworkSimulatorPeerOptionsUI extends the basic peer configuration
	//!        with chat specific options
	//!
	class NetworkSimulatorPeerOptionsUI : public AbstractDialogExtenderNode	{
		SL_DECLARE_BASE(PeerOptionsUI)
	public:

		CREATE_FUNC(NetworkSimulatorPeerOptionsUI)

		virtual void configure() SL_OVERRIDE;

		virtual void onUIAction(CCObject *sender, CCControlEvent controlEvent) SL_OVERRIDE;

		virtual void extendDialog( Dialog* dialog ) SL_OVERRIDE;

		SL_SYNTHESIZE(PeerNode*, _peerNode, PeerNode);

	protected:
		~NetworkSimulatorPeerOptionsUI();
	};


	NetworkSimulatorPeerOptionsUI::~NetworkSimulatorPeerOptionsUI()
	{

	}


	void NetworkSimulatorPeerOptionsUI::configure()
	{
		Peer* peer(getPeerNode()->getPeer());
		CCDictionary* peerSetting(peer->getPeerConfiguration());
		Dictionary::setObjectIfNotExist(peerSetting, CCBool::create(false),SL_SERIALIZEKEY_PEER_NETWORKSIMULATOR_ENABLE);
		Dictionary::setObjectIfNotExist(peerSetting, CCFloat::create(0.2f),SL_SERIALIZEKEY_PEER_NETWORKSIMULATOR_PACKETLOSS);
		Dictionary::setObjectIfNotExist(peerSetting, CCInteger::create(50),SL_SERIALIZEKEY_PEER_NETWORKSIMULATOR_MIN_EXTRA_PING);
		Dictionary::setObjectIfNotExist(peerSetting, CCInteger::create(10),SL_SERIALIZEKEY_PEER_NETWORKSIMULATOR_EXTRA_PING_VARIANCE);
	}

	void NetworkSimulatorPeerOptionsUI::extendDialog( Dialog* dialog )
	{
		CCControlColumn* ctrlColumn(dialog->getCtrlColumn());
		if(ctrlColumn != nullptr)	{
			Peer* peer(getPeerNode()->getPeer());
			CCDictionary* peerSetting(peer->getPeerConfiguration());

			CCSize preferredSize(CCSizeMake(0,0));

			CCControlColumn* ctrlPeerOptionsContainer(ControlUtils::createControlColumn(nullptr,preferredSize));
			CCControlBase* ctrlContainer;

			ctrlContainer = nl::ControlUtils::createCheckBox("Enable network simulator", preferredSize);
			ctrlContainer->setValueKey(SL_SERIALIZEKEY_PEER_NETWORKSIMULATOR_ENABLE);
			ctrlContainer->getCtrlLayoutFlags().addFlag(ECtrlLayoutFlag_ResetPreferredSizeOnSerialize);
			ctrlContainer->deserialize(peerSetting);
			ctrlPeerOptionsContainer->addNode(ctrlContainer);

			ctrlContainer = ControlUtils::createEditBox("Packet loss probability (0-1):", preferredSize);
			ctrlContainer->setValueKey(SL_SERIALIZEKEY_PEER_NETWORKSIMULATOR_PACKETLOSS);
			ctrlContainer->getCtrlLayoutFlags().addFlag(ECtrlLayoutFlag_ResetPreferredSizeOnSerialize);
			ctrlContainer->deserialize(peerSetting);
			ctrlPeerOptionsContainer->addNode(ctrlContainer);

			ctrlContainer = ControlUtils::createEditBox("Min extra ping (ms):", preferredSize);
			ctrlContainer->setValueKey(SL_SERIALIZEKEY_PEER_NETWORKSIMULATOR_MIN_EXTRA_PING);
			ctrlContainer->getCtrlLayoutFlags().addFlag(ECtrlLayoutFlag_ResetPreferredSizeOnSerialize);
			ctrlContainer->deserialize(peerSetting);
			ctrlPeerOptionsContainer->addNode(ctrlContainer);

			ctrlContainer = ControlUtils::createEditBox("Extra ping variance (ms):", preferredSize);
			ctrlContainer->setValueKey(SL_SERIALIZEKEY_PEER_NETWORKSIMULATOR_EXTRA_PING_VARIANCE);
			ctrlContainer->getCtrlLayoutFlags().addFlag(ECtrlLayoutFlag_ResetPreferredSizeOnSerialize);
			ctrlContainer->deserialize(peerSetting);
			ctrlPeerOptionsContainer->addNode(ctrlContainer);

			ctrlPeerOptionsContainer->setPreferredSize(preferredSize);
			ctrlPeerOptionsContainer->needsLayout();

			ctrlColumn->addNodeAt(ctrlPeerOptionsContainer, 1);

			ctrlColumn->setPreferredSize(preferredSize);
			ctrlColumn->needsLayout();
		}
	}

	void NetworkSimulatorPeerOptionsUI::onUIAction( CCObject *sender, CCControlEvent controlEvent )
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




	PeerOptionsDialog* PeerOptionsDialog::createForPeerNode( PeerNode* peerNode, IDialogExtender* dialogExtender )	{
		PeerOptionsDialog* ctrl = new PeerOptionsDialog();
		ctrl->autorelease();
		ctrl->initWidthPeerNode(peerNode, dialogExtender);
		return ctrl;
	}

	bool PeerOptionsDialog::initWidthPeerNode(PeerNode* peerNode, IDialogExtender* dialogExtender)	{
		CCDictionary* peerSetting(peerNode->getPeer()->getPeerConfiguration());
		if(peerSetting == nullptr)	{
			SL_PROCESS_APP()->log(ELogType_Error,"failed to retrieve peer configuration");
			return false;
		}

		setDataSource(peerSetting);

		// create the peer options
		CCSize btnContainerSize(CCSizeMake(0,0));

		CCArray* optionCtrls = CCArray::create();
		CCSize checkBoxPreferredSize(CCSizeMake(0,0));
		CCControlCheckBox* ctrlCheckBox;

		optionCtrls->addObject(peerNode->createPeerInfoControl());

		optionCtrls->addObject(ControlUtils::createLabel("Extensions"));

		ctrlCheckBox = nl::ControlUtils::createCheckBox("Packet logger", checkBoxPreferredSize);
		ctrlCheckBox->setValueKey(SL_SERIALIZEKEY_PEER_USE_PACKETLOGGER);
		optionCtrls->addObject(ctrlCheckBox);

		ctrlCheckBox = nl::ControlUtils::createCheckBox("Fully connected mesh", checkBoxPreferredSize);
		ctrlCheckBox->setValueKey(SL_SERIALIZEKEY_PEER_USE_FULLYCONNECTEDMESH);
		optionCtrls->addObject(ctrlCheckBox);

		ctrlCheckBox = nl::ControlUtils::createCheckBox("RPC 4", checkBoxPreferredSize);
		ctrlCheckBox->setValueKey(SL_SERIALIZEKEY_PEER_USE_RPC4);
		optionCtrls->addObject(ctrlCheckBox);

		ctrlCheckBox = nl::ControlUtils::createCheckBox("Replicamanager", checkBoxPreferredSize);
		ctrlCheckBox->setValueKey(SL_SERIALIZEKEY_PEER_USE_REPLICAMANAGER);
		optionCtrls->addObject(ctrlCheckBox);

		CCControlColumn* ctrlOptionsContainer = ControlUtils::createControlColumn(optionCtrls,btnContainerSize);
		ctrlOptionsContainer->getCtrlFlags().removeFlag(ECtrlFlag_UseScissorTest);

		CCArray* dialogCtrls = CCArray::create();
		dialogCtrls->addObject(ctrlOptionsContainer);
		dialogCtrls->addObject(createOkBtn());

		CCControlColumn* ctrlDialogContainer = ControlUtils::createControlColumn(dialogCtrls,btnContainerSize);
		ctrlDialogContainer->getCtrlFlags().removeFlag(ECtrlFlag_UseScissorTest);

		const CCSize& ctrlSize(ctrlDialogContainer->getContentSize());
		setPreferredSize(ctrlSize);

		// call the base class here we don't want all those background sprites
		bool initialized(SLBaseClass::init());

		addChild(ctrlDialogContainer);
		setCtrlColumn(ctrlDialogContainer);

		bool needsNewLayout(false);

		// as the network simulator is only available in debug mode
		// add the corresponding ui only in debug mode
#ifdef _DEBUG
		{
			NetworkSimulatorPeerOptionsUI* dlgExtender(NetworkSimulatorPeerOptionsUI::create());
			dlgExtender->setPeerNode(peerNode);
			dlgExtender->configure();
			addChild(dlgExtender);
			dlgExtender->extendDialog(this);
			ctrlDialogContainer->redoLayout();
			needsNewLayout = true;
		}
#endif

		if(dialogExtender != nullptr)	{
			dialogExtender->extendDialog(this);
			ctrlDialogContainer->redoLayout();
			needsNewLayout = true;
		}

		if(needsNewLayout)	{
			const CCSize& ctrlNewSize(ctrlDialogContainer->getContentSize());
			setPreferredSize(ctrlNewSize);
			needsLayout();
		}

		// read the data from the data store
		deserializeAll(getDataSource());
		return initialized;
	}

	void PeerOptionsDialog::onUIAction( CCObject *sender, CCControlEvent controlEvent )
	{
#ifdef _DEBUG
		// this is actually not needed but nice while debugging stuff
		// and not terminating the app the usual way
		// note: the base class implementation handles the same functionality
		//       including a call to remove from parent

		bool doRemoveFromParent(false);
		bool doSaveState(false);
		// determine the sender tag to dispatch the action
		const int tag(Dialog::getTag(sender));
		switch(tag)	{
		case SL_CTRLID_OK:	{
			serializeAll(getDataSource());
			doRemoveFromParent = true;
			doSaveState = true;
							}
							break;
		case SL_CTRLID_CANCEL:
			doRemoveFromParent = true;
			break;
		}

		if(doSaveState)	{
			NetworkState* networkState(NetworkState::sharedInstance());
			networkState->save();
		}
		if(doRemoveFromParent)	{
			removeFromParent();
		}
#else
		SLBaseClass::onUIAction(sender,controlEvent);
#endif
	}




}


