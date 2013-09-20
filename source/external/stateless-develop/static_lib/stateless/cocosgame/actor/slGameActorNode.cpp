//!---------------------------------------------------------------------------
//! \file "slGameActorNode.cpp"
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
#include "slGameActorNode.h"
#include "slActorSprite.h"
#include "slGameplayLayer.h"

#include "network/slReplicaComponent.h"

namespace sl	{

	GameActorNode::GameActorNode()
		:_actorSprite(nullptr)
	{
	}

	GameActorNode::~GameActorNode()
	{

	}

	GameActorNode* GameActorNode::create( CCDictionary* dictionary )
	{
		GameActorNode *actor = new GameActorNode();
		if (actor && actor->initWithDictionary(dictionary))
		{
			actor->autorelease();
			return actor;
		}
		CC_SAFE_DELETE(actor);
		return NULL;
	}

	bool GameActorNode::initWithDictionary( CCDictionary* dictionary )
	{
		return true;
	}

	bool GameActorNode::createActorSpriteWithDictionary( CCDictionary* dictionary )
	{
		if(dictionary == nullptr)	{
			SL_PROCESS_APP()->log(ELogType_Error, "%s - dictionary can not be null", __FUNCTION__);
			return false;	
		}

		// remove previously created ActorSprites if they exist
		CCArray* removeChildren(CCArray::create());
		CCArray* children(getChildren());
		CCObject* child;
		CCARRAY_FOREACH(children, child)
		{
			if(dynamic_cast<ActorSprite*>(child) != nullptr)	{
				removeChildren->addObject(child);
			}	
		}

		CCARRAY_FOREACH(removeChildren, child)
		{
			dynamic_cast<ActorSprite*>(child)->removeFromParent();
		}

		_actorSprite = ActorSprite::createWithDictionary(dictionary);
		if(_actorSprite != nullptr)	{
			addChild(_actorSprite);
		}

		// note: components need to be updated now
		SLSize idx(0);
		IComponent* component(_components->componentAt(idx));
		while(component != nullptr)	{
			// for actor components the actor sprite changes
			GameActorComponent* actorComponent(dynamic_cast<GameActorComponent*>(component));
			if(actorComponent != nullptr)	{
				actorComponent->setActorSprite(_actorSprite);
			}

			// for replica components the construction dictionary changes
			ReplicaComponent* replicaComponent(dynamic_cast<ReplicaComponent*>(component));
			if(replicaComponent != nullptr)	{
				replicaComponent->setConstructionDictionary(dictionary);
			}

			// TODO @CP : add a virtual onActorSpriteChanged

			++idx;
			component = _components->componentAt(idx);
		}

		return true;
	}

	GameplayLayer* GameActorNode::getGameplayLayer() const	{
		CCNode* parent(getParent());
		GameplayLayer* gameplayLayer(nullptr);
		while(parent != nullptr)	{
			gameplayLayer = dynamic_cast<GameplayLayer*>(parent);
			if(gameplayLayer == nullptr)	{
				parent = parent->getParent();
			}
			else	{
				break;
			}
		}
		return gameplayLayer;
	}

	bool GameActorNode::isDestroyed() const
	{
		bool isDestroyed(const_cast<GameActorNode*>(this)->getActorNodeFlags().hasFlag(EActorNodeFlag_IsDestroyed));
		if(isDestroyed == false)	{
			ActorSprite* actorSprite(getActorSprite());
			if(actorSprite != nullptr)	{
				isDestroyed = actorSprite->getActorFlags().hasFlag(EActorFlag_IsDestroyed);
			}
		}
		return isDestroyed;
	}

}


