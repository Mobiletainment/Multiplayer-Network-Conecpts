#include "stdafx.h"
//-----------------------------------------------------------------------------
// Copyright (c) 2011, Jan Fietz, Cyrus Preuss
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without modification, 
// are permitted provided that the following conditions are met:
// 
// * Redistributions of source code must retain the above copyright notice, 
//   this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice, 
//   this list of conditions and the following disclaimer in the documentation 
//   and/or other materials provided with the distribution.
// * Neither the name of EduNetGames nor the names of its contributors
//   may be used to endorse or promote products derived from this software
//   without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON 
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

#include "OpenSteerUtilities/AbstractVehicleUtilities.h"
#include "OpenSteer/Renderer.h"

namespace OpenSteer	{
	//-----------------------------------------------------------------------------
	// draws a gray disk on the XZ plane under a given vehicle
	void 
		VehicleUtilities::highlightVehicleUtility( const AbstractVehicle& vehicle )
	{
		if( &vehicle != NULL )
		{
			if( vehicle.movesPlanar() )
			{
				drawXZDisk( vehicle.radius(), vehicle.position(), gGray60, 20 );
			}
			else
			{
				VehicleUtilities::circleHighlightVehicleUtility( vehicle );
			}
		}
	}

	//-----------------------------------------------------------------------------
	// draws a gray circle on the XZ plane under a given vehicle
	void 
		VehicleUtilities::circleHighlightVehicleUtility( const AbstractVehicle& vehicle )
	{
		if (&vehicle != NULL) drawXZCircle (vehicle.radius () * 1.1f,
			vehicle.position(),
			gGray60,
			20);
	}


	//-----------------------------------------------------------------------------
	// draw a box around a vehicle aligned with its local space
	// xxx not used as of 11-20-02
	void 
		VehicleUtilities::drawBoxHighlightOnVehicle (const AbstractVehicle& v,
		const Color& color)
	{
		if (&v)
		{
			const float diameter = v.radius() * 2;
			const Vec3 size (diameter, diameter, diameter);
			drawBoxOutline (v, size, color);
		}
	}

	//-----------------------------------------------------------------------------
	// draws a colored circle (perpendicular to view axis) around the center
	// of a given vehicle.  The circle's radius is the vehicle's radius times
	// radiusMultiplier.
	void 
		VehicleUtilities::drawCircleHighlightOnVehicle (const AbstractVehicle& v,
		const float radiusMultiplier,
		const Color& color)
	{
		if (&v)
		{
			const Vec3& cPosition = Camera::accessInstance().position();
			draw3dCircle  (v.radius() * radiusMultiplier,  // adjusted radius
				v.position(),                   // center
				v.position() - cPosition,       // view axis
				color,                          // drawing color
				20);                            // circle segments
		}
	}

	void 
		VehicleUtilities::selectNextVehicle (void)
	{
		if (SteeringVehicle::getSelectedVehicle() != NULL)
		{
			// get a container of all vehicles
			const AVGroup& all = Plugin::allVehiclesOfSelectedPlugin ();
			const AVIterator first = all.begin();
			const AVIterator last = all.end();

			// find selected vehicle in container
			AVIterator s = std::find (first, last, SteeringVehicle::getSelectedVehicle());
			if( s == last )
			{
				// if the search failed, use NULL
				SteeringVehicle::setSelectedVehicle( NULL );
				if( all.size() > 0 )
				{
					SteeringVehicle::setSelectedVehicle( all[0] );
				}
			}
			else
			{
				// normally select the next vehicle in container
				++s;
				if( s == last )
				{
					// if we are at the end of the container, select the first vehicle
					s = first;
				}
				SteeringVehicle::setSelectedVehicle( (*s) );
			}
		}
	}

	//-----------------------------------------------------------------------------
	// select vehicle nearest the given screen position (e.g.: of the mouse)
	void 
		VehicleUtilities::selectVehicleNearestScreenPosition (int x, int y)
	{
		SteeringVehicle::setSelectedVehicle( findVehicleNearestScreenPosition (x, y) );
	}

	//-----------------------------------------------------------------------------
	// Find the AbstractVehicle whose screen position is nearest the given window
	// coordinates, typically the mouse position.  Returns NULL if there are no
	// AbstractVehicles.
	//
	// This works by constructing a line in 3d space between the camera location
	// and the "mouse point".  Then it measures the distance from that line to the
	// centers of each AbstractVehicle.  It returns the AbstractVehicle whose
	// distance is smallest.
	//
	// xxx Issues: Should the distanceFromLine test happen in "perspective space"
	// xxx or in "screen space"?  Also: I think this would be happy to select a
	// xxx vehicle BEHIND the camera location.
	AbstractVehicle* 
		VehicleUtilities::findVehicleNearestScreenPosition( int x, int y )
	{
		// find the direction from the camera position to the given pixel
		const Vec3 direction = directionFromCameraToScreenPosition ( 
			x,y , static_cast<int>( drawGetWindowHeight() ) );

		// iterate over all vehicles to find the one whose center is nearest the
		// "eye-mouse" selection line
		float minDistance = FLT_MAX;       // smallest distance found so far
		AbstractVehicle* nearest = NULL;   // vehicle whose distance is smallest
		const AVGroup& vehicles = Plugin::allVehiclesOfSelectedPlugin();
		if (&vehicles == NULL)
		{
			return NULL;
		}

		for (AVIterator i = vehicles.begin(); i != vehicles.end(); i++)
		{
			// distance from this vehicle's center to the selection line:
			const float d = distanceFromLine ((**i).position(),
				Camera::accessInstance().position(),
				direction);

			// if this vehicle-to-line distance is the smallest so far,
			// store it and this vehicle in the selection registers.
			if (d < minDistance)
			{
				minDistance = d;
				nearest = *i;
			}
		}

		return nearest;
	}

}
