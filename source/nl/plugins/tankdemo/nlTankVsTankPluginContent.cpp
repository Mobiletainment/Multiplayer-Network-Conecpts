//!---------------------------------------------------------------------------
//! \file "nlTankVsTankPluginContent.cpp"
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
#include "nlTankVsTankPluginContent.h"
#include "nlReplicaManagerUIActionDispatcher.h"
#include "nlTankVsTankGameplayLayer.h"
#include "nlTankVsTankGameLogicNode.h"
#include "nlTankVsTankClientLogicNode.h"

#include "components/slTankWeaponComponent.h"

#include <stateless/cocosgame/slCocosGame.h>

#include <nl/plugin/nlPlugin.h>

namespace nl	{

	CCControlBase* createTankVsTankGameplayLayer( 
		const char* className, 
		const CCSize& preferredSize,
		CCScale9Sprite* backgroundSprite
		)	{
			TankVsTankGameplayLayer* gameplayLayer(TankVsTankGameplayLayer::create());
			gameplayLayer->setPreferredSize(preferredSize);
			gameplayLayer->needsLayout();
			return gameplayLayer;
	}


	PluginLayerContent* createPluginLayerContent()	{
		return TankVsTankPluginContent::create();
	}


	TankVsTankPluginContent::TankVsTankPluginContent()
	{
		// a little control magic here
		ControlFactory::addCreateControlCallBack(TankVsTankGameplayLayer::staticClassName(), createTankVsTankGameplayLayer);
		setRightLayerClassName(CCString::create(TankVsTankGameplayLayer::staticClassName()));

		setNetworkArchitecture(ENetworkArchitecture_SINGLEPLAYER);
		setNumSubLayers(1);
	}

	TankVsTankPluginContent::~TankVsTankPluginContent()	{
		ControlFactory::removeCreateControlCallbacks();
	}

	bool TankVsTankPluginContent::init()
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
			"extend the implementation of nl::GameReplicaComponent\n"
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

	void TankVsTankPluginContent::onUIAction( CCObject *sender, CCControlEvent controlEvent )
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


	void TankVsTankPluginContent::addRootMenuControls( CCArray* ctrlRowArray )	{
		ctrlRowArray->addObject(createRootButton("Information", SL_CTRLID_INFO));
		ctrlRowArray->addObject(createRootButton("Single Player", SL_CTRLID_QUADLAYER_RECREATE_TWINLAYER));
		ctrlRowArray->addObject(createRootButton("Client Server", SL_CTRLID_QUADLAYER_RECREATE_CLIENTSERVER));
		ctrlRowArray->addObject(createRootButton("Peer to Peer", SL_CTRLID_QUADLAYER_RECREATE_PEERTOPEER));
		ctrlRowArray->addObject(createRootButton("Tasks", SL_CTRLID_TASKS));
	}

	void TankVsTankPluginContent::addSubLayerContent( 
		SLSize idx, 
		CCControlBase* twinLayer, 
		CCControlBase* leftLayer, 
		CCControlBase* rightLayer )	
	{
		nl::PeerNode* peerNode(nullptr);

		TankVsTankGameplayLayer* gameplayLayer = addGameplayLayer(rightLayer);

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

					// now add some game logic to the server side
					TankVsTankGameLogicNode* gameLogicNode(TankVsTankGameLogicNode::create());
					gameLogicNode->setPeer(peerNode->getPeer());
					gameLogicNode->setGameplayLayer(gameplayLayer);
					// the twin layer is the master to add the game logic node to
					twinLayer->addChild(gameLogicNode);


				}
				else	{
					// some clients
					nl::PeerNode* clientPeerNode(nl::ClientPeerNode::create());
					twinLayer->addChild(clientPeerNode);
					clientPeerNode->createPeerUI(leftLayer);
					peerNode = clientPeerNode;

					// now add some client logic to the client side
					TankVsTankClientLogicNode* gameLogicNode(TankVsTankClientLogicNode::create());
					gameLogicNode->setPeer(peerNode->getPeer());
					gameLogicNode->setGameplayLayer(gameplayLayer);
					// the twin layer is the master to add the game logic node to
					twinLayer->addChild(gameLogicNode);
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
			// single player
			// can also test server game logic
			TankVsTankGameLogicNode* gameLogicNode(TankVsTankGameLogicNode::create());
			gameLogicNode->setGameplayLayer(gameplayLayer);
			// the twin layer is the master to add the game logic node to
			twinLayer->addChild(gameLogicNode);

			addGameContent(idx, nullptr, gameplayLayer);
		}
	}

	TankVsTankGameplayLayer* TankVsTankPluginContent::addGameplayLayer( CCLayer* parentLayer )
	{
		// check if the parentLayer is already a TankVsTankGameplayLayer
		// in which case nothing needs to be done here
		TankVsTankGameplayLayer* gameplayLayer(dynamic_cast<TankVsTankGameplayLayer*>(parentLayer));
		if(gameplayLayer == nullptr)	{
			gameplayLayer = TankVsTankGameplayLayer::create();
			gameplayLayer->setPreferredSize(parentLayer->getContentSize());
			gameplayLayer->needsLayout();

			parentLayer->addChild(gameplayLayer);
		}

		return gameplayLayer;
	}

	void TankVsTankPluginContent::addGameContent( SLSize idx, PeerNode* peerNode, CCControlBase* parentLayer )
	{
		switch(getNetworkArchitecture())	{
		case ENetworkArchitecture_SINGLEPLAYER:	
			{
				addSinglePlayerGameContent(parentLayer);
			}
			break;
		case ENetworkArchitecture_PEERTOPEER:	
			{
				// note: this might be useful to add some static content
			}
			break;
		case ENetworkArchitecture_CLIENTSERVER:	
			{
				// note: this might be useful to add some static content
			}
			break;
		}
	}

	void TankVsTankPluginContent::addGameContentUI( SLSize idx, PeerNode* peerNode, TankVsTankGameplayLayer* parentLayer )
	{
		// first create a ui dispatch object
		ReplicaManagerUIActionDispatcher* wrapper(ReplicaManagerUIActionDispatcher::create());
		parentLayer->addChild(wrapper);

		wrapper->_replicaManagerCreator.setGameplayLayer(parentLayer);

		// hand over the replica manger plugin creator to the peer which will finally create the replica manager
		peerNode->getPeer()->setReplicaManagerCreator(&wrapper->_replicaManagerCreator);

		CCSize ctrlsPreferredSize(CCSizeMake(0,0));
		CCArray* ctrls = CCArray::create();
		CCControlButton* ctrlBtn;

		if(peerNode->getPeer()->getTopology() != CLIENT)
		{
			ctrlBtn = nl::ControlUtils::createButton("Create Replica");
			ctrlBtn->setTag(SL_CTRLID_REPLICA_CREATE);
			ctrlBtn->addTargetWithActionForControlEvents(
				wrapper, 
				cccontrol_selector(ReplicaManagerUIActionDispatcher::onUIAction), 
				CCControlEventTouchUpInside);
			ctrls->addObject(ctrlBtn);
		}
		ctrlBtn = nl::ControlUtils::createButton("List Replicas");
		ctrlBtn->setTag(SL_CTRLID_REPLICA_LIST);
		ctrlBtn->addTargetWithActionForControlEvents(
			wrapper, 
			cccontrol_selector(ReplicaManagerUIActionDispatcher::onUIAction), 
			CCControlEventTouchUpInside);
		ctrls->addObject(ctrlBtn);


		CCControlColumn* ctrlContainer = ControlUtils::createControlColumn(ctrls,ctrlsPreferredSize);
		ctrlContainer->needsLayout();
		parentLayer->addChild(ctrlContainer);
	}


	GameActorNode* TankVsTankPluginContent::addActorNode( 
		CCControlBase* parentLayer, 
		const char* textureName, 
		const CCPoint& position )	{
			CCDictionary* constructionDictionary(TankVsTankGameLogicNode::createTankCreationDictionary(textureName, position));
			GameActorNode* actor(GameActorNode::create(constructionDictionary));
			actor->createActorSpriteWithDictionary(constructionDictionary);
			actor->getActorSprite()->getActorFlags().removeFlag(EActorFlag_DrawVehicle);

			TankWeaponComponent* weaponComponent(TankWeaponComponent::create());
			weaponComponent->addToActorNode(actor);

			parentLayer->addChild(actor);
			return actor;
	}

	void TankVsTankPluginContent::addSinglePlayerGameContent( CCControlBase* parentLayer )
	{
		GameActorNode* actor;
		AbstractVehicle* vehicle;
		// create 4 tanks for 4 players
		CCPoint position(100,100);
		actor = addActorNode(parentLayer, "bluetank.png", position);
		vehicle = actor->getActorSprite()->getVehicle();
		vehicle->setForward((Vec3(vehicle->position()) * -1.0).normalized());
		vehicle->setUnitSideFromForwardAndUp();

		position.x -= 200;
		actor = addActorNode(parentLayer, "redtank.png", position);
		vehicle = actor->getActorSprite()->getVehicle();
		vehicle->setForward((Vec3(vehicle->position()) * -1.0).normalized());
		vehicle->setUnitSideFromForwardAndUp();

		position.y -= 200;
		actor = addActorNode(parentLayer, "yellowtank.png", position);
		vehicle = actor->getActorSprite()->getVehicle();
		vehicle->setForward((Vec3(vehicle->position()) * -1.0).normalized());
		vehicle->setUnitSideFromForwardAndUp();

		position.x += 200;
		actor = addActorNode(parentLayer, "greentank.png", position);
		vehicle = actor->getActorSprite()->getVehicle();
		vehicle->setForward((Vec3(vehicle->position()) * -1.0).normalized());
		vehicle->setUnitSideFromForwardAndUp();
	}

}


