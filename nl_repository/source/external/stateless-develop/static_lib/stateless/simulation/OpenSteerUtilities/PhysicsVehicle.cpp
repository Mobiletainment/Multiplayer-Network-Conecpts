//!---------------------------------------------------------------------------
//! \file "PhysicsVehicle.cpp"
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


#include "OpenSteerUtilities/PhysicsVehicle.h"
#include "OpenSteerUtilities/AbstractVehicleMath.h"
#include "OpenSteer/AbstractPlayer.h"
#include "OpenSteer/AbstractController.h"
#include "OpenSteerUtilities/AbstractVehicleUtilities.h"

namespace OpenSteer
{
	//-----------------------------------------------------------------------------
	PhysicsVehicle::PhysicsVehicle()
		:_maxLocalLinearVelocity(64.0f)
	{
		_maxLocalAngularVelocity = Vec3::up;
		_maxLocalAngularVelocity *= 0.5f;
	}

	//-----------------------------------------------------------------------------
	PhysicsVehicle::~PhysicsVehicle()
	{

	}

	//-----------------------------------------------------------------------------
	void PhysicsVehicle::draw( const float currentTime, const float elapsedTime )
	{
		if( ( SteeringVehicle::getNearestMouseVehicle() == this ) )	{
			// highlight vehicle nearest mouse
			VehicleUtilities::highlightVehicleUtility( *this );
		}
		BaseClass::draw( currentTime, elapsedTime );
	}

	//-----------------------------------------------------------------------------
	void PhysicsVehicle::updateTick( const float accumTime, const float deltaTime )
	{
		float ctrlMove(0.0f);
		float ctrlRotate(0.0f);
		const AbstractController* controller(getController());
		if(controller != nullptr)	{
			ctrlMove = controller->getActionValue(EControllerAction_Move);
			ctrlRotate = controller->getActionValue(EControllerAction_Yaw);

			if(ctrlMove != 0.0f || ctrlRotate != 0.0f)	{
				// you might set a breakpoint here ...
				bool test(true);
				test = false;
			}
		}
		updateAngularVelocityYaw(ctrlRotate, deltaTime);
		updateLinearVelocity(ctrlMove, deltaTime);

		// integrate with given linear and angular velocity
		_internalMotionState.integrateMotionState(deltaTime);
	}

	//-----------------------------------------------------------------------------
	// per frame simulation update
	void PhysicsVehicle::update( const float currentTime, const float elapsedTime )
	{
		// prevent any updates with zero or negative delta time

		// prevent any updates where the elapsed time is > 5 sec
		// this might be a result of debugging
		// or is plainly very very bad

		if( ( elapsedTime <= 0.0f )	|| ( elapsedTime > 5.0f ) ) {
			return;
		}

		bool ticked(false);

		// just update the tick structure
		_physicsUpdateTick.setAnimationInterval(getPhysicsUpdateFrameTime());
		SLSize ticks(_physicsUpdateTick.update(elapsedTime, ticked));
		if(ticked == false)	{
			// just do nothing
			// a feature is missing here ...
			// state interpolation
			return;
		}

		// now read the internal motion state in case needed
		if(_dirtyLocalSpaceData)	{
			_internalMotionState.readLocalSpaceData(*this);
			_dirtyLocalSpaceData = false;
		}

		// set the updateTick counter to the right value
		// this is a very interesting value for networking
		// assuming the physics frame rate does not change

		_updateTicks = ticks;

		// following code should be stable enough to change the physics frame rate during runtime

		const float deltaTime(static_cast<float>(_physicsUpdateTick.getAnimationInterval()));

		SLSize deltaTicks(ticks - _physicsUpdateTick.getLastAnimationTick());
		float tickDeltaTime(static_cast<float>(deltaTicks) * deltaTime);

		float accumTime(static_cast<float>(_physicsUpdateTick.getTickedAccumTime()));
		accumTime -= tickDeltaTime;

		for(SLSize i(0); i < deltaTicks; ++i)	{
			updateTick(accumTime, deltaTime);
			accumTime += deltaTime;
		}

		// pass over the internal 'fixed frame time' motion state to the 'render state'
		_motionState = _internalMotionState;

		SLTimeInterval missedDeltaTime(_physicsUpdateTick.getAccumTime() - _physicsUpdateTick.getTickedAccumTime());
		if(missedDeltaTime != 0.0)	{
#if 0
			printf("Missed dt(%f)\n", missedDeltaTime);
#endif
			// integrate with given linear and angular velocity
			_motionState.integrateMotionState(static_cast<float>(missedDeltaTime));
		}
		
		// and finally write the motion state back to the local space
		_motionState.writeLocalSpaceData(*this);

		if( _proximityToken != NULL )	{
			// notify proximity database that our position has changed
			_proximityToken->updateForNewPosition( position() );
		}

		// annotation
		annotationVelocityAcceleration (5, 0);
		recordTrailVertex(static_cast<float>(_physicsUpdateTick.getAccumTime()), position());
	}

	//-------------------------------------------------------------------------
	const AbstractController* PhysicsVehicle::getController( void ) const	{
		AbstractPlayer* player(getPlayer());
		if( player != nullptr )	{
			return player->getController();
		}
		return nullptr;
	}

	//-------------------------------------------------------------------------
	void PhysicsVehicle::updateAngularVelocityYaw( float ctrlValue, float dt )	{
		// note: work in local space first
#if OPENSTEER_Z_ISUP
		float targetYawVelocity = ctrlValue * _maxLocalAngularVelocity.z * SL_TWO_PI;
		Vec3 newTargetLocalAngularVelocity(0,0,targetYawVelocity);
		Vec3 blended;
		float blendFactor = fabs(ctrlValue) < 0.01 ? dt * 3.0f : dt * 2.0f;
		blendFactor = max( min(1.0f, blendFactor) , -1.0f);
		blended.setInterpolate3(_internalMotionState._localAngularVelocity, newTargetLocalAngularVelocity, blendFactor);
		_internalMotionState._localAngularVelocity = blended;
#else
		assert(false);
#endif
		// now convert the local velocities to world space
		_internalMotionState._angularVelocity = _internalMotionState._worldTransform.getBasis() * _internalMotionState._localAngularVelocity;
	}

	//-------------------------------------------------------------------------
	void PhysicsVehicle::updateLinearVelocity( float ctrlValue, float dt )	{
		// note: work in local space first
#if OPENSTEER_Z_ISUP
		Vec3 newTargetLocalLinearVelocity(0,1,0);
		newTargetLocalLinearVelocity.y = _maxLocalLinearVelocity * ctrlValue;
		Vec3 currentLocalLinearVelocity = _internalMotionState._localLinearVelocity;
		Vec3 blended;
		float blendFactor = fabs(ctrlValue) < 0.01 ? dt * 3.0f : dt * 1.0f; 
		blendFactor = max( min(1.0f, blendFactor) , -1.0f);
		blended.setInterpolate3(currentLocalLinearVelocity, newTargetLocalLinearVelocity, blendFactor);
		_internalMotionState._localLinearVelocity = blended;
#else
		assert(false);
#endif
		// now convert the local velocities to world space
		_internalMotionState._linearVelocity = _internalMotionState._worldTransform.getBasis() * _internalMotionState._localLinearVelocity;
	}

}


