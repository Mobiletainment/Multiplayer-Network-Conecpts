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

#include "VehicleUpdates.h"
#include "OpenSteer/GlobalData.h"


namespace OpenSteer
{
	//-----------------------------------------------------------------------------
	void EulerVehicleUpdate::setVehicle( AbstractVehicle* vehicle )
	{
		_vehicle = vehicle;
		if( _vehicle != NULL )	{
			// store current world transform
			writeLocalSpaceToTransform( *_vehicle, _motionState._worldTransform );
		}
	}

	//-----------------------------------------------------------------------------
	void EulerVehicleUpdate::integrateMotionState(
		const osScalar currentTime, const osScalar elapsedTime )
	{
		_smoothedLinearAcceleration = Vec3::zero;
		PhysicsMotionState motionState;
		PhysicsMotionState extrapolatedMotionState;
		motionState.readLocalSpaceData( vehicle().getLocalSpaceData() );

		if( motionState._linearVelocity.length() > vehicle().maxSpeed() )	{
			motionState._linearVelocity =
				motionState._linearVelocity.truncateLength( vehicle().maxSpeed() );
		}

		motionState.integrateMotionState(
			extrapolatedMotionState, elapsedTime );

		if( extrapolatedMotionState._linearVelocity.length() > vehicle().maxSpeed() )	{
			motionState._linearVelocity =
				extrapolatedMotionState._linearVelocity.truncateLength( vehicle().maxSpeed() );
		}

		extrapolatedMotionState.writeLocalSpaceData( vehicle() );
		//	vehicle().setSpeed( extrapolatedMotionState._linearVelocity.length() );
		// eliminate speed overshooting
	}

	//-----------------------------------------------------------------------------
	void EulerVehicleUpdate::update( const osScalar currentTime, const osScalar elapsedTime )
	{
		// only in case a custom has been set ?
		BaseClass::update( currentTime, elapsedTime );

		{
			ET_PROFILE( EulerVehicleUpdate );

			switch( getUpdateMode() )
			{
			case( EEulerUpdateMode_Accelerate ):
				{
					// compute acceleration and velocity
					const Vec3 newVelocity = updateLinearVelocity( currentTime, elapsedTime );

					// update Speed
					vehicle().setSpeed( newVelocity.length() );

					if( vehicle().speed() > 0 )	{
						// Euler integrate (per frame) velocity into position
						const osVector3 newPosition = vehicle().position() + ( newVelocity * elapsedTime );
						vehicle().setPosition( newPosition );

						// regenerate local space (by default: align vehicle's forward axis with
						// new velocity, but this behavior may be overridden by derived classes.)
						Vec3 newForward = newVelocity.normalized();
						vehicle().regenerateLocalSpace( newForward, elapsedTime );
					}
					else	{
						// maybe smth to turn at zero speed ?
					}
				}
				break;
			case( EEulerUpdateMode_IntegrateMotionState ):
				{
					integrateMotionState( currentTime, elapsedTime );
				}
				break;
			}
		}
	}

	//-----------------------------------------------------------------------------
	Vec3 EulerVehicleUpdate::updateLinearVelocity( const osScalar currentTime, const osScalar elapsedTime )
	{
		Vec3 newVelocity = vehicle().velocity();

		// damp out abrupt changes and oscillations in steering acceleration
		// (rate is proportional to time step, then clipped into useful range)
		if( elapsedTime > 0 )	{
			// compute acceleration and velocity
			Vec3 newAcceleration = ( getForce() / vehicle().mass() );

			const float smoothRate = clip (9 * elapsedTime, 0.15f, 0.4f);
			blendIntoAccumulator( smoothRate,
				newAcceleration,
				_smoothedLinearAcceleration );
			// Euler integrate (per frame) acceleration into velocity
			newVelocity += _smoothedLinearAcceleration * elapsedTime;

		}
		// enforce speed limit anyways
		// as the velocity might have been set from an external call
		newVelocity = newVelocity.truncateLength( vehicle().maxSpeed () );

		return newVelocity;
	}

	//-----------------------------------------------------------------------------
	void EulerVehicleUpdate::updateMotionState( const osScalar currentTime,
		const osScalar elapsedTime
		)
	{
		bool success =
			_motionState.updateMotionState( &vehicle(), currentTime, elapsedTime );
		if( success == false )	{
			// this is a serious error what to do ?
		}
		else	{
			if( getUpdateMode() == EEulerUpdateMode_IntegrateMotionState )	{
				// do not do anything in this case
				return;
			}

			vehicle().setAngularVelocity( _motionState._angularVelocity );

			Vec3 linearVelocity = _motionState._linearVelocity;
			// do not apply to high linear velocities !!!
			if( linearVelocity.length() > vehicle().maxSpeed() )	{
				linearVelocity = linearVelocity.truncateLength( vehicle().maxSpeed() );
			}

			vehicle().setLinearVelocity( linearVelocity );
		}
	}

	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------

	//-----------------------------------------------------------------------------
	SteeringForceVehicleUpdate::SteeringForceVehicleUpdate( AbstractVehicle* vehicle ):
	BaseClass( vehicle ),
		_force(Vec3::zero)
	{
		AbstractVehicleMath::compressFixedLengthVector(
			_force, _vehicle->maxForce (), _compressedForce );
	}

	//-----------------------------------------------------------------------------
	Vec3 SteeringForceVehicleUpdate::determineCombinedSteering( const osScalar elapsedTime ) const
	{
		ET_PROFILE( determineCombinedSteeringForce );

		const Vec3 force = vehicle().determineCombinedSteering( elapsedTime );
		Vec3 adjustedForce = vehicle().adjustRawSteeringForce( force, elapsedTime );

		// enforce limit on magnitude of steering force
		// for a 2d vehicle set y to zero
		if( vehicle().movesPlanar() )
		{
#if OPENSTEER_Z_ISUP
			float& forceCorrection = adjustedForce.z;
#else
			float& forceCorrection = adjustedForce.y;
#endif
			if( forceCorrection != 0 )
			{
				const float adjust = ::fabs( forceCorrection );
				forceCorrection = 0;
				float adjustedForceLength = adjustedForce.length();
				if( adjustedForceLength > 0 )
				{
					adjustedForce = adjustedForce / adjustedForceLength; // normalize
					adjustedForceLength += adjust; // adjust
					adjustedForce = adjustedForce * adjustedForceLength;
				}
			}
		}
		return adjustedForce.truncateLength( vehicle().maxForce () );
	}

	//-----------------------------------------------------------------------------
	void SteeringForceVehicleUpdate::setForce( const Vec3& force, bool compress )	{
		if( compress == true )	{
			AbstractVehicleMath::compressFixedLengthVector(
				force, vehicle().maxForce (), _compressedForce );

			// expand again to avoid inconsistencies
			AbstractVehicleMath::expandFixedLengthVector(
				_compressedForce, vehicle().maxForce (), _force );

			if( force.length() > 0.0f )	{
				_forceCompressionError = (force.length() - _force.length());
				_force = force;
			}
		}
		else	{
			_force = force;
		}
	}

	//-----------------------------------------------------------------------------
	void SteeringForceVehicleUpdate::update( 
		const osScalar currentTime, const osScalar elapsedTime )
	{
		// only in case a custom has been set ?
		BaseClass::update( currentTime, elapsedTime );

		{
			ET_PROFILE( SteeringForceVehicleUpdate );

			// this is not right as this is part of the physics engine
			// ...
			// 	if( vehicle().isRemoteObject() )
			// 	{
			// 		return;
			// 	}

			// in case this update is enabled
			if( isEnabled() )	{
				const Vec3 force = determineCombinedSteering( elapsedTime );
				const bool compress( vehicle().isRemoteObject() == false );
				setForce( force, compress );
			}
		}
	}
}

