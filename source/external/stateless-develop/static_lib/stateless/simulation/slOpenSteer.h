//!---------------------------------------------------------------------------
//! \file "slOpenSteer.h"
//! \brief <brief-doc-todo>  
//! \details <details-doc-todo>
//! \author Cyrus Preuss
//!
//! This file is part of the Stateless Games SDK
//! Usage of the Stateless Games SDK is subject to an appropriate license agreement.
//!
//! Copyright (c) 2012-2013 Cyrus Preuss. All rights reserved.
//!----------------------------------------------------------------------------


#pragma once
#ifndef __SLOPENSTEER_H__
#define __SLOPENSTEER_H__


// predefine namespace
namespace OpenSteer	{

}

// allow namespace OpenSteer usage of
// - namespace sl
namespace OpenSteer	{
	using namespace sl;
}


#include "OpenSteer/Vec3.h"
#include "OpenSteer/Color.h"

#include "OpenSteer/Controller.h"
#include "OpenSteer/Entity.h"
#include "OpenSteer/Player.h"

#include "OpenSteer/Draw.h"
#include "OpenSteer/SteeringVehicle.h"
#include "OpenSteer/GlobalData.h"

#include "OpenSteer/PolylineSegmentedPathwaySingleRadius.h"


#include "OpenSteerUtilities/AbstractVehicleGroup.h"
#include "OpenSteerUtilities/AbstractVehicleUtilities.h"
#include "OpenSteerUtilities/LocalController.h"
#include "OpenSteerUtilities/PluginArray.h"
#include "OpenSteerUtilities/EntityFactory.h"
#include "OpenSteerUtilities/PhysicsVehicleDeprecated.h"
#include "OpenSteerUtilities/PhysicsVehicle.h"

#include "OpenSteerUtilities/OpenSteerUT.h"

//-----------------------------------------------------------------------------

#endif  // __SLOPENSTEER_H__
