//!---------------------------------------------------------------------------
//! \file "slGameActor.h"
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
#ifndef __SLGAMEACTOR_H__
#define __SLGAMEACTOR_H__

// definition of additional class ids

#define SL_CID_PHYSICSVEHICLE static_cast<OS_CID>(OS_CID__LASTRESERVED + 1)
#define SL_CID_PEDESTRIAN static_cast<OS_CID>(OS_CID__LASTRESERVED + 2)
#define SL_CID_TANK static_cast<OS_CID>(OS_CID__LASTRESERVED + 3)
#define SL_CID_TANK_PROCECTILE static_cast<OS_CID>(OS_CID__LASTRESERVED + 4)

namespace sl	{

	class GameplayLayer;
	class GameActorNode;


	//! @brief the root class for all actors
	//!
	class slCocosGame_API GameActor : public AbstractActor	{
		SL_DECLARE_BASE(AbstractActor)
	public:

		virtual bool init() SL_OVERRIDE;

		virtual void onEnter() SL_OVERRIDE;
		virtual void onExit() SL_OVERRIDE;

		// CCTouchDelegate interface touch events
		virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
		virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

		void createOpenSteerEntity(EntityClassId classId);

		GameActorNode* getGameActorNode() const;

		virtual GameplayLayer* getGameplayLayer() const SL_ABSTRACT;

		CC_SYNTHESIZE(AbstractEntity*, _entity, Entity)
		CC_SYNTHESIZE_READONLY_PASS_BY_REF(bool, _ownsEntity, OwnsEntity)

		static AbstractEntityFactory& sharedOpensteerEntityFactory();

	protected:
		GameActor();
		virtual ~GameActor();
	private:

	};

	//! @brief StaticActor is never moving
	//!
	class slCocosGame_API StaticActor : public GameActor	{
		SL_DECLARE_BASE(GameActor)
	public:

	};

	//! @brief DynamicActor is potentially moving
	//!
	class slCocosGame_API DynamicActor : public GameActor	{
		SL_DECLARE_BASE(GameActor)
	public:

	};

	//! @brief VehicleActor is a dynamic actor driven by an opensteer vehicle
	//!
	class slCocosGame_API VehicleActor : public DynamicActor	{
		SL_DECLARE_BASE(DynamicActor)
	public:

		// CCNode interface
		virtual void draw() SL_OVERRIDE;

		virtual void onEnter() SL_OVERRIDE;
		virtual void onExit() SL_OVERRIDE;

		AbstractVehicle* getVehicle() const;
	private:
		void drawVehicle();

	};




}


#endif // __SLGAMEACTOR_H__
