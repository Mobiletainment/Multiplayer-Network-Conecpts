//!---------------------------------------------------------------------------
//! \file "nlPlayerReplicaComponent.cpp"
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
#include "nlPlayerReplicaComponent.h"

namespace nl	{


	//-------------------------------------------------------------------------
	// PlayerReplicaComponent
	//-------------------------------------------------------------------------

	PlayerReplicaComponent::PlayerReplicaComponent()	{
		_replica.setName(PlayerReplicaComponent::staticClassName());

		// initialize control values
		_ctrlValues._forwardBackward =
			_ctrlValues._leftRight =
			_ctrlValues._shoot = 0.0f;

		_ctrlValues._controlledReplicaNetworkId = UNASSIGNED_NETWORK_ID;
	}

	PlayerReplicaComponent::~PlayerReplicaComponent()	{

	}

	void PlayerReplicaComponent::preUpdate( float delta ) 	{
		SLBaseClass::preUpdate(delta);
		// preUpdate will always be called once per frame
	}

	void PlayerReplicaComponent::postUpdate( float delta ) 	{
		SLBaseClass::postUpdate(delta);
		// postUpdate will always be called once per frame
	}

	void PlayerReplicaComponent::serializeConstruction(RakNet::BitStream *constructionBitstream)	{

	}

	bool PlayerReplicaComponent::deserializeConstruction(RakNet::BitStream *constructionBitstream)	{
		return true;
	}

	void PlayerReplicaComponent::serializeDestruction(RakNet::BitStream *destructionBitstream)	{

	}

	bool PlayerReplicaComponent::deserializeDestruction(RakNet::BitStream *destructionBitstream)	{
		return true;
	}

	// authority code
	void PlayerReplicaComponent::preSerialize()	{
		// TODO: @student : prevent updates from the past
	}

	// authority code
	RakNet::RM3SerializationResult PlayerReplicaComponent::serialize(RakNet::SerializeParameters *serializeParameters)	{
		// TODO: @student : think about compression

		RakNet::BitStream& bitStream(serializeParameters->outputBitstream[0]);
		bitStream.WriteAlignedBytes( (const unsigned char *)&_ctrlValues, sizeof(ControllerValues) );

		return RakNet::RM3SR_SERIALIZED_ALWAYS;
	}

	// receive code
	void PlayerReplicaComponent::deserialize(RakNet::DeserializeParameters *deserializeParameters)	{
		RakNet::BitStream& bitStream(deserializeParameters->serializationBitstream[0]);
		if (bitStream.GetNumberOfBitsUsed()==0)	{
			return;
		}
		bitStream.ReadAlignedBytes( (unsigned char *)&_ctrlValues, sizeof(ControllerValues) );
#if 0
		_replica.getPeer()->log(ELogType_Status, "lr(%.02f) fb(%.02f) fire(%.02f)", 
			_ctrlValues._leftRight, 
			_ctrlValues._forwardBackward, 
			_ctrlValues._shoot);
#endif
	}

}


