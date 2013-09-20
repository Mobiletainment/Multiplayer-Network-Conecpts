//!---------------------------------------------------------------------------
//! \file "nlLocalPlayerReplicaComponent.h"
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
#ifndef __SLLOCALPLAYERREPLICACOMPONENT_H__
#define __SLLOCALPLAYERREPLICACOMPONENT_H__

#include "nlPlayerReplicaComponent.h"
#include "nlTankPlayerReplicaComponent.h"

namespace nl	{

	class LocalPlayerReplicaComponent : public PlayerReplicaComponent	{
		SL_DECLARE_BASE(PlayerReplicaComponent)
	public:
		CREATE_FUNC(LocalPlayerReplicaComponent)

		SL_IMPLEMENT_CLASSNAME(LocalPlayerReplicaComponent)

		virtual void preUpdate( float delta ) SL_OVERRIDE;
		virtual void postUpdate( float delta ) SL_OVERRIDE;

		virtual void serializeConstruction(RakNet::BitStream *constructionBitstream) SL_OVERRIDE;
		virtual bool deserializeConstruction(RakNet::BitStream *constructionBitstream) SL_OVERRIDE;

		virtual void serializeDestruction(RakNet::BitStream *destructionBitstream) SL_OVERRIDE;
		virtual bool deserializeDestruction(RakNet::BitStream *destructionBitstream) SL_OVERRIDE;

		virtual void preSerialize() SL_OVERRIDE;

		virtual RakNet::RM3SerializationResult serialize(RakNet::SerializeParameters *serializeParameters) SL_OVERRIDE;
		virtual void deserialize(RakNet::DeserializeParameters *deserializeParameters) SL_OVERRIDE;

		SL_SYNTHESIZE_RETAIN(TankPlayerReplicaComponent*, _serverPlayerReplicaComponent, TankPlayerReplicaComponent);

	protected:
		LocalPlayerReplicaComponent();
		virtual ~LocalPlayerReplicaComponent();
	private:
		bool _createServerPlayerReplica;
		AnimationTick _physicsTick;
	};

}


#endif // __SLLOCALPLAYERREPLICACOMPONENT_H__
