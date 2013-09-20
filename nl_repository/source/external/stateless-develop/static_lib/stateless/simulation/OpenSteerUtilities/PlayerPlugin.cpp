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

#include "PlayerPlugin.h"

//#include "EduNetCommon/EduNetDraw.h"
//#include "EduNetProfile/GraphPlot.h"
#include "OpenSteerUtilities/CameraPlugin.h"
#include "OpenSteerUtilities/GridPlugin.h"

#include "OpenSteer/Camera.h"

#include <math.h>

namespace OpenSteer	{
	//-----------------------------------------------------------------------------
	void PlayerPlugin::initGui( void* userdata )
	{
#if 0
		GLUI* glui = ::getRootGLUI();
		GLUI_Panel* pluginPanel = static_cast<GLUI_Panel*>( userdata );
		//	glui->add_statictext_to_panel( pluginPanel, "no options" );
		glui->add_checkbox_to_panel( pluginPanel, "Show Motionstate", &_showMotionStatePlot);
#endif
	}


	//-----------------------------------------------------------------------------
	void PlayerPlugin::open (void)
	{
		_Vehicle.reset();
		SteeringVehicle::setSelectedVehicle( &_Vehicle );
		_Vehicles.push_back( &_Vehicle );

		// initialize camera
		CameraPlugin::init2dCamera( *SteeringVehicle::getSelectedVehicle() );
		Camera::accessInstance().setPosition (
			10,
			CameraPlugin::camera2dElevation,
			10);
		Camera::accessInstance().fixedPosition.set( 40, 40, 40 );
	}

	//-----------------------------------------------------------------------------
	void PlayerPlugin::update (const float currentTime, const float elapsedTime)
	{
		if( isEnabled() == false )
		{
			return;
		}
		AbstractVehicleGroup vehicles( _Vehicles );
		vehicles.update( currentTime, elapsedTime );
		if( 0 == _showMotionStatePlot )
		{
			return;
		}
		if( SteeringVehicle::getSelectedVehicle() != NULL )
		{
#if 0
			// update motion state plot
			_MotionStateProfile.recordUpdate( SteeringVehicle::getSelectedVehicle(), currentTime, elapsedTime );
#endif
		}
	}

	//-----------------------------------------------------------------------------
	void PlayerPlugin::redraw (const float currentTime, const float elapsedTime)
	{
		if( isVisible() == false )
		{
			return;
		}
		if( SteeringVehicle::getSelectedVehicle() != NULL )
		{
			// update camera, tracking test vehicle
			CameraPlugin::updateCamera (currentTime, elapsedTime, *SteeringVehicle::getSelectedVehicle() );
			// draw "ground plane"
			GridPlugin::gridUtility( SteeringVehicle::getSelectedVehicle()->position() );
		}

		AbstractVehicleGroup vehicles( _Vehicles );
		vehicles.redraw( currentTime, elapsedTime );

		if( 0 != _showMotionStatePlot )
		{
			// draw motion state plot
			if( SteeringVehicle::getSelectedVehicle() != NULL )
			{
#if 0
				_MotionStateProfile.draw( currentTime );
#endif
			}
		}

	}

}

