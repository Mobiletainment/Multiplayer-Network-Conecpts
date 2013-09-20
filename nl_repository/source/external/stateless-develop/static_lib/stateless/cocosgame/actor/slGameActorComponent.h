//!---------------------------------------------------------------------------
//! \file "slGameActorComponent.h"
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
#ifndef __SLGAMEACTORCOMPONENT_H__
#define __SLGAMEACTORCOMPONENT_H__

namespace sl	{

	class ActorSprite;

	class GameplayLayer;
	class GameActorNode;

	class slCocosGame_API GameActorComponent : public ActorComponent	{

		friend class GameObjectReplica;

		SL_DECLARE_BASE(ActorComponent)
	public:
		CREATE_FUNC(GameActorComponent)

		//! creates an actor node and adds it to the game play scene
		virtual void createActorNode(CCDictionary* parameters = nullptr) SL_OVERRIDE;
		virtual void createActorSprite(CCDictionary* parameters = nullptr) SL_VIRTUAL_DECLARE;

		CC_SYNTHESIZE(GameplayLayer*, _gameplayLayer, GameplayLayer)

		//! the actor sprite representing the visual and physics
		CC_SYNTHESIZE(ActorSprite*, _actorSprite, ActorSprite)

		//! retrieve the game actor node
		GameActorNode* getGameActorNode() const;
	protected:
		GameActorComponent();
		virtual ~GameActorComponent();

		//! removes the hosting actor from the game play scene
		virtual void removeFromGameplayLayer() SL_VIRTUAL_DECLARE;

	};

	typedef sltl::TCCSmartPointer<GameActorComponent> GameActorComponentPtr;

}


#endif // __SLGAMEACTORCOMPONENT_H__
