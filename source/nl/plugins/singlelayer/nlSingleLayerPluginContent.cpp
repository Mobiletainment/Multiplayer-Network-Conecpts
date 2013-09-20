//!---------------------------------------------------------------------------
//! \file "nlSingleLayerPluginContent.cpp"
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
#include "nlSingleLayerPluginContent.h"

#include "stateless/cocosgame/slCocosgame.h"



namespace nl	{

	PluginLayerContent* createPluginLayerContent()	{
		return SingleLayerPluginContent::create();
	}

	SingleLayerPluginContent::SingleLayerPluginContent()
		:_gameplayLayer(nullptr)
	{
		setNetworkArchitecture(ENetworkArchitecture_SINGLEPLAYER);
	}

	SingleLayerPluginContent::~SingleLayerPluginContent()
	{
	}

	bool SingleLayerPluginContent::init()
	{
		CCDictionary* dataSource(NetworkState::sharedInstance()->getSubDictionary(SL_SERIALIZEKEY_PLUGIN_GLOBAL_SETTINGS));

		// plugin information
		dataSource->setObject(CCString::create(
			"sample of a single layer implementation"
			),SL_SERIALIZEKEY_PLUGIN_INFO_BRIEF);

		dataSource->setObject(CCString::create(
			"Nothing special\n"
			),SL_SERIALIZEKEY_PLUGIN_INFO_DETAIL);

		// task information
		dataSource->setObject(CCString::create(
			"Nothing special\n"
			),SL_SERIALIZEKEY_PLUGIN_TASKS);

		dataSource->setObject(CCString::create(
			"Nothing special\n"
			),SL_SERIALIZEKEY_PLUGIN_TASK_HINTS);


		return SLBaseClass::init();
	}

	void SingleLayerPluginContent::addContent(CCLayer* parentLayer)	{
		const CCSize& contentSize(parentLayer->getContentSize());

		_gameplayLayer = GameplayLayer::create();

		parentLayer->addChild(_gameplayLayer);
		_gameplayLayer->setPreferredSize(parentLayer->getContentSize());
		_gameplayLayer->needsLayout();
	}

	void SingleLayerPluginContent::onLayerResized(CCLayer* parentLayer)	{
		const CCSize& contentSize(parentLayer->getContentSize());
		_gameplayLayer->setPreferredSize(parentLayer->getContentSize());
		_gameplayLayer->needsLayout();
	}

	void SingleLayerPluginContent::addRootMenuControls( CCArray* ctrlRowArray )
	{
	}

	void SingleLayerPluginContent::addSubLayerContent( 
		SLSize idx, 
		CCControlBase* twinLayer, 
		CCControlBase* leftLayer, 
		CCControlBase* rightLayer )
	{
	}

	void SingleLayerPluginContent::onUIAction( CCObject *sender, CCControlEvent controlEvent )
	{
	}
}


