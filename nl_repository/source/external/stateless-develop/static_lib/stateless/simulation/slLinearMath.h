#pragma once
#ifndef __SLLINEARMATH_H__
#define __SLLINEARMATH_H__

//!---------------------------------------------------------------------------
//! \file "slLinearMath.h"
//! \brief <brief-doc-todo>  
//! \details <details-doc-todo>
//! \author Cyrus Preuss
//!
//! This file is part of the Stateless Games SDK
//! Usage of the Stateless Games SDK is subject to an appropriate license agreement.
//!
//! Copyright (c) 2012-2013 Cyrus Preuss. All rights reserved.
//!----------------------------------------------------------------------------




#include "slSimulationMacros.h"
#include "slSimulationEnums.h"
#include "slSimulationTypes.h"

#include "slBulletPhysics.h"

#include "OpenSteerUtilities/PhysicsMotionState.h"

//-----------------------------------------------------------------------------

namespace sl	{
	class LinearMathUtil	{
	public:

		static void testPhysicsMotionState();

		static void integrateMotionState(
			SLTimeInterval dt,
			const PhysicsMotionState& source, 
			PhysicsMotionState& target);
	};
}

#endif  // __SLLINEARMATH_H__
