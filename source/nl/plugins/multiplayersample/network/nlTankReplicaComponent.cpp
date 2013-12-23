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
		, _labelName(nullptr)
	{
		_replica.setName(TankReplicaComponent::staticClassName());
		_replicationTick.setAnimationFrequency(5);
		setTankName(new CCString("-1"));
	}

	TankReplicaComponent::~TankReplicaComponent()	{
	}

	// preUpdate will always be called once per frame
	void TankReplicaComponent::preUpdate( float delta )
	{
		ActorSprite* actorSprite(getActorSprite());
		if(actorSprite != nullptr)
		{	
			if (_labelKillCount == nullptr) //if the kill-count label hasn't been created yet, create it
			{
				CCPoint labelPosition(0, -10); //we don't want the label to directly stick on the tank
				_labelKillCount = dynamic_cast<CCLabelTTF*>( ControlUtils::createLabel("Name: x, Killcount: 0", kCCTextAlignmentLeft) );
				_labelKillCount->setAnchorPoint(ccp(0.5f, 0.5f));
				_labelKillCount->setPosition(labelPosition);
				_labelKillCount->setVisible(true);
				setKillCount(0);
				actorSprite->addChild(_labelKillCount); //add the label as Child of the actor
				
			}

			//Update the kill-count label to displyy the current kill-count
			//maintain readability of the kill-count label by keeping it's direction horizontal when rotating the tank
			float rotateHorizontally = 360 - actorSprite->getRotation();
			_labelKillCount->setRotation(rotateHorizontally);
			
			
			actorSprite->getActorFlags().removeFlag(EActorFlag_DrawVehicle);
			actorSprite->getActorFlags().addFlag(EActorFlag_IsTank);
		}

		if (this->getTopology() == CLIENT)
		{
			int i = 0;
		}

		SLBaseClass::preUpdate(delta);
	}

	void TankReplicaComponent::increaseKillCount()
	{
		setKillCount(_killCount + 1);
	}

	void TankReplicaComponent::setKillCount(int newKillCount)
	{
		_killCount = newKillCount;

		//Update the label here instead of the update-Method, so it just gets updated when its content changes
		_labelKillCount->setString(CCString::createWithFormat("Tank: %i, Kills: %i", this->getName(), _killCount)->getCString());
	}

	void TankReplicaComponent::postUpdate( float delta )
	{	
		// server / authority code
		if(getTopology() == SERVER)	{
		}
		SLBaseClass::postUpdate(delta);
	}

	// server / authority code
	void TankReplicaComponent::preSerialize()	{
	}

	// server / authority code
	RakNet::RM3SerializationResult TankReplicaComponent::serialize(RakNet::SerializeParameters *serializeParameters)	{
		if (getTopology()==CLIENT)	{
			return RM3SR_DO_NOT_SERIALIZE;
		}
		return DynamicActorReplicaComponent::serialize(serializeParameters);
	}

	// client / receive code
	void TankReplicaComponent::deserialize(RakNet::DeserializeParameters *deserializeParameters)
	{
		
		DynamicActorReplicaComponent::deserialize(deserializeParameters);
		/*
#if 0 // massive log spam if you like
		_replica.getPeer()->log(ELogType_Status, "lr(%.02f) fb(%.02f) fire(%.02f)", 
			_ctrlValues._leftRight, 
			_ctrlValues._forwardBackward, 
			_ctrlValues._shoot);
#endif
			*/
	}
}


