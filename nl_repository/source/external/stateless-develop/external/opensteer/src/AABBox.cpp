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
#include "stdafx.h"

#include "OpenSteer/AABBox.h"
#include "OpenSteer/AbstractVehicle.h"

#include "OpenSteer/Renderer.h"

//-----------------------------------------------------------------------------
namespace OpenSteer {

	AABBox::AABBox( const Vec3 &min, const Vec3& max ): _min( min ), _max( max )
	{
		Vec3 distance = _max - _min;
		distance *= 0.5f;
		_center = _min + distance;
		_extent = distance;
	}

	void AABBox::initializeWithCenterAndExtent( const Vec3 &center, const Vec3& extent )
	{
		_center = center;
		_extent = extent;
		_min = _center;
		_min -= _extent;
		_max = _center;
		_max += _extent;
	}

	bool AABBox::insideX(const Vec3& p) const 
	{
		if(p.x < _min.x || p.x > _max.x)	
			return false;
		return true;
	}

	bool AABBox::insideY(const Vec3& p) const 
	{
		if(p.y < _min.y || p.y > _max.y)	
			return false;
		return true;
	}

	bool AABBox::insideZ(const Vec3& p) const 
	{
		if(p.z < _min.z || p.z > _max.z)	
			return false;
		return true;
	}

	bool AABBox::inside( const Vec3& p ) const
	{
		if( insideX( p ) == false )
		{
			return false;
		}
		if( insideY( p ) == false )
		{
			return false;
		}
		if( insideZ( p ) == false )
		{
			return false;
		}
		return true;
	}

	bool AABBox::inside( const AbstractLocalSpace& localSpace ) const
	{
		const Vec3& position = localSpace.position();
		return inside(position);
	}

	bool AABBox::insideXZ( const Vec3& p ) const
	{
		if( insideX( p ) == false )
		{
			return false;
		}
		if( insideZ( p ) == false )
		{
			return false;
		}
		return true;
	}

	bool AABBox::insideXZ( const AbstractLocalSpace& localSpace ) const
	{
		const Vec3& position = localSpace.position();
		return insideXZ( position );
	}

	bool AABBox::insideXZWithRadius( const AbstractLocalSpace& localSpace ) const
	{
		const Vec3& position = localSpace.position();
		osScalar radius = 0.0f;
		const AbstractVehicle* vehicle = dynamic_cast<const AbstractVehicle*>(&localSpace);
		if( vehicle != nullptr )
		{
			radius = vehicle->radius();
		}
		if( 0 != radius )
		{
			// increase with radius
			Vec3 tempExtent( _extent );
 			tempExtent.x += radius;
			tempExtent.y += radius;
			tempExtent.z += radius;
			AABBox tempBox;
			tempBox.initializeWithCenterAndExtent( _center, tempExtent );
			return tempBox.insideXZ( position );
		}
		return insideXZ( position );
	}

	bool AABBox::insideXY( const Vec3& p ) const
	{
		if( insideX( p ) == false )
		{
			return false;
		}
		if( insideY( p ) == false )
		{
			return false;
		}
		return true;
	}

	bool AABBox::insideXY( const AbstractLocalSpace& localSpace ) const
	{
		const Vec3& position = localSpace.position();
		return insideXY( position );
	}

	bool AABBox::insideXYWithRadius( const AbstractLocalSpace& localSpace ) const
	{
		const Vec3& position = localSpace.position();
		osScalar radius = 0.0f;
		const AbstractVehicle* vehicle = dynamic_cast<const AbstractVehicle*>(&localSpace);
		if( vehicle != nullptr )
		{
			radius = vehicle->radius();
		}
		if( 0 != radius )
		{
			// increase with radius
			Vec3 tempExtent( _extent );
			tempExtent.x += radius;
			tempExtent.y += radius;
			tempExtent.z += radius;
			AABBox tempBox;
			tempBox.initializeWithCenterAndExtent( _center, tempExtent );
			return tempBox.insideXY( position );
		}
		return insideXY( position );
	}

	void AABBox::draw( const Color& color ) const 
	{
		Vec3 b,c;
		b = Vec3( _min.x, 0, _max.z);
		c = Vec3( _max.x, 0, _min.z);
		drawLineAlpha(_min, b, color, 1.0f);
		drawLineAlpha(b, _max, color, 1.0f);
		drawLineAlpha(_max, c, color, 1.0f);
		drawLineAlpha(c,_min, color, 1.0f);
	}

	void AABBox::draw( void ) const 
	{
		Vec3 b,c;
		b = Vec3( _min.x, 0, _max.z);
		c = Vec3( _max.x, 0, _min.z);
		Color color(1.0f,1.0f,0.0f);
		drawLineAlpha(_min, b, color, 1.0f);
		drawLineAlpha(b, _max, color, 1.0f);
		drawLineAlpha(_max, c, color, 1.0f);
		drawLineAlpha(c,_min, color, 1.0f);
	}

} //! namespace OpenSteer    



