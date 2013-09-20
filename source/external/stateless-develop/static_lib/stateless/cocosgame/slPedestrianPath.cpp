//!---------------------------------------------------------------------------
//! \file "slPedestrianPath.cpp"
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

#include "slPedestrianPath.h"

namespace OpenSteer	{


	//-----------------------------------------------------------------------------
	/**
	* Creates a path of the form of an eight. Data provided by Nick Porcino.
	*/
	PolylineSegmentedPathwaySingleRadius* createPath (const osVector3& offset, float scale = 1.0f)
	{
		PolylineSegmentedPathwaySingleRadius* path = NULL;
		//	const float pathRadius = 2.0 * scale;
		//	const float pathRadius = 1.0;
		const float pathRadius = 2.0f * scale;

		const PolylineSegmentedPathwaySingleRadius::size_type pathPointCount = 16;
		osVector3 pathPoints[pathPointCount] = {
			osVector3( -12.678730011f, 0.0144290002063f, 0.523285984993f ),
			osVector3( -10.447640419f, 0.0149269998074f, -3.44138407707f ),
			osVector3( -5.88988399506f, 0.0128290001303f, -4.1717581749f ),
			osVector3( 0.941263973713f, 0.00330199999735f, 0.350513994694f ),
			osVector3( 5.83484792709f, -0.00407700007781f, 6.56243610382f ),
			osVector3( 11.0144147873f, -0.0111180003732f, 10.175157547f ),
			osVector3( 15.9621419907f, -0.0129949999973f, 8.82364273071f ),
			osVector3( 18.697883606f, -0.0102310003713f, 2.42084693909f ),
			osVector3( 16.0552558899f, -0.00506500015035f, -3.57153511047f ),
			osVector3( 10.5450153351f, 0.00284500000998f, -9.92683887482f ),
			osVector3( 5.88374519348f, 0.00683500012383f, -8.51393127441f ),
			osVector3( 3.17790007591f, 0.00419700006023f, -2.33129906654f ),
			osVector3( 1.94371795654f, 0.00101799995173f, 2.78656601906f ),
			osVector3( -1.04967498779f, 0.000867999973707f, 5.57114219666f ),
			osVector3( -7.58111476898f, 0.00634300010279f, 6.13661909103f ),
			osVector3( -12.4111375809f, 0.0108559997752f, 3.5670940876f )
		};

		for( size_t i = 0; i < pathPointCount; ++i )
		{
			pathPoints[i] *= scale;
			pathPoints[i] += offset;
#if OPENSTEER_Z_ISUP
			pathPoints[i].swapYZ();
			pathPoints[i].z = 0.0f;
#endif
		}


		path = SL_NEW PolylineSegmentedPathwaySingleRadius (pathPointCount,
			pathPoints,
			pathRadius,
			true);//false);

		path->setStartPoint( pathPoints[0] );
		path->setEndPoint( pathPoints[pathPointCount-1] );
		return path;
	}

}

namespace sl	{
	//-----------------------------------------------------------------------------
	PolylineSegmentedPathwaySingleRadius* PedestrianPath::createTestPath( float scale )
	{
		return createPath( osVector3::zero, scale );
	}
}



