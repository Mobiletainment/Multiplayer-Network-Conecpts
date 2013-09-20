//!---------------------------------------------------------------------------
//! \file "slGameActor.cpp"
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
#include "slGameActor.h"
#include "slGameActorNode.h"

#include "slGameplayLayer.h"

#include "vehicle/slPedestrian.h"


namespace sl	{

	// note: a VehicleClassIdMixin mixes the functionality of getClassId into the
	//       corresponding vehicle implementation
	//       so you can have one 'physical' implementation for different classIds
	typedef VehicleClassIdMixin<PhysicsVehicle, SL_CID_PHYSICSVEHICLE> TPhysicsVehicle;
	typedef VehicleClassIdMixin<Pedestrian, SL_CID_PEDESTRIAN> TPedestrian;
	typedef VehicleClassIdMixin<PhysicsVehicle, SL_CID_TANK> TTankVehicle;
	typedef VehicleClassIdMixin<PhysicsVehicle, SL_CID_TANK_PROCECTILE> TTankProjectileVehicle;


	//-------------------------------------------------------------------------
	// EntityFactory
	//-------------------------------------------------------------------------

	class EntityFactory : public EntityFactoryArray
	{
		ET_DECLARE_BASE( EntityFactoryArray );
	public:
		EntityFactory();
		virtual ~EntityFactory();

		static EntityFactory& sharedInstance();
	private:
		TEntityFactory<TPhysicsVehicle> _physicsVehicleFactory;
		TEntityFactory<TPedestrian> _pedestrianFactory;
		TEntityFactory<TTankVehicle> _tankFactory;
		TEntityFactory<TTankProjectileVehicle> _tankProjecticeFactory;
	};

	EntityFactory::EntityFactory()	{
		addEntityFactory(&_physicsVehicleFactory);
		addEntityFactory(&_pedestrianFactory);
		addEntityFactory(&_tankFactory);
		addEntityFactory(&_tankProjecticeFactory);
	};

	EntityFactory::~EntityFactory()	{
	};

	EntityFactory& EntityFactory::sharedInstance()	{
		static EntityFactory instance;
		return instance;
	}

	//-------------------------------------------------------------------------
	// GameActor
	//-------------------------------------------------------------------------

	GameActor::GameActor()
		:_entity(nullptr)
		,_ownsEntity(false)
	{
		// initially we like debugging so we activate some flags
		getActorFlags().addFlag(EActorFlag_DrawVehicle);
	}

	GameActor::~GameActor()
	{
		if(_ownsEntity)	{
			if(_entity != nullptr)	{
				_entity->setUserobject(nullptr);
			}
			OS_SAFE_DELETE(_entity)
		}
	}

	GameActorNode* GameActor::getGameActorNode() const
	{
		return dynamic_cast<GameActorNode*>(getParent());
	}

	void GameActor::createOpenSteerEntity( EntityClassId classId )	{
		if(_ownsEntity)	{
			if(_entity != nullptr)	{
				_entity->setUserobject(nullptr);
			}
			OS_SAFE_DELETE(_entity)
		}
		_ownsEntity = false;

		_entity = EntityFactory::sharedInstance().createEntity(classId);

		if(_entity != nullptr)	{
			_entity->setUserobject(this);
			_ownsEntity = true;
		}
	}

	bool GameActor::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
	{
		if (!isTouchInside(pTouch) )
		{
			return false;
		}
		return true;
	}

	void GameActor::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
	{
		if (isTouchInside(pTouch) )	{
			setTouchEnabled(false);
			// note: getParent as the sprite is contained inside a master node
			getParent()->removeFromParent();
		}
	}

	bool GameActor::init()
	{
		SLBaseClass::init();
		setTouchEnabled(false);
		return true;
	}

	AbstractEntityFactory& GameActor::sharedOpensteerEntityFactory()	{
		return EntityFactory::sharedInstance();
	}

	void GameActor::onEnter()
	{
		SLBaseClass::onEnter();
	}

	void GameActor::onExit()
	{
		SLBaseClass::onExit();
	}


	//-------------------------------------------------------------------------
	// VehicleActor
	//-------------------------------------------------------------------------


	AbstractVehicle* VehicleActor::getVehicle() const
	{
		return dynamic_cast<AbstractVehicle*>(getEntity());
	}

	void VehicleActor::draw()
	{
		SLBaseClass::draw();
		drawVehicle();
	}

	void VehicleActor::drawVehicle()
	{
		if(getActorFlags().hasFlag(EActorFlag_DrawVehicle) == false)	{
			return;
		}

		AbstractVehicle* vehicle(getVehicle());
		if(vehicle == nullptr)	{
			return;
		}

		if(vehicle->isRemoteObject())	{
			return;
		}

		const CCSize& contentSize(getContentSize());

		ccColor4B color;
		float lineWidth(3);
		glLineWidth( lineWidth);

		// 1 line
		CCPoint line[1][2];

		// forward
		line[0][0] = CCPoint(contentSize.width * 0.5f,contentSize.height * 0.5f);
		line[0][1] = CCPoint(contentSize.width * 0.5f,contentSize.height * 0.5f + contentSize.height * 2.0f);
		color = ccc4(127,0,0,127);
		ccDrawColor4B(color.r, color.g, color.b, color.a);
		ccDrawLine( line[0][0], line[0][1] );

		// side
		const Vec3& side = Vec3::side;
		line[0][0] = CCPoint(contentSize.width * 0.5f,contentSize.height * 0.5f);
		line[0][1] = CCPoint(contentSize.width * 0.5f + side.x * contentSize.height * 2.0f,contentSize.height * 0.5f + side.y * contentSize.height * 2.0f);
		color = ccc4(0,0,127,127);
		ccDrawColor4B(color.r, color.g, color.b, color.a);
		ccDrawLine( line[0][0], line[0][1] );

		// steering force
		const Vec3& currentSteeringForce(vehicle->currentSteeringForce());
		Vec3 localSteeringForce;
		const float steeringForceLength(currentSteeringForce.length());
		if(steeringForceLength > 0)	{
			localSteeringForce = vehicle->localizeDirection(vehicle->currentSteeringForce());
			localSteeringForce.normalize();
			line[0][0] = CCPoint(contentSize.width * 0.5f,contentSize.height * 0.5f);
			line[0][1] = CCPoint(contentSize.width * 0.5f + localSteeringForce.x * contentSize.height * 2.0f,contentSize.height * 0.5f + localSteeringForce.y * contentSize.height * 2.0f);

			color = ccc4(127,0,127,127);

			ccDrawColor4B(color.r, color.g, color.b, color.a);
			ccDrawLine( line[0][0], line[0][1] );
		}
	}

	void VehicleActor::onEnter()
	{
		// debug entry
		SLBaseClass::onEnter();
		GameplayLayer* gameplayLayer(getGameplayLayer());
		if(gameplayLayer != nullptr)	{
			AbstractVehicle* vehicle(getVehicle());
			if(vehicle != nullptr)	{
				Pedestrian* pedestrian(dynamic_cast<Pedestrian*>(vehicle));
				if(pedestrian != nullptr)	{
					if(gameplayLayer->getPath() == nullptr)	{
						gameplayLayer->createPath();
					}
					pedestrian->setPath(gameplayLayer->getPath());
				}
				gameplayLayer->registerVehicle(vehicle);
			}
		}
	}

	void VehicleActor::onExit()
	{
		GameplayLayer* gameplayLayer(getGameplayLayer());
		if(gameplayLayer != nullptr)	{
			gameplayLayer->deregisterVehicle(getVehicle());
		}
		SLBaseClass::onExit();
	}




}


