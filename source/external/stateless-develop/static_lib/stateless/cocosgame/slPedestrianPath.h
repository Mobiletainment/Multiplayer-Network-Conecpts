#pragma once
#ifndef __SLPEDESTRIANPATH_H__
#define __SLPEDESTRIANPATH_H__

//!---------------------------------------------------------------------------
//! \file "slPedestrianPath.h"
//! \brief <brief-doc-todo>  
//! \details <details-doc-todo>
//! \author Cyrus Preuss
//!
//! This file is part of the Stateless Games SDK
//! Usage of the Stateless Games SDK is subject to an appropriate license agreement.
//!
//! Copyright (c) 2012-2013 Cyrus Preuss. All rights reserved.
//!----------------------------------------------------------------------------



namespace OpenSteer {
	class PolylineSegmentedPathwaySingleRadius;
}

namespace sl	{
	class slCocosGame_API PedestrianPath	{
	public:
		static PolylineSegmentedPathwaySingleRadius* createTestPath( float scale );
	};
}


#endif //  __SLPEDESTRIANPATH_H__
