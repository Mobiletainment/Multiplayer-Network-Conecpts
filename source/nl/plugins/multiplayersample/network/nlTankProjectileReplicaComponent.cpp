//!---------------------------------------------------------------------------
//! \file "nlTankProjectileReplicaComponent.cpp"
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
#include "nlTankProjectileReplicaComponent.h"
#include "nlReplicationRules.h"

namespace nl	{

	TankProjectileReplicaComponent::TankProjectileReplicaComponent()
	{
		_replica.setName(TankProjectileReplicaComponent::staticClassName());
		printf("TankProjectile Instantiated\n");
		_replicationTick.setAnimationFrequency(500);
		_lifeTimeTick.setAnimationInterval(500);
	}

	TankProjectileReplicaComponent::~TankProjectileReplicaComponent()	{
	}

	// preUpdate will always be called once per frame
	void TankProjectileReplicaComponent::preUpdate( float delta ) 
	{
		// note: the player does not need to be updated
		//       as the controller values do not need to change
		//       for a guided missile it would be a little different

		ActorSprite* actorSprite(getActorSprite());
		if(actorSprite != nullptr)	{
			actorSprite->getActorFlags().removeFlag(EActorFlag_DrawVehicle);
			actorSprite->getActorFlags().addFlag(EActorFlag_IsTankProjectile);
			// check if the component player is playing the actors sprite vehicle
			if(_player.isPlaying() == false)	{
				_player.accessController()->setActionValue(EControllerAction_Move, 1.0f);
				_player.play(actorSprite->getVehicle());
			}
		}

		SLBaseClass::preUpdate(delta);
	}

	void TankProjectileReplicaComponent::postUpdate( float delta ) 	{
		// server / authority code
		_lifeTimeTick.update(delta);
		if(_lifeTimeTick.getAnimationTicks() > 0)	{
			if(getTopology() == SERVER)	{
				getActorSprite()->getActorFlags().addFlag(EActorFlag_IsDestroyed);
			}
			else if(getTopology() == SINGLEPLAYER)	{
				getActorSprite()->getActorFlags().addFlag(EActorFlag_IsDestroyed);
			}
			// nothing on the client
			// the server will send the destruction
		}

		SLBaseClass::postUpdate(delta);
	}

	RakNet::RM3SerializationResult TankProjectileReplicaComponent::serialize(RakNet::SerializeParameters *serializeParameters)
	{	
		return RM3SR_DO_NOT_SERIALIZE;	
	}

	// client / receive code
	void TankProjectileReplicaComponent::deserialize(RakNet::DeserializeParameters *deserializeParameters)
	{
		SL_ASSERT("Error: Deserializing TankProjectileReplica. Something went wrong, because serialization/deserialization of TankProjectileReplica should not be triggered!");
	}
}


