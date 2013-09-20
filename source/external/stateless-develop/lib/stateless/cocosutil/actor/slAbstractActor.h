//!---------------------------------------------------------------------------
//! \file "slAbstractActor.h"
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
#ifndef __SLABSTRACTACTOR_H__
#define __SLABSTRACTACTOR_H__

namespace OpenSteer	{
	class AbstractEntity;
}


namespace sl	{
	using namespace OpenSteer;

	class GameplayLayer;
	class ActorNode;

	enum EActorFlags
	{
		EActorFlag_Undefined = 0,
		EActorFlag_DrawVehicle = 1 << 0,
		EActorFlag_DrawText = 1 << 1,
		EActorFlag_IsDestroyed = 1 << 2,
		EActorFlag_IsTank = 1 << 3,
		EActorFlag_IsTankProjectile = 1 << 4,
		EActorFlag_000 = 1 << 5,
		EActorFlag_001 = 1 << 6,
		EActorFlag_002 = 1 << 7,
		EActorFlag_003 = 1 << 8,
		EActorFlag_004 = 1 << 9,
		EActorFlag_005 = 1 << 10,
		EActorFlag_006 = 1 << 11,
		EActorFlag_007 = 1 << 12,
		EActorFlag_008 = 1 << 13,
		EActorFlag_009 = 1 << 14,
		EActorFlag_010 = 1 << 15,
	};

#pragma warning(push)
#pragma warning(disable:4251) // needs to have dll-interface to be used by clients of class
	class slCocosUtil_API ActorFlags0	{

	};

	class slCocosUtil_API ActorFlags : public sltl::FlagSetMixin<ActorFlags0, EActorFlags>	{

	};

#pragma warning(pop)

	//! @brief the root class for all actors
	//!
	class slCocosUtil_API AbstractActor : public CCPhysicsSprite, public CCTouchDelegate	{
		SL_DECLARE_BASE(CCPhysicsSprite)
	public:

		virtual bool init() SL_OVERRIDE;

		virtual void onEnter() SL_OVERRIDE;
		virtual void onExit() SL_OVERRIDE;

		// CCTouchDelegate interface touch events
		virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) SL_OVERRIDE;
		virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) SL_OVERRIDE;

		void setTouchEnabled(bool value);

		CCPoint getTouchLocation(CCTouch* touch);

		bool isTouchInside(CCTouch* touch);

		ActorNode* getActorNode() const;

		CC_SYNTHESIZE_RETAIN(CCDictionary*, _constructionDictionary, ConstructionDictionary)
		SL_SYNTHESIZE_GET_BY_REF(ActorFlags, _actorFlags, ActorFlags)

		//! the instigator is the actor instigating the existence for this actor
		//! so in case of a projectile it would be the projectile creator
		CC_SYNTHESIZE_RETAIN(AbstractActor*, _instigator, Instigator)

	protected:
		AbstractActor();
		virtual ~AbstractActor();
	private:

	};





}


#endif // __SLABSTRACTACTOR_H__
