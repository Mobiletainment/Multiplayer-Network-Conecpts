//!---------------------------------------------------------------------------
//! \file "slActorSprite.cpp"
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
#include "slActorSprite.h"
#include "slGameActorNode.h"

#include "slGameplayLayer.h"

namespace sl	{


	ActorSprite::ActorSprite()
		:_accumTime(0)
		,_label(nullptr)
	{
		_gamePosition = CCPointMake(0,0);
	}

	ActorSprite::~ActorSprite()	{

	}

	ActorSprite* ActorSprite::createWithDictionary(CCDictionary* dictionary)	{
		ActorSprite *sprite = new ActorSprite();
		if (sprite && sprite->initWithDictionary(dictionary))	{
			sprite->autorelease();

			sprite->_label = dynamic_cast<CCLabelTTF*>( ControlUtils::createLabel("xxx", kCCTextAlignmentLeft) );
			sprite->_label->setVisible(false);
			sprite->addChild(sprite->_label);

			return sprite;
		}
		CC_SAFE_DELETE(sprite);
		return nullptr;
	}

	bool ActorSprite::initWithDictionary(CCDictionary* dictionary)	{
		SL_ASSERT(dictionary != nullptr);

		setConstructionDictionary(dictionary);

		const char* filename = ValueConversion::getStringForObject(dictionary->objectForKey("sprite"))->getCString();
		bool initialized(SLBaseClass::initWithFile(filename));
		if(initialized)	{
			const int vehicleClassIdValue = Dictionary::getInteger(dictionary, "vehicleClassId", SL_CID_PEDESTRIAN);
			const OS_CID vehicleClassId(static_cast<OS_CID>(vehicleClassIdValue));
			const float x = Dictionary::getFloat(dictionary, "x");
			const float y = Dictionary::getFloat(dictionary, "y");
			const float maxSpeed = Dictionary::getFloat(dictionary, "maxSpeed", 64);
			const float maxForce = Dictionary::getFloat(dictionary, "maxForce", 64);
			// assuming a quad
			const float radius = Dictionary::getFloat(dictionary, "radius", 
				getContentSize().width * 0.5f);

			Vec3 newForward(
				Dictionary::getFloat(dictionary, "fx", 0), 
				Dictionary::getFloat(dictionary, "fy", 1),
				Dictionary::getFloat(dictionary, "fz", 0)
				);

			Vec3 newLinearVelocity(
				Dictionary::getFloat(dictionary, "lvx", 0), 
				Dictionary::getFloat(dictionary, "lvy", 0),
				Dictionary::getFloat(dictionary, "lvz", 0)
				);

			Vec3 newAngularVelocity(
				Dictionary::getFloat(dictionary, "avx", 0), 
				Dictionary::getFloat(dictionary, "avy", 0),
				Dictionary::getFloat(dictionary, "avz", 0)
				);

			createOpenSteerEntity(vehicleClassId);

			AbstractVehicle* vehicle(getVehicle());
			if(vehicle != nullptr)	{
				vehicle->setMaxSpeed(maxSpeed);
				vehicle->setMaxForce(maxForce);
				vehicle->setRadius(radius);
				vehicle->regenerateLocalSpace(newForward, 0.0f);
				vehicle->setLinearVelocity(newLinearVelocity);
				vehicle->setAngularVelocity(newAngularVelocity);

				PhysicsVehicle* physVehicle(dynamic_cast<PhysicsVehicle*>(vehicle));
				if(physVehicle != nullptr)	{
					physVehicle->setMaxLocalLinearVelocity(maxSpeed);
				}

			}

			// this will also update the vehicle position
			CCPoint gamePosition(x,y);
			setGamePosition(gamePosition);

			if(vehicle != nullptr)	{
				_lastForward = vehicle->forward();
				_lastPosition = vehicle->position();
			}

			scheduleUpdate();
		}
		return initialized;
	}

	// a very ugly hack for the moment ...
	// just play everything in singleplayer mode
	// TODO: convert to a feature ...
	void ActorSprite::updateSinglePlayer(float dt)	{
		GameplayLayer* gameplayLayer(getGameplayLayer());
		if(gameplayLayer == nullptr)	{
			return;
		}

		// retrieve the actors vehicle if any
		AbstractVehicle* vehicle(nullptr);
		if(getOwnsEntity())	{
			vehicle = getVehicle();
		}
		if(vehicle)	{
			if(vehicle->getClassId() == SL_CID_PEDESTRIAN)	{
				// this vehicle is steering force driven
			}
			else	{
				LocalPlayer& localPlayer(gameplayLayer->getLocalPlayer());
				if(gameplayLayer->isSelected())	{
					if(vehicle->getPlayer() == nullptr)	{
						localPlayer.play(vehicle);
					}
				}
				else	{
					if(vehicle->getPlayer() != nullptr)	{
						vehicle->getPlayer()->play(nullptr);
					}
				}
			}
		}
		gameplayLayer->dispatchActorSpriteSinglePlayerAction(this);
	}

	// blends from client to server with blendFactor
	Vec3 interpolateToServerVector(const Vec3& clientVec, const Vec3& serverVec, float serverVecFactor)	{
		serverVecFactor = sl::max( sl::min(1.0f, serverVecFactor) , -1.0f);
		Vec3 blended;
		blended.setInterpolate3(clientVec, serverVec, serverVecFactor);
		return blended;
	}

	void ActorSprite::update(float dt)	{

		GameplayLayer* gameplayLayer(getGameplayLayer());
		if(gameplayLayer == nullptr)	{
			return;
		}

		// increment the local timebase
		// note: _accumtime has double precision
		_accumTime += dt;

		// retrieve the network topology
		Peer* peer(gameplayLayer->getPeer());
		ENetwokTopology networkTopology(peer != nullptr ? peer->getTopology() : SINGLEPLAYER);

		if(networkTopology == SINGLEPLAYER)	{
			updateSinglePlayer(dt);
		}

		// perform a preUpdate on all attached actor components
		// take a look inside the preUpdate implementation of the components
		GameActorNode* actorNode(dynamic_cast<GameActorNode*>(getParent()));
		if(actorNode != nullptr)	{
			actorNode->preUpdateComponents(dt);
		}

		// retrieve the actors vehicle if any
		// if an ActorSprite owns an entity it does not necessarily mean it owns a vehicle
		AbstractVehicle* vehicle(nullptr);
		if(getOwnsEntity())	{
			vehicle = getVehicle();
		}

		if(vehicle)	{
			// TODO @CP : vehicle update needs to take a double as _accumtime !
			//      later ...
			// note: this call is for demo purpose only
			//       we can easily update all vehicles in a row using one call
			//       in the game play layer
			//       this would also allow doing this in a different thread while
			//       rendering ...
			//       but for now keep things simple
			vehicle->update(osScalar(_accumTime), dt);

#if 0 // experimental render client interpolation
			float interpolationFactor( 0.9f * (1.0f / (dt * 25.0f)) );
			Vec3 interpolatedForward = interpolateToServerVector(_lastForward, vehicle->forward(), 
				interpolationFactor);
			interpolatedForward.normalize();
			const Vec3& forward(interpolatedForward);

			Vec3 interpolatedPosition = interpolateToServerVector(_lastPosition, vehicle->position(), 
				interpolationFactor);
			const Vec3& position(interpolatedPosition);
#else
			const Vec3& position(vehicle->position());
			const Vec3& forward(vehicle->forward());
#endif
			const Vec3& velocity(vehicle->velocity());

			// TODO @CP : a vehicle function would be nice here
			//      later ...
			float newRotation = atan2(-forward.y, forward.x) * 180 / SL_PI;
			newRotation += 90;
			// avoid negative rotations
			if(newRotation < 0.0f)	{
				newRotation += 360.0f;
			}

			setRotation(newRotation);

			_gamePosition.x = position.x;
			_gamePosition.y = position.y;

			// a little book keeping
			_lastForward = vehicle->forward();
			_lastPosition = vehicle->position();
		}

		updateVisualFromGamePosition(_gamePosition);

		// TODO @CP : make annotations an option
		//      later ...
		bool annotate(networkTopology != SINGLEPLAYER);
		if(annotate)	{
			if(getActorFlags().hasFlag(EActorFlag_IsTankProjectile))	{
				annotate = false;
			}
		}

		annotateSprite(dt, annotate);

		// perform a postUpdate on all attached actor components
		// take a look inside the postUpdate implementation of the components
		if(actorNode != nullptr)	{
			actorNode->postUpdateComponents(dt);
		}

		// finally update the base class implementation
		SLBaseClass::update(dt);
	}


	void ActorSprite::annotateSprite( float dt, bool annotate )	{
		if(annotate == false)	{
			// just hide the annotation label and exit
			_label->setVisible(false);
			return;
		}
		// retrieve the actors vehicle if any
		AbstractVehicle* vehicle(nullptr);
		if(getOwnsEntity())	{
			vehicle = getVehicle();
		}
		if(vehicle != nullptr)	{
			if(vehicle->getClassId() == SL_CID_PEDESTRIAN)	{
				// this vehicle is steering force driven
				// just hide the annotation label and exit
				_label->setVisible(false);
				return;
			}
			else	{
				AbstractPlayer* player(vehicle->getPlayer());
				if(player != nullptr)	{
					const Vec3& position(vehicle->position());
					const Vec3& forward(vehicle->forward());
					const Vec3& velocity(vehicle->velocity());

					float lr(0);
					float fb(0);

					const AbstractController* controller(player->getController());
					lr = controller->getActionValue(EControllerAction_Yaw);
					fb = controller->getActionValue(EControllerAction_Move);
					// not so interesting actually
					if((fb + lr) == 0.0f )	{
						// just hide the annotation label and exit
						_label->setVisible(false);
						return;
					}

					_label->setString(CCString::createWithFormat(
						"pos (%.02f, %.02f)\n"
						"rotation: %.02f\n"
						"velocity:%.03f\n"
						"ctrl yaw:%.03f move:%.03f"
						,
						position.x, position.y,
						getRotation(),
						velocity.length(), 
						lr, fb
						)->getCString());

					CCPoint labelPosition(0, -_label->getContentSize().height);
					_label->setRotation(360 - getRotation());
					_label->setPosition(labelPosition);
					_label->setVisible(true);
				}
			}
		}
	}


	GameplayLayer* ActorSprite::getGameplayLayer() const	{
		CCNode* parent(getParent());
		GameplayLayer* gameplayLayer(nullptr);
		while(parent != nullptr)	{
			gameplayLayer = dynamic_cast<GameplayLayer*>(parent);
			if(gameplayLayer == nullptr)	{
				parent = parent->getParent();
			}
			else	{
				break;
			}
		}
		return gameplayLayer;
	}

	void ActorSprite::setGamePosition( const CCPoint& gamePosition )	{
		AbstractVehicle* vehicle(getVehicle());
		if(vehicle)	{
			// very very simple update method
			vehicle->setPosition(Vec3(gamePosition.x, gamePosition.y, 0));
		}
		_gamePosition = gamePosition;
		updateVisualFromGamePosition(_gamePosition);
	}

	void ActorSprite::updateVisualFromGamePosition( const CCPoint &position )	{
		// we want 0,0 to be the center of the parent layer
		CCNode* parent(getGameplayLayer());
		if(parent)	{
			const CCSize& parentSize(parent->getContentSize());
			SLBaseClass::setPosition(CCPointMake(
				position.x + parentSize.width * 0.5f,
				position.y + parentSize.height * 0.5f)
				);
		}
		else	{
			SLBaseClass::setPosition(position);
		}
	}


}


