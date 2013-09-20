//!---------------------------------------------------------------------------
//! \file "Controller.cpp"
//! \brief <brief-doc-todo>  
//! \details <details-doc-todo>
//! \author Cyrus Preuss
//!
//! This file is part of the Stateless Games SDK
//! Usage of the Stateless Games SDK is subject to an appropriate license agreement.
//!
//! Copyright (c) 2012-2013 Cyrus Preuss. All rights reserved.
//!----------------------------------------------------------------------------
#include "stdafx.h"

#include "OpenSteer/Controller.h"
#include "OpenSteer/Utilities.h"
#include "OpenSteer/GlobalData.h"
#include <sstream>

namespace OpenSteer	{
	//-----------------------------------------------------------------------------
	Controller::Controller()
	{
		reset();
	}

	//-----------------------------------------------------------------------------
	Controller::~Controller()
	{

	}

	//-----------------------------------------------------------------------------
	void Controller::update( const osScalar currentTime, const osScalar elapsedTime )
	{
		// note: this will also call a custom update in case present
		//       debug for the local player to see the effect
		BaseClass::update( currentTime, elapsedTime );

		float backwardForward = 
			getActionValue( EControllerAction_Forward ) - 
			getActionValue( EControllerAction_Backward );
		float leftRight = 
			getActionValue(EControllerAction_Left) - 
			getActionValue(EControllerAction_Right);

		backwardForward = clamp( backwardForward, -1.0f, 1.0f );
		leftRight = clamp( leftRight, -1.0f, 1.0f );
#if 0 // debugging
		if( (backwardForward + leftRight) != 0.0f)	{
			bool test(true);
			test = false;
		}
#endif

		// default implementation
		// pass backwardForward value to move axis
		setAxisValue(EControllerAction_Move,backwardForward);
		setActionValue(EControllerAction_Move,backwardForward);

		// pass leftRight value to yaw axis
		setAxisValue(EControllerAction_Yaw,leftRight);
		setActionValue(EControllerAction_Yaw,leftRight);
	}

	//-----------------------------------------------------------------------------
	void Controller::reset( void )
	{
		for( size_t i = 0; i < EAxis_Count; ++i )	{
			_axisMapped[i] = false;
			_axisValue[i] = 0.0f;
		}
		for( size_t i = 0; i < EControllerAction_Count; ++i )	{
			_axisMapping[i][0] = -1;
			_axisMapping[i][1] = -1;
			_actionValue[i] = 0.0f;
		}
	}

	//-----------------------------------------------------------------------------
	void Controller::resetValues( void )
	{
		for( size_t i = 0; i < EAxis_Count; ++i )	{
			_axisValue[i] = 0.0f;
		}
		for( size_t i = 0; i < EControllerAction_Count; ++i )	{
			_actionValue[i] = 0.0f;
		}
	}

	//-----------------------------------------------------------------------------
	void Controller::setAxisValue( size_t axis, float fValue )
	{
		if( axis < EAxis_Count )
		{
			fValue = clamp( fValue, -1.0f, 1.0f );
			// as we want to submit this stuff to the network we also need to
			// set the corresponding action value
			for( EControllerAction i = EControllerAction_First; i < EControllerAction_Count; i = (EControllerAction)(i + 1) )
			{
				if( _axisMapping[i][0] == axis )
				{
					setActionValue( i, fValue );
					break;
				}
				else if( _axisMapping[i][1] == axis )
				{
					setActionValue( i, fValue );
					break;
				}
			}
		}
	}

	//-----------------------------------------------------------------------------
	void Controller::setActionValue( EControllerAction action, float fValue )
	{
		EControllerAction eClampedAction = clamp( action, EControllerAction_First, EControllerAction_Last );
		if( eClampedAction == action )
		{
			_actionValue[action] = fValue;
		}
	}

	//-----------------------------------------------------------------------------
	void Controller::setAxisMapping( size_t axis, EControllerAction action, size_t uiIdx )
	{
		EControllerAction eClampedAction = clamp( action, EControllerAction_First, EControllerAction_Last );
		uiIdx = clamp( uiIdx, (size_t)0, (size_t)1 );
		if( ( axis < EAxis_Count ) && ( eClampedAction == action ) )
		{
			_axisMapping[action][uiIdx] = axis;
			_axisMapped[axis] = true;
		}
	}

	//-----------------------------------------------------------------------------
	bool Controller::hasAxisMapping( size_t axis ) const
	{
		if( axis < EAxis_Count )
		{
			return _axisMapped[axis];
		}
		return false;
	}

	//-----------------------------------------------------------------------------
	float Controller::getActionValue( EControllerAction action ) const
	{
		EControllerAction eClampedAction = clamp( action, EControllerAction_First, EControllerAction_Last );
		if( eClampedAction == action )
		{
			return _actionValue[action];
		}
		return 0.0f;
	}

}
