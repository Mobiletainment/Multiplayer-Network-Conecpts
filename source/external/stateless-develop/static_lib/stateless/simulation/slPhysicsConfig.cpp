//!---------------------------------------------------------------------------
//! \file "slPhysicsConfig.cpp"
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
#include "slPhysicsConfig.h"
#include "slUpdatePeriod.h"


namespace sl	{


	//-------------------------------------------------------------------------
	// AbstractPhysicsConfig
	//-------------------------------------------------------------------------
	AbstractPhysicsConfig::AbstractPhysicsConfig()
		:_frameTime(0.0)
		,_frequency(0.0)
	{

	}

	//-------------------------------------------------------------------------
	// PhysicsConfig
	//-------------------------------------------------------------------------
	PhysicsConfig& PhysicsConfig::sharedInstance()
	{
		static PhysicsConfig physicsConfig;
		return physicsConfig;
	}


	PhysicsConfig::PhysicsConfig()
	{
		// defaulting to 50 hz
		setFrequency(50);
		// the saem but see the difference ...
		setFrameTime(0.02);
	}

	void PhysicsConfig::setFrameTime( SLTimeInterval frameTime )
	{
		if( frameTime == _frameTime )	{
			return;
		}

		_frameTime = max(0.0, frameTime);

		if( _frameTime > 0.0 )	{
			_frequency = 1.0 / _frameTime;;
		}
		else	{
			_frequency = DoubleMathLimits::getInfinity();
		}
	}

	void PhysicsConfig::setFrequency( SLDouble frequency )
	{
		if( frequency == _frequency )	{
			return;
		}

		_frequency = max(0.0, frequency);

		if( frequency > 0.0 )	{
			_frameTime = 1.0 / frequency;
		}
		else	{
			_frameTime = DoubleMathLimits::getInfinity();
		}
	}

}

