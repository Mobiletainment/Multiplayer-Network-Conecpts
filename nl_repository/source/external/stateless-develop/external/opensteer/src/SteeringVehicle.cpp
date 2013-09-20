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
//
//
// SteeringVehicle
//
// A steerable point mass with a velocity-aligned local coordinate system.
// SteeringVehicle is useful for developing prototype vehicles in OpenSteerDemo,
// it is the base class for vehicles in the PlugIns supplied with OpenSteer.
// Note that SteeringVehicle is provided only as sample code.  Your application
// can use the OpenSteer library without using SteeringVehicle, as long as you
// implement the AbstractVehicle protocol.
//
// SteeringVehicle makes use of the "mixin" concept from OOP.  To quote
// "Mixin-Based Programming in C++" a clear and helpful paper by Yannis
// Smaragdakis and Don Batory (http://citeseer.nj.nec.com/503808.html):
//
//     ...The idea is simple: we would like to specify an extension without
//     predetermining what exactly it can extend. This is equivalent to
//     specifying a subclass while leaving its superclass as a parameter to be
//     determined later. The benefit is that a single class can be used to
//     express an incremental extension, valid for a variety of classes...
// 
// In OpenSteer, vehicles are defined by an interface: an abstract base class
// called AbstractVehicle.  Implementations of that interface, and related
// functionality (like steering behaviors and vehicle physics) are provided as
// template-based mixin classes.  The intent of this design is to allow you to
// reuse OpenSteer code with your application's own base class.
//
// 10-04-04 bk:  put everything into the OpenSteer namespace
// 01-29-03 cwr: created
//
//
//-----------------------------------------------------------------------------
#include "stdafx.h"


#include "OpenSteer/SteeringVehicle.h"
#include "OpenSteer/GlobalSelection.h"
#include <algorithm>
#include <iomanip>
#ifndef NOT_OPENSTEERDEMO  //! only when building OpenSteerDemo
#include "OpenSteer/Renderer.h"
#endif //! NOT_OPENSTEERDEMO
#include "OpenSteer/Camera.h"
#include "OpenSteer/GlobalData.h"

namespace OpenSteer	{

	//-----------------------------------------------------------------------------
	// currently selected vehicle.  Generally the one the camera follows and
	// for which additional information may be displayed.  Clicking the mouse
	// near a vehicle causes it to become the Selected Vehicle.
	// GlobalData _collect3DAnnotations
	//-----------------------------------------------------------------------------
	// constructor
	SteeringVehicle::SteeringVehicle (void)
		:_movesPlanar(true)
		,_enabled(true)
		,_customUpdated( nullptr )
		,_proximityToken( nullptr )
	{
		// set inital state
		reset ();

		// note: do not set a global selected vehicle in case there is none
		//       a vehicle is not nessecarily part of the current scene
		// 	if( SteeringVehicle::getSelectedVehicle() == nullptr )
		// 	{
		// 		SteeringVehicle::selectedVehicle = this;
		// 	}
	}


	//-----------------------------------------------------------------------------
	// destructor
	SteeringVehicle::~SteeringVehicle (void)
	{
		// cleanup the proximity token in case there is one
		allocateProximityToken( nullptr );
		setParentEntity( nullptr );
		if( this == SteeringVehicle::getSelectedVehicle() )
		{
			SteeringVehicle::setSelectedVehicle( nullptr );
		}
		if( this == SteeringVehicle::getNearestMouseVehicle() )
		{
			SteeringVehicle::setNearestMouseVehicle( nullptr );
		}
		if( this == Camera::getLocalSpaceToTrack() )
		{
			Camera::setLocalSpaceToTrack( nullptr );
		}
	}

	//-----------------------------------------------------------------------------
	void SteeringVehicle::setSelectedVehicle( AbstractVehicle* vehicle )
	{
		GlobalSelection::setSelectedVehicle( vehicle );
	}

	//-----------------------------------------------------------------------------
	void SteeringVehicle::setNearestMouseVehicle( AbstractVehicle* vehicle )
	{
		GlobalSelection::setNearestMouseVehicle( vehicle );
	}

	//-----------------------------------------------------------------------------
	AbstractVehicle* SteeringVehicle::getSelectedVehicle( void )
	{
		return GlobalSelection::getSelectedVehicle();
	}

	//-----------------------------------------------------------------------------
	AbstractVehicle* SteeringVehicle::getNearestMouseVehicle( void )
	{
		return GlobalSelection::getNearestMouseVehicle();
	}

	//-----------------------------------------------------------------------------
	void SteeringVehicle::collect3DTextAnnotation( std::ostringstream& stream )
	{
		stream << "#"
			<< name()
			<< "-"
			<< getEntityId()
			<< std::endl;
		const char* spacer = "      ";
		stream << spacer << "speed: " << (speed() * 3.6f) << "km/h" << std::endl;
		//	annote << spacer << "2: cam dist: " << camDistance << std::endl;

	}

	//-----------------------------------------------------------------------------
	void SteeringVehicle::draw( const float /*currentTime*/, const float /*elapsedTime*/ ) 
	{
		//	if (&selected && &nearMouse && annotationIsOn())
		//	if( SteeringVehicle::getSelectedVehicle() == this )
		bool annotate = ( ( SteeringVehicle::getNearestMouseVehicle() == this ) && isAnnotated() );
		if( annotate == false )
		{
			// maybe this vehicle is close to the nearest mouse vehicle
			AbstractVehicle* vehicle = this;
			const float nearDistance = 6;
			const Vec3& vp = vehicle->position();
			if( SteeringVehicle::getNearestMouseVehicle() != nullptr )
			{
				const Vec3& np = SteeringVehicle::getNearestMouseVehicle()->position();
				if( (Vec3::distance (vp, np) < nearDistance) )
				{
					annotate = true;
				}
			}
			if( annotate == false )
			{
				if( SteeringVehicle::getSelectedVehicle() != nullptr )
				{
					const Vec3& np = SteeringVehicle::getSelectedVehicle()->position();
					if( (Vec3::distance (vp, np) < nearDistance) )
					{
						annotate = true;
					}
				}
			}
		}
		if( ( annotate == true ) && ( GlobalData::getInstance()->_collect3DAnnotations != 0 ) )
		{
			std::ostringstream stream;
			stream << std::setprecision(2) << std::setiosflags(std::ios::fixed);
			collect3DTextAnnotation( stream );
			stream	<< std::ends;

			Color textColor (0.8f, 1, 0.8f);
			Vec3 textOffset (0, 0.25f, 0);
			if( isRemoteObject() )
			{
				textColor.setR( 1.0f );
				textColor.setG( 0.70f );
				textColor.setB( 0.70f );
				textColor.setA( 0.95f );
			}
			else
			{
				textColor.setR( 0.20f );
				textColor.setG( 1.00f );
				textColor.setB( 0.20f );
				textColor.setA( 0.95f );
				textOffset.y += 1.0f;
			}
			const Vec3 textPos = position() + textOffset;
			draw2dTextAt3dLocation( 
				stream, textPos, textColor, drawGetWindowWidth(), drawGetWindowHeight() );

			stream.flush();
		}
	};

	//-----------------------------------------------------------------------------
	// switch to new proximity database
	void SteeringVehicle::allocateProximityToken( ProximityDatabase* proximityDatabase ) 
	{
		// delete this vehicle's token in the old proximity database
		OS_SAFE_DELETE( _proximityToken );
		if( proximityDatabase != nullptr )	{
			// allocate a token for this vehicle in the proximity database
			_proximityToken = proximityDatabase->allocateToken( this );
			if( _proximityToken != nullptr )
			{
				// notify proximity database that our position has initially changed
				_proximityToken->updateForNewPosition( position() );
			}
		}
	}

	//-----------------------------------------------------------------------------
	// adjust the steering force passed to applySteeringForce.
	//
	// allows a specific vehicle class to redefine this adjustment.
	// default is to disallow backward-facing steering at low speed.
	//
	// xxx should the default be this ad-hocery, or no adjustment?
	// xxx experimental 8-20-02
	//
	// parameter names commented out to prevent compiler warning from "-W"
	Vec3 
		SteeringVehicle::adjustRawSteeringForce (const Vec3& force,
		const float /* deltaTime */) const
	{
		const float maxAdjustedSpeed = 0.2f * maxSpeed ();

		if ((speed () > maxAdjustedSpeed) || (force == Vec3::zero))
		{
			return force;
		}
		else
		{
			const float range = speed() / maxAdjustedSpeed;
			// const float cosine = interpolate (pow (range, 6), 1.0f, -1.0f);
			// const float cosine = interpolate (pow (range, 10), 1.0f, -1.0f);
			// const float cosine = interpolate (pow (range, 20), 1.0f, -1.0f);
			// const float cosine = interpolate (pow (range, 100), 1.0f, -1.0f);
			// const float cosine = interpolate (pow (range, 50), 1.0f, -1.0f);
			const float cosine = interpolate (pow (range, 20), 1.0f, -1.0f);
			return limitMaxDeviationAngle (force, cosine, forward());
		}
	}

	//-----------------------------------------------------------------------------
	// xxx experimental 9-6-02
	//
	// apply a given braking force (for a given dt) to our momentum.
	//
	// (this is intended as a companion to applySteeringForce, but I'm not sure how
	// well integrated it is.  It was motivated by the fact that "braking" (as in
	// "capture the flag" endgame) by using "forward * speed * -rate" as a steering
	// force was causing problems in adjustRawSteeringForce.  In fact it made it
	// get NAN, but even if it had worked it would have defeated the braking.
	//
	// maybe the guts of applySteeringForce should be split off into a subroutine
	// used by both applySteeringForce and applyBrakingForce?
	void 
		SteeringVehicle::applyBrakingForce (const float rate, const float deltaTime)
	{
		const float rawBraking = speed () * rate;
		const float clipBraking = ((rawBraking < maxForce ()) ? rawBraking : maxForce ());
		setSpeed (speed () - (clipBraking * deltaTime));
	}

	//-----------------------------------------------------------------------------
	// determine a raw steering force
	Vec3
		SteeringVehicle::determineCombinedSteering (const float /*elapsedTime*/)
	{
		return Vec3::zero;
	}

	//-----------------------------------------------------------------------------
	// apply a given steering force to our momentum,
	// adjusting our orientation to maintain velocity-alignment.
	void 
		SteeringVehicle::applySteeringForce (const Vec3& force,
		const float elapsedTime)
	{

		const Vec3 adjustedForce = adjustRawSteeringForce (force, elapsedTime);
		//	const Vec3 adjustedForce = force;//adjustRawSteeringForce (force, elapsedTime);

		// enforce limit on magnitude of steering force
		_currentSteeringForce = adjustedForce.truncateLength (maxForce ());

		// compute acceleration and velocity
		Vec3 newAcceleration = (_currentSteeringForce / mass());
		Vec3 newVelocity = velocity();

		// damp out abrupt changes and oscillations in steering acceleration
		// (rate is proportional to time step, then clipped into useful range)
		if (elapsedTime > 0)    {
			const float smoothRate = clip (9 * elapsedTime, 0.15f, 0.4f);
			blendIntoAccumulator (smoothRate,
				newAcceleration,
				_smoothedAcceleration);
		}

		// Euler integrate (per frame) acceleration into velocity
		newVelocity += _smoothedAcceleration * elapsedTime;

		// enforce speed limit
		newVelocity = newVelocity.truncateLength (maxSpeed ());

		// update Speed
		setSpeed (newVelocity.length());
		setLinearVelocity( newVelocity );

		// Euler integrate (per frame) velocity into position
		setPosition (position() + (newVelocity * elapsedTime));

		// regenerate local space (by default: align vehicle's forward axis with
		// new velocity, but this behavior may be overridden by derived classes.)
		float newSpeed = newVelocity.length();
		if( newSpeed > 0 )	{
			Vec3 newForward = newVelocity / newSpeed;
			regenerateLocalSpace( newForward, elapsedTime );
		}
		else
		{
			// maybe smth to turn at zero speed ?
		}

		// maintain path curvature information
		measurePathCurvature (elapsedTime);

		// running average of recent positions
		blendIntoAccumulator (elapsedTime * 0.06f, // QQQ
			position (),
			_smoothedPosition);
	}

	//-----------------------------------------------------------------------------
	// the default version: keep FORWARD parallel to velocity, change UP as
	// little as possible.
	//
	// parameter names commented out to prevent compiler warning from "-W"
	void 
		SteeringVehicle::regenerateLocalSpace (const Vec3& newForward,
		const float /* elapsedTime */)
	{
		// adjust orthonormal basis vectors to be aligned with new velocity
		regenerateOrthonormalBasisUF( newForward );
	}

	//-----------------------------------------------------------------------------
	// alternate version: keep FORWARD parallel to velocity, adjust UP according
	// to a no-basis-in-reality "banking" behavior, something like what birds and
	// airplanes do

	// XXX experimental cwr 6-5-03
	void 
		SteeringVehicle::regenerateLocalSpaceForBanking (const Vec3& newForward,
		const float elapsedTime)
	{
		// the length of this global-upward-pointing vector controls the vehicle's
		// tendency to right itself as it is rolled over from turning acceleration
		const Vec3 globalUp (0, 0.2f, 0);

		// acceleration points toward the center of local path curvature, the
		// length determines how much the vehicle will roll while turning
		const Vec3 accelUp = _smoothedAcceleration * 0.05f;

		// combined banking, sum of UP due to turning and global UP
		const Vec3 bankUp = accelUp + globalUp;

		// blend bankUp into vehicle's UP basis vector
		const float smoothRate = elapsedTime * 3;
		Vec3 tempUp = up();
		blendIntoAccumulator (smoothRate, bankUp, tempUp);
		setUp (tempUp.normalized());

		//  annotationLine (position(), position() + (globalUp * 4), gWhite);  // XXX
		//  annotationLine (position(), position() + (bankUp   * 4), gOrange); // XXX
		//  annotationLine (position(), position() + (accelUp  * 4), gRed);    // XXX
		//  annotationLine (position(), position() + (up ()    * 1), gYellow); // XXX

		// adjust orthonormal basis vectors to be aligned with new velocity
		regenerateOrthonormalBasisUF( newForward );
	}


	//-----------------------------------------------------------------------------
	// measure path curvature (1/turning-radius), maintain smoothed version
	void 
		SteeringVehicle::measurePathCurvature (const float elapsedTime)
	{
		if (elapsedTime > 0)
		{
			const Vec3 dP = _lastPosition - position ();
			const Vec3 dF = (_lastForward - forward ()) / dP.length ();
			const Vec3 lateral = dF.perpendicularComponent (forward ());
			const float sign = (lateral.dot (side ()) < 0) ? 1.0f : -1.0f;
			_curvature = lateral.length() * sign;
			blendIntoAccumulator (elapsedTime * 4.0f,
				_curvature,
				_smoothedCurvature);
			_lastForward = forward ();
			_lastPosition = position ();
		}
	}


	//-----------------------------------------------------------------------------
	// draw lines from vehicle's position showing its velocity and acceleration
	void 
		SteeringVehicle::annotationVelocityAcceleration (float maxLengthA, 
		float maxLengthV)
	{
		const float desat = 0.4f;
		const float aScale = maxLengthA / maxForce ();
		const float vScale = maxLengthV / maxSpeed ();
		const Vec3& p = position();
		const Color aColor (desat, desat, 1); // bluish
		const Color vColor (    1, desat, 1); // pinkish

		annotationLine (p, p + (velocity ()           * vScale), vColor);
		annotationLine (p, p + (_smoothedAcceleration * aScale), aColor);
	}

}

//-----------------------------------------------------------------------------
