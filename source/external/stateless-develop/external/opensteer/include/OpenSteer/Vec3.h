//-----------------------------------------------------------------------------
//
//
//! OpenSteer -- Steering Behaviors for Autonomous Characters
//
//! Copyright (c) 2002-2005, Sony Computer Entertainment America
//! Original author: Craig Reynolds <craig_reynolds@playstation.sony.com>
//
//! Permission is hereby granted, free of charge, to any person obtaining a
//! copy of this software and associated documentation files (the "Software"),
//! to deal in the Software without restriction, including without limitation
//! the rights to use, copy, modify, merge, publish, distribute, sublicense,
//! and/or sell copies of the Software, and to permit persons to whom the
//! Software is furnished to do so, subject to the following conditions:
//
//! The above copyright notice and this permission notice shall be included in
//! all copies or substantial portions of the Software.
//
//! THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//! IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//! FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
//! THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//! LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//! FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//! DEALINGS IN THE SOFTWARE.
//
//
//-----------------------------------------------------------------------------
//
//! Vec3: OpenSteer's generic type for 3d vectors
//
//! This file defines the class Vec3, which is used throughout OpenSteer to
//! manipulate 3d geometric data.  It includes standard vector operations (like
//! vector addition, subtraction, scale, dot, cross...) and more idiosyncratic
//! utility functions.
//
//! When integrating OpenSteer into a preexisting 3d application, it may be
//! important to use the 3d vector type of that application.  In that case Vec3
//! can be changed to inherit from the preexisting application' vector type and
//! to match the interface used by OpenSteer to the interface provided by the
//! preexisting 3d vector type.
//
//! 10-04-04 bk:  put everything into the OpenSteer namespace
//! 03-26-03 cwr: created to replace for Hiranabe-san's execellent but larger
//!               vecmath package (http://objectclub.esm.co.jp/vecmath/)
//
//-----------------------------------------------------------------------------


#ifndef OPENSTEER_VEC3_H
#define OPENSTEER_VEC3_H


#include "OpenSteer/OpenSteerMacros.h"
#include "OpenSteer/OpenSteerTypes.h"
#include "OpenSteer/Utilities.h"  //! for interpolate, etc.


// using the bullet linear math library
#if OS_HAVE_BULLETLINEARMATH
#  include "LinearMath/btVector3.h"
#endif


namespace OpenSteer {

	//-----------------------------------------------------------------------------
#if OS_HAVE_BULLETLINEARMATH
	template <typename T>
	struct wrapper : public T
	{
		wrapper() {}
		wrapper(const T& rhs) : T(rhs) {}
	};

	typedef btVector3 TVec3Base;
	class Vec3Base : public TVec3Base
	{
	public:
		//! three-dimensional Cartesian coordinates
		osScalar &x;
		osScalar &y;
		osScalar &z;

		Vec3Base( void ):
		x(m_floats[0]), y(m_floats[1]), z(m_floats[2])
		{

		}
	protected:
	private:
	};

	class Vec3 : public Vec3Base
	{
	public:

		//------------------------------------------ generic 3d vector operations

		//! constructors
		Vec3 (void) : Vec3Base()
		{
			// note: even though this is the most inefficient way to initialize a vector
			//       we keep this signature for compatibility
			x = y = z = OS_SCALAR(0.0);
		}

		Vec3 ( const Vec3& kOther );
		Vec3 (osScalar X, osScalar Y, osScalar Z);
		Vec3 (const btVector3& other);


#else // not OS_HAVE_BULLETLINEARMATH
	class Vec3
	{
	private:
//		btVector3 _data;
	public:

		//------------------------------------------ generic 3d vector operations

		//! three-dimensional Cartesian coordinates
		osScalar x, y, z;

		//! constructors
		Vec3 (void): x( 0.0f ), y( 0.0f ), z( 0.0f ) {}
		Vec3 ( const Vec3& kOther ) : x( kOther.x ), y( kOther.y ), z( kOther.z ) {}
		Vec3 (osScalar X, osScalar Y, osScalar Z) : x( X ), y( Y ), z( Z ) {}

		//! vector addition
		Vec3 operator+ (const Vec3& v) const {return Vec3 (x+v.x, y+v.y, z+v.z);}

		//! vector subtraction
		Vec3 operator- (const Vec3& v) const {return Vec3 (x-v.x, y-v.y, z-v.z);}

#endif // not OS_HAVE_BULLETLINEARMATH

		//! unary minus
		Vec3 operator- (void) const {return Vec3 (-x, -y, -z);}

		//! vector times scalar product (scale length of vector times argument)
		Vec3 operator* (const osScalar s) const {return Vec3 (x * s, y * s, z * s);}

		//! vector divided by a scalar (divide length of vector by argument)
		Vec3 operator/ (const osScalar s) const {return Vec3 (x / s, y / s, z / s);}

#if OS_HAVE_BULLETLINEARMATH
		// functions defined by bullet
#else
		//! dot product
		osScalar dot (const Vec3& v) const {return (x * v.x) + (y * v.y) + (z * v.z);}

		//! length
		osScalar length (void) const {return sqrtXXX (lengthSquared ());}

		//! normalized: returns normalized version (parallel to this, length = 1)
		Vec3 normalized (void) const
		{
			//! skip divide if length is zero
			const osScalar len = length ();
			return (len>0) ? (*this)/len : (*this);
		}

#endif

		//! length squared
		osScalar lengthSquared (void) const {return dot (*this);}

#if OS_HAVE_BULLETLINEARMATH
		// functions defined by bullet
#else

		//! normalize: returns normalized version (parallel to this, length = 1)
		const Vec3& normalize (void)
		{
			//! skip divide if length is zero
			const osScalar len = length ();
			if( len > 0 )
			{
				(*this) = (*this)/len;
			}
			return (*this);
		}

#endif

		//! cross product (modify "*this" to be A x B)
		//! [XXX  side effecting -- deprecate this function?  XXX]
		void cross(const Vec3& a, const Vec3& b)
		{
			*this = Vec3 ((a.y * b.z) - (a.z * b.y),
				(a.z * b.x) - (a.x * b.z),
				(a.x * b.y) - (a.y * b.x));
		}

		//! assignment
		const Vec3& operator= (const Vec3& v);
#if 0
		{
#if OS_HAVE_BULLETLINEARMATH
			// call placement new to initialize references
			::new( (void*) this ) ( Vec3 );
#endif
			x=v.x; y=v.y; z=v.z; 
			return *this;
		}
#endif

#if OS_HAVE_BULLETLINEARMATH
		// assignment
		const Vec3& operator= (const btVector3& v);
#if 0
		{
			// call placement new to initialize references
			::new( (void*) this ) ( Vec3 );
			x=v.x(); y=v.y(); z=v.z(); 
			return *this;
		}
#endif
#endif

		//! set XYZ coordinates to given three floats
		Vec3 set (const osScalar _x, const osScalar _y, const osScalar _z)
		{x = _x; y = _y; z = _z; return *this;}

		//! +=
		Vec3 operator+= (const Vec3& v) {return *this = (*this + v);}

		//! -=
		Vec3 operator-= (const Vec3& v) {return *this = (*this - v);}

		//! *=
		Vec3 operator*= (const osScalar& s) {return *this = (*this * s);}


		Vec3 operator/=( osScalar d ) { return *this = (*this / d);  }

		//! equality/inequality
		bool operator== (const Vec3& v) const {return x==v.x && y==v.y && z==v.z;}
		bool operator!= (const Vec3& v) const {return !(*this == v);}

		//! @todo Remove - use @c distance from the Vec3Utilitites header instead.
		//! XXX experimental (4-1-03 cwr): is this the right approach?  defining
		//! XXX "Vec3 distance (vec3, Vec3)" collided with STL's distance template.
		static osScalar distance (const Vec3& a, const Vec3& b){ return(a-b).length();}

		//---------------------------- utility member functions used in OpenSteer

		//! return component of vector parallel to a unit basis vector
		//! (IMPORTANT NOTE: assumes "basis" has unit magnitude (length==1))

		inline Vec3 parallelComponent (const Vec3& unitBasis) const
		{
			const osScalar projection = dot (unitBasis);
			return unitBasis * projection;
		}

		//! return component of vector perpendicular to a unit basis vector
		//! (IMPORTANT NOTE: assumes "basis" has unit magnitude (length==1))

		inline Vec3 perpendicularComponent (const Vec3& unitBasis) const
		{
			return (*this) - parallelComponent (unitBasis);
		}

		//! clamps the length of a given vector to maxLength.  If the vector is
		//! shorter its value is returned unaltered, if the vector is longer
		//! the value returned has length of maxLength and is paralle to the
		//! original input.

		Vec3 truncateLength (const osScalar maxLength) const
		{
			const osScalar maxLengthSquared = maxLength * maxLength;
			const osScalar vecLengthSquared = lengthSquared ();
			if (vecLengthSquared <= maxLengthSquared)
				return *this;
			else
				return (*this) * (maxLength / sqrtXXX (vecLengthSquared));
		}

		// note: CP addition
		Vec3 setLength (const osScalar newLength) const
		{
			if( OS_SCALAR( 0.0 ) == newLength )
			{
				return Vec3::zero;
			}
			else
			{
				const osScalar vecLengthSquared = lengthSquared ();
				if( vecLengthSquared > 0 )
				{
					return (*this) * (newLength / sqrtXXX (vecLengthSquared));
				}
				else
				{
					return Vec3::zero;
				}
			}
		}


		// note: CP addition
		// forces a 3d position onto the specified X,Y,Z (aka []=0) plane
		Vec3 setXtoZero (void) const {return Vec3 (0, y, z);}

		Vec3 setYtoZero (void) const {return Vec3 (x, 0, z);}

		Vec3 setZtoZero (void) const {return Vec3 (x, y, 0);}

		//! swaps Y and Z components of this vector
		void swapYZ (void) const {
			osScalar temp(getY());
			y = z;
			z = temp;
		}

		//! rotate this vector about the global Y (up) axis by the given angle
		Vec3 rotateAboutGlobalY (osScalar angle) const 
		{
			const osScalar s = sinXXX (angle);
			const osScalar c = cosXXX (angle);
			return Vec3 ((x * c) + (z * s),
				(y),
				(z * c) - (x * s));
		}

		//! version for caching sin/cos computation
		Vec3 rotateAboutGlobalY (osScalar angle, osScalar& sin, osScalar& cos) const 
		{
			//! is both are zero, they have not be initialized yet
			if (sin==0 && cos==0)
			{
				sin = sinXXX (angle);
				cos = cosXXX (angle);
			}
			return Vec3 ((x * cos) + (z * sin),
				(y),
				(z * cos) - (x * sin));
		}

		//! if this position is outside sphere, push it back in by one diameter

		Vec3 sphericalWrapAround (const Vec3& center, osScalar radius) const
		{
			const Vec3 offset = *this - center;
			const osScalar r = offset.length();
			if (r > radius)
				return *this + ((offset/r) * radius * -2);
			else
				return *this;
		}

		//! names for frequently used vector constants
		static const Vec3& zero;
		static const Vec3& side;
		static const Vec3& up;
		static const Vec3& forward;
	};


	const Vec3 gZero(0,0,0);
#if OPENSTEER_Z_ISUP
	const Vec3 gUp     (0,0,1);
	const Vec3 gForward(0,1,0);
	const Vec3 gSide   (1,0,0);
#else
	const Vec3 gUp     (0,1,0);
	const Vec3 gForward(0,0,1);
	const Vec3 gSide   (-1,0,0);
#endif


	//-----------------------------------------------------------------------------
	//! scalar times vector product ("osScalar * Vec3")


	inline Vec3 operator* (osScalar s, const Vec3& v) {return v*s;}


	//! return cross product a x b
	inline Vec3 crossProduct(const Vec3& a, const Vec3& b)
	{
		Vec3 result((a.y * b.z) - (a.z * b.y),
			(a.z * b.x) - (a.x * b.z),
			(a.x * b.y) - (a.y * b.x));
		return result;
	}


	//-----------------------------------------------------------------------------
	//! default character stream output method

#ifndef NOT_OPENSTEERDEMO  //! only when building OpenSteerDemo

	inline std::ostream& operator<< (std::ostream& o, const Vec3& v)
	{
		return o << "(" << v.x << "," << v.y << "," << v.z << ")";
	}

#endif //! NOT_OPENSTEERDEMO

	//-----------------------------------------------------------------------------
	//! Returns a position randomly distributed inside a sphere of unit radius
	//! centered at the origin.  Orientation will be random and length will range
	//! between 0 and 1


	Vec3 randomVectorInUnitRadiusSphere (void);


	//-----------------------------------------------------------------------------
	//! Returns a position randomly distributed on a disk of unit radius
	//! on the XZ (Y=0) plane, centered at the origin.  Orientation will be
	//! random and length will range between 0 and 1


	Vec3 randomVectorOnUnitRadiusXZDisk (void);


	//-----------------------------------------------------------------------------
	//! Returns a position randomly distributed on the surface of a sphere
	//! of unit radius centered at the origin.  Orientation will be random
	//! and length will be 1


	inline Vec3 randomUnitVector (void)
	{
		return randomVectorInUnitRadiusSphere().normalized();
	}


	//-----------------------------------------------------------------------------
	//! Returns a position randomly distributed on a circle of unit radius
	//! on the XZ (Y=0) plane, centered at the origin.  Orientation will be
	//! random and length will be 1


	inline Vec3 randomUnitVectorOnXZPlane (void)
	{
		return randomVectorInUnitRadiusSphere().setYtoZero().normalized();
	}

	inline Vec3 randomUnitVectorOnXYPlane (void)
	{
		return randomVectorInUnitRadiusSphere().setZtoZero().normalized();
	}


	//-----------------------------------------------------------------------------
	//! used by limitMaxDeviationAngle / limitMinDeviationAngle below


	Vec3 vecLimitDeviationAngleUtility (const bool insideOrOutside,
		const Vec3& source,
		const osScalar cosineOfConeAngle,
		const Vec3& basis);


	//-----------------------------------------------------------------------------
	//! Enforce an upper bound on the angle by which a given arbitrary vector
	//! diviates from a given reference direction (specified by a unit basis
	//! vector).  The effect is to clip the "source" vector to be inside a cone
	//! defined by the basis and an angle.


	inline Vec3 limitMaxDeviationAngle (const Vec3& source,
		const osScalar cosineOfConeAngle,
		const Vec3& basis)
	{
		return vecLimitDeviationAngleUtility (true, //! force source INSIDE cone
			source,
			cosineOfConeAngle,
			basis);
	}


	//-----------------------------------------------------------------------------
	//! Enforce a lower bound on the angle by which a given arbitrary vector
	//! diviates from a given reference direction (specified by a unit basis
	//! vector).  The effect is to clip the "source" vector to be outside a cone
	//! defined by the basis and an angle.


	inline Vec3 limitMinDeviationAngle (const Vec3& source,
		const osScalar cosineOfConeAngle,
		const Vec3& basis)
	{    
		return vecLimitDeviationAngleUtility (false, //! force source OUTSIDE cone
			source,
			cosineOfConeAngle,
			basis);
	}


	//-----------------------------------------------------------------------------
	//! Returns the distance between a point and a line.  The line is defined in
	//! terms of a point on the line ("lineOrigin") and a UNIT vector parallel to
	//! the line ("lineUnitTangent")


	inline osScalar distanceFromLine (const Vec3& point,
		const Vec3& lineOrigin,
		const Vec3& lineUnitTangent)
	{
		const Vec3 offset = point - lineOrigin;
		const Vec3 perp = offset.perpendicularComponent (lineUnitTangent);
		return perp.length();
	}


	//-----------------------------------------------------------------------------
	//! given a vector, return a vector perpendicular to it (note that this
	//! arbitrarily selects one of the infinitude of perpendicular vectors)


	Vec3 findPerpendicularIn3d (const Vec3& direction);


	//-----------------------------------------------------------------------------
	//! candidates for global utility functions
	//
	//! dot
	//! cross
	//! length
	//! distance
	//! normalized


	typedef wrapper<Vec3> Vec3Wrapped;

} //! namespace OpenSteer


//-----------------------------------------------------------------------------
#endif //! OPENSTEER_VEC3_H
