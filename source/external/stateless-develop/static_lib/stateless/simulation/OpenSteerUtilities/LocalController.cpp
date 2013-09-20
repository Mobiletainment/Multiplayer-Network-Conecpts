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

#include "LocalController.h"
#include "OpenSteerUT.h"

namespace OpenSteer
{
	//-----------------------------------------------------------------------------
	LocalController::LocalController()
	{
		configure();
	}

	//-----------------------------------------------------------------------------
	LocalController::~LocalController()
	{

	}

	//-----------------------------------------------------------------------------
	void LocalController::configure( void )	{
		setAxisMapping( 'w', EControllerAction_Forward );
		setAxisMapping( 'a', EControllerAction_Left );
		setAxisMapping( 's', EControllerAction_Backward );
		setAxisMapping( 'd', EControllerAction_Right );
		setAxisMapping( 'k', EControllerAction_Kick );
		setAxisMapping( 'f', EControllerAction_Shoot );
		setAxisMapping( 9, EControllerAction_NextVehicle );
	}

	//-----------------------------------------------------------------------------
	bool LocalController::keyboardFunc( unsigned char key, int x, int y )	{
		key = tolower(key);
		LocalController& controller(*this);
		if( controller.hasAxisMapping( key ) == true )
		{
			controller.setAxisValue( key, 1.0f );
			return true;
		}
		return false;
	}

	//-----------------------------------------------------------------------------
	bool LocalController::keyboardFuncUp( unsigned char key, int x, int y )	{
		key = tolower(key);
		LocalController& controller(*this);
		if( controller.hasAxisMapping( key ) == true )
		{
			controller.setAxisValue( key, 0.0f );
			return true;
		}
		return false;
	}

	//-----------------------------------------------------------------------------
	void LocalController::updateCustom( 
		AbstractUpdated* parent, const osScalar currentTime, const osScalar elapsedTime )
	{
		// passes all local controller values to the target controller
		// so in this case the update custom function is used for serialization
		AbstractController* targetController = dynamic_cast<AbstractController*>(parent);
		if( targetController != nullptr )	{
			for( size_t i = 0; i < EControllerAction_Count; ++i )	{
				EControllerAction action(static_cast<EControllerAction>(i));
				// pass the action values from here to the actual 'in game' controller
				targetController->setActionValue( action, getActionValue( action ) );
			}
		}
	}


}
