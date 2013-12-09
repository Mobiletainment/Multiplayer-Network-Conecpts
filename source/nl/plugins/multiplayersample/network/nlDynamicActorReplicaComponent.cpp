//!---------------------------------------------------------------------------
//! \file "nlDynamicActorReplicaComponent.cpp"
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
#include "nlDynamicActorReplicaComponent.h"
#include "nlReplicationRules.h"

#define NL_DYNAMICACTORREPLICA_HAS_SERIALIZE_LOG 0
#define NL_DYNAMICACTOREPLICA_HAS_SERIALIZE_CONSTRUCTION_LOG 0
#define NL_DYNAMICACTOREPLICA_HAS_REMOVEFROMGAMEPLAYLAYER_LOG 0

namespace nl	{


	DynamicActorReplicaComponent::DynamicActorReplicaComponent()
		:_tickToReplicate(0)
		,_tickReplicated(0)
		,_sampleVehicleState(false)
	{
		_replica.setName(DynamicActorReplicaComponent::staticClassName());

		ServerAuthorityReplicationRule* replicationRule(ServerAuthorityReplicationRule::create());
		replicationRule->_replica = getReplica();
		_replica.setReplicationRule(replicationRule);

		_replicationTick.setAnimationFrequency(5);
	}

	DynamicActorReplicaComponent::~DynamicActorReplicaComponent()
	{
	}

	void DynamicActorReplicaComponent::preUpdate( float delta ) 
	{
		// preUpdate will always be called once per frame
		SLBaseClass::preUpdate(delta);

		// so here we can control the replication frequency
		if(_tickSerialized == _tickToReplicate)	{
			_tickReplicated = _tickToReplicate;
		}

		bool animationTickChanged(false);
		SLSize ticks(_replicationTick.update( delta, animationTickChanged ));
		if(animationTickChanged)	{
			if( _tickReplicated == _tickToReplicate )	{
				_tickToReplicate = ticks;
				if(getTopology() == SERVER)	{
					_sampleVehicleState = true;
#if NL_DYNAMICACTORREPLICA_HAS_SERIALIZE_LOG
					getReplica()->getPeer()->log(ELogType_Info, "preUpdate replicate %d", _tickToReplicate);
#endif
				}
			}
		}
	}

	void DynamicActorReplicaComponent::postUpdate( float delta ) 
	{
		// server / authority code

		if(getTopology() == SERVER)	{
			// this boolean is dependent from the replication frequency
			if(_sampleVehicleState)	{
				if(getActorSprite() == nullptr)	{
					return;
				}
				// TODO @student : the vehicle has a function updateTicks
				//                 you could add this to the datagram ...
				// TODO @student : compressed datagram
				// TODO @student : extended datagram
				AbstractVehicle* vehicle(getActorSprite()->getVehicle());
				if(vehicle != nullptr)	{
					_localSpaceData = vehicle->getLocalSpaceData();
					_motionState.readLocalSpaceData(_localSpaceData);

					_actorDatagram._x = _localSpaceData._position.x;
					_actorDatagram._y = _localSpaceData._position.y;

					_actorDatagram._fx = _localSpaceData._forward.x;
					_actorDatagram._fy = _localSpaceData._forward.y;

					_actorDatagram._lvx = _localSpaceData._linearVelocity.x;
					_actorDatagram._lvy = _localSpaceData._linearVelocity.y;

					_actorDatagram._avz = _localSpaceData._angularVelocity.z;
				}
				_sampleVehicleState = false;
			}
		}

		SLBaseClass::postUpdate(delta);
	}

	// server / authority code
	void DynamicActorReplicaComponent::serializeConstruction(RakNet::BitStream *constructionBitstream)	{
		CCDictionary* constructionDictionary(getConstructionDictionary());
		if(constructionDictionary != nullptr)	{
			// the vehicle position and orientation might have changed
			// so we need to perform an update here
			AbstractVehicle* vehicle(getActorSprite()->getVehicle());
			if(vehicle != nullptr)	{
				// TODO @student : modify the construction dictionary to spawn at the right place, with the right direction and right initial velocities
				
			}
			CCString* constructionJSON = CCJSONConverter::strFrom(constructionDictionary);
			constructionBitstream->Write(constructionJSON->getCString());

			getReplica()->getPeer()->log(ELogType_Info, "%s - Dictionary", constructionJSON->getCString());	

		}
	}

	// client / slave code
	bool DynamicActorReplicaComponent::deserializeConstruction(RakNet::BitStream *constructionBitstream)	{
		RakNet::BitStream& bitStream(*constructionBitstream);
		if (bitStream.GetNumberOfBitsUsed()==0)
			return false;

		RakNet::RakString rakString;
		bitStream.Read(rakString);

#if NL_DYNAMICACTOREPLICA_HAS_SERIALIZE_CONSTRUCTION_LOG
		getReplica()->getPeer()->log(ELogType_Info, "deserializeConstruction %s ", rakString.C_String());
#endif

		setConstructionDictionary(CCJSONConverter::dictionaryFrom(rakString.C_String()));
		createActorSprite(getConstructionDictionary());
		return true;
	}

	// server / authority code
	void DynamicActorReplicaComponent::serializeDestruction(RakNet::BitStream *destructionBitstream)	{
	}

	// client / slave code
	bool DynamicActorReplicaComponent::deserializeDestruction(RakNet::BitStream *destructionBitstream)	{
		return true;
	}

	// server / authority code
	void DynamicActorReplicaComponent::preSerialize()
	{
	}

	// TODO @student : compressed datagram

	// server / authority code
	RakNet::RM3SerializationResult DynamicActorReplicaComponent::serialize(RakNet::SerializeParameters *serializeParameters)
	{
		// note: take a look at preUpdate to understand how this works
		if(_tickReplicated == _tickToReplicate)	{
			return RakNet::RM3SR_DO_NOT_SERIALIZE;	
		}
		_tickSerialized = _tickToReplicate;
#if NL_DYNAMICACTORREPLICA_HAS_SERIALIZE_LOG
		getReplica()->getPeer()->log(ELogType_Info, "serialize %d ", _tickToReplicate);
#endif

		RakNet::BitStream& bitStream(serializeParameters->outputBitstream[0]);
		bitStream.WriteAlignedBytes( (const unsigned char *)&_actorDatagram, sizeof(Dynamic2DActorDatagram) );

		return RakNet::RM3SR_SERIALIZED_ALWAYS_IDENTICALLY;
	}

	// client / slave code
	void DynamicActorReplicaComponent::deserialize(RakNet::DeserializeParameters *deserializeParameters)
	{
		// a little base class magic
		SLBaseClass::deserialize(deserializeParameters);

		RakNet::BitStream& bitStream(deserializeParameters->serializationBitstream[0]);
		if (bitStream.GetNumberOfBitsUsed()==0)	{
			return;
		}

		bitStream.ReadAlignedBytes( (unsigned char *)&_actorDatagram, sizeof(Dynamic2DActorDatagram) );

		if(getActorSprite() == nullptr)	{
			return;
		}

		AbstractVehicle* vehicle(getActorSprite()->getVehicle());
		if(vehicle != nullptr)	{

			Vec3 serverPosition(
				_actorDatagram._x,
				_actorDatagram._y,
				0.0f
				);

			Vec3 serverForward(
				_actorDatagram._fx,
				_actorDatagram._fy,
				0.0f
				);

			Vec3 serverLinearVelocity(
				_actorDatagram._lvx,
				_actorDatagram._lvy,
				0.0f
				);

			Vec3 serverAngularVelocity(
				0.0f,
				0.0f,
				_actorDatagram._avz
				);

			// TODO @student : client side interpolation factor UI
			// TODO @student : move interpolarion code to the right place

			float linearInterpolationFactor(0.75f);

			Vec3 newPosition(ProtocolUtilities::interpolateNetVector(vehicle->position(),
				serverPosition, linearInterpolationFactor));

			Vec3 newForward(ProtocolUtilities::interpolateNetVector(vehicle->forward(),
				serverForward, linearInterpolationFactor));

			Vec3 newLinearVelocity(ProtocolUtilities::interpolateNetVector(vehicle->linearVelocity(),
				serverLinearVelocity, linearInterpolationFactor));

			Vec3 newAngularVelocity(ProtocolUtilities::interpolateNetVector(vehicle->angularVelocity(),
				serverAngularVelocity, linearInterpolationFactor));

			vehicle->setPosition(newPosition);
			vehicle->regenerateLocalSpace(newForward, 0.0f);

			vehicle->setLinearVelocity(newLinearVelocity);
			vehicle->setAngularVelocity(newAngularVelocity);

			// very important to notify the vehicle that 
			// it's internal state has been changed from the outside
			// see PhysicsVehicle::update
			vehicle->setDirty();

			_localSpaceData = vehicle->getLocalSpaceData();
			_motionState.readLocalSpaceData(_localSpaceData);
		}
	}

	void DynamicActorReplicaComponent::createActorNode(CCDictionary* parameters)	{
		// just a debug entry
		SLBaseClass::createActorNode(parameters);
	}

	void DynamicActorReplicaComponent::createActorSprite(CCDictionary* parameters)	{
		// just a debug entry
		SLBaseClass::createActorSprite(parameters);
	}

	void DynamicActorReplicaComponent::removeFromGameplayLayer()	{
#if NL_DYNAMICACTOREPLICA_HAS_REMOVEFROMGAMEPLAYLAYER_LOG
		if(getReplica())	{
			if(getReplica()->getPeer())	{
				getReplica()->getPeer()->log(ELogType_Info, ">> REPLICA CLEANUP removeFromGameplayLayer %s ", getName());
			}
		}
#endif
		// just a debug entry
		SLBaseClass::removeFromGameplayLayer();
	}

}


