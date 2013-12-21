//!---------------------------------------------------------------------------
//! \file "nlTankVsTankGameLogicNode.cpp"
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
#include "nlTankVsTankGameLogicNode.h"
#include "network/nlGameStateReplicaComponent.h"
#include "network/nlTankReplicaComponent.h"


namespace nl	{

	TankVsTankGameLogicNode::TankVsTankGameLogicNode()
		:_gameplayLayer(nullptr)
	{
		//int max = _spectators->count();

	}

	TankVsTankGameLogicNode::~TankVsTankGameLogicNode()
	{
	}

	bool TankVsTankGameLogicNode::init()	{
		bool initialized(SLBaseClass::init());

		_spectators = new CCArray();

		return initialized;
	}

	void TankVsTankGameLogicNode::update( float dt )
	{
		SLBaseClass::update(dt);

		SLTimeInterval accumTime(getAccumTime());

		GameplayLayer* gameplayLayer(getGameplayLayer());
		if(gameplayLayer != nullptr)
		{
			Peer* peer(getPeer());

			// iterate over all children and check if Actors are destroyed

			size_t activePlayers = 0; //count how many active players we have
			size_t dyingPlayers = 0;
			

			CCArray* destroyedChildren(CCArray::create());
			CCObject* child = nullptr;
			CCARRAY_FOREACH(gameplayLayer->getChildren(), child)
			{
				GameActorNode* actorNode(dynamic_cast<GameActorNode*>(child));
				if(actorNode != nullptr)
				{
					if(actorNode->isDestroyed())
					{
						destroyedChildren->addObject(actorNode);
						++dyingPlayers;
						TankPlayerReplicaComponent* replicaComponent = getTankPlayerReplicaComponentFromActorNode(actorNode);
					}
					else
					{
						// find the TankPlayerReplica component by iterating over all components of the GameActor until we find it
						TankPlayerReplicaComponent* replicaComponent = getTankPlayerReplicaComponentFromActorNode(actorNode);
						
						if(replicaComponent != nullptr)
						{
							//if we already have <4> active players, set the player to spectator mode
							if (activePlayers >= PLAYER_LIMIT)
								replicaComponent->setSpectatorMode(true);

							if (replicaComponent->isSpectatorMode() == false)
								++activePlayers; //if the player isn't in spectator mode, count as active player
							else
							{
								if (_spectators->indexOfObject(replicaComponent) == UINT_MAX)
									_spectators->addObject(replicaComponent); //add the spectator to the list so we can disable spectator mode it if not enough active players are playing
							}
						}
					}
				}
			}

			int activatePlayers = (int)PLAYER_LIMIT - activePlayers;

			for (int i = 0; i < activatePlayers; i++)
			{
				if (i < _spectators->count())
				{
					TankPlayerReplicaComponent* playerReplica(dynamic_cast<TankPlayerReplicaComponent*>(_spectators->objectAtIndex(i)));
					getPeer()->log(ELogType_Info, "Replica index: %i", playerReplica->idx());
					playerReplica->setSpectatorMode(false);
				}
			}

			for (int i = 0; i < activatePlayers; i++)
			{
				if (i < _spectators->count())
				{
					_spectators->removeObjectAtIndex(0);
				}
			}


			CCARRAY_FOREACH(destroyedChildren, child)
			{
				GameActorNode* actorNode(dynamic_cast<GameActorNode*>(child));
				if(actorNode != nullptr)	{
					// find the replica component
					bool replicaComponentFound(false);
					ComponentArray* components(actorNode->getComponents());
					SLSize idx(0);
					IComponent* component(components->componentAt(idx));
					while(component != nullptr)	{
						TankReplicaComponent* replicaComponent(dynamic_cast<TankReplicaComponent*>(component));
						if(replicaComponent != nullptr)	{
							replicaComponent->handleDestroyedActorNode();
							replicaComponentFound = true;

							break;
						}
						++idx;
						component = components->componentAt(idx);
					}

					if(replicaComponentFound == false)	{
						// spawn an effect ???
					}
				}
			}
		}

		// brute force super simple collision detection

		AVGroup allTankVehicles;
		AVGroup allProjectileVehicles;

		EntityClassIdQuery tankQuery(SL_CID_TANK);
		AVQuery allTanks(getGameplayLayer()->allVehicles(), &tankQuery);
		allTanks.find(allTankVehicles);

		EntityClassIdQuery projectileQuery(SL_CID_TANK_PROCECTILE);
		AVQuery allProjectiles(getGameplayLayer()->allVehicles(), &projectileQuery);
		allProjectiles.find(allProjectileVehicles);

		AVGroup::iterator projectile(allProjectileVehicles.begin());
		AVGroup::iterator projectileEnd(allProjectileVehicles.end());
		while(projectile != projectileEnd)	{
			AVGroup::iterator tank(allTankVehicles.begin());
			AVGroup::iterator tankEnd(allTankVehicles.end());
			while(tank != tankEnd)	{

				Vec3 distance = (*tank)->position() - (*projectile)->position();
				if(distance.length() < (*tank)->radius())	{
					GameActor* tankActor((GameActor*)(*tank)->getUserobject());
					GameActor* projectileActor((GameActor*)(*projectile)->getUserobject());

					// check the instigator to prevent killing yourself
					if(projectileActor->getInstigator() != tankActor)	{
						if(tankActor->getActorNode()->isDestroyed())	{

						}
						else	{
							// now we destroy both of them
							tankActor->getActorFlags().addFlag(EActorFlag_IsDestroyed);
							tankActor->getActorNode()->getActorNodeFlags().addFlag(EActorNodeFlag_IsDestroyed);

							SL_PROCESS_APP()->log(ELogType_Message, "hit !!!");
						}

						projectileActor->getActorFlags().addFlag(EActorFlag_IsDestroyed);
						projectileActor->getActorNode()->getActorNodeFlags().addFlag(EActorNodeFlag_IsDestroyed);
					}
				}
				++tank;
			}
			++projectile;
		}
	}

	TankPlayerReplicaComponent* TankVsTankGameLogicNode::getTankPlayerReplicaComponentFromActorNode(ActorNode *actorNode)
	{
		// find the TankPlayerReplica component by iterating over all components of the GameActor until we find it
		ComponentArray* components(actorNode->getComponents());
		SLSize idx(0);
		IComponent* component(components->componentAt(idx));
		while(component != nullptr)
		{
			TankPlayerReplicaComponent* replicaComponent(dynamic_cast<TankPlayerReplicaComponent*>(component));

			if(replicaComponent != nullptr)
			{
				return replicaComponent;
			}
			++idx;
			component = components->componentAt(idx);
		}
	}

	void TankVsTankGameLogicNode::onEnter()	{
		SLBaseClass::onEnter();
	}

	void TankVsTankGameLogicNode::onExit()	{
		setGameplayLayer(nullptr);
		SLBaseClass::onExit();
	}

	void TankVsTankGameLogicNode::onNewIncommingConnection(PeerWrapper* peerWrapper)	{
		getPeer()->log( ELogType_Info, "%s - received new incomming connection from %s with GUID %s", 
			getClassName(),
			peerWrapper->getSystemAddress().ToString(true), peerWrapper->getGUID().ToString());

	}

	void TankVsTankGameLogicNode::onConnectionLost(PeerWrapper* peerWrapper)	{
		getPeer()->log( ELogType_Info, "%s - received lost connection from %s with GUID %s", 
			getClassName(),
			peerWrapper->getSystemAddress().ToString(true), peerWrapper->getGUID().ToString());
	}

	void TankVsTankGameLogicNode::onConnectionDisconnected(PeerWrapper* peerWrapper)	{
		getPeer()->log( ELogType_Info, "%s - received dissonnected from %s with GUID %s", 
			getClassName(),
			peerWrapper->getSystemAddress().ToString(true), peerWrapper->getGUID().ToString());
	}

	void TankVsTankGameLogicNode::onPeerIsConnected(PeerWrapper* peerWrapper)
	{
		getPeer()->log(ELogType_Info, "%s - received peer is connected", getClassName());

		//DONE @David: creating gameStateReplica
		getPeer()->getReplicaManager()->createReplica(nlGameStateReplicaComponent::staticClassName(),nullptr);
	}

	void TankVsTankGameLogicNode::onPeerConnectionAttemptFailed(PeerWrapper* peerWrapper)	{
		getPeer()->log(ELogType_Info, "%s - received peer connection attempt failed", getClassName());
	}

	void TankVsTankGameLogicNode::onPeerFailedToConnect(PeerWrapper* peerWrapper)	{
		getPeer()->log(ELogType_Info, "%s - received peer failed to connected", getClassName());
	}

	void TankVsTankGameLogicNode::onPeerWillDisconnect(PeerWrapper* peerWrapper)	{
		getPeer()->log(ELogType_Info, "%s - received peer will disconnect", getClassName());
	}



	CCDictionary* TankVsTankGameLogicNode::createTankCreationDictionary()
	{
		static SLSize vehicleIdx(0);

		// to create the local player replica
		CCDictionary* constructionDictionary(createTankCreationDictionary(vehicleIdx));

		++vehicleIdx;
		if(vehicleIdx == 4)	{
			vehicleIdx = 0;
		}
		return constructionDictionary;
	}

	void TankVsTankGameLogicNode::createTankProjectileCreationDictionary( 
		CCDictionary* parentDictionary)	
	{
		CCDictionary* constructionDictionary(CCDictionary::create());

		// retrieve the parent sprite texture to check the name
		SLAString projectileTextureName;
		const CCString* parentSprite(parentDictionary->valueForKey("sprite"));
		if(parentSprite->compare("bluetank.png") == 0)	{
			projectileTextureName.assign("bluemissile.png");
		}
		else if(parentSprite->compare("redtank.png") == 0)	{
			projectileTextureName.assign("redmissile.png");
		}
		else if(parentSprite->compare("yellowtank.png") == 0)	{
			projectileTextureName.assign("yellowmissile.png");
		}
		else if(parentSprite->compare("greentank.png") == 0)	{
			projectileTextureName.assign("greenmissile.png");
		}

		constructionDictionary->setObject(CCString::create(projectileTextureName.c_str()), "sprite");
		constructionDictionary->setObject(CCInteger::create(SL_CID_TANK_PROCECTILE), "vehicleClassId");

		parentDictionary->setObject(constructionDictionary, "projectile");
	}

	CCDictionary* TankVsTankGameLogicNode::createTankCreationDictionary( const char* textureName, const CCPoint& position )
	{
		CCDictionary* constructionDictionary(CCDictionary::create());
		constructionDictionary->setObject(CCString::create(textureName), "sprite");
		constructionDictionary->setObject(CCFloat::create(position.x), "x");
		constructionDictionary->setObject(CCFloat::create(position.y), "y");
		constructionDictionary->setObject(CCInteger::create(SL_CID_TANK), "vehicleClassId");
		//constructionDictionary->setObject(CCString::create("John"), "username");

		Vec3 vecPosition( position.x, position.y, 0.0f );
		Vec3 vecForward( (Vec3(vecPosition) * -1.0).normalized() );

		constructionDictionary->setObject(CCFloat::create(vecForward.x), "fx");
		constructionDictionary->setObject(CCFloat::create(vecForward.y), "fy");

		TankVsTankGameLogicNode::createTankProjectileCreationDictionary(constructionDictionary);
		return constructionDictionary;
	}

	CCDictionary* TankVsTankGameLogicNode::createTankCreationDictionary( SLSize idx )
	{
		// create 4 tanks for 4 players
		CCPoint position(100,100);
		if(idx == 0 || idx > 3)	{
			CCDictionary* constructionDictionary(createTankCreationDictionary("bluetank.png", position));
			return constructionDictionary;
		}
		position.x -= 200;
		if(idx == 1)	{
			CCDictionary* constructionDictionary(createTankCreationDictionary("redtank.png", position));
			return constructionDictionary;
		}
		position.y -= 200;
		if(idx == 2)	{
			CCDictionary* constructionDictionary(createTankCreationDictionary("yellowtank.png", position));
			return constructionDictionary;
		}
		position.x += 200;
		if(idx == 3)	{
			CCDictionary* constructionDictionary(createTankCreationDictionary("greentank.png", position));
			return constructionDictionary;
		}
		return nullptr;
	}
}


