//!---------------------------------------------------------------------------
//! \file "nlPedestrianSamplePluginContent.cpp"
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
#include "nlPedestrianSamplePluginContent.h"
#include "nlReplicaManagerUIActionDispatcher.h"

#include <stateless/cocosgame/slCocosGame.h>

#include <nl/plugin/nlPlugin.h>

namespace nl	{

	PluginLayerContent* createPluginLayerContent()	{
		return PedestrianSamplePluginContent::create();
	}


	PedestrianSamplePluginContent::PedestrianSamplePluginContent()
	{
		setPluginHasPhysics(true);

		// a little control magic here
		ControlFactory::addCreateControlCallBack(GameplayLayer::staticClassName(), createGameplayLayer);
		setRightLayerClassName(CCString::create(GameplayLayer::staticClassName()));

		setNetworkArchitecture(ENetworkArchitecture_SINGLEPLAYER);
		setNumSubLayers(1);
	}

	bool PedestrianSamplePluginContent::init()
	{
		CCDictionary* dataSource(NetworkState::sharedInstance()->getSubDictionary(SL_SERIALIZEKEY_PLUGIN_GLOBAL_SETTINGS));

		// plugin information
		dataSource->setObject(CCString::create(
			"sample of replica based client/server simulation"
			),SL_SERIALIZEKEY_PLUGIN_INFO_BRIEF);

		dataSource->setObject(CCString::create(
			"This sample features RakNet replicas\n"
			"using the implementation of sl::GameObjectReplica\n"
			"\n"
			"Take a special look at the project slCososgame."
			),SL_SERIALIZEKEY_PLUGIN_INFO_DETAIL);

		// task information
		dataSource->setObject(CCString::create(
			"extend the implementation of nl::StudentReplicaComponent\n"
			"- implement client side interpolation\n"
			"- implement a traffic profiler\n"
			"- implement a binary protocol\n"
			"- extend the implementation to feature a peer to peer architecture\n"
			),SL_SERIALIZEKEY_PLUGIN_TASKS);

		dataSource->setObject(CCString::create(
			"- search for TODO: @student to find some entry points\n"
			"- read the related RakNet documentation\n"
			"- take a look at provided compression source code\n"
			"- recap what has been said about splitting messages into chunks devided by id's\n"
			"\n"
			"ask us if you do not understand ..."
			),SL_SERIALIZEKEY_PLUGIN_TASK_HINTS);


		return SLBaseClass::init();
	}

	void PedestrianSamplePluginContent::onUIAction( CCObject *sender, CCControlEvent controlEvent )
	{
		CCControlQuadLayer* quadLayer(getQuadLayer());

		bool dispatched(true);
		// determine the sender tag to dispatch the action
		const int tag(Dialog::getTag(sender));
		switch(tag)	{
		case SL_CTRLID_QUADLAYER_RECREATE_TWINLAYER:	
			{
				if(getNetworkArchitecture() != ENetworkArchitecture_SINGLEPLAYER)	{
					setNetworkArchitecture(ENetworkArchitecture_SINGLEPLAYER);
					recreateSubLayers(1);
				}
			}
			break;
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


	void PedestrianSamplePluginContent::addRootMenuControls( CCArray* ctrlRowArray )	{
		ctrlRowArray->addObject(createRootButton("Information", SL_CTRLID_INFO));
		ctrlRowArray->addObject(createRootButton("Single Player", SL_CTRLID_QUADLAYER_RECREATE_TWINLAYER));
		ctrlRowArray->addObject(createRootButton("Client Server", SL_CTRLID_QUADLAYER_RECREATE_CLIENTSERVER));
		ctrlRowArray->addObject(createRootButton("Peer to Peer", SL_CTRLID_QUADLAYER_RECREATE_PEERTOPEER));
		ctrlRowArray->addObject(createRootButton("Tasks", SL_CTRLID_TASKS));
	}

	void PedestrianSamplePluginContent::addSubLayerContent( 
		SLSize idx, 
		CCControlBase* twinLayer, 
		CCControlBase* leftLayer, 
		CCControlBase* rightLayer )	
	{
		nl::PeerNode* peerNode(nullptr);

		GameplayLayer* gameplayLayer = addGameplayLayer(rightLayer);

		// the twin layer is the master to add the peer node to
		switch(getNetworkArchitecture())	{
		case ENetworkArchitecture_SINGLEPLAYER:	
			{
				// no peer nodes
				leftLayer->setVisible(false);
				twinLayer->needsLayout();
			}
			break;
		case ENetworkArchitecture_PEERTOPEER:	
			{
				// only peer to peer nodes
				nl::PeerNode* peerToPeerNode(nl::PeerToPeerNode::create());
				twinLayer->addChild(peerToPeerNode);
				peerToPeerNode->createPeerUI(leftLayer);
				peerNode = peerToPeerNode;
			}
			break;
		case ENetworkArchitecture_CLIENTSERVER:	
			{
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
			break;
		}

		if(peerNode != nullptr)	{
			addGameContentUI(idx, peerNode, gameplayLayer);
			gameplayLayer->setPeer(peerNode->getPeer());
			peerNode->addUI(PeerOptionsUI::create());
		}
		else	{
			addGameContent(idx, nullptr, gameplayLayer);
		}
	}

	GameplayLayer* PedestrianSamplePluginContent::addGameplayLayer( CCLayer* parentLayer )
	{
		// check if the parentLayer is already a GameplayLayer
		// in which case nothing needs to be done here
		GameplayLayer* gameplayLayer(dynamic_cast<GameplayLayer*>(parentLayer));
		if(gameplayLayer == nullptr)	{
			gameplayLayer = GameplayLayer::create();
			gameplayLayer->setPreferredSize(parentLayer->getContentSize());
			gameplayLayer->needsLayout();

			parentLayer->addChild(gameplayLayer);
		}
		return gameplayLayer;
	}

	void PedestrianSamplePluginContent::addGameContent( SLSize idx, PeerNode* peerNode, CCControlBase* parentLayer )
	{
		switch(getNetworkArchitecture())	{
		case ENetworkArchitecture_SINGLEPLAYER:	
			{
				for(SLSize i(0); i < 10; ++i)	{
					CCDictionary* parameters(CCDictionary::create());
					int assetIdx(randomIntLowerUpper(0,3));
					SLAString textureName[4] =	{
						"bluetank.png",
						"redtank.png",
						"yellowtank.png",
						"greentank.png",
					};
					parameters->setObject(CCString::create(textureName[assetIdx].c_str()), "sprite");
					parameters->setObject(CCFloat::create(randomLowerUpper(-100.0f, 100.0f)), "x");
					parameters->setObject(CCFloat::create(randomLowerUpper(-100.0f, 100.0f)), "y");
					GameActorNode* actor(GameActorNode::create(parameters));
					actor->createActorSpriteWithDictionary(parameters);
					parentLayer->addChild(actor);
				}
			}
			break;
		case ENetworkArchitecture_PEERTOPEER:	
			{
			}
			break;
		case ENetworkArchitecture_CLIENTSERVER:	
			{
			}
			break;
		}
	}

	void PedestrianSamplePluginContent::addGameContentUI( SLSize idx, PeerNode* peerNode, GameplayLayer* parentLayer )
	{
		// first create a ui dispatch object
		ReplicaManagerUIActionDispatcher* wrapper(ReplicaManagerUIActionDispatcher::create());
		parentLayer->addChild(wrapper);

		wrapper->_replicaManagerCreator.setGameplayLayer(parentLayer);

		// hand over the replica manger plugin creator to the peer which will finally create the replica manager
		peerNode->getPeer()->setReplicaManagerCreator(&wrapper->_replicaManagerCreator);

		if(peerNode->getPeer()->getTopology() != CLIENT)	{
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

			CCControlRow* ctrlContainer = ControlUtils::createControlRow(ctrls,ctrlsPreferredSize);
			ctrlContainer->needsLayout();
			parentLayer->addChild(ctrlContainer);
		}
	}
}


