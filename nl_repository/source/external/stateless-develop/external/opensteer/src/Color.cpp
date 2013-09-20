//-----------------------------------------------------------------------------
//
//
// OpenSteer -- Steering Behaviors for Autonomous Characters
//
// Copyright (c) 2002-2005, Sony Computer Entertainment America
// Original author: Craig Reynolds <craig_reynolds@playstation.sony.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
//
//-----------------------------------------------------------------------------
#include "stdafx.h"

#include "OpenSteer/Color.h"
#include "OpenSteer/Vec3.h"

#include <cassert>

namespace OpenSteer	{


	const Color Color::_gBlack(0.0f, 0.0f, 0.0f);
	const Color Color::_gWhite(1.0f, 1.0f, 1.0f);

	const Color Color::_gRed(1.0f, 0.0f, 0.0f); 
	const Color Color::_gGreen(0.0f, 1.0f, 0.0f);
	const Color Color::_gBlue(0.0f, 0.0f, 1.0f);
	const Color Color::_gYellow(1.0f, 1.0f, 0.0f);
	const Color Color::_gCyan(0.0f, 1.0f, 1.0f);
	const Color Color::_gMagenta(1.0f, 0.0f, 1.0f);
	const Color Color::_gOrange(1.0f, 0.5f, 0.0f);

	const Color Color::_gDarkRed(0.5f, 0.0f, 0.0f);
	const Color Color::_gDarkGreen(0.0f, 0.5f, 0.0f);
	const Color Color::_gDarkBlue(0.0f, 0.0f, 0.5f);
	const Color Color::_gDarkYellow(0.5f, 0.5f, 0.0f);
	const Color Color::_gDarkCyan(0.0f, 0.5f, 0.5f);
	const Color Color::_gDarkMagenta(0.5f, 0.0f, 0.5f);
	const Color Color::_gDarkOrange(0.5f, 0.25f, 0.0f);

	const Color Color::_gGray10(0.1f);
	const Color Color::_gGray20(0.2f);
	const Color Color::_gGray30(0.3f);
	const Color Color::_gGray40(0.4f);
	const Color Color::_gGray50(0.5f);
	const Color Color::_gGray60(0.6f);
	const Color Color::_gGray70(0.7f);
	const Color Color::_gGray80(0.8f);


	Color::Color()
		: r_(1.0f), g_(1.0f), b_(1.0f), a_ (1.0f)
	{

	}


	Color::Color( float greyValue )
		: r_( greyValue ), g_( greyValue ), b_( greyValue ), a_ (1.0f)
	{

	}


	Color::Color( float rValue, float gValue, float bValue, float aValue )
		: r_( rValue ), g_( gValue ), b_( bValue ), a_( aValue )
	{

	}


	Color::Color( Vec3 const& vector )
		: r_( vector.x ), g_( vector.y ), b_( vector.z ), a_ (1.0f)
	{

	}



	float 
		Color::r() const
	{
		return r_;
	}


	float 
		Color::g() const
	{
		return g_;
	}


	float 
		Color::b() const
	{
		return b_;
	}


	float 
		Color::a() const
	{
		return a_;
	}



	void 
		Color::setR( float value )
	{
		r_ = value;
	}


	void 
		Color::setG( float value )
	{
		g_ = value;
	}


	void 
		Color::setB( float value )
	{
		b_ = value;
	}

	void 
		Color::setA( float value )
	{
		a_ = value;
	}

	void
		Color::set( float rValue, float gValue, float bValue, float aValue )
	{
		r_ = rValue;
		g_ = gValue;
		b_ = bValue;
		a_ = aValue;
	}


	Vec3 
		Color::convertToVec3() const
	{
		return Vec3( r_, g_, b_ );
	}


	Color& 
		Color::operator+=( Color const& other )
	{
		r_ += other.r_;
		g_ += other.g_;
		b_ += other.b_;
		return *this;
	}


	Color& 
		Color::operator-=( Color const& other )
	{
		r_ -= other.r_;
		g_ -= other.g_;
		b_ -= other.b_;
		return *this;    
	}


	Color& 
		Color::operator*=( float factor )
	{
		r_ *= factor;
		g_ *= factor;
		b_ *= factor;
		return *this;    
	}


	Color& 
		Color::operator/=( float factor )
	{
		assert( 0.0f != factor && "Division by zero." );
		return operator*=( 1.0f / factor );
	}

	Color 
		grayColor( float value )
	{
		return Color( value );
	}

	Color 
		operator+( Color const& lhs, Color const& rhs )
	{
		Color result( lhs );
		return result += rhs;
	}

	Color 
		operator-( Color const& lhs, Color const& rhs )
	{
		Color result( lhs );
		return result -= rhs;    
	}

	Color 
		operator*( Color const& lhs, float rhs )
	{
		Color result( lhs );
		return result *= rhs;
	}


	Color 
		operator*( float lhs, Color const& rhs )
	{
		return operator*( rhs, lhs );
	}

	Color 
		operator/( Color const& lhs, float rhs )
	{
		Color result( lhs );
		return result /= rhs;
	}

}






