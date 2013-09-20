//!---------------------------------------------------------------------------
//! \file "nlStudentReplicaComponent.cpp"
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
#include "nlStudentReplicaComponent.h"

// TODO: @student: general: search for SetAutoSerializeInterval

#define NL_STUDENTREPLICA_HAS_SERIALIZE_LOG 0

namespace nl	{


	StudentReplicaComponent::StudentReplicaComponent()
		:_tickToReplicate(0)
		,_tickReplicated(0)
		,_jsonObject(nullptr)
		,_sampleVehicleState(false)
	{
		_replicationTick.setAnimationFrequency(5);
	}

	StudentReplicaComponent::~StudentReplicaComponent()
	{
		setConstructionDictionary(nullptr);
		setJSONObject(nullptr);
	}

	void StudentReplicaComponent::preUpdate( float delta ) 
	{
		SLBaseClass::preUpdate(delta);
		// preUpdate will always be called once per frame
		// so here you can control the replication frequency

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
#if NL_STUDENTREPLICA_HAS_SERIALIZE_LOG
					getReplica()->getPeer()->log(ELogType_Info, "preUpdate replicate %d", _tickToReplicate);
#endif
				}
			}
		}
	}

	void StudentReplicaComponent::postUpdate( float delta ) 
	{
		// server / authority code

		if(getTopology() == SERVER)	{
			// this boolean is dependent from the replication frequency
			if(_sampleVehicleState)	{
				if(getActorSprite() == nullptr)	{
					return;
				}
				AbstractVehicle* vehicle(getActorSprite()->getVehicle());
				if(vehicle != nullptr)	{
					_localSpaceData = vehicle->getLocalSpaceData();
					_motionState.readLocalSpaceData(_localSpaceData);

					// TODO: @student : include linear and angular velocity into the protocol
					// TODO: @student : include a sequence number into the protocol and make sure that
					//                 you do not update the client with out of date packages
					// TODO: @student : design and implement a smaller protocol

					CCDictionary* vehicleDictionary(CCDictionary::create());
					vehicleDictionary->setObject(CCFloat::create(_localSpaceData._position.x), "x");
					vehicleDictionary->setObject(CCFloat::create(_localSpaceData._position.y), "y");
					vehicleDictionary->setObject(CCFloat::create(_localSpaceData._forward.x), "fx");
					vehicleDictionary->setObject(CCFloat::create(_localSpaceData._forward.y), "fy");

					vehicleDictionary->setObject(CCFloat::create(_localSpaceData._angularVelocity.z), "az");

					vehicleDictionary->setObject(CCFloat::create(_localSpaceData._linearVelocity.x), "lx");
					vehicleDictionary->setObject(CCFloat::create(_localSpaceData._linearVelocity.y), "ly");


					setJSONObject(CCJSONConverter::strFrom(vehicleDictionary));
				}
				_sampleVehicleState = false;
			}
		}
		SLBaseClass::postUpdate(delta);
	}

	// server / authority code
	void StudentReplicaComponent::serializeConstruction(RakNet::BitStream *constructionBitstream)	{
		CCDictionary* constructionDictionary(getConstructionDictionary());
		if(constructionDictionary != nullptr)	{
			// the vehicle position and orientation might have changed
			// so we need to perform an update here
			AbstractVehicle* vehicle(getActorSprite()->getVehicle());
			if(vehicle != nullptr)	{
				const LocalSpaceData& localSpaceData(vehicle->getLocalSpaceData());

				constructionDictionary->setObject(CCInteger::create(SL_CID_PHYSICSVEHICLE), "vehicleClassId");

				constructionDictionary->setObject(CCFloat::create(localSpaceData._position.x), "x");
				constructionDictionary->setObject(CCFloat::create(localSpaceData._position.y), "y");
				constructionDictionary->setObject(CCFloat::create(localSpaceData._forward.x), "fx");
				constructionDictionary->setObject(CCFloat::create(localSpaceData._forward.y), "fy");
			}
			CCString* constructionJSON = CCJSONConverter::strFrom(constructionDictionary);
			constructionBitstream->Write(constructionJSON->getCString());
		}
	}

	// client / slave code
	bool StudentReplicaComponent::deserializeConstruction(RakNet::BitStream *constructionBitstream)	{
		RakNet::BitStream& bitStream(*constructionBitstream);
		if (bitStream.GetNumberOfBitsUsed()==0)
			return false;

		RakNet::RakString rakString;
		bitStream.Read(rakString);

		setConstructionDictionary(CCJSONConverter::dictionaryFrom(rakString.C_String()));
		createActorSprite(getConstructionDictionary());
		return true;
	}

	// server / authority code
	void StudentReplicaComponent::serializeDestruction(RakNet::BitStream *destructionBitstream)	{
	}

	// client / slave code
	bool StudentReplicaComponent::deserializeDestruction(RakNet::BitStream *destructionBitstream)	{
		return true;
	}

	// server / authority code
	void StudentReplicaComponent::preSerialize()
	{
	}

	// server / authority code
	RakNet::RM3SerializationResult StudentReplicaComponent::serialize(RakNet::SerializeParameters *serializeParameters)
	{
		// note: take a look at preUpdate to understand how this works
		if(_tickReplicated == _tickToReplicate)	{
			return RakNet::RM3SR_DO_NOT_SERIALIZE;	
		}
		_tickSerialized = _tickToReplicate;
#if NL_STUDENTREPLICA_HAS_SERIALIZE_LOG
		getReplica()->getPeer()->log(ELogType_Info, "serialize %d ", _tickToReplicate);
#endif

		RakNet::BitStream& bitStream(serializeParameters->outputBitstream[0]);

		CCString* json = getJSONObject();
		if(json != nullptr)	{
			bitStream.Write(json->getCString());
		}

		return RakNet::RM3SR_SERIALIZED_ALWAYS_IDENTICALLY;
	}

	// client / slave code
	void StudentReplicaComponent::deserialize(RakNet::DeserializeParameters *deserializeParameters)
	{
		// a little base class magic
		SLBaseClass::deserialize(deserializeParameters);

		RakNet::BitStream& bitStream(deserializeParameters->serializationBitstream[0]);
		if (bitStream.GetNumberOfBitsUsed()==0)	{
			return;
		}

		RakNet::RakString rakString;
		bitStream.Read(rakString);

		if(getActorSprite() == nullptr)	{
			return;
		}

		AbstractVehicle* vehicle(getActorSprite()->getVehicle());
		if(vehicle != nullptr)	{
			// added : do not change velocities based on controller values
			PhysicsVehicle* physVehicle(dynamic_cast<PhysicsVehicle*>(vehicle));
			physVehicle->setUpdateVelocities(false);

			CCDictionary* vehicleDictionary = CCJSONConverter::dictionaryFrom(rakString.C_String());

			Vec3 newPosition(
				Dictionary::getFloat(vehicleDictionary, "x", 1), 
				Dictionary::getFloat(vehicleDictionary, "y", 1),
				0
				);
			Vec3 newForward(
				Dictionary::getFloat(vehicleDictionary, "fx", 1), 
				Dictionary::getFloat(vehicleDictionary, "fy", 1),
				0
				);

			Vec3 newLinearVelocity(
				Dictionary::getFloat(vehicleDictionary, "lx", 1), 
				Dictionary::getFloat(vehicleDictionary, "ly", 1),
				0
				);

			Vec3 newAngularVelocity(
				0,
				0,
				Dictionary::getFloat(vehicleDictionary, "az", 1)
				);

			// TODO: @student : implement client side interpolation.

			vehicle->setPosition(newPosition);
			vehicle->regenerateLocalSpace(newForward, 0.0f);

			// do this only one time ! - else interpolate ...
			vehicle->setAngularVelocity(newAngularVelocity);
			vehicle->setLinearVelocity(newLinearVelocity);

			// added
			vehicle->setDirty();

			_localSpaceData = vehicle->getLocalSpaceData();
			_motionState.readLocalSpaceData(_localSpaceData);
		}
	}

	void StudentReplicaComponent::createActorNode(CCDictionary* parameters)	{
		// just a debug entry
		SLBaseClass::createActorNode(parameters);
	}

	void StudentReplicaComponent::createActorSprite(CCDictionary* parameters)	{
		// just a debug entry
		SLBaseClass::createActorSprite(parameters);
	}

	void StudentReplicaComponent::removeFromGameplayLayer()
	{
		SLBaseClass::removeFromGameplayLayer();
	}

}


