//!---------------------------------------------------------------------------
//! \file "nlLocalPlayerReplicaComponent.cpp"
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
#include "nlLocalPlayerReplicaComponent.h"

#include "nlGameContentReplicaManager.h"

#include "nlReplicationRules.h"

#include "../nlTankVsTankGameLogicNode.h"
#include "../nlTankVsTankGameplayLayer.h"
namespace nl	{


	//-------------------------------------------------------------------------
	// LocalPlayerReplicaComponent
	//-------------------------------------------------------------------------

	LocalPlayerReplicaComponent::LocalPlayerReplicaComponent()
		:_serverPlayerReplicaComponent(nullptr)
		,_createServerPlayerReplica(false)
		,_tankActorNode(nullptr)
	{
		_replica.setName(LocalPlayerReplicaComponent::staticClassName());
		ClientToServerReplicationRule* replicationRule(ClientToServerReplicationRule::create());
		replicationRule->_replica = getReplica();
		_replica.setReplicationRule(replicationRule);

		_physicsTick.setAnimationFrequency(50);
	}

	LocalPlayerReplicaComponent::~LocalPlayerReplicaComponent()	{
		setTankPlayerReplicaComponent(nullptr);
	}

	void LocalPlayerReplicaComponent::preUpdate( float delta ) 	{
		SLBaseClass::preUpdate(delta);
		
		//@David: maintain and update the local update tick
		if (getTopology() == CLIENT)
		{
			bool animationTickChanged(false);
			_ctrlValues._updateTick = _physicsTick.update(delta, animationTickChanged);
			if (animationTickChanged)
			{

			}
		}

		// preUpdate will always be called once per frame

		// DONE @student : 2.1. send controller values to the server
		// TODO @student : 3. what should happen on the server now ?


		if(getTopology() == CLIENT)	{
#if 1
			// so here we can read our client side controller values
			GameplayLayer* gameplayLayer(getGameActorNode()->getGameplayLayer());
			LocalPlayer& localPlayer(gameplayLayer->getLocalPlayer());
			const AbstractController* controller(localPlayer.getController());

			// and write the values to the data structure which will be serialized to the server
			_ctrlValues._leftRight = controller->getActionValue(EControllerAction_Yaw);
			_ctrlValues._forwardBackward = controller->getActionValue(EControllerAction_Move);
			_ctrlValues._shoot = controller->getActionValue(EControllerAction_Shoot);
			
#endif
		}
		else if(getTopology() == SERVER)	{

			// DONE: By @David 4. create the server tank player
			//                   hint: this is the right place

			// this is the update
			TankPlayerReplicaComponent* tankPlayerReplicaComponent(getTankPlayerReplicaComponent());
			if(tankPlayerReplicaComponent != nullptr)	{
				// without this line the whole stuff will not work
				// here we pass over the client controller values to the
				// server authority player
#if 1
				tankPlayerReplicaComponent->setControllerValues(_ctrlValues);
#endif
			}

			// this is the creation
			if(_createServerPlayerReplica && tankPlayerReplicaComponent == nullptr)	{
				/*
				... insert missing code - smth essential is missing down below
				*/

				// we need the replica manager here
				GameContentReplicaManager* replicaManager(dynamic_cast<GameContentReplicaManager*>(_replica.getPeer()->getReplicaManager()));
				if(replicaManager != nullptr)	{
					// to create the local player replica
					GameObjectReplica* serverPlayerReplica = dynamic_cast<GameObjectReplica*>(replicaManager->createReplica(TankPlayerReplicaComponent::staticClassName(), nullptr));
					if(serverPlayerReplica != nullptr)	{
						// note: now we've got the server -> client ctrl value sender

						TankPlayerReplicaComponent* tankPlayerReplicaComponent(dynamic_cast<TankPlayerReplicaComponent*>(serverPlayerReplica->getReplicaComponent()));
						setTankPlayerReplicaComponent(tankPlayerReplicaComponent);

						_replica.getPeer()->log(ELogType_Message, 
							"%s deserializeConstruction TankPlayerReplicaComponent created",
							getClassName());
					}
				}
				_createServerPlayerReplica = false;
			}
		}
	}

	void LocalPlayerReplicaComponent::postUpdate( float delta )
	{	
		// postUpdate will always be called once per frame
		if(isActorNodeDestroyed())	{
		}
		else	
		{
			if (getTopology()==CLIENT)
			{
				
			}
			else if (getTopology()==SERVER)
			{
				//check if the player has been destroyed
				TankPlayerReplicaComponent* tankPlayerReplicaComponent(getTankPlayerReplicaComponent());
				TankReplicaComponent* tankReplicaComponent(tankPlayerReplicaComponent->getTankReplicaComponent());
				GameActorNode* gameActorNode(tankPlayerReplicaComponent->getTankActorNode());
				if(tankReplicaComponent != nullptr)
				{
					if(gameActorNode != nullptr)
					{
						if(gameActorNode->isDestroyed()) //the gameActorNode is destroyed
						{
							if(tankPlayerReplicaComponent->getActorNode()->isDestroyed() == false) //the playerReplica's ActorNode is not destroyed
							{
								tankPlayerReplicaComponent->setTankActorNode(nullptr);
								tankPlayerReplicaComponent->setTankReplicaComponent(nullptr);

								//Player was destroyed -> enable spectator mode
								tankPlayerReplicaComponent->setSpectatorMode(true);
								SL_PROCESS_APP()->log(ELogType_Info,"Player destroyed & spectator mode enabled");

							}
							_ctrlValues._controlledReplicaNetworkId = UNASSIGNED_NETWORK_ID;
						}

						//tankReplicaComponent->setKillCount(_ctrlValues._killCount);
						//SL_PROCESS_APP()->log(ELogType_Info, "Kill count: %f", _ctrlValues._killCount);

					}
				}
			}
		}



		SLBaseClass::postUpdate(delta);
	}

	void LocalPlayerReplicaComponent::serializeConstruction(RakNet::BitStream *constructionBitstream)	{

	}

	bool LocalPlayerReplicaComponent::deserializeConstruction(RakNet::BitStream *constructionBitstream)	{
		if(getTopology() == SERVER)	{
			_replica.getPeer()->log(ELogType_Message, "%s deserializeConstruction", getClassName());

			_replica.getPeer()->log(ELogType_Message, 
				"%s deserializeConstruction TankPlayerReplicaComponent needs to be created",
				getClassName());

			// DONE: By @David 2.2 create the tank player

			// but be careful do not do this here at this place
			// the replica manager will go crazy !!!
			_createServerPlayerReplica = true;
		}
		return true;
	}

	void LocalPlayerReplicaComponent::serializeDestruction(RakNet::BitStream *destructionBitstream)	{

	}

	bool LocalPlayerReplicaComponent::deserializeDestruction(RakNet::BitStream *destructionBitstream)	{
		if(getTopology() == SERVER)	{
			_replica.getPeer()->log(ELogType_Status, "%s deserializeDestruction", getClassName());

			_replica.getPeer()->log(ELogType_Status, 
				"%s deserializeDestruction ServerPlayerReplicaComponents need to be destroyed",
				getClassName());

			// TODO: @student 15. the tank player and his tank need to be deleted
			//                   hint: checkout the flag EActorNodeFlag_IsDestroyed

			// but be careful. do not mess with the replica manager at this place
			// the replica manager will go crazy

			/*
			... insert missing code - smth essential is missing down below
			*/
			//getTank Player
			TankPlayerReplicaComponent* tankPlayerReplicaComponent(getTankPlayerReplicaComponent());
			if(tankPlayerReplicaComponent != nullptr)
			{
				tankPlayerReplicaComponent->getActorNode()->getActorNodeFlags().addFlag(EActorNodeFlag_IsDestroyed);
				TankReplicaComponent* tankReplicaComponent(tankPlayerReplicaComponent->getTankReplicaComponent());
				tankReplicaComponent->getActorNode()->getActorNodeFlags().addFlag((EActorNodeFlag_IsDestroyed));
				tankPlayerReplicaComponent->setTankReplicaComponent(nullptr);
				tankPlayerReplicaComponent->setTankActorNode(nullptr);
			}
			setTankPlayerReplicaComponent(nullptr);
		}
		return true;
	}

	// client / authority code
	void LocalPlayerReplicaComponent::preSerialize()	{
	}

	// client / authority code
	RakNet::RM3SerializationResult LocalPlayerReplicaComponent::serialize(RakNet::SerializeParameters *serializeParameters)	{
		return SLBaseClass::serialize(serializeParameters);
	}

	// server / receive code
	void LocalPlayerReplicaComponent::deserialize(RakNet::DeserializeParameters *deserializeParameters)	{
		SLBaseClass::deserialize(deserializeParameters);
	}

}


