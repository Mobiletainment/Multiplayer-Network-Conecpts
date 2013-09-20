//!---------------------------------------------------------------------------
//! \file "nlTankVsTankGameplayLayer.h"
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
#ifndef __NLTANKVSTANKGAMEPLAYLAYER_H__
#define __NLTANKVSTANKGAMEPLAYLAYER_H__

#include <stateless/cocosgame/slCocosGame.h>

namespace OpenSteer	{
	class Plugin;
}

namespace nl	{

	class TankVsTankGameplayLayer : public GameplayLayer	{
		SL_DECLARE_BASE(GameplayLayer)
	public:
		CREATE_FUNC(TankVsTankGameplayLayer)

		SL_IMPLEMENT_CLASSNAME(TankVsTankGameplayLayer)

		virtual bool init() SL_OVERRIDE;

		virtual void update(float dt) SL_OVERRIDE;

		virtual void onEnter() SL_OVERRIDE;
		virtual void onExit() SL_OVERRIDE;

		virtual void visit() SL_OVERRIDE;
		virtual void draw() SL_OVERRIDE;

		virtual void dispatchActorSpriteSinglePlayerAction( ActorSprite* actorSprite ) SL_OVERRIDE;

	protected:
		TankVsTankGameplayLayer();
		virtual ~TankVsTankGameplayLayer();


	private:
	};



}


#endif // __NLTANKVSTANKGAMEPLAYLAYER_H__
