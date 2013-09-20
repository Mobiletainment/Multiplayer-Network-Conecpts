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
// Multiple pursuit (for testing pursuit)
//
// 08-22-02 cwr: created 
//
//
//-----------------------------------------------------------------------------
#include "stdafx.h"


#include "OpenSteer/SteeringVehicle.h"
#include "demo/OpenSteerDemo.h"
#include "OpenSteer/Color.h"

namespace {


    using namespace OpenSteer;

    //-----------------------------------------------------------------------------
    // This Plugin uses two vehicle types: MpWanderer and MpPursuer.  They have
    // a common base class, MpBase, which is a specialization of SteeringVehicle.


    class MpBase : public SteeringVehicle
    {
    public:

        // constructor
        MpBase () {reset ();}

        // reset state
        void reset (void)
        {
            SteeringVehicle::reset (); // reset the vehicle 
            setSpeed (0);            // speed along Forward direction.
            setMaxForce (5.0);       // steering force is clipped to this magnitude
            setMaxSpeed (3.0);       // velocity is clipped to this magnitude
            gaudyPursuitAnnotation = true; // select use of 9-color annotation
        }

        // draw into the scene
        void draw ( const float /*currentTime*/, const float /*elapsedTime*/ )
        {
            drawBasic2dCircularVehicle (*this, bodyColor);
            drawTrail ();
        }

        // for draw method
        Color bodyColor;
    };


    class MpWanderer : public MpBase
    {
    public:

        // constructor
        MpWanderer () {reset ();}

        // reset state
        void reset (void)
        {
            MpBase::reset ();
            bodyColor.set (0.4f, 0.6f, 0.4f); // greenish
        }

        // one simulation step
        void update (const float currentTime, const float elapsedTime)
        {
            const Vec3 wander2d = steerForWander (elapsedTime).setYtoZero ();
            const Vec3 steer = forward() + (wander2d * 3);
            applySteeringForce (steer, elapsedTime);

            // for annotation
            recordTrailVertex (currentTime, position());
        }

    };


    class MpPursuer : public MpBase
    {
    public:

        // constructor
        MpPursuer (MpWanderer* w) {wanderer = w; reset ();}

        // reset state
        void reset (void)
        {
            MpBase::reset ();
            bodyColor.set (0.6f, 0.4f, 0.4f); // redish
            randomizeStartingPositionAndHeading ();
        }

        // one simulation step
        void update (const float currentTime, const float elapsedTime)
        {
            // when pursuer touches quarry ("wanderer"), reset its position
            const float d = Vec3::distance (position(), wanderer->position());
            const float r = radius() + wanderer->radius();
            if (d < r) reset ();

            const float maxTime = 20; // xxx hard-to-justify value
            applySteeringForce (steerForPursuit (*wanderer, maxTime), elapsedTime);

            // for annotation
            recordTrailVertex (currentTime, position());
        }

        // reset position
        void randomizeStartingPositionAndHeading (void)
        {
            // randomize position on a ring between inner and outer radii
            // centered around the home base
            const float inner = 20;
            const float outer = 30;
            const float radius = frandom2 (inner, outer);
            const Vec3 randomOnRing = randomUnitVectorOnXZPlane () * radius;
            setPosition (wanderer->position() + randomOnRing);

            // randomize 2D heading
            randomizeHeadingOnXZPlane ();
        }

        MpWanderer* wanderer;
    };


    //-----------------------------------------------------------------------------
    // Plugin for OpenSteerDemo


    class MpPlugIn : public Plugin
    {
    public:
		MpPlugIn():wanderer(nullptr){}
        const char* name (void) const {return "Multiple Pursuit";}

        float selectionOrderSortKey (void) const {return 0.04f;}

        virtual ~MpPlugIn() {} // be more "nice" to avoid a compiler warning

        void open (void)
        {
            // create the wanderer, saving a pointer to it
            wanderer = OS_NEW MpWanderer;
            allMP.push_back (wanderer);

            // create the specified number of pursuers, save pointers to them
            const int pursuerCount = 30;
            for (int i = 0; i < pursuerCount; i++)
                allMP.push_back (new MpPursuer (wanderer));
            pBegin = allMP.begin() + 1;  // iterator pointing to first pursuer
            pEnd = allMP.end();          // iterator pointing to last pursuer

            // initialize camera
            SteeringVehicle::setSelectedVehicle( wanderer );
            Camera::accessInstance().mode = Camera::cmStraightDown;
            Camera::accessInstance().fixedDistDistance = OpenSteerDemo::cameraTargetDistance;
            Camera::accessInstance().fixedDistVOffset = OpenSteerDemo::camera2dElevation;
        }

        void update (const float currentTime, const float elapsedTime)
        {
            // update the wanderer
            wanderer->update (currentTime, elapsedTime);
          
            // update each pursuer
            for (iterator i = pBegin; i != pEnd; i++)
            {
                ((MpPursuer&) (**i)).update (currentTime, elapsedTime);
            }
        }

        void redraw (const float currentTime, const float elapsedTime)
        {
            // selected vehicle (user can mouse click to select another)
            AbstractVehicle& selected = *SteeringVehicle::getSelectedVehicle();

            // vehicle nearest mouse (to be highlighted)
            AbstractVehicle& nearMouse = *OpenSteerDemo::vehicleNearestToMouse ();

            // update camera
            OpenSteerDemo::updateCamera (currentTime, elapsedTime, selected);

            // draw "ground plane"
            OpenSteerDemo::gridUtility (selected.position());

            // draw each vehicles
            for (iterator i = allMP.begin(); i != pEnd; i++) (**i).draw ( currentTime, elapsedTime );

            // highlight vehicle nearest mouse
            OpenSteerDemo::highlightVehicleUtility (nearMouse);
            OpenSteerDemo::circleHighlightVehicleUtility (selected);
        }

        void close (void)
        {
            // delete wanderer, all pursuers, and clear list
			if( wanderer )
			{
				delete (wanderer);
				wanderer = nullptr;

 				pBegin = allMP.begin();  // iterator pointing to first pursuer
 				pEnd = allMP.end();      // iterator pointing to last pursuer
				if( pBegin != pEnd )
				{
					++pBegin;
					for (iterator i = pBegin; i != pEnd; i++)
					{
						AbstractVehicle* v = *i;
// TODO: fix strange crash here
//						delete v;
					}
				}
				
				allMP.clear();
			}
        }

        void reset (void)
        {
            // reset wanderer and pursuers
            wanderer->reset ();
            for (iterator i = pBegin; i != pEnd; i++) ((MpPursuer&)(**i)).reset ();

            // immediately jump to default camera position
            Camera::accessInstance().doNotSmoothNextMove ();
            Camera::accessInstance().resetLocalSpace ();
        }

        const AVGroup& allVehicles (void) const {return (const AVGroup&) allMP;}
		AVGroup& allVehicles (void) {return ( AVGroup&) allMP;}

        // a group (STL vector) of all vehicles
        std::vector<MpBase*> allMP;
        typedef std::vector<MpBase*>::const_iterator iterator;
        iterator pBegin, pEnd;

        MpWanderer* wanderer;
    };


    MpPlugIn gMpPlugIn;



    //-----------------------------------------------------------------------------


} // anonymous namespace    
