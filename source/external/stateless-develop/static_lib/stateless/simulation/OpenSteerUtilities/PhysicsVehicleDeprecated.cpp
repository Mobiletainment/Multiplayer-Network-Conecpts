#include "stdafx.h"
//-----------------------------------------------------------------------------
// Copyright (c) 2011, Jan Fietz, Cyrus Preuss
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
// * Neither the name of EduNetGames nor the names of its contributors
//   may be used to endorse or promote products derived from this software
//   without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------
#include "OpenSteerUtilities/PhysicsVehicleDeprecated.h"
#include "OpenSteerUtilities/AbstractVehicleMath.h"
#include "OpenSteer/AbstractPlayer.h"
#include "OpenSteer/AbstractController.h"
#include "OpenSteerUtilities/AbstractVehicleUtilities.h"

namespace OpenSteer	{

	//-----------------------------------------------------------------------------
#pragma warning(push)
	// warning C4355: 'this' : used in base member initializer list
#pragma warning(disable: 4355) 
	PhysicsVehicleDeprecated::PhysicsVehicleDeprecated()
		:_eulerUpdate(this)
		,_steeringForceUpdate(this)
	{
	}
#pragma warning(pop)

	//-----------------------------------------------------------------------------
	PhysicsVehicleDeprecated::~PhysicsVehicleDeprecated()	{

	}

	//-----------------------------------------------------------------------------
	void PhysicsVehicleDeprecated::draw( const float currentTime, const float elapsedTime )	{
		if( ( SteeringVehicle::getNearestMouseVehicle() == this ) )
		{
			// highlight vehicle nearest mouse
			VehicleUtilities::highlightVehicleUtility( *this );
		}
		BaseClass::draw( currentTime, elapsedTime );
	}


	void PhysicsVehicleDeprecated::updateTick( const float accumTime, const float deltaTime )
	{
		// note: feature to allow customizing the vehicle behaviour
		AbstractUpdated* customUpdated(getCustomUpdated());
		if( customUpdated != nullptr )	{
			// in case the custom updater decides to call the base class
			// prevent infinite recursion, store the custom updater locally
			// and restore it once done with the update
			setCustomUpdated( nullptr );
			customUpdated->updateCustom( this, accumTime, deltaTime );
			setCustomUpdated( customUpdated );
			return;
		}

		{
			ET_PROFILE( updatePhysicsVehicle );
			if( this == SteeringVehicle::getSelectedVehicle() )	{
				if( isRemoteObject() == false )	{
					setAnnotationMode( EAnnotationMode_local );
				}
				else	{
					setAnnotationMode( EAnnotationMode_global );
				}
			}
			else	{
				setAnnotationMode( EAnnotationMode_global );
			}

			EEulerUpdateMode eulerUpdateMode(_eulerUpdate.getUpdateMode());
			switch( eulerUpdateMode )
			{
			case( EEulerUpdateMode_Accelerate ):
				{
					// compute steering forces
					_steeringForceUpdate.update( accumTime, deltaTime );
					osVector3 steeringForce = _steeringForceUpdate.getForce();
					_currentSteeringForce = steeringForce;
					_eulerUpdate.setForce( steeringForce );
				}
				break;
			case( EEulerUpdateMode_IntegrateMotionState ):
				{
				}
				break;
			}

			// integrate results
			_eulerUpdate.update( accumTime, deltaTime );

			// finally update the current motion state
			_eulerUpdate.updateMotionState( accumTime, deltaTime );

			// annotation
			annotationVelocityAcceleration (5, 0);
			recordTrailVertex (accumTime, position());

			if( _proximityToken != NULL )	{
				// notify proximity database that our position has changed
				_proximityToken->updateForNewPosition( position() );
			}
		}
	}

	//-----------------------------------------------------------------------------
	// per frame simulation update
	void PhysicsVehicleDeprecated::update( const float currentTime, const float elapsedTime )	{
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

	}
}


