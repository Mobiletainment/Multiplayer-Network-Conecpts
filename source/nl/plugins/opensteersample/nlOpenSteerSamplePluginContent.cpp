//!---------------------------------------------------------------------------
//! \file "nlOpenSteerSamplePluginContent.cpp"
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
#include "nlOpenSteerSamplePluginContent.h"

#include <stateless/cocosgame/slCocosGame.h>
#include <nl/plugin/nlPlugin.h>

namespace nl	{

	PluginLayerContent* createPluginLayerContent()	{
		return OpenSteerSamplePluginContent::create();
	}

	OpenSteerSamplePluginContent::OpenSteerSamplePluginContent()
	{
		setPluginHasPhysics(true);

		// a little control magic here
		ControlFactory::addCreateControlCallBack(GameplayLayer::staticClassName(), createGameplayLayer);
		setRightLayerClassName(CCString::create(GameplayLayer::staticClassName()));

		setNetworkArchitecture(ENetworkArchitecture_SINGLEPLAYER);
		setNumSubLayers(1);
	}

	bool OpenSteerSamplePluginContent::init()
	{
		CCDictionary* dataSource(NetworkState::sharedInstance()->getSubDictionary(SL_SERIALIZEKEY_PLUGIN_GLOBAL_SETTINGS));

		// plugin information
		dataSource->setObject(CCString::create(
			"single player OpenSteer pedestrians sample"
			),SL_SERIALIZEKEY_PLUGIN_INFO_BRIEF);

		dataSource->setObject(CCString::create(
			"This sample features OpenSteer vehicles\n"
			"wrapped into cocos2d nodes\n"
			"\n"
			"Take a special look at the project slCososgame.\n"
			"- sl::Actor\n"
			"- sl::ActorComponent\n"
			"- sl::ActorNode\n"
			"- sl::ActorSprite\n"
			),SL_SERIALIZEKEY_PLUGIN_INFO_DETAIL);

		// task information
		dataSource->setObject(CCString::create(
			"get familiar with the sl::GameplayLayer and its children\n"
			"get familiar with the namespace OpenSteer and its children\n"
			"take a look at the project slSimulation\n"
			"get familiar with the cocos2d update cycle\n"
			),SL_SERIALIZEKEY_PLUGIN_TASKS);

		dataSource->setObject(CCString::create(
			"- search for TODO: @student to find some entry points\n"
			"- read the related source and documentation\n"
			"\n"
			"ask us if you do not understand ..."
			),SL_SERIALIZEKEY_PLUGIN_TASK_HINTS);


		return SLBaseClass::init();
	}

	void OpenSteerSamplePluginContent::onUIAction( CCObject *sender, CCControlEvent controlEvent )
	{
		const int tag(Dialog::getTag(sender));
		switch(tag)	{
		case SL_CTRLID_ADD_ACTORNODE:	
			{
				GameplayLayer* gameplayLayer = GameplayLayer::getGameplayLayer(sender);
				for(SLSize i(0); i < 5; ++i)	{
					addActorNode(gameplayLayer);
				}
			}
			break;
		default:
			break;
		}
		SLBaseClass::onUIAction(sender, controlEvent);
	}

	void OpenSteerSamplePluginContent::addRootMenuControls( CCArray* ctrlRowArray )	{
		ctrlRowArray->addObject(createRootButton("Information", SL_CTRLID_INFO));
		ctrlRowArray->addObject(createRootButton("Single view", SL_CTRLID_QUADLAYER_RECREATE_TWINLAYER));
		ctrlRowArray->addObject(createRootButton("Quad View", SL_CTRLID_QUADLAYER_RECREATE));
		ctrlRowArray->addObject(createRootButton("Tasks", SL_CTRLID_TASKS));
	}

	void OpenSteerSamplePluginContent::addSubLayerContent( 
		SLSize idx, 
		CCControlBase* twinLayer, 
		CCControlBase* leftLayer, 
		CCControlBase* rightLayer )	
	{
		// the twin layer is the master to add the peer node to
		// no peers here
		leftLayer->setVisible(false);
		twinLayer->needsLayout();
		GameplayLayer* gameplayLayer = addGameplayLayer(rightLayer);
		addGameContent(gameplayLayer);
	}

	GameplayLayer* OpenSteerSamplePluginContent::addGameplayLayer( CCLayer* parentLayer )
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

		// add one button to perform additional actor creation
		CCSize ctrlPreferredSize(CCSizeMake(0,0));
		CCArray* ctrls = CCArray::create();


		CCControlBase* ctrl;
		ctrl = ControlUtils::createValueBox("Vehiclecount:", ctrlPreferredSize);
		ctrl->setValueKey(SL_SERIALIZEKEY_NUM_VEHICLES);
		ctrls->addObject(ctrl);

		ctrl->deserialize(
			gameplayLayer->getGameState()->getSubDictionary("vehicle_simulation"));

		gameplayLayer->getSerializeControls().add(ctrl);


		ctrl = nl::ControlUtils::createCheckBox("Draw vehicles", ctrlPreferredSize);
		ctrl->setValueKey(SL_SERIALIZEKEY_VEHICLE_DEBUG_DRAW);
		ctrls->addObject(ctrl);

		ctrl->deserialize(
			gameplayLayer->getGameState()->getSubDictionary("vehicle_simulation"));

		Dictionary::setBool(gameplayLayer->getGameState()->getSubDictionary("vehicle_simulation"), SL_SERIALIZEKEY_VEHICLE_DEBUG_DRAW, true);
		gameplayLayer->getSerializeControls().add(ctrl);





		CCControlButton* ctrlBtn;
		ctrlBtn = nl::ControlUtils::createButton("Add Actors");
		ctrlBtn->setTag(SL_CTRLID_ADD_ACTORNODE);
		ctrlBtn->addTargetWithActionForControlEvents(
			this, 
			cccontrol_selector(OpenSteerSamplePluginContent::onUIAction), 
			CCControlEventTouchUpInside);
		ctrls->addObject(ctrlBtn);

		CCControlRow* ctrlContainer = ControlUtils::createControlRow(ctrls,ctrlPreferredSize);
		ctrlContainer->getCtrlFlags().removeFlag(ECtrlFlag_UseScissorTest);
		ctrlContainer->needsLayout();
		gameplayLayer->addChild(ctrlContainer);

		return gameplayLayer;
	}

	void OpenSteerSamplePluginContent::addActorNode( CCControlBase* parentLayer )	{
		CCDictionary* constructionDictionary(CCDictionary::create());
		int assetIdx(randomIntLowerUpper(0,3));
		SLAString textureName[4] =	{
			"bluetank.png",
			"redtank.png",
			"yellowtank.png",
			"greentank.png",
		};
		constructionDictionary->setObject(CCString::create(textureName[assetIdx].c_str()), "sprite");
		constructionDictionary->setObject(CCFloat::create(randomLowerUpper(-100.0f, 100.0f)), "x");
		constructionDictionary->setObject(CCFloat::create(randomLowerUpper(-100.0f, 100.0f)), "y");
		GameActorNode* actor(GameActorNode::create(constructionDictionary));
		actor->createActorSpriteWithDictionary(constructionDictionary);

		actor->getActorSprite()->setTouchEnabled(true);

		parentLayer->addChild(actor);
	}

	void OpenSteerSamplePluginContent::addGameContent( CCControlBase* parentLayer )
	{
		for(SLSize i(0); i < 5; ++i)	{
			addActorNode(parentLayer);
		}
	}

}


