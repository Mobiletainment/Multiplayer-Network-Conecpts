//!---------------------------------------------------------------------------
//! \file "slGameActorNode.h"
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
#ifndef __SLGAMEACTORNODE_H__
#define __SLGAMEACTORNODE_H__


namespace sl	{

	class ActorSprite;
	class GameplayLayer;




	//! @brief a GameActorNode is an object hosting at least one visual GameActor and a Component array
	//!
	//! The GameActorNode allows access to the most relevant game objects
	class slCocosGame_API GameActorNode : public ActorNode
	{
		SL_DECLARE_BASE(ActorNode)
	public:
		static GameActorNode* create(CCDictionary* dictionary);

		//! initialize with a dictionary
		virtual bool initWithDictionary(CCDictionary* dictionary) SL_VIRTUAL_DECLARE;

		//! creates an ActorSprite based on the passed dictionary and adds it to this node
		//! 
		//! previously generated actor sprites will be removed from this node
		virtual bool createActorSpriteWithDictionary(CCDictionary* dictionary) SL_VIRTUAL_DECLARE;

		//! retrieve the GameplayLayer this GameActorNode has been added to
		GameplayLayer* getGameplayLayer() const;

		virtual bool isDestroyed() const SL_OVERRIDE;

		//! the actor components array
		//! 
		//! do not directly add components to this array
		//! use the ActorComponents member function addToActorNode instead
		CC_SYNTHESIZE_READONLY(ActorSprite*, _actorSprite, ActorSprite);

	protected:
		GameActorNode();
		virtual ~GameActorNode();

	private:

	};

}


#endif // __SLAMEACTORNODE_H__
