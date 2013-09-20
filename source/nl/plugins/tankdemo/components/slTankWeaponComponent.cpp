//!---------------------------------------------------------------------------
//! \file "slTankWeaponComponent.cpp"
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
#include "slTankWeaponComponent.h"
#include "network/nlTankProjectileReplicaComponent.h"


namespace sl	{


	TankWeaponComponent::TankWeaponComponent()
		:_updateTick(0)
	{
		// shoot 2 times per second
		_fireTick.setAnimationFrequency(2);
	}

	TankWeaponComponent::~TankWeaponComponent()
	{

	}

	void TankWeaponComponent::preUpdate( float delta ) 	{
		SLBaseClass::preUpdate(delta);
		// preUpdate will always be called once per frame
		++_updateTick;

		// we need the ActorSprite here ...
		ActorSprite* actorSprite(getGameActorNode()->getActorSprite());
		if(actorSprite != nullptr)	{
			bool vehicleCanShoot(false);
			AbstractVehicle* vehicle(actorSprite->getVehicle());
			if(vehicle)	{
				if(vehicle->getClassId() == SL_CID_PEDESTRIAN)	{
					// pedestrians do not shoot right ?
				}
				else	{
					vehicleCanShoot = true;
				}

				if(vehicleCanShoot)	{
					AbstractPlayer* vehiclePlayer(vehicle->getPlayer());
					if(vehiclePlayer != nullptr)	{
						const AbstractController* controller(vehiclePlayer->getController());
						float shoot = controller->getActionValue(EControllerAction_Shoot);
						if(shoot > 0.0f)	{
							CCDictionary* constructionDictionaryTemplate(Dictionary::getDictionary(actorSprite->getConstructionDictionary(),"projectile"));
							if(constructionDictionaryTemplate != nullptr)	{
								bool animationTickChanged(false);
								SLSize ticks(_fireTick.update( delta, animationTickChanged ));
								if(animationTickChanged)	{
//									SL_PROCESS_APP()->log(ELogType_Message, "%X - shoot - updateTick(%d) delta(%f) accum(%f)", 
//										this, _updateTick, delta, _fireTick.getAccumTime());
									CCDictionary* constructionDictionary(Dictionary::copyDictionary(constructionDictionaryTemplate));
									Vec3 launchPos(vehicle->position());
									Vec3 launchForward(vehicle->forward());

									launchPos += launchForward * vehicle->radius();

									constructionDictionary->setObject(CCFloat::create(launchPos.x), "x");
									constructionDictionary->setObject(CCFloat::create(launchPos.y), "y");

									constructionDictionary->setObject(CCFloat::create(launchForward.x), "fx");
									constructionDictionary->setObject(CCFloat::create(launchForward.y), "fy");

									const float projectileMaxSpeed(vehicle->linearVelocity().length() + vehicle->maxSpeed() * 2);
									constructionDictionary->setObject(CCFloat::create(projectileMaxSpeed), "maxSpeed");

									Vec3 launchLinearVelocity(launchForward);
									launchLinearVelocity *= projectileMaxSpeed;
									constructionDictionary->setObject(CCFloat::create(launchLinearVelocity.x), "lvx");
									constructionDictionary->setObject(CCFloat::create(launchLinearVelocity.y), "lvy");

									// try to find this replica via the replica manager
									ReplicaManager* replicaManager(nullptr);

									Peer* peer(getGameActorNode()->getGameplayLayer()->getPeer());
									if(peer != nullptr)	{
										replicaManager = peer->getReplicaManager(); 
									}
									if(replicaManager != nullptr)	{
										if(peer->getTopology() == SERVER)	{
											GameObjectReplica* gameObjectReplica(dynamic_cast<GameObjectReplica*>(
												replicaManager->createReplica(nl::TankProjectileReplicaComponent::staticClassName(), constructionDictionary)));
											if(gameObjectReplica != nullptr)	{
												ReplicaComponent* replicaComponent(gameObjectReplica->getReplicaComponent());
												if(replicaComponent != nullptr)	{
													replicaComponent->getGameActorNode()->getActorSprite()->setInstigator(
														getGameActorNode()->getActorSprite());
												}
											}
										}
									}
									else	{
										nl::TankProjectileReplicaComponent* replicaComponent(nl::TankProjectileReplicaComponent::create());
										replicaComponent->setGameplayLayer(getGameActorNode()->getGameplayLayer());
										replicaComponent->createActorNode(nullptr);
										replicaComponent->createActorSprite(constructionDictionary);
										if(replicaComponent != nullptr)	{
											replicaComponent->getGameActorNode()->getActorSprite()->setInstigator(
												getGameActorNode()->getActorSprite());
										}
									}

								}
							}
						}
					}
				}
			}
		}
	}

	void TankWeaponComponent::postUpdate( float delta ) 	{
		SLBaseClass::postUpdate(delta);
		// postUpdate will always be called once per frame
	}


}


