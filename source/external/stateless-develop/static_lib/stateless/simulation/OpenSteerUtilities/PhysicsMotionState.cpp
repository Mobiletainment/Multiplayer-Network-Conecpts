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

#include "PhysicsMotionState.h"

namespace OpenSteer
{

	//-------------------------------------------------------------------------
	bool PhysicsMotionState::readLocalSpaceData( const LocalSpaceData& localSpaceData )
	{
		if( writeLocalSpaceToTransform( localSpaceData, _worldTransform ) )
		{
			_linearVelocity = localSpaceData._linearVelocity;
			_angularVelocity = localSpaceData._angularVelocity;
			_linearVelocityScalar = _linearVelocity.length();
			_angularVelocityScalar = _angularVelocity.length();

			// in local space
			_localBasis = _worldTransform.getBasis().inverse();
			_localLinearVelocity = (_localBasis * _linearVelocity);
			_localAngularVelocity = (_localBasis * _angularVelocity);
			return true;
		}
		else
		{
			// this is a serious error what to do ?
			return false;
		}
	}

	//-------------------------------------------------------------------------
	void PhysicsMotionState::writeLocalSpaceData( AbstractLocalSpace& localSpace )
	{
		writeTransformToLocalSpace( _worldTransform, localSpace );
		localSpace.setAngularVelocity( _angularVelocity );
		localSpace.setLinearVelocity( _linearVelocity );
	}

	//-------------------------------------------------------------------------
	bool PhysicsMotionState::updateMotionState( AbstractVehicle* vehicle, const osScalar currentTime, 
		const osScalar elapsedTime
		)
	{
		if( vehicle == nullptr )	{
			return false;
		}
		return updateMotionState( vehicle->getLocalSpaceData(), currentTime, elapsedTime );
	}

	//-------------------------------------------------------------------------
	bool PhysicsMotionState::updateMotionState( const LocalSpaceData& localSpaceData, const osScalar currentTime, 
		const osScalar elapsedTime
		)
	{
		// store new world transform
		btTransform worldTransform1;
		if( writeLocalSpaceToTransform( localSpaceData, worldTransform1 ) )	{
			updateMotionState( worldTransform1, currentTime, elapsedTime );
			// open steer is always treating the forward vector
			// as linear velocity direction
			// so - right now add this little correction here ???
#if 0
			// nope ...
			_linearVelocity = localSpaceData._forward;
			_linearVelocity *= _linearVelocityScalar;
			_localLinearVelocity = Vec3.forward;
			_localLinearVelocity *= _linearVelocityScalar;
#endif
			return true;
		}
		else
		{
			// this is a serious error what to do ?
			return false;
		}
	}

	//-------------------------------------------------------------------------
	void PhysicsMotionState::updateMotionState( 
		const class btTransform& worldTransform1,
		const osScalar currentTime,
		const osScalar elapsedTime
		)
	{
		// do not do this twice
		if( _lastUpdateTime == currentTime )	{
			return;
		}
		_lastUpdateTime = currentTime;

		// just for readability
		const class btTransform& worldTransform0(_worldTransform);

		// compute motion state data
		// in world space
		bool doCalculation(true);
		if( elapsedTime == 0.0f )	{
			// do nothing in this case
			return;
		}
		else if( elapsedTime < 0.001f )	{
			// clamp very very small timeframes ?
			// elapsedTime = 0.001f;
			return;
		}

		calculateVelocity( worldTransform0, worldTransform1, elapsedTime,
			_linearVelocity, _angularVelocity );

		// in local space
		_localBasis = worldTransform1.getBasis().inverse();
		_localLinearVelocity = _localBasis * _linearVelocity;
		_localAngularVelocity = _localBasis * _angularVelocity;
		_linearVelocityScalar = _linearVelocity.length();
		_angularVelocityScalar = _angularVelocity.length();

		_worldTransform = worldTransform1;
	}

	//-------------------------------------------------------------------------
	void PhysicsMotionState::integrateMotionState( PhysicsMotionState& target, const osScalar elapsedTime, size_t steps ) const
	{
		PhysicsMotionState localMotionState = (*this);
		for( size_t ui = 0; ui < steps; ++ui )	{
			localMotionState.integrateMotionState( target, elapsedTime );
			localMotionState = target;
		}
	}

	void PhysicsMotionState::integrateMotionState( 
		const osScalar elapsedTime )	{
		integrateMotionState((*this), elapsedTime);
	}

	//-------------------------------------------------------------------------
	void PhysicsMotionState::integrateMotionState( 
		PhysicsMotionState& target, 
		const osScalar elapsedTime ) const
	{
		if( elapsedTime <= 0 )	{
			target = (*this);
			return;
		}

		// local variable in case
		// the target is the physics motion state itself
		btTransform targetWorldTransform;

		btTransformUtil::integrateTransform(
			_worldTransform,
			_linearVelocity,
			_angularVelocity,
			elapsedTime,
			targetWorldTransform );

		// apply the new transform to our target
		target._worldTransform = targetWorldTransform;

		// transform local velocities to new world space
		target._linearVelocity = target._worldTransform.getBasis() * _localLinearVelocity;
		target._angularVelocity = target._worldTransform.getBasis() * _localAngularVelocity;

		target._linearVelocityScalar = target._linearVelocity.length();
		target._angularVelocityScalar = target._angularVelocity.length();

		// compute new local space velocities
		target._localBasis = target._worldTransform.getBasis().inverse();

		// set local velocities to integrated transform
		target._localLinearVelocity = target._localBasis * target._linearVelocity;
		target._localAngularVelocity = target._localBasis * target._angularVelocity;
	}

} // end namespace OpenSteer

