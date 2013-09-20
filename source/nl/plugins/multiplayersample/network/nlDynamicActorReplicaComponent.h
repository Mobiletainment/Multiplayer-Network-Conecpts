//!---------------------------------------------------------------------------
//! \file "nlDynamicActorReplicaComponent.h"
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
#ifndef __NLDYNAMICACTORREPLICACOMPONENT_H__
#define __NLDYNAMICACTORREPLICACOMPONENT_H__

#include <stateless/cocosgame/slCocosGame.h>

#include "nlProtocolStructures.h"

namespace nl	{


	class DynamicActorReplicaComponent : public ReplicaComponent	{
		SL_DECLARE_BASE(ReplicaComponent)
	public:
		CREATE_FUNC(DynamicActorReplicaComponent)

		SL_IMPLEMENT_CLASSNAME(DynamicActorReplicaComponent)

		virtual void preUpdate( float delta ) SL_OVERRIDE;
		virtual void postUpdate( float delta ) SL_OVERRIDE;

		virtual void serializeConstruction(RakNet::BitStream *constructionBitstream) SL_OVERRIDE;
		virtual bool deserializeConstruction(RakNet::BitStream *constructionBitstream) SL_OVERRIDE;

		virtual void serializeDestruction(RakNet::BitStream *destructionBitstream) SL_OVERRIDE;
		virtual bool deserializeDestruction(RakNet::BitStream *destructionBitstream) SL_OVERRIDE;

		virtual void preSerialize() SL_OVERRIDE;

		virtual RakNet::RM3SerializationResult serialize(RakNet::SerializeParameters *serializeParameters) SL_OVERRIDE;
		virtual void deserialize(RakNet::DeserializeParameters *deserializeParameters) SL_OVERRIDE;

		virtual void createActorNode(CCDictionary* parameters = nullptr) SL_OVERRIDE;
		virtual void createActorSprite(CCDictionary* parameters = nullptr) SL_VIRTUAL_DECLARE;

		SL_SYNTHESIZE_PASS_BY_REF(Dynamic2DActorDatagram, _actorDatagram, ActorDatagram)

	protected:
		DynamicActorReplicaComponent();
		virtual ~DynamicActorReplicaComponent();

		virtual void removeFromGameplayLayer() SL_OVERRIDE;

		PhysicsMotionState _motionState;
		LocalSpaceData _localSpaceData;
		AnimationTick _replicationTick;
		SLSize _tickToReplicate;
		SLSize _tickReplicated;
		SLSize _tickSerialized;
		bool _sampleVehicleState;


	private:
	};


}


#endif // __NLTANKPROJECTILEREPLICACOMPONENT_H__
