//!---------------------------------------------------------------------------
//! \file "nlTankVsTankGameplayLayer.cpp"
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
#include "nlTankVsTankGameplayLayer.h"
#include "network/nlGameContentReplicaManager.h"
#include "network/nlLocalPlayerReplicaComponent.h"

namespace nl	{

	TankVsTankGameplayLayer::TankVsTankGameplayLayer()		
	{
	}

	TankVsTankGameplayLayer::~TankVsTankGameplayLayer()
	{
	}

	bool TankVsTankGameplayLayer::init()	{
		bool initialized(SLBaseClass::init());
		return initialized;
	}

	void TankVsTankGameplayLayer::visit()	{
		SLBaseClass::visit();
	}

	void TankVsTankGameplayLayer::draw()
	{
		SLBaseClass::draw();
	}

	void TankVsTankGameplayLayer::update( float dt )	{
		SLBaseClass::update(dt);
	}

	void TankVsTankGameplayLayer::onEnter()	{
		SLBaseClass::onEnter();
	}

	void TankVsTankGameplayLayer::onExit()	{
		SLBaseClass::onExit();
	}

	void TankVsTankGameplayLayer::dispatchActorSpriteSinglePlayerAction( ActorSprite* actorSprite )	{
		// a test hoc you might implement to prepare game play functionality
	}

}


