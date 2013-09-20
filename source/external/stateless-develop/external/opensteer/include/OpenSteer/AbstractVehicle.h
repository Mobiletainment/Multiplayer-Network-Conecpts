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
//
//! AbstractVehicle: pure virtual base class for generic steerable vehicles
//
//! 10-04-04 bk:  put everything into the OpenSteer namespace
//! 01-30-03 cwr: created 
//
//
//-----------------------------------------------------------------------------

#ifndef OPENSTEER_ABSTRACTVEHICLE_H
#define OPENSTEER_ABSTRACTVEHICLE_H


#include "OpenSteer/LocalSpace.h"
#include "OpenSteer/Proximity.h"


//! STL vector containers
#include <vector>


//-----------------------------------------------------------------------------

namespace OpenSteer {

	//-------------------------------------------------------------------------
	enum ESerializeDataType
	{
		ESerializeDataType_Position,
		ESerializeDataType_Forward,
		ESerializeDataType_Side,
		ESerializeDataType_Up,
		ESerializeDataType_Force,
		ESerializeDataType_Radius,
		ESerializeDataType_Speed,
		ESerializeDataType_Orientation,
		ESerializeDataType_CompressedOrientation1,
		ESerializeDataType_CompressedOrientation2,
		ESerializeDataType_CompressedForce,
		ESerializeDataType_AngularVelocity,
		ESerializeDataType_LinearVelocity,
		ESerializeDataType_UpdateTicks,
		ESerializeDataType_ControllerAction,
		ESerializeDataType_Count,
	};

	
	class AbstractVehicle;
	typedef AbstractProximityDatabase<AbstractVehicle*> ProximityDatabase;
	typedef AbstractTokenForProximityDatabase<AbstractVehicle*> ProximityToken;
	
    class AbstractVehicle : public AbstractEntityUpdatedLocalSpace 
    {
    public:
        virtual ~AbstractVehicle() { /* Nothing to do. */ }
        
        //! mass (defaults to unity so acceleration=force)
        virtual float mass (void) const OS_ABSTRACT;
        virtual float setMass (float) OS_ABSTRACT;

        //! size of bounding sphere, for obstacle avoidance, etc.
        virtual float radius (void) const OS_ABSTRACT;
        virtual float setRadius (float) OS_ABSTRACT;


        //! speed of vehicle  (may be faster than taking magnitude of velocity)
        virtual float speed (void) const OS_ABSTRACT;
        virtual float setSpeed (float) OS_ABSTRACT;

        //! groups of (pointers to) abstract vehicles, and iterators over them
        typedef std::vector<AbstractVehicle*> group;
        typedef group::const_iterator iterator;    

        //-----------------------------------------------------------------------
        //! XXX this vehicle-model-specific functionality functionality seems out
        //! XXX of place on the abstract base class, but for now it is expedient

        //! the maximum steering force this vehicle can apply
        virtual float maxForce (void) const OS_ABSTRACT;
        virtual float setMaxForce (float) OS_ABSTRACT;

        //! the maximum speed this vehicle is allowed to move
        virtual float maxSpeed (void) const OS_ABSTRACT;
        virtual float setMaxSpeed (float) OS_ABSTRACT;

		virtual float relativeSpeed (void) const OS_ABSTRACT;

		//! dp - added to support heterogeneous flocks
//		virtual void update(const float currentTime, const float elapsedTime) OS_ABSTRACT;
		//! CP ++
		virtual void draw( const float currentTime, const float elapsedTime ) OS_ABSTRACT;
		virtual void reset( void ) OS_ABSTRACT;
		virtual void allocateProximityToken( ProximityDatabase* pd ) OS_ABSTRACT;

		virtual void regenerateLocalSpace( const Vec3& newForward,
                                           const float elapsedTime) OS_ABSTRACT;
		
		virtual void applySteeringForce (const Vec3& force,
										 const float elapsedTime) OS_ABSTRACT;
		
        virtual Vec3 determineCombinedSteering (const float elapsedTime) OS_ABSTRACT;

        //! adjust the steering force passed to applySteeringForce.
        //! allows a specific vehicle class to redefine this adjustment.
        //! default is to disallow backward-facing steering at low speed.
        virtual Vec3 adjustRawSteeringForce (const Vec3& force,
                                             const float deltaTime) const OS_ABSTRACT;

		virtual bool movesPlanar( void ) const OS_ABSTRACT;

		virtual const Vec3& currentSteeringForce() const OS_ABSTRACT;
		//! CP --
   };


    //! more convenient short names for AbstractVehicle group and iterator
    typedef AbstractVehicle::group AVGroup;
    typedef AbstractVehicle::iterator AVIterator;

} //! namespace OpenSteer


//-----------------------------------------------------------------------------
#endif //! OPENSTEER_ABSTRACTVEHICLE_H
