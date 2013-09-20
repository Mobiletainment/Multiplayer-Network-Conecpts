//!---------------------------------------------------------------------------
//! \file "nlPlayerReplicaComponent.h"
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
#ifndef __SLPLAYERREPLICACOMPONENT_H__
#define __SLPLAYERREPLICACOMPONENT_H__

#include <stateless/cocosgame/slCocosGame.h>

#include "nlProtocolStructures.h"

namespace sl	{

	class GameplayLayer;
	class ActorSprite;
	class GameActorNode;
}

namespace nl	{



	class PlayerReplicaComponent : public ReplicaComponent	{
		SL_DECLARE_BASE(ReplicaComponent)
	public:

		CREATE_FUNC(PlayerReplicaComponent)

		SL_IMPLEMENT_CLASSNAME(PlayerReplicaComponent)

		virtual void preUpdate( float delta ) SL_OVERRIDE;
		virtual void postUpdate( float delta ) SL_OVERRIDE;

		virtual void serializeConstruction(RakNet::BitStream *constructionBitstream) SL_OVERRIDE;
		virtual bool deserializeConstruction(RakNet::BitStream *constructionBitstream) SL_OVERRIDE;

		virtual void serializeDestruction(RakNet::BitStream *destructionBitstream) SL_OVERRIDE;
		virtual bool deserializeDestruction(RakNet::BitStream *destructionBitstream) SL_OVERRIDE;

		virtual void preSerialize() SL_OVERRIDE;

		virtual RakNet::RM3SerializationResult serialize(RakNet::SerializeParameters *serializeParameters) SL_OVERRIDE;
		virtual void deserialize(RakNet::DeserializeParameters *deserializeParameters) SL_OVERRIDE;

		SL_SYNTHESIZE_GET_BY_REF(Player, _player, Player)
		SL_SYNTHESIZE_PASS_BY_REF(ControllerValues, _ctrlValues, ControllerValues)

	protected:
		PlayerReplicaComponent();
		virtual ~PlayerReplicaComponent();

	private:
	};

}


#endif // __SLPLAYERREPLICACOMPONENT_H__
