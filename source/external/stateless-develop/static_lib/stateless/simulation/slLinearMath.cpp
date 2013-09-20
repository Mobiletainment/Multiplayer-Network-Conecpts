//!---------------------------------------------------------------------------
//! \file "slLinearMath.cpp"
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
#include "slLinearMath.h"



namespace sl	{


	void LinearMathUtil::testPhysicsMotionState()
	{
		PhysicsMotionState source;
		source._linearVelocity = Vec3(1,0,0);
		PhysicsMotionState target;
		source.integrateMotionState(target, 0.333f);
	}

	void LinearMathUtil::integrateMotionState( 
		SLTimeInterval dt, 
		const PhysicsMotionState& source, 
		PhysicsMotionState& target )
	{
		source.integrateMotionState(target, osScalar(dt));
	}


}


