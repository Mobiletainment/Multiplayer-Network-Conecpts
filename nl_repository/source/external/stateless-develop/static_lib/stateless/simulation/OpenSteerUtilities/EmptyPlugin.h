#ifndef __EMPTYPLUGIN_H__
#define __EMPTYPLUGIN_H__

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

//-----------------------------------------------------------------------------
// implements a plugin doing nothing
#include "slUpdatePeriod.h"
#include "EduNetCommon/InstanceCount.h"

#include "OpenSteer/Plugin.h"
#include "OpenSteer/SteeringVehicle.h"
#include "OpenSteerUtilities/AbstractVehicleGroup.h"
#include "OpenSteerUtilities/PhysicsVehicleDeprecated.h"
#if 0
#include "OpenSteerUtilities/AbstractVehicleMotionStatePlot.h"
#endif

// anonymous namespace
namespace OpenSteer{


	class EmptyVehicle : public PhysicsVehicleDeprecated
	{
		ET_DECLARE_BASE( PhysicsVehicleDeprecated )
	public:

		EmptyVehicle():_entityId( EmptyVehicle::_instanceCount.constructor() )
		{
			reset();
		}

		virtual ~EmptyVehicle()
		{
			EmptyVehicle::_instanceCount.destructor();
		}

		// reset vehicle state
		virtual void reset (void)
		{
			BaseClass::reset();
			setMaxForce( 9 );
			// velocity is clipped to this magnitude
			setMaxSpeed( 9 );

			_UpdatePeriod.setPeriodTime( 0.5f );
		}

		// per frame simulation update
		virtual void update (const float currentTime, const float elapsedTime)
		{
			BaseClass::update( currentTime, elapsedTime );

			// test the UpdatePeriod
			size_t uiTicks = _UpdatePeriod.updateDeltaTime( elapsedTime );
			if( uiTicks >= 1 )
			{
				bool bTest = true;
				bTest  = false;
			}
		}

		// draw this character/vehicle into the scene
		virtual void draw (const float currentTime, const float elapsedTime)
		{
			drawBasic2dCircularVehicle (*this, gGray50);
		}
		sl::UpdatePeriodFloat _UpdatePeriod;
		static InstanceTracker _instanceCount;
		size_t _entityId;
		ET_IMPLEMENT_CLASS_NO_COPY( EmptyVehicle )
	};


	class EmptyPlugin : public Plugin
	{
		ET_DECLARE_BASE(Plugin)
	public:

		// construction / destruction
		EmptyPlugin (bool addToRegistry = true):BaseClass(addToRegistry),
			_showMotionStatePlot(0)
		{};
		virtual ~EmptyPlugin() {}

		virtual const char* name (void) const {return "EmptyPlugin";}

		virtual float selectionOrderSortKey (void) const {return 1.0f;}

		virtual void open (void);

		virtual void update (const float currentTime, const float elapsedTime);

		virtual void redraw (const float currentTime, const float elapsedTime);

		virtual void close (void)
		{
		}

		virtual void reset (void)
		{
			AbstractVehicleGroup vehicles( _Vehicles );
			vehicles.reset( );
		}

		const AVGroup& allVehicles (void) const {return _Vehicles;}
		AVGroup& allVehicles (void) {return _Vehicles;}

		void initGui( void* userdata );
#if 0
		AbstractVehicleMotionStatePlot _MotionStateProfile;
#endif
		int _showMotionStatePlot;
	private:
		AVGroup _Vehicles; // for allVehicles
		EmptyVehicle _Vehicle;
		ET_IMPLEMENT_CLASS_NO_COPY( EmptyPlugin )
	};
}
#endif //__EMPTYPLUGIN_H__