//!---------------------------------------------------------------------------
//! \file "nlProtocolStructures.cpp"
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
#include "nlProtocolStructures.h"

namespace nl	{

	// note: this is just the very very basic version of clientside interpolation
	//       it looks way better if you spread this over a couple of frames
	//       say 3-5 physics frames (depends on the receive frequency

	// blends from client to server with blendFactor
	Vec3 ProtocolUtilities::interpolateNetVector(const Vec3& clientVec, const Vec3& serverVec, float serverVecFactor)	{
		serverVecFactor = sl::max( sl::min(1.0f, serverVecFactor) , -1.0f);
		Vec3 blended;
		blended.setInterpolate3(clientVec, serverVec, serverVecFactor);
		return blended;
	}

}

