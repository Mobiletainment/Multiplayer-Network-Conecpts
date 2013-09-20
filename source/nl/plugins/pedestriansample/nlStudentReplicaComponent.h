//!---------------------------------------------------------------------------
//! \file "nlStudentReplicaComponent.h"
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
#ifndef __NLSTUDENTREPLICACOMPONENT_H__
#define __NLSTUDENTREPLICACOMPONENT_H__

#include <stateless/cocosgame/slCocosGame.h>


namespace nl	{


	class StudentReplicaComponent : public ReplicaComponent	{
		SL_DECLARE_BASE(ReplicaComponent)
	public:
		CREATE_FUNC(StudentReplicaComponent)

		virtual void preUpdate( float delta ) SL_OVERRIDE;
		virtual void postUpdate( float delta ) SL_OVERRIDE;

		virtual void serializeConstruction(RakNet::BitStream *constructionBitstream);
		virtual bool deserializeConstruction(RakNet::BitStream *constructionBitstream);

		virtual void serializeDestruction(RakNet::BitStream *destructionBitstream);
		virtual bool deserializeDestruction(RakNet::BitStream *destructionBitstream);

		virtual void preSerialize() SL_OVERRIDE;

		virtual RakNet::RM3SerializationResult serialize(RakNet::SerializeParameters *serializeParameters) SL_OVERRIDE;
		virtual void deserialize(RakNet::DeserializeParameters *deserializeParameters) SL_OVERRIDE;

		virtual void createActorNode(CCDictionary* parameters = nullptr) SL_OVERRIDE;
		virtual void createActorSprite(CCDictionary* parameters = nullptr) SL_VIRTUAL_DECLARE;

		CC_SYNTHESIZE_RETAIN(CCString*, _jsonObject, JSONObject)

	protected:
		StudentReplicaComponent();
		virtual ~StudentReplicaComponent();

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


#endif // __NLSTUDENTREPLICACOMPONENT_H__
