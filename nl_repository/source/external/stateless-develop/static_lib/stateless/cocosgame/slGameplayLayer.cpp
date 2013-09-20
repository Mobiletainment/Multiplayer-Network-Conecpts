//!---------------------------------------------------------------------------
//! \file "slGameplayLayer.cpp"
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
#include "slGameplayLayer.h"
#include "slPedestrianPath.h"
#include "actor/slGameActorNode.h"
#include "actor/slActorSprite.h"

namespace sl	{

	// callback function

	CCControlBase* createGameplayLayer( 
		const char* className, 
		const CCSize& preferredSize,
		CCScale9Sprite* backgroundSprite
		)	{
			GameplayLayer* gameplayLayer(GameplayLayer::create());
			gameplayLayer->setPreferredSize(preferredSize);
			gameplayLayer->needsLayout();
			return gameplayLayer;
	}


	CCControlBaseArray GameplayLayer::_gameplayLayers;
	InstanceCount GameplayLayer::_instanceCount;

	// a helper opject to deal with cocos2d garbage collection
	class ProximityDbInstance : public CCObject
	{
	public:
		CREATE_FUNC(ProximityDbInstance)

		virtual bool init() {return true;}

		SL_SYNTHESIZE(ProximityDatabase*, _proximityDatabase, ProximityDatabase)
	protected:
		ProximityDbInstance()
			:_proximityDatabase(nullptr)
		{

		}
		~ProximityDbInstance()	{
			SL_SAFE_DELETE( _proximityDatabase );
		}
	};


	GameplayLayer::GameplayLayer()
		:_osPlugin(nullptr)
		,_accumTime(0.0f)
		,_gameState(PersistentDictionary::create())
		,_proximityDatabase(nullptr)
		,_peer(nullptr)
		,_path(nullptr)
		,_drawVehicles(true)
	{
		_gameplayLayers.add(this);

		_gameState->retain();

		_serializeCtrlsTick.setAnimationInterval(1.0);

		getCtrlFlags().addFlag(ECtrlFlag_UseScissorTest);

		_layerIdx = _instanceCount.constructor();
		createProximityDatabase();
	}

	GameplayLayer::~GameplayLayer()
	{
		removeKeyObserver();
		SL_SAFE_DELETE(_path)
		SL_SAFE_DELETE(_osPlugin)

		destroyProximityDatabase();


		SL_SAFE_RELEASE(_gameState);

		_gameplayLayers.remove(this);
		_instanceCount.destructor();
	}

	void GameplayLayer::createPath()	{
		SL_SAFE_DELETE(_path)
		_path = PedestrianPath::createTestPath(20);
		getGameplayLayerFlags().addFlag(EGameplayLayerFlag_HasPath);
	}

	bool GameplayLayer::init()	{
		CCScale9Sprite* backgroundSprite(CCScale9Sprite::create("mp_background.png"));
		backgroundSprite->setInsetLeft(2);
		backgroundSprite->setInsetTop(2);
		backgroundSprite->setInsetRight(2);
		backgroundSprite->setInsetBottom(2);

		bool initialized(initWithBackGroundSprite(backgroundSprite));
		if(initialized)	{
			getCtrlFlags().addFlag(ECtrlFlag_DrawFocusRect);
		}

		setTouchEnabled(true);

		// this leads to update being called once per frame
		// before actors being updated
		scheduleUpdateWithPriority(-1000);

		return initialized;
	}

	void GameplayLayer::visit()	{
		SLBaseClass::visit();
	}

	void GameplayLayer::draw()
	{
		SLBaseClass::draw();

		// now draw everything attached plugins want to draw

		if(_path != nullptr)	{
			typedef PolylineSegmentedPathwaySingleRadius::size_type size_type;

			const CCSize& contentSize(getContentSize());

			const Vec3 offset(
				contentSize.width * 0.5f, 
				contentSize.height * 0.5f,
				0.0f
				);
			Color pathColor = gYellow;
			pathColor.setA(0.5f);
			// draw a line along each segment of path
			const PolylineSegmentedPathwaySingleRadius& path = *_path;
			size_type pointCount(path.pointCount());
			for (size_type i = 1; i < pointCount; ++i ) 	{
				drawLine (path.point( i )+offset, path.point( i-1)+offset , pathColor);
			}
			drawLine(path.point( 0 ) + offset, path.point( pointCount - 1) + offset , pathColor);
		}
	}



	void GameplayLayer::drawLine( 
		const Vec3& from, 
		const Vec3& to, 
		const Color& color ) const
	{
		ccColor4F colorF = ccc4f(color.r(), color.g(), color.b(), color.a());
		ccColor4B colorB = ccc4BFromccc4F(colorF);
		float lineWidth(3);

		glLineWidth( lineWidth);
		ccDrawColor4B(colorB.r, colorB.g, colorB.b, colorB.a);

		// 1 line
		CCPoint line[2] = {
			CCPoint(from.x,from.y),
			CCPoint(to.x,to.y)
		};

		ccDrawLine( line[0], line[1] );
	}


	void GameplayLayer::onKeyDown( CCObject* keyObject )	{
		unsigned char key(static_cast<unsigned char>(keyObject->m_uID));
		_localPlayer.getLocalController().keyboardFunc(key,0,0);
	}

	void GameplayLayer::onKeyUp( CCObject* keyObject )	{
		unsigned char key(static_cast<unsigned char>(keyObject->m_uID));
		_localPlayer.getLocalController().keyboardFuncUp(key,0,0);
	}

	void GameplayLayer::needsLayout()	{
		CCNode* parentNode(getParent());
		if(parentNode != nullptr)	{
			CCControlBase* parentControl(dynamic_cast<CCControlBase*>(parentNode));
			if(parentControl != nullptr)	{
				// check if this is a twinlayer
				// in this case we let the twin layer do the job
				CCControlTwinLayer* twinLayerControl(dynamic_cast<CCControlTwinLayer*>(parentControl));
				if(twinLayerControl == nullptr)	{
					CCSize effectiveSize(parentControl->getContentSize());
					effectiveSize.width -= parentControl->getHorizontalMargin() * 2;
					effectiveSize.height -= parentControl->getVerticalMargin() * 2;
					setPreferredSize(effectiveSize);
				}
			}
			else	{
				setPreferredSize(parentNode->getContentSize());
			}
			SLBaseClass::needsLayout();
		}
	}

	void GameplayLayer::layoutChildren( ELayoutMode layoutMode, bool resize /*= false*/ )
	{

	}

	bool GameplayLayer::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )	{
		if (isTouchInside(pTouch) )	{
//			CCLOG("CCControlBase::ccTouchBegan - isTouchInside");
			return true;
		}
		if (!isEnabled() || !isVisible() || !hasVisibleParents() )	{
			//			CCLOG("CCControlBase::ccTouchBegan");
		}
		// return true if we claim this touch
		// so we will receive further touch events
		return false;
	}

	void GameplayLayer::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )	{
		if (isTouchInside(pTouch) )	{
			if(isSelected() == false)	{
				_gameplayLayers.setSelected(false);
				setSelected(true);
			}
//			CCLOG("CCControlBase::ccTouchEnded inside");
		}
		else	{
//			CCLOG("CCControlBase::ccTouchEnded outside");
		}
	}

	void GameplayLayer::update( float dt )	{
		_accumTime += dt;

		// note: the _local player has a special
		//       feature to not use the local control value reader
		//       but in our case we will not use this feature
		//       as it makes things more complicated than needed
		//       so we let the local controller do it's job
		//       read keyboard input and be happy with this functionality
		_localPlayer.setUseLocalController(true);
		_localPlayer.update(static_cast<osScalar>(_accumTime), dt);

		// we need to do a little maintenance here
		// ActorNode children without sprites will not receive their
		// pre update and post update calls
		// so we have to care about this manually
		// hmmm think about it

		// first capture the children as some code might actually try to remove them from this layer
		CCObject* child(nullptr);
		CCArray* capturedChildren(CCArray::create());
		CCARRAY_FOREACH(getChildren(), child)
		{
			GameActorNode* gameActorNode(dynamic_cast<GameActorNode*>(child));
			if(gameActorNode != nullptr)	{
				if(gameActorNode->getActorSprite() == nullptr)	{
					capturedChildren->addObject(gameActorNode);
				}
			}
			else	{
				ActorNode* actorNode(dynamic_cast<ActorNode*>(child));
				if(actorNode != nullptr)	{
					capturedChildren->addObject(actorNode);
				}
			}
		}

		// now work on the captured children
		CCArray* updateChildren(CCArray::create());
		CCARRAY_FOREACH(capturedChildren, child)
		{
			ActorNode* actorNode(dynamic_cast<ActorNode*>(child));
			if(actorNode != nullptr)	{
				actorNode->preUpdateComponents(dt);
				updateChildren->addObject(actorNode);
			}
		}

		SLBaseClass::update(dt);

		CCARRAY_FOREACH(updateChildren, child)
		{
			GameActorNode* gameActorNode(dynamic_cast<GameActorNode*>(child));
			if(gameActorNode != nullptr)	{
				if(gameActorNode->getActorSprite() == nullptr)	{
					gameActorNode->postUpdateComponents(dt);
				}
			}
			else	{
				ActorNode* actorNode(dynamic_cast<ActorNode*>(child));
				if(actorNode != nullptr)	{
					actorNode->postUpdateComponents(dt);
				}
			}
		}

		updateSerialization(dt);
	}

	void GameplayLayer::updateSerialization(float dt)	{

		bool ticked(false);
		_serializeCtrlsTick.update(dt, ticked);
		if(ticked == false)	{
			return;
		}

		CCObject* child(nullptr);
		// iterate over serialize controls and deserialize
		SLSize ctrlIdx(0);
		CCControlBase* ctrl(getSerializeControls().getControlAt(ctrlIdx));
		while(ctrl != nullptr)	{
			ctrl->deserialize(ctrl->getDataSource());

			// a little special code here ...
			if(strcmp(ctrl->getValueKey(), SL_SERIALIZEKEY_VEHICLE_DEBUG_DRAW) == 0)
			{
				bool newDrawVehicles( Dictionary::getBool(getGameState()->getSubDictionary("vehicle_simulation"), SL_SERIALIZEKEY_VEHICLE_DEBUG_DRAW) );
				if(newDrawVehicles != _drawVehicles)	{
					// now iterate over all actors and set the draw vehicle flag

					CCARRAY_FOREACH(getChildren(), child)
					{
						GameActorNode* gameActorNode(dynamic_cast<GameActorNode*>(child));
						if(gameActorNode != nullptr)	{
							ActorSprite* actorSprite(gameActorNode->getActorSprite());
							if(actorSprite != nullptr)	{
								if(newDrawVehicles)	{
									actorSprite->getActorFlags().addFlag(EActorFlag_DrawVehicle);
								}
								else	{
									actorSprite->getActorFlags().removeFlag(EActorFlag_DrawVehicle);
								}
							}
						}
					}

					_drawVehicles = newDrawVehicles;
				}
			}

			++ctrlIdx;
			ctrl = getSerializeControls().getControlAt(ctrlIdx);
		}
	}

	void GameplayLayer::onEnter()	{
		SLBaseClass::onEnter();
	}

	void GameplayLayer::onExit()	{
		setPeer(nullptr);
		SLBaseClass::onExit();
	}


	void GameplayLayer::setSelected( bool value )	{
		if(isSelected() != value)	{
			SLBaseClass::setSelected(value);
			if(isSelected())	{
				addKeyObserver();
			}
			else	{
				removeKeyObserver();
			}
		}
	}

	void GameplayLayer::addKeyObserver()	{
		CCNotificationCenter* notificationCenter(CCNotificationCenter::sharedNotificationCenter());
		notificationCenter->addObserver(this, callfuncO_selector(GameplayLayer::onKeyDown), NL_NOTIFY_KEY_DOWN, nullptr );
		notificationCenter->addObserver(this, callfuncO_selector(GameplayLayer::onKeyUp), NL_NOTIFY_KEY_UP, nullptr );
	}

	void GameplayLayer::removeKeyObserver()	{
		CCNotificationCenter* notificationCenter(CCNotificationCenter::sharedNotificationCenter());
		notificationCenter->removeObserver(this, NL_NOTIFY_KEY_DOWN);
		notificationCenter->removeObserver(this, NL_NOTIFY_KEY_UP);

		// reset stored controller values
		_localPlayer.getLocalController().resetValues();
	}

	void GameplayLayer::createProximityDatabase()
	{
		// save pointer to old PD
		ProximityDatabase* oldPD(_proximityDatabase);

		// allocate new PD
		const int totalPD = 2;
		int cyclePD(0);
		switch (cyclePD = (cyclePD + 1) % totalPD)
		{
		case 0:
			{
				const osVector3 center;
				const float div = 20.0f;
				const osVector3 divisions (div, div, 1.0f);
				// need better way to get this
				const float diameter = 80.0f; 
				const osVector3 dimensions (diameter, diameter, diameter);
				typedef LQProximityDatabase<AbstractVehicle*> LQPDAV;
				_proximityDatabase = SL_NEW LQPDAV (center, dimensions, divisions);
				break;
			}
		case 1:
			{
				_proximityDatabase = SL_NEW BruteForceProximityDatabase<AbstractVehicle*> ();
				break;
			}
		}

		// delete old PD (if any)
		SL_SAFE_DELETE( oldPD );
	}

	void GameplayLayer::destroyProximityDatabase()
	{
		// note: destroying the proximity database right now can lead to crashes
		//       thats why we have this code here
		// TODO: @CP do better LATER
		ProximityDbInstance* defferedDestruct(ProximityDbInstance::create());
		defferedDestruct->setProximityDatabase(_proximityDatabase);
		_proximityDatabase = nullptr;
//		SL_SAFE_DELETE( _proximityDatabase );
	}

	void GameplayLayer::registerVehicle( AbstractVehicle* vehicle )
	{
		AbstractVehicleGroup group(allVehicles());
		group.addVehicle(vehicle, _proximityDatabase);

		CCDictionary* vehicleSimulationDictionary(getGameState()->getSubDictionary("vehicle_simulation"));
		Dictionary::setInteger(vehicleSimulationDictionary, SL_SERIALIZEKEY_NUM_VEHICLES, allVehicles().size());
	}

	void GameplayLayer::deregisterVehicle( AbstractVehicle* vehicle )
	{
		AbstractVehicleGroup group(allVehicles());
		vehicle->allocateProximityToken(nullptr);
		group.removeVehicle(vehicle);

		CCDictionary* vehicleSimulationDictionary(getGameState()->getSubDictionary("vehicle_simulation"));
		Dictionary::setInteger(vehicleSimulationDictionary, SL_SERIALIZEKEY_NUM_VEHICLES, allVehicles().size());
	}

	const AVGroup& GameplayLayer::allVehicles( void ) const	{
		return _allVehicles;
	}

	AVGroup& GameplayLayer::allVehicles( void )	{
		return _allVehicles;
	}


	GameplayLayer* GameplayLayer::getGameplayLayer(CCObject* child)	{
		CCNode* node(dynamic_cast<CCNode*>(child));
		if(node == nullptr)	{
			return nullptr;
		}

		CCNode* parent(node);
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

	void GameplayLayer::dispatchActorSpriteSinglePlayerAction( ActorSprite* actorSprite )	{
		// a hook you might implement to prepare game play functionality
	}



}


