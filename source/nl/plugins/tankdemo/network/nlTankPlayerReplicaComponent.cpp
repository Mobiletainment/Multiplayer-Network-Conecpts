//!---------------------------------------------------------------------------
//! \file "nlTankPlayerReplicaComponent.cpp"
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
#include "nlTankPlayerReplicaComponent.h"
#include "nlReplicationRules.h"

#include "nlGameContentReplicaManager.h"
#include "../nlTankVsTankGameLogicNode.h"

namespace nl	{

	//-------------------------------------------------------------------------
	// TankPlayerReplicaComponent
	//-------------------------------------------------------------------------

	TankPlayerReplicaComponent::TankPlayerReplicaComponent()
		:_tankReplicaComponent(nullptr)
		,_tankActorNode(nullptr)
		,_lastControlledReplicaNetworkId(RakNet::UNASSIGNED_NETWORK_ID)
	{
		_replica.setName(TankPlayerReplicaComponent::staticClassName());

		ServerAuthorityReplicationRule* replicationRule(ServerAuthorityReplicationRule::create());
		replicationRule->_replica = getReplica();
		_replica.setReplicationRule(replicationRule);
	}

	TankPlayerReplicaComponent::~TankPlayerReplicaComponent()	{
		setTankActorNode(nullptr);
		setTankReplicaComponent(nullptr);
	}

	void TankPlayerReplicaComponent::preUpdate( float delta ) 	{
		// preUpdate will always be called once per frame

		SLBaseClass::preUpdate(delta);

		// pass over the controller values to a controller object
		_player.accessController()->setActionValue(EControllerAction_Yaw, _ctrlValues._leftRight);
		_player.accessController()->setActionValue(EControllerAction_Move, _ctrlValues._forwardBackward);
		_player.accessController()->setActionValue(EControllerAction_Shoot, _ctrlValues._shoot);

		// TODO: @student : the tank replica component property on the client is null
		//                 you can replicate the played replica networkid (which is done anyways already)
		//                 find the replica on the client side and let the client play
		//                 but be careful the tank might get shot ...

		if (getTopology()==CLIENT)	{
			// this is actually client code
			if(_lastControlledReplicaNetworkId != _ctrlValues._controlledReplicaNetworkId)
			{
				if(_lastControlledReplicaNetworkId != UNASSIGNED_NETWORK_ID)	{
					_lastControlledReplicaNetworkId = UNASSIGNED_NETWORK_ID;
					setTankReplicaComponent(nullptr);
				}
			}


			if((_ctrlValues._controlledReplicaNetworkId != UNASSIGNED_NETWORK_ID) &&
				(getTankReplicaComponent() == nullptr))
			{
				_lastControlledReplicaNetworkId = _ctrlValues._controlledReplicaNetworkId;
				// try to find this replica via the replica manager
				ReplicaManager* replicaManager(getReplicaManager());
				if(replicaManager != nullptr)	{
					// this is actually a pretty inefficient implementation
					// a hash table lookup better would be way faster actually
					// take a look at the implementation ...
					GameObjectReplica* vehicleReplica(dynamic_cast<GameObjectReplica*>(
						replicaManager->getReplicaByNetworkID(_ctrlValues._controlledReplicaNetworkId)));
					if(vehicleReplica != nullptr)	{
						// note: now we need to connect this vehicle with some form of controller
						TankReplicaComponent* tankReplicaComponent(dynamic_cast<TankReplicaComponent*>(
							vehicleReplica->getReplicaComponent()));
						if(tankReplicaComponent != nullptr)	{
							setTankReplicaComponent(tankReplicaComponent);
						}
					}
				}
			}

			if(getTankReplicaComponent() != nullptr)	{
				if(getTankReplicaComponent()->getActorSprite() != nullptr)	{
					AbstractVehicle* vehicle(getTankReplicaComponent()->getActorSprite()->getVehicle());
					if(vehicle != nullptr)	{
						_player.play(vehicle);
					}
				}
			}
		}
		else if (getTopology()==SERVER)	{
			if(getTankReplicaComponent() != nullptr)	{
				if(getTankReplicaComponent()->getActorSprite() != nullptr)	{
					AbstractVehicle* vehicle(getTankReplicaComponent()->getActorSprite()->getVehicle());
					if(vehicle != nullptr)	{
						_player.play(vehicle);
					}
				}
			}
		}

	}

	// postUpdate will always be called once per frame
	void TankPlayerReplicaComponent::postUpdate( float delta ) 	{
		if(isActorNodeDestroyed())	{
			// TODO: @student : nothing todo here right ?
		}
		else	{
			if (getTopology()==CLIENT)	{
			}
			else if (getTopology()==SERVER)	{
				TankReplicaComponent* tankReplicaComponent(getTankReplicaComponent());
				GameActorNode* tankActorNode(getTankActorNode());
				if(tankReplicaComponent != nullptr)	{
					if(tankActorNode != nullptr)	{
						if(tankActorNode->isDestroyed())	{
							// player not destroyed
							if(getActorNode()->isDestroyed() == false)	{
								setTankActorNode(nullptr);
								setTankReplicaComponent(nullptr);
							}
							_ctrlValues._controlledReplicaNetworkId = UNASSIGNED_NETWORK_ID;
						}
					}
				}


				if(getTankReplicaComponent() == nullptr)	{

					// TODO: @student : create the server vehicle replica

					// we need the replica manager here
					ReplicaManager* replicaManager(getReplicaManager());
					if(replicaManager != nullptr)	{

						// to create the local player replica
						CCDictionary* constructionDictionary(TankVsTankGameLogicNode::createTankCreationDictionary());

						GameObjectReplica* vehicleReplica(dynamic_cast<GameObjectReplica*>(replicaManager->createReplica(TankReplicaComponent::staticClassName(), constructionDictionary)));
						if(vehicleReplica != nullptr)	{
							// note: now we need to connect this vehicle with some form of controller
							TankReplicaComponent* tankReplicaComponent(dynamic_cast<TankReplicaComponent*>(vehicleReplica->getReplicaComponent()));
							setTankReplicaComponent(tankReplicaComponent);
							setTankActorNode(tankReplicaComponent->getGameActorNode());

							_ctrlValues._controlledReplicaNetworkId = vehicleReplica->GetNetworkID();

							_replica.getPeer()->log(ELogType_Message, 
								"%s created a player tank object with networkId %d",
								getClassName(), _ctrlValues._controlledReplicaNetworkId);
						}
					}
				}
			}
		}

		SLBaseClass::postUpdate(delta);
	}

	void TankPlayerReplicaComponent::serializeConstruction(RakNet::BitStream *constructionBitstream)	{
		// TODO: smth usefull
		constructionBitstream->Write(_replica.getName().C_String());
	}

	bool TankPlayerReplicaComponent::deserializeConstruction(RakNet::BitStream *constructionBitstream)	{
		RakNet::BitStream& bitStream(*constructionBitstream);
		if (bitStream.GetNumberOfBitsUsed()==0)
			return false;

		return true;
	}

	void TankPlayerReplicaComponent::serializeDestruction(RakNet::BitStream *destructionBitstream)	{

	}

	bool TankPlayerReplicaComponent::deserializeDestruction(RakNet::BitStream *destructionBitstream)	{
		return true;
	}

	// server / authority code
	void TankPlayerReplicaComponent::preSerialize()	{
	}

	// server / authority code
	RakNet::RM3SerializationResult TankPlayerReplicaComponent::serialize(RakNet::SerializeParameters *serializeParameters)	{
		if (getTopology()==CLIENT)	{
			return RM3SR_DO_NOT_SERIALIZE;
		}
		return SLBaseClass::serialize(serializeParameters);
	}

	// client / receive code
	void TankPlayerReplicaComponent::deserialize(RakNet::DeserializeParameters *deserializeParameters)	{
		SLBaseClass::deserialize(deserializeParameters);

#if 0 // massive log spam if you like
		_replica.getPeer()->log(ELogType_Status, "lr(%.02f) fb(%.02f) fire(%.02f)", 
			_ctrlValues._leftRight, 
			_ctrlValues._forwardBackward, 
			_ctrlValues._shoot);
#endif

	}

}


