#ifndef __VEHICLEUPDATES_H__
#define __VEHICLEUPDATES_H__

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

#include "AbstractVehicleUpdate.h"


namespace OpenSteer
{


	enum EEulerUpdateMode
	{
		EEulerUpdateMode_Accelerate,
		EEulerUpdateMode_IntegrateMotionState,
		EEulerUpdateMode_Count,
	};

	//-------------------------------------------------------------------------
	class EulerVehicleUpdate : public AbstractVehicleUpdate
	{
		OS_DECLARE_BASE(AbstractVehicleUpdate)
	public:
		EulerVehicleUpdate( AbstractVehicle* vehicle ):
		BaseClass( vehicle ),
			_smoothedLinearAcceleration(Vec3::zero),
			_updateMode( EEulerUpdateMode_Accelerate )
		{
		}
		virtual ~EulerVehicleUpdate(){}

		void setForce( const Vec3& force ){ _force = force; }
		const Vec3& getForce( void ) const { return _force; }
		const PhysicsMotionState& getMotionState( void ) const { return _motionState; }
		PhysicsMotionState& accessMotionState( void ) { return _motionState; }

		//---------------------------------------------------------------------
		virtual void update( const osScalar currentTime, const osScalar elapsedTime );

		virtual void setVehicle( AbstractVehicle* vehicle );

		//---------------------------------------------------------------------
		EEulerUpdateMode getUpdateMode( void ) const { return _updateMode; }
		void setUpdateMode( EEulerUpdateMode eMode ){ _updateMode = eMode; }
		void updateMotionState( const osScalar currentTime, const osScalar elapsedTime );
	private:

		void integrateMotionState( const osScalar currentTime, const osScalar elapsedTime );
		Vec3 updateLinearVelocity( const osScalar currentTime, const osScalar elapsedTime );

		Vec3 _force;
		PhysicsMotionState _motionState;
		Vec3 _smoothedLinearAcceleration;
		EEulerUpdateMode _updateMode;
	};

	//-------------------------------------------------------------------------
	class SteeringForceVehicleUpdate : public AbstractVehicleUpdate {
		OS_DECLARE_BASE(AbstractVehicleUpdate)
	public:
		SteeringForceVehicleUpdate( AbstractVehicle* vehicle );
		virtual ~SteeringForceVehicleUpdate(){}

		Vec3 determineCombinedSteering( const osScalar elapsedTime ) const;

		void setForce( const Vec3& force, bool compress = true );
		const Vec3& getForce( void ) const { return _force; }
		const CompressedVector& getCompressedForce( void ) const { return _compressedForce; }

		//---------------------------------------------------------------------
		virtual void update( const osScalar /*currentTime*/, const osScalar elapsedTime );

	private:
		Vec3 _force;
		CompressedVector _compressedForce;
		float _forceCompressionError;
	};

}




#endif  // __VEHICLEUPDATES_H__
