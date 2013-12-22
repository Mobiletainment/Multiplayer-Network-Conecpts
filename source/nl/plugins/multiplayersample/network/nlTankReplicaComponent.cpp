//!---------------------------------------------------------------------------
//! \file "nlTankReplicaComponent.cpp"
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
#include "nlTankReplicaComponent.h"
#include "nlReplicationRules.h"

namespace nl	{
	TankReplicaComponent::TankReplicaComponent()
	: _labelKillCount(nullptr)
	, _killCount(0)
	{
		_replica.setName(TankReplicaComponent::staticClassName());
		_replicationTick.setAnimationFrequency(5);
	}

	TankReplicaComponent::~TankReplicaComponent()	{
	}

	// preUpdate will always be called once per frame
	void TankReplicaComponent::preUpdate( float delta ) 	{
		ActorSprite* actorSprite(getActorSprite());
		if(actorSprite != nullptr)	{
			
			if (_labelKillCount == nullptr) //if the kill-count label hasn't been created yet, create it
			{
				_labelKillCount = dynamic_cast<CCLabelTTF*>( ControlUtils::createLabel("Tank: 0", kCCTextAlignmentLeft) );
				_labelKillCount->setAnchorPoint(ccp(0.5f, 0.5f));
				CCPoint labelPosition(0, -10);
				_labelKillCount->setPosition(labelPosition);
				_labelKillCount->setVisible(true);
				
				actorSprite->addChild(_labelKillCount, 0, 54322);
			}

			//Update the kill-count label to displyy the current kill-count
			//maintain readability of the kill-count label by keeping it's direction horizontal when rotating the tank
			_labelKillCount->setRotation(360 - actorSprite->getRotation());
			//increaseKillCount();
			

			actorSprite->getActorFlags().removeFlag(EActorFlag_DrawVehicle);
			actorSprite->getActorFlags().addFlag(EActorFlag_IsTank);
		}
		SLBaseClass::preUpdate(delta);
	}

	void TankReplicaComponent::increaseKillCount()
	{
		++_killCount;
		
	}

	void TankReplicaComponent::setKillCount(int newKillCount)
	{
		_killCount = newKillCount;
		_labelKillCount->setString(CCString::createWithFormat("Killcount Tank: %i", _killCount)->getCString());
	}

	void TankReplicaComponent::postUpdate( float delta )
	{	
		// server / authority code
		if(getTopology() == SERVER)	{
		}
		SLBaseClass::postUpdate(delta);
	}
}


