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
	TankReplicaComponent::TankReplicaComponent() : _isSpectator(false)
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
			actorSprite->getActorFlags().removeFlag(EActorFlag_DrawVehicle);
			actorSprite->getActorFlags().addFlag(EActorFlag_IsTank);
		}
		SLBaseClass::preUpdate(delta);
	}

	void TankReplicaComponent::postUpdate( float delta ) 	{
		// server / authority code
		if(getTopology() == SERVER)	{
		}
		SLBaseClass::postUpdate(delta);
	}
}


