#pragma once
#ifndef __SLSIMULATION_H__
#define __SLSIMULATION_H__

//!---------------------------------------------------------------------------
//! \file "slSimulation.h"
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

#include "slPhysicsConfig.h"
#include "slLinearMath.h"
#include "slUpdatePeriod.h"

#include "slOpenSteer.h"

// allow namespace sl usage of
// - namespace OpenSteer
namespace sl	{
	using namespace OpenSteer;
}

// allow namespace nl usage of
// - namespace OpenSteer
namespace nl	{
	using namespace OpenSteer;
}

#endif  // __SLSIMULATION_H__
