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

#include "slCompressed.h"
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
		_ctrlValues._killCount = 0;
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

	void PlayerReplicaComponent::serializeConstruction(RakNet::BitStream *constructionBitstream)
	{
		
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
		// TODO @student : prevent updates from the past
	}

	// authority code
	RakNet::RM3SerializationResult PlayerReplicaComponent::serialize(RakNet::SerializeParameters *serializeParameters)	{
		// TODO @student : think about compression and implement a minimal version for these control values

		RakNet::BitStream& bitStream(serializeParameters->outputBitstream[0]);

		//OLD WAY:
		//bitStream.WriteAlignedBytes( (const unsigned char *)&_ctrlValues, sizeof(ControllerValues) );
		//NEW
		
		Compressed_ControllerValues comValues;
		comValues._forwardBackward	=	TCompressedFixpoint<float,char,8>::writeCompress(_ctrlValues._forwardBackward	, -1.0f, 1.0f );
		comValues._leftRight		=	TCompressedFixpoint<float,char,8>::writeCompress(_ctrlValues._leftRight			, -1.0f, 1.0f );
		comValues._shoot			=	TCompressedFixpoint<float,char,8>::writeCompress(_ctrlValues._shoot				, -1.0f, 1.0f );
		comValues._killCount		=	_ctrlValues._killCount;

		comValues._updateTick = _ctrlValues._updateTick;
		comValues._controlledReplicaNetworkId = _ctrlValues._controlledReplicaNetworkId;
		
		if(getTopology() == SERVER) 
		{
			//_replica.getPeer()->log(ELogType_Status, "Server %d", _ctrlValues._updateTick);

		}
		
		bitStream.WriteAlignedBytes( (const unsigned char *)&comValues, sizeof(Compressed_ControllerValues));
		
		return RakNet::RM3SR_SERIALIZED_ALWAYS;
	}

	// receive code
	void PlayerReplicaComponent::deserialize(RakNet::DeserializeParameters *deserializeParameters)	{
		RakNet::BitStream& bitStream(deserializeParameters->serializationBitstream[0]);
		if (bitStream.GetNumberOfBitsUsed()==0)	{
			return;
		}
		
		//NEW
		Compressed_ControllerValues comValues;
		bitStream.ReadAlignedBytes( (unsigned char *)&comValues, sizeof(Compressed_ControllerValues) );
		
		_ctrlValues._forwardBackward			=TCompressedFixpoint<float,char,8>::readInflate(comValues._forwardBackward				, -1.0f, 1.0f );
		_ctrlValues._leftRight					=TCompressedFixpoint<float,char,8>::readInflate(comValues._leftRight					, -1.0f, 1.0f );
		_ctrlValues._shoot						=TCompressedFixpoint<float,char,8>::readInflate(comValues._shoot						, -1.0f, 1.0f );
		_ctrlValues._killCount					=comValues._killCount;
		_ctrlValues._updateTick = comValues._updateTick;
		_ctrlValues._controlledReplicaNetworkId =comValues._controlledReplicaNetworkId;
		
		//OLD:
		//bitStream.ReadAlignedBytes( (unsigned char *)&_ctrlValues, sizeof(ControllerValues) );
#if 1
		if(_ctrlValues._leftRight != 0.0f)	{
		_replica.getPeer()->log(ELogType_Status, "lr(%.02f) fb(%.02f) fire(%.02f) killCount(%0.02f)", 
			_ctrlValues._leftRight, 
			_ctrlValues._forwardBackward, 
			_ctrlValues._shoot,
			_ctrlValues._killCount);
		}
#endif
	}

}


