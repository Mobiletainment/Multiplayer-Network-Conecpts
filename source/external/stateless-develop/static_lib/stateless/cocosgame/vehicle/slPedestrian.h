#pragma once
#ifndef __SLPEDESTRIAN_H__
#define __SLPEDESTRIAN_H__

//!---------------------------------------------------------------------------
//! \file "slPedestrian.h"
//! \brief <brief-doc-todo>  
//! \details <details-doc-todo>
//! \author Cyrus Preuss
//!
//! This file is part of the Stateless Games SDK
//! Usage of the Stateless Games SDK is subject to an appropriate license agreement.
//!
//! Copyright (c) 2012-2013 Cyrus Preuss. All rights reserved.
//!----------------------------------------------------------------------------


namespace sl	{
	//-----------------------------------------------------------------------------
	class Pedestrian : public PhysicsVehicleDeprecated
	{
		SL_DECLARE_BASE( PhysicsVehicleDeprecated )
	public:

		Pedestrian();
		virtual ~Pedestrian();

		SL_IMPLEMENT_CLASSNAME( Pedestrian )

		virtual void reset( void );
		virtual void update( const float currentTime, const float elapsedTime);
		virtual osVector3 determineCombinedSteering( const float elapsedTime );
		virtual void draw( const float currentTime, const float elapsedTime );

		virtual void annotatePathFollowing( const osVector3& future,
			const osVector3& onPath,
			const osVector3& target,
			const float outside );

		virtual void annotateAvoidCloseNeighbor( const AbstractVehicle& other,
			const float /*additionalDistance*/);

		virtual void annotateAvoidNeighbor (const AbstractVehicle& threat,
			const float /*steer*/,
			const osVector3& ourFuture,
			const osVector3& threatFuture );

		virtual void annotateAvoidObstacle( const float minDistanceToCollision );

		virtual AbstractVehicle* cloneVehicle( void ) const;

		virtual void setPath( PolylineSegmentedPathwaySingleRadius* _path )	{
			path = _path;
		}

		bool hasPath() const { return path!=NULL; } 

	private:


		AVGroup _neighbors;

		// path to be followed by this pedestrian
		// XXX Ideally this should be a generic Pathway, but we use the
		// XXX getTotalPathLength and radius methods (currently defined only
		// XXX on PolylinePathway) to set random initial positions.  Could
		// XXX there be a "random position inside path" method on Pathway?
		PolylineSegmentedPathwaySingleRadius* path;

		// direction for path following (upstream or downstream)
		int pathDirection;


	};

}




#endif //  __SLPEDESTRIAN_H__
