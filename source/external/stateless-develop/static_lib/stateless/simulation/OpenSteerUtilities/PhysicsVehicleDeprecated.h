#ifndef __PHYSICSVEHICLEDEPRECATED_H__
#define __PHYSICSVEHICLEDEPRECATED_H__

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

#include "EduNetCore/EduNetMacros.h"
#include "OpenSteer/SteeringVehicle.h"
#include "OpenSteerUtilities/VehicleUpdates.h"

#include "slPhysicsConfig.h"

//-----------------------------------------------------------------------------
namespace OpenSteer
{
	//-------------------------------------------------------------------------
	class PhysicsVehicleDeprecated : public SteeringVehicle
	{
		ET_DECLARE_BASE( SteeringVehicle )
	public:
		PhysicsVehicleDeprecated();
		virtual ~PhysicsVehicleDeprecated();

		OS_IMPLEMENT_CLASSNAME( PhysicsVehicleDeprecated )

		//---------------------------------------------------------------------------
		// extension
		void setMovesPlanar( bool movesPlanar )	{
			_movesPlanar = movesPlanar;
		}

		const EulerVehicleUpdate& getEulerUpdate( void ) const	{
			return _eulerUpdate;
		}

		EulerVehicleUpdate& accessEulerUpdate( void )	{
			return _eulerUpdate;
		}

		const SteeringForceVehicleUpdate& getSteeringForceUpdate( void ) const	{
			return _steeringForceUpdate;
		}

		SteeringForceVehicleUpdate& accessSteeringForceUpdate( void )	{
			return _steeringForceUpdate;
		}

		virtual void draw( const float currentTime, const float elapsedTime );
		virtual void update (const float currentTime, const float elapsedTime);

		SLTimeInterval getPhysicsUpdateFrameTime( void ) const;
	protected:
		EulerVehicleUpdate _eulerUpdate;
		SteeringForceVehicleUpdate _steeringForceUpdate;

		AnimationTick _physicsUpdateTick;
	private:
		void updateTick( const float accumTime, const float deltaTime );

	};

	EF_FORCEINLINE
	SLTimeInterval PhysicsVehicleDeprecated::getPhysicsUpdateFrameTime( void ) const	{
		return PhysicsConfig::sharedInstance().getFrameTime();
	}

}


#endif // __PHYSICSVEHICLEDEPRECATED_H__