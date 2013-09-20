//!---------------------------------------------------------------------------
//! \file "slGameActorComponent.cpp"
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
#include "slGameActorComponent.h"
#include "slGameplayLayer.h"

#include "slGameActorNode.h"
#include "vehicle/slPedestrian.h"

namespace sl	{


	GameActorComponent::GameActorComponent()
		:_gameplayLayer(nullptr)
		,_actorSprite(nullptr)
	{
	}

	GameActorComponent::~GameActorComponent()
	{

	}

	void GameActorComponent::createActorNode(CCDictionary* parameters)	{
		if(parameters == nullptr)	{
			// the actor itself does not need any parameters right now
		}

		// TODO: think about adding a reference here
		_actorNode = GameActorNode::create(parameters);
		_actorSprite = getGameActorNode()->getActorSprite();

		ComponentArray* components(_actorNode->getComponents());
		components->addComponent(this);
		getGameplayLayer()->addChild(_actorNode);
	}


	void GameActorComponent::createActorSprite(CCDictionary* parameters)	{
		if(parameters == nullptr)	{
			parameters = CCDictionary::create();
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
		}
		getGameActorNode()->createActorSpriteWithDictionary(parameters);
		_actorSprite = getGameActorNode()->getActorSprite();
	}

	GameActorNode* GameActorComponent::getGameActorNode() const	{
		return dynamic_cast<GameActorNode*>(_actorNode);
	}

	void GameActorComponent::removeFromGameplayLayer()
	{
		if(_actorNode != nullptr)	{
			SL_ASSERT(_actorNode!=nullptr);
			_actorNode->removeFromParent();
			_actorNode = nullptr;
		}
	}



}


