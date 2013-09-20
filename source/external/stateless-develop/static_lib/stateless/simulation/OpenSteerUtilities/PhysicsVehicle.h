#pragma once
#ifndef __PHYSICSVEHICLE_H__
#define __PHYSICSVEHICLE_H__

//!---------------------------------------------------------------------------
//! \file "PhysicsVehicle.h"
//! \brief <brief-doc-todo>  
//! \details <details-doc-todo>
//! \author Cyrus Preuss
//!
//! This file is part of the Stateless Games SDK
//! Usage of the Stateless Games SDK is subject to an appropriate license agreement.
//!
//! Copyright (c) 2012-2013 Cyrus Preuss. All rights reserved.
//!----------------------------------------------------------------------------


#include "EduNetCore/EduNetMacros.h"
#include "OpenSteer/SteeringVehicle.h"
#include "OpenSteerUtilities/PhysicsMotionState.h"

#include "slPhysicsConfig.h"

//-----------------------------------------------------------------------------
namespace OpenSteer
{
	//-------------------------------------------------------------------------
	class PhysicsVehicle : public SteeringVehicle
	{
		ET_DECLARE_BASE( SteeringVehicle )
	public:
		PhysicsVehicle();
		virtual ~PhysicsVehicle();

		OS_IMPLEMENT_CLASSNAME( PhysicsVehicle )

		//---------------------------------------------------------------------------
		// extension
		void setMovesPlanar( bool movesPlanar )	{
			_movesPlanar = movesPlanar;
		}

		virtual void draw( const float currentTime, const float elapsedTime );
		virtual void update (const float currentTime, const float elapsedTime);

		SLTimeInterval getPhysicsUpdateFrameTime( void ) const;

		OS_SYNTHESIZE_PASS_BY_REF(float, _maxLocalLinearVelocity, MaxLocalLinearVelocity)
		OS_SYNTHESIZE_PASS_BY_REF(Vec3, _maxLocalAngularVelocity, MaxLocalAngularVelocity)
		OS_SYNTHESIZE(bool, _updateVelocities, UpdateVelocities)
	protected:

		const AbstractController* getController( void ) const;
		virtual void updateAngularVelocityYaw(float ctrlValue, float dt);
		virtual void updateLinearVelocity(float ctrlValue, float dt);

		PhysicsMotionState _motionState;
		PhysicsMotionState _internalMotionState;

		AnimationTick _physicsUpdateTick;
	private:
		void updateTick( const float accumTime, const float deltaTime );

	};

	EF_FORCEINLINE
	SLTimeInterval PhysicsVehicle::getPhysicsUpdateFrameTime( void ) const	{
		return PhysicsConfig::sharedInstance().getFrameTime();
	}

}


#endif // __PHYSICSVEHICLE_H__
