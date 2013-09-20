//!---------------------------------------------------------------------------
//! \file "nlReplicaSamplePluginContent.cpp"
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
#include "nlReplicaSamplePluginContent.h"

#include "nlSampleReplicaManager.h"

namespace nl	{

	PluginLayerContent* createPluginLayerContent()	{
		return ReplicaSamplePluginContent::create();
	}

	//-------------------------------------------------------------------------
	// ReplicaManagerCreator
	//-------------------------------------------------------------------------

	ReplicaManagerCreator::ReplicaManagerCreator()
		:_replicaManager(nullptr)
	{

	}

	RakNet::PluginInterface2* ReplicaManagerCreator::createPlugin(Peer* parentPeer, ENetwokTopology topolgy)	{
		_replicaManager = new SampleReplicaManager(topolgy);
		_replicaManager->setPeer(parentPeer);
		return _replicaManager;
	}

	void ReplicaManagerCreator::destroyPlugin(RakNet::PluginInterface2* plugin)	{
		CC_SAFE_DELETE(plugin);
		_replicaManager = nullptr;
	}

	//-------------------------------------------------------------------------
	// ReplicaManagerUIActionDispatcher
	//-------------------------------------------------------------------------

	bool ReplicaManagerUIActionDispatcher::init()	{
		bool initialized(SLBaseClass::init());
		return initialized;
	}

	void ReplicaManagerUIActionDispatcher::onUIAction( CCObject *sender, CCControlEvent controlEvent )	{

		// check the replica manager pointer
		// as we do not want to crash here
		SampleReplicaManager* plugin(_replicaManagerCreator.getReplicaManager());
		if(plugin == nullptr)	{
			return;
		}

		// determine the sender tag to dispatch the action
		const int tag(Dialog::getTag(sender));
		switch(tag)	{
		case SL_CTRLID_REPLICA_CREATE:	{
			plugin->createReplicas();
										}
										break;
		case SL_CTRLID_REPLICA_READ:	{
			plugin->readReplicas();
										}
										break;
		case SL_CTRLID_REPLICA_UPDATE:	{
			plugin->updateReplicas();
										}
										break;
		case SL_CTRLID_REPLICA_DELETE:	{
			plugin->deleteReplicas();
										}
										break;
		case SL_CTRLID_REPLICA_LIST:	{
			plugin->listReplicas();
										}
										break;
		}

	}

	//-------------------------------------------------------------------------
	// ReplicaSamplePluginContent
	//-------------------------------------------------------------------------

	ReplicaSamplePluginContent::ReplicaSamplePluginContent()
		:_isClientServerMode(true)
	{
		setNetworkArchitecture(ENetworkArchitecture_CLIENTSERVER);
		setLeftLayerClassName(CCString::create(CCControlColumn::staticClassName()));
		setRightLayerClassName(CCString::create(CCControlColumn::staticClassName()));
	}

	bool ReplicaSamplePluginContent::init()
	{
		CCDictionary* dataSource(NetworkState::sharedInstance()->getSubDictionary(SL_SERIALIZEKEY_PLUGIN_GLOBAL_SETTINGS));

		// plugin information
		dataSource->setObject(CCString::create(
			"basic raknet replica sample"
			),SL_SERIALIZEKEY_PLUGIN_INFO_BRIEF);

		dataSource->setObject(CCString::create(
			"This sample features derivates of:\n"
			"- RakNet::ReplicaManager3\n"
			"- RakNet::Replica3\n"
			"- Connection_RM3\n"
			"\n"
			"It also features the usage of:\n"
			"RakNet::VariableDeltaSerializer\n"
			),SL_SERIALIZEKEY_PLUGIN_INFO_DETAIL);

		// task information
		dataSource->setObject(CCString::create(
			"Implement a specific client replica.\n"
			"The requested functionality is:\n"
			"- created on a client\n"
			"- replicated to the server\n"
			"- not constructed on other clients\n"
			"- not replicated to other clients\n"
			),SL_SERIALIZEKEY_PLUGIN_TASKS);

		dataSource->setObject(CCString::create(
			"- read the related RakNet documentation\n"
			"- take a look at sl::AbstractReplica and the Query<yxz> implementation\n"
			"- instead of implementing a derivate try implementing a sl::ReplicationRule for nl::SampleReplica\n"
			"- read and debug the code\n"
			"\n"
			"ask us if you do not understand ..."
			),SL_SERIALIZEKEY_PLUGIN_TASK_HINTS);

		return SLBaseClass::init();
	}

	void ReplicaSamplePluginContent::onUIAction( CCObject *sender, CCControlEvent controlEvent )
	{

		CCControlQuadLayer* quadLayer(getQuadLayer());

		bool dispatched(true);
		// determine the sender tag to dispatch the action
		const int tag(Dialog::getTag(sender));
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


	void ReplicaSamplePluginContent::addRootMenuControls( CCArray* ctrlRowArray )	{
		ctrlRowArray->addObject(createRootButton("Information", SL_CTRLID_INFO));
		ctrlRowArray->addObject(createRootButton("Client Server", SL_CTRLID_QUADLAYER_RECREATE_CLIENTSERVER));
		ctrlRowArray->addObject(createRootButton("Peer to Peer", SL_CTRLID_QUADLAYER_RECREATE_PEERTOPEER));
		ctrlRowArray->addObject(createRootButton("Tasks", SL_CTRLID_TASKS));
	}

	void ReplicaSamplePluginContent::addSubLayerContent( 
		SLSize idx, 
		CCControlBase* twinLayer, 
		CCControlBase* leftLayer, 
		CCControlBase* rightLayer )	
	{
		// the twin layer is the master to add the peer node to

		nl::PeerNode* peerNode;

		bool peerToPeerSample(getNetworkArchitecture() == ENetworkArchitecture_PEERTOPEER);
		if(peerToPeerSample)	{
	 		// only peer to peer nodes
			nl::PeerNode* peerToPeerNode(nl::PeerToPeerNode::create());
			twinLayer->addChild(peerToPeerNode);
			peerToPeerNode->createPeerUI(leftLayer);
			peerNode = peerToPeerNode;
		}
		else	{
			if(idx == 0)	{
				// the server
				nl::PeerNode* serverPeerNode(nl::ServerPeerNode::create());
				twinLayer->addChild(serverPeerNode);
				serverPeerNode->createPeerUI(leftLayer);
				peerNode = serverPeerNode;
			}
			else	{
				// some clients
				nl::PeerNode* clientPeerNode(nl::ClientPeerNode::create());
				twinLayer->addChild(clientPeerNode);
				clientPeerNode->createPeerUI(leftLayer);
				peerNode = clientPeerNode;
			}
		}

		addGameContent(idx, peerNode, rightLayer);

		peerNode->addUI(PeerOptionsUI::create());
	}

	void ReplicaSamplePluginContent::addGameContent( SLSize idx, PeerNode* peerNode, CCControlBase* parentLayer )
	{
		// first create a ui dispatch object
		ReplicaManagerUIActionDispatcher* wrapper(ReplicaManagerUIActionDispatcher::create());
		parentLayer->addChild(wrapper);

		peerNode->getPeer()->setReplicaManagerCreator(&wrapper->_replicaManagerCreator);

		CCSize ctrlsPreferredSize(CCSizeMake(0,0));
		CCArray* ctrls = CCArray::create();

		CCControlButton* ctrlBtn;
		ctrlBtn = nl::ControlUtils::createButton("Create Replica");
		ctrlBtn->setTag(SL_CTRLID_REPLICA_CREATE);
		ctrlBtn->addTargetWithActionForControlEvents(
			wrapper, 
			cccontrol_selector(ReplicaManagerUIActionDispatcher::onUIAction), 
			CCControlEventTouchUpInside);
		ctrls->addObject(ctrlBtn);

		ctrlBtn = nl::ControlUtils::createButton("Delete Replica");
		ctrlBtn->setTag(SL_CTRLID_REPLICA_DELETE);
		ctrlBtn->addTargetWithActionForControlEvents(
			wrapper, 
			cccontrol_selector(ReplicaManagerUIActionDispatcher::onUIAction), 
			CCControlEventTouchUpInside);
		ctrls->addObject(ctrlBtn);

		ctrlBtn = nl::ControlUtils::createButton("Update Replica");
		ctrlBtn->setTag(SL_CTRLID_REPLICA_UPDATE);
		ctrlBtn->addTargetWithActionForControlEvents(
			wrapper, 
			cccontrol_selector(ReplicaManagerUIActionDispatcher::onUIAction), 
			CCControlEventTouchUpInside);
		ctrls->addObject(ctrlBtn);

		ctrlBtn = nl::ControlUtils::createButton("List Replicas");
		ctrlBtn->setTag(SL_CTRLID_REPLICA_LIST);
		ctrlBtn->addTargetWithActionForControlEvents(
			wrapper, 
			cccontrol_selector(ReplicaManagerUIActionDispatcher::onUIAction), 
			CCControlEventTouchUpInside);
		ctrls->addObject(ctrlBtn);

		CCControlColumn* ctrlContainer = ControlUtils::createControlColumn(ctrls,ctrlsPreferredSize);
		parentLayer->addChild(ctrlContainer);
	}

}


