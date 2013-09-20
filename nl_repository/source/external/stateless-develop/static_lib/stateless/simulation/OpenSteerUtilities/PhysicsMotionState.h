#ifndef __PHYSICSMOTIONSTATE_H__
#define __PHYSICSMOTIONSTATE_H__

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

#include "OpenSteerUtilities/AbstractVehicleMath.h"


namespace OpenSteer
{
	typedef struct PhysicsMotionState_t PhysicsMotionState;

	typedef 
	struct PhysicsMotionState_t
	{
		PhysicsMotionState_t()
			:_linearVelocity(Vec3::zero)
			,_localLinearVelocity(Vec3::zero)
			,_angularVelocity(Vec3::zero)
			,_localAngularVelocity(Vec3::zero)
			,_linearVelocityScalar(0)
			,_angularVelocityScalar(0)
			,_lastUpdateTime(0)
		{
			_worldTransform.setIdentity();
			_localBasis = _worldTransform.getBasis().inverse();
		}
		btTransform _worldTransform;
		btMatrix3x3 _localBasis;
		Vec3 _linearVelocity;
		Vec3 _localLinearVelocity;
		Vec3 _angularVelocity;
		Vec3 _localAngularVelocity;
		float _linearVelocityScalar;
		float _angularVelocityScalar;
		osScalar _lastUpdateTime;

		void reset( void )
		{
			_worldTransform.setIdentity();
			_localBasis = _worldTransform.getBasis().inverse();
			_linearVelocity =
			_localLinearVelocity = 
			_angularVelocity =
			_localAngularVelocity = Vec3::zero;
			_linearVelocityScalar =
			_angularVelocityScalar =
			_lastUpdateTime = 0;
		}

		bool readLocalSpaceData( const LocalSpaceData& localSpaceData );
		void writeLocalSpaceData( AbstractLocalSpace& localSpace );

		bool updateMotionState( AbstractVehicle* vehicle, const osScalar currentTime, 
			const osScalar elapsedTime
			);

		bool updateMotionState( const LocalSpaceData& localSpaceData, const osScalar currentTime, 
			const osScalar elapsedTime
			);

		void updateMotionState( 
			const btTransform& worldTransform1,
			const osScalar currentTime,
			const osScalar elapsedTime
			);

		void integrateMotionState( const osScalar elapsedTime );
		void integrateMotionState( PhysicsMotionState& target, const osScalar elapsedTime ) const;
		void integrateMotionState( PhysicsMotionState& target, const osScalar elapsedTime, size_t steps ) const;

	} PhysicsMotionState;

}




#endif  // __PHYSICSMOTIONSTATE_H__
