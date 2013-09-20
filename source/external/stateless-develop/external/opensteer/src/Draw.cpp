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
// Draw
//
// This is a first stab at a graphics module for OpenSteerDemo.  It is intended
// to encapsulate all functionality related to 3d graphics as well as windows
// and graphics input devices such as the mouse.
//
// However this is purely an OpenGL-based implementation.  No special effort
// has been made to keep the "OpenGL way" from leaking through.  Attempting to
// port this to another graphics substrate may run into modularity problems.
//
// In any case, all calls to the underlying graphics substrate should be made
// from this module only.
//
// 10-04-04 bk:  put everything into the OpenSteer namespace
// 06-25-02 cwr: created 
//
//
//-----------------------------------------------------------------------------
#include "stdafx.h"

#include "OpenSteer/OpenSteerMacros.h"
#include "OpenSteer/Draw.h"
#include "OpenSteer/AbstractPlugin.h"


#include <iomanip>
#include <sstream>

#if OS_HAVE_GLUT
#else
#  define HAVE_NO_GLUT
#endif

// Include headers for OpenGL (gl.h), OpenGL Utility Library (glu.h)
//
// XXX In Mac OS X these headers are located in a different directory.
// XXX Need to revisit conditionalization on operating system.
#if __APPLE__ && __MACH__
    #include <OpenGL/gl.h>   // for Mac OS X
    #include <OpenGL/glu.h>   // for Mac OS X
    #ifndef HAVE_NO_GLUT
        #include <GLUT/glut.h>   // for Mac OS X
    #endif
#else
    #ifdef _MSC_VER
        #include <windows.h>
    #endif
    #include <GL/glu.h>     // for Linux and Windows
	#ifdef _MSC_VER
		#ifndef HAVE_NO_GLUT
		#include "GL/glut.h"   // for Windows
		#endif
	#else
		
		#include <GL/glut.h>   // for Linux
		
	#endif
#endif


#include "OpenSteer/Vec3.h"

// To include OpenSteer::round.
#include "OpenSteer/Utilities.h"

//-----------------------------------------------------------------------------
#include "OpenSteer/OpenSteerMacros.h"
#if OS_HAVE_PROFILE
#  include "iprof/prof.h" 
#endif

// GL interface
// Collected the available abstractions here as a first step
// to swapping in different graphics libraries

namespace OpenSteer
{

	bool updatePhaseActive = false;

	void glVertexVec3 (const OpenSteer::Vec3& v);


	void warnIfInUpdatePhase2( const char* name);

	inline
	void warnIfInUpdatePhase (const char* name)
	{
		if (updatePhaseActive)
		{
			warnIfInUpdatePhase2 (name);
		}
	}

}

namespace {
    //-------------------------------------------------------------------------
    // emit an OpenGL vertex based on a Vec3

    
    inline 
	void iglVertexVec3 (const OpenSteer::Vec3& v)
    {
        glVertex3f (v.x, v.y, v.z);
    }


    //-------------------------------------------------------------------------
    // OpenGL-specific routine for error check, report, and exit
    void 
    checkForGLError (const char* locationDescription)
    {
        // normally (when no error) just return
        const int lastGlError = glGetError();
        if (lastGlError == GL_NO_ERROR) return;

        // otherwise print vaguely descriptive error message, then exit
        std::cerr << std::endl << "exiting caused by an OpenGL error ";
        switch (lastGlError)
        {
        case GL_INVALID_ENUM:      std::cerr << "GL_INVALID_ENUM";      break;
        case GL_INVALID_VALUE:     std::cerr << "GL_INVALID_VALUE";     break;
        case GL_INVALID_OPERATION: std::cerr << "GL_INVALID_OPERATION"; break;
        case GL_STACK_OVERFLOW:    std::cerr << "GL_STACK_OVERFLOW";    break;
        case GL_STACK_UNDERFLOW:   std::cerr << "GL_STACK_UNDERFLOW";   break;
        case GL_OUT_OF_MEMORY:     std::cerr << "GL_OUT_OF_MEMORY";     break;
    #ifndef _WIN32
        case GL_TABLE_TOO_LARGE:   std::cerr << "GL_TABLE_TOO_LARGE";   break;
    #endif
        }
        if (locationDescription) std::cerr << " in " << locationDescription;
        std::cerr << std::endl << std::endl << std::flush;
        /// @todo - a library should never bail, this is an application function
        // throw an exception? Call a registered exit hook?
        //OpenSteer::OpenSteerDemo::exit (1);
    }

    //-----------------------------------------------------------------------------
    // draw 3d "graphical annotation" lines, used for debugging    
    inline void iDrawLine (const OpenSteer::Vec3& startPoint,
                           const OpenSteer::Vec3& endPoint,
                           const OpenSteer::Color& color)
    {
        OpenSteer::warnIfInUpdatePhase ("iDrawLine");
        glColor3f (color.r(), color.g(), color.b());
        glBegin (GL_LINES);
        glVertexVec3 (startPoint);
        glVertexVec3 (endPoint);
        glEnd ();
    }

    //-----------------------------------------------------------------------------
    // Draw a single OpenGL triangle given three Vec3 vertices.
    
    inline void iDrawTriangle (const OpenSteer::Vec3& a,
                               const OpenSteer::Vec3& b,
                               const OpenSteer::Vec3& c,
                               const OpenSteer::Color& color)
    {
        OpenSteer::warnIfInUpdatePhase ("iDrawTriangle");
//        glColor3f (color.r(), color.g(), color.b());
		glColor4f (color.r(), color.g(), color.b(), color.a());
        glBegin (GL_TRIANGLES);
        {
            OpenSteer::glVertexVec3 (a);
            OpenSteer::glVertexVec3 (b);
            OpenSteer::glVertexVec3 (c);
        }
        glEnd ();
    }


    //-------------------------------------------------------------------------
    // Draw a single OpenGL quadrangle given four Vec3 vertices, and color.   
    inline void iDrawQuadrangle (const OpenSteer::Vec3& a,
                                 const OpenSteer::Vec3& b,
                                 const OpenSteer::Vec3& c,
                                 const OpenSteer::Vec3& d,
                                 const OpenSteer::Color& color)
    {
        OpenSteer::warnIfInUpdatePhase ("iDrawQuadrangle");
//        glColor3f (color.r(), color.g(), color.b());
		glColor4f (color.r(), color.g(), color.b(), color.a());
        glBegin (GL_QUADS);
        {
            OpenSteer::glVertexVec3 (a);
            OpenSteer::glVertexVec3 (b);
            OpenSteer::glVertexVec3 (c);
            OpenSteer::glVertexVec3 (d);
        }
        glEnd ();
    }

    //-------------------------------------------------------------------------
    // Between matched sets of these two calls, assert that all polygons
    // will be drawn "double sided", that is, without back-face culling    
    inline void beginDoubleSidedDrawing (void)
    {
        glPushAttrib (GL_ENABLE_BIT);
        glDisable (GL_CULL_FACE);
    }


    inline 
	void endDoubleSidedDrawing (void)
    {
        glPopAttrib ();
    }

	inline
    GLint begin2dDrawing (float w, float h)
    {
        // store OpenGL matrix mode
        GLint originalMatrixMode;
        glGetIntegerv (GL_MATRIX_MODE, &originalMatrixMode);

        // clear projection transform
        glMatrixMode (GL_PROJECTION);
        glPushMatrix ();
        glLoadIdentity ();

        // set up orthogonal projection onto window's screen space
        glOrtho (0.0f, w, 0.0f, h, -1.0f, 1.0f);

        // clear model transform
        glMatrixMode (GL_MODELVIEW);
        glPushMatrix ();
        glLoadIdentity ();

        // return original matrix mode for saving (stacking)
        return originalMatrixMode;
    }

	inline
    void end2dDrawing (GLint originalMatrixMode)
    {
        // restore previous model/projection transformation state
        glPopMatrix ();
        glMatrixMode (GL_PROJECTION);
        glPopMatrix ();

        // restore OpenGL matrix mode
        glMatrixMode (originalMatrixMode);
    }

}   // end anonymous namespace

namespace OpenSteer	{
	//-----------------------------------------------------------------------------
	void glVertexVec3 (const Vec3& v)
	{
		iglVertexVec3 (v);
	}

	//-----------------------------------------------------------------------------
	float OpenGLRenderer::drawGetWindowHeight(void)
	{
		return extern_drawGetWindowHeight();
	}

	//-----------------------------------------------------------------------------
	float OpenGLRenderer::drawGetWindowWidth(void)
	{
		return extern_drawGetWindowWidth();
	}

	//-----------------------------------------------------------------------------
	GLint OpenGLRenderer::begin2dDrawing (float w, float h)
	{
		// store OpenGL matrix mode
		GLint originalMatrixMode;
		glGetIntegerv (GL_MATRIX_MODE, &originalMatrixMode);

		// clear projection transform
		glMatrixMode (GL_PROJECTION);
		glPushMatrix ();
		glLoadIdentity ();

		// set up orthogonal projection onto window's screen space
		glOrtho (0.0f, w, 0.0f, h, -1.0f, 1.0f);

		// clear model transform
		glMatrixMode (GL_MODELVIEW);
		glPushMatrix ();
		glLoadIdentity ();

		// return original matrix mode for saving (stacking)
		return originalMatrixMode;
	}

	//-----------------------------------------------------------------------------
	void OpenGLRenderer::end2dDrawing (GLint originalMatrixMode)
	{
		// restore previous model/projection transformation state
		glPopMatrix ();
		glMatrixMode (GL_PROJECTION);
		glPopMatrix ();

		// restore OpenGL matrix mode
		glMatrixMode (originalMatrixMode);
	}

	//-----------------------------------------------------------------------------
	void 
		OpenGLRenderer::glVertexVec3 (const Vec3& v)
	{
		iglVertexVec3 (v);
	}

	//-----------------------------------------------------------------------------
	// warn when draw functions are called during OpenSteerDemo's update phase
	void 
		warnIfInUpdatePhase2 (const char* name)
	{
		std::ostringstream message;
		message << "use annotation (during simulation update, do not call ";
		message << name;
		message << ")";
		message << std::ends;
		std::cerr << message;       // send message to cerr, let host app worry about where to redirect it
	}

	//-----------------------------------------------------------------------------
	void 
		OpenGLRenderer::drawLine (const Vec3& startPoint,
		const Vec3& endPoint,
		const Color& color)
	{
		iDrawLine (startPoint, endPoint, color);
	}

	//-----------------------------------------------------------------------------
	// draw a line with alpha blending

	// see also glAlphaFunc
	// glBlendFunc (GL_SRC_ALPHA)
	// glEnable (GL_BLEND)
	void 
		OpenGLRenderer::drawLineAlpha (const Vec3& startPoint,
		const Vec3& endPoint,
		const Color& color,
		const float alpha)
	{
		warnIfInUpdatePhase ("drawLineAlpha");
		glColor4f (color.r(), color.g(), color.b(), alpha);
		glBegin (GL_LINES);
		glVertexVec3 (startPoint);
		glVertexVec3 (endPoint);
		glEnd ();
	}

	//-----------------------------------------------------------------------------
	void 
		OpenGLRenderer::drawTriangle (const Vec3& a,
		const Vec3& b,
		const Vec3& c,
		const Color& color)
	{
		iDrawTriangle (a, b, c, color);
	}

	//-----------------------------------------------------------------------------
	void 
		OpenGLRenderer::drawQuadrangle (const Vec3& a,
		const Vec3& b,
		const Vec3& c,
		const Vec3& d,
		const Color& color)
	{
		iDrawQuadrangle (a, b, c, d, color);
	}


	//-------------------------------------------------------------------------
	// draws a "wide line segment": a rectangle of the given width and color
	// whose mid-line connects two given endpoints
	void 
		OpenGLRenderer::drawXZWideLine (const Vec3& startPoint,
		const Vec3& endPoint,
		const Color& color,
		float width)
	{
		warnIfInUpdatePhase ("drawXZWideLine");

		const Vec3 offset = endPoint - startPoint;
		const Vec3 along = offset.normalized();
		const Vec3 perp = gGlobalSpace.localRotateForwardToSide (along);
		const Vec3 radius = perp * width / 2;

		const Vec3 a = startPoint + radius;
		const Vec3 b = endPoint + radius;
		const Vec3 c = endPoint - radius;
		const Vec3 d = startPoint - radius;

		iDrawQuadrangle (a, b, c, d, color);
	}

	//-------------------------------------------------------------------------
	// General purpose circle/disk drawing routine.  Draws circles or disks (as
	// specified by "filled" argument) and handles both special case 2d circles
	// on the XZ plane or arbitrary circles in 3d space (as specified by "in3d"
	// argument)
	void 
		OpenGLRenderer::drawCircleOrDisk (const float radius,
		const Vec3& axis,
		const Vec3& center,
		const Color& color,
		const int segments,
		const bool filled,
		const bool in3d)
	{
		LocalSpace ls;
		if (in3d)
		{
			// define a local space with "axis" as the Y/up direction
			// (XXX should this be a method on  LocalSpace?)
			const Vec3 unitAxis = axis.normalized ();
			const Vec3 unitPerp = findPerpendicularIn3d (axis).normalized ();
			ls.setUp (unitAxis);
			ls.setForward (unitPerp);
			ls.setPosition (center);
			ls.setUnitSideFromForwardAndUp ();
		}

		// make disks visible (not culled) from both sides 
		if (filled) beginDoubleSidedDrawing ();

		// point to be rotated about the (local) Y axis, angular step size
		Vec3 pointOnCircle (radius, 0, 0);
		const float step = (2 * OPENSTEER_M_PI) / segments;

		// set drawing color
		glColor3f (color.r(), color.g(), color.b());

		// begin drawing a triangle fan (for disk) or line loop (for circle)
		glBegin (filled ? GL_TRIANGLE_FAN : GL_LINE_LOOP);

		// for the filled case, first emit the center point
		if (filled) iglVertexVec3 (in3d ? ls.position() : center);

		// rotate p around the circle in "segments" steps
		float sin=0, cos=0;
		const int vertexCount = filled ? segments+1 : segments;
		for (int i = 0; i < vertexCount; i++)
		{
			// emit next point on circle, either in 3d (globalized out
			// of the local space), or in 2d (offset from the center)
			iglVertexVec3 (in3d ?
				ls.globalizePosition (pointOnCircle) :
			(Vec3) (pointOnCircle + center));

			// rotate point one more step around circle
			pointOnCircle = pointOnCircle.rotateAboutGlobalY (step, sin, cos);
		}

		// close drawing operation
		glEnd ();
		if (filled) endDoubleSidedDrawing ();
	}

	//-------------------------------------------------------------------------
	void 
		OpenGLRenderer::draw3dCircleOrDisk (const float radius,
		const Vec3& center,
		const Vec3& axis,
		const Color& color,
		const int segments,
		const bool filled)
	{
		// draw a circle-or-disk in the given local space
		drawCircleOrDisk (radius, axis, center, color, segments, filled, true);
	}

	//-------------------------------------------------------------------------
	// drawing utility used by both drawXZCircle and drawXZDisk
	void 
		OpenGLRenderer::drawXZCircleOrDisk (const float radius,
		const Vec3& center,
		const Color& color,
		const int segments,
		const bool filled)
	{
		// draw a circle-or-disk on the XZ plane
		drawCircleOrDisk (radius, Vec3::zero, center, color, segments, filled, false);
	}

	//-------------------------------------------------------------------------
	void OpenGLRenderer::drawXZCircle (const float radius,
		const Vec3& center,
		const Color& color,
		const int segments)
	{
		warnIfInUpdatePhase ("drawXZCircle");
		drawXZCircleOrDisk (radius, center, color, segments, false);
	}

	//-------------------------------------------------------------------------
	void OpenGLRenderer::drawXZDisk (const float radius,
		const Vec3& center,
		const Color& color,
		const int segments)
	{
		warnIfInUpdatePhase ("drawXZDisk");
		drawXZCircleOrDisk (radius, center, color, segments, true);
	}

	//-------------------------------------------------------------------------
	void OpenGLRenderer::draw3dCircle (const float radius,
		const Vec3& center,
		const Vec3& axis,
		const Color& color,
		const int segments)
	{
		warnIfInUpdatePhase ("draw3dCircle");
		draw3dCircleOrDisk (radius, center, axis, color, segments, false);
	}

	//-------------------------------------------------------------------------
	void OpenGLRenderer::draw3dDisk (const float radius,
		const Vec3& center,
		const Vec3& axis,
		const Color& color,
		const int segments)
	{
		warnIfInUpdatePhase ("draw3dDisk");
		draw3dCircleOrDisk (radius, center, axis, color, segments, true);
	}

	//-------------------------------------------------------------------------
	// draw a circular arc on the XZ plane, from a start point, around a center,
	// for a given arc length, in a given number of segments and color.  The
	// sign of arcLength determines the direction in which the arc is drawn.
	//
	// XXX maybe this should alow allow "in3d" cricles about an given axis
	// XXX maybe there should be a "filled" version of this
	// XXX maybe this should be merged in with drawCircleOrDisk
	void 
		OpenGLRenderer::drawXZArc (const Vec3& start,
		const Vec3& center,
		const float arcLength,
		const int segments,
		const Color& color)
	{
		warnIfInUpdatePhase ("drawXZArc");

		// "spoke" is initially the vector from center to start,
		// it is then rotated around its tail
		Vec3 spoke = start - center;

		// determine the angular step per segment
		const float radius = spoke.length ();
		const float twoPi = 2 * OPENSTEER_M_PI;
		const float circumference = twoPi * radius;
		const float arcAngle = twoPi * arcLength / circumference;
		const float step = arcAngle / segments;

		// set drawing color
		glColor3f (color.r(), color.g(), color.b());

		// begin drawing a series of connected line segments
		glBegin (GL_LINE_STRIP);

		// draw each segment along arc
		float sin=0, cos=0;
		for (int i = 0; i < segments; i++)
		{
			// emit next point on arc
			iglVertexVec3 (spoke + center);

			// rotate point to next step around circle
			spoke = spoke.rotateAboutGlobalY (step, sin, cos);
		}

		// close drawing operation
		glEnd ();
	}

	//-------------------------------------------------------------------------
	// a simple 2d vehicle on the XZ plane
	//-----------------------------------------------------------------------------
	void 
		OpenGLRenderer::drawBasic2dCircularVehicle (const AbstractVehicle& vehicle,
		const Color& color)
	{
		drawBasic2dCircularLocalSpace(
			vehicle.getLocalSpaceData(),
			color, vehicle.radius() );
	}

	//-----------------------------------------------------------------------------
	void 
		OpenGLRenderer::drawBasic2dCircularLocalSpace (const LocalSpaceData& vehicle,
		const Color& color, float fRadius, bool bDrawCircle, float fUpOffset )
	{
		// "aspect ratio" of body (as seen from above)
		const float x = 0.5f;
		const float y = sqrtXXX (1 - (x * x));

		// radius and position of vehicle
		const float r = fRadius;
		//	const Vec3& p = vehicle._position;
		Vec3 p = vehicle._position;
		if( fUpOffset != 0 )
		{
			p += vehicle._up * fUpOffset;
		}

		// shape of triangular body
		//	const Vec3 u = r * fUpOffset * Vec3::up; // slightly up
		const Vec3 u = r * 0.05f * Vec3::up; // slightly up
		const Vec3 f = r * vehicle._forward;
		const Vec3 s = r * vehicle._side * x;
		const Vec3 b = r * vehicle._forward * -y;

		// draw double-sided triangle (that is: no (back) face culling)
		beginDoubleSidedDrawing ();
		iDrawTriangle(
			p + f + u,
			p + b - s + u,
			p + b + s + u,
			color );
		endDoubleSidedDrawing ();

		if( bDrawCircle == true )
		{
			// draw the circular collision boundary
			drawXZCircle (r, p + u, gWhite, 20);
		}
	}

	//-------------------------------------------------------------------------
	// a simple 3d vehicle
	void 
		OpenGLRenderer::drawBasic3dSphericalVehicle (const AbstractVehicle& vehicle,
		const Color& color)
	{
		// "aspect ratio" of body (as seen from above)
		const float x = 0.5f;
		const float y = sqrtXXX (1 - (x * x));

		// radius and position of vehicle
		const float r = vehicle.radius();
		const Vec3& p = vehicle.position();

		// body shape parameters
		const Vec3 f = r * vehicle.forward();
		const Vec3 s = r * vehicle.side() * x;
		const Vec3 u = r * vehicle.up() * x * 0.5f;
		const Vec3 b = r * vehicle.forward() * -y;

		// vertex positions
		const Vec3 nose   = p + f;
		const Vec3 side1  = p + b - s;
		const Vec3 side2  = p + b + s;
		const Vec3 top    = p + b + u;
		const Vec3 bottom = p + b - u;

		// colors
		const float j = +0.05f;
		const float k = -0.05f;
		const Color color1 = color + Color(j, j, k);
		const Color color2 = color + Color(j, k, j);
		const Color color3 = color + Color(k, j, j);
		const Color color4 = color + Color(k, j, k);
		const Color color5 = color + Color(k, k, j);

		// draw body
		iDrawTriangle (nose,  side1,  top,    color1);  // top, side 1
		iDrawTriangle (nose,  top,    side2,  color2);  // top, side 2
		iDrawTriangle (nose,  bottom, side1,  color3);  // bottom, side 1
		iDrawTriangle (nose,  side2,  bottom, color4);  // bottom, side 2
		iDrawTriangle (side1, side2,  top,    color5);  // top back
		iDrawTriangle (side2, side1,  bottom, color5);  // bottom back
	}


	// drawBasic3dSphericalVehicle with a supplied draw routine
	// provided so non-OpenGL based apps can draw a boid
	void 
		OpenGLRenderer::drawBasic3dSphericalVehicle (drawTriangleRoutine draw, const AbstractVehicle& vehicle,
		const Color& color)
	{
		// "aspect ratio" of body (as seen from above)
		const float x = 0.5f;
		const float y = sqrtXXX (1 - (x * x));

		// radius and position of vehicle
		const float r = vehicle.radius();
		const Vec3& p = vehicle.position();

		// body shape parameters
		const Vec3 f = r * vehicle.forward();
		const Vec3 s = r * vehicle.side() * x;
		const Vec3 u = r * vehicle.up() * x * 0.5f;
		const Vec3 b = r * vehicle.forward() * -y;

		// vertex positions
		const Vec3 nose   = p + f;
		const Vec3 side1  = p + b - s;
		const Vec3 side2  = p + b + s;
		const Vec3 top    = p + b + u;
		const Vec3 bottom = p + b - u;

		// colors
		const float j = +0.05f;
		const float k = -0.05f;
		const Color color1 = color + Color (j, j, k);
		const Color color2 = color + Color (j, k, j);
		const Color color3 = color + Color (k, j, j);
		const Color color4 = color + Color (k, j, k);
		const Color color5 = color + Color (k, k, j);

		// draw body
		draw (nose,  side1,  top,    color1);  // top, side 1
		draw (nose,  top,    side2,  color2);  // top, side 2
		draw (nose,  bottom, side1,  color3);  // bottom, side 1
		draw (nose,  side2,  bottom, color4);  // bottom, side 2
		draw (side1, side2,  top,    color5);  // top back
		draw (side2, side1,  bottom, color5);  // bottom back
	}

	//-------------------------------------------------------------------------
	// draw a (filled-in, polygon-based) square checkerboard grid on the XZ
	// (horizontal) plane.
	//
	// ("size" is the length of a side of the overall grid, "subsquares" is the
	// number of subsquares along each edge (for example a standard checkboard
	// has eight), "center" is the 3d position of the center of the grid,
	// color1 and color2 are used for alternating subsquares.)
	void 
		OpenGLRenderer::drawXZCheckerboardGrid (const float size,
		const int subsquares,
		const Vec3& center,
		const Color& color1,
		const Color& color2)
	{
		const float half = size * 0.5f;
		const float spacing = size / subsquares;

		beginDoubleSidedDrawing ();
		{
			bool flag1 = false;
			float p = -half;
			Vec3 corner;
			for (int i = 0; i < subsquares; i++)
			{
				bool flag2 = flag1;
				float q = -half;
				for (int j = 0; j < subsquares; j++)
				{
					corner.set (p, 0, q);
					corner += center;
					iDrawQuadrangle (corner,
						corner + Vec3 (spacing, 0,       0),
						corner + Vec3 (spacing, 0, spacing),
						corner + Vec3 (0,       0, spacing),
						flag2 ? color1 : color2);
					flag2 = !flag2;
					q += spacing;
				}
				flag1 = !flag1;
				p += spacing;
			}
		}
		endDoubleSidedDrawing ();
	}


	//-------------------------------------------------------------------------
	// draw a square grid of lines on the XZ (horizontal) plane.
	//
	// ("size" is the length of a side of the overall grid, "subsquares" is the
	// number of subsquares along each edge (for example a standard checkboard
	// has eight), "center" is the 3d position of the center of the grid, lines
	// are drawn in the specified "color".)
	void 
		OpenGLRenderer::drawXZLineGrid (const float size,
		const int subsquares,
		const Vec3& center,
		const Color& color)
	{
		warnIfInUpdatePhase ("drawXZLineGrid");

		const float half = size * 0.5f;
		const float spacing = size / subsquares;

		// set grid drawing color
		glColor3f (color.r(), color.g(), color.b());

		// draw a square XZ grid with the given size and line count
		glBegin (GL_LINES);
		float q = -half;
		for (int i = 0; i < (subsquares + 1); i++)
		{
			const Vec3 x1 (q, 0, +half); // along X parallel to Z
			const Vec3 x2 (q, 0, -half);
			const Vec3 z1 (+half, 0, q); // along Z parallel to X
			const Vec3 z2 (-half, 0, q);

			iglVertexVec3 (x1 + center);
			iglVertexVec3 (x2 + center);
			iglVertexVec3 (z1 + center);
			iglVertexVec3 (z2 + center);

			q += spacing;
		}
		glEnd ();
	}

	//-------------------------------------------------------------------------
	// draw a square grid of lines on the XZ (horizontal) plane.
	//
	// ("size" is the length of a side of the overall grid, "subsquares" is the
	// number of subsquares along each edge (for example a standard checkboard
	// has eight), "center" is the 3d position of the center of the grid, lines
	// are drawn in the specified "color".)
	void 
		OpenGLRenderer::drawXYLineGrid (const float size,
		const int subsquares,
		const Vec3& center,
		const Color& color)
	{
		warnIfInUpdatePhase ("drawXZLineGrid");

		const float half = size * 0.5f;
		const float spacing = size / subsquares;

		// set grid drawing color
		glColor3f (color.r(), color.g(), color.b());

		// draw a square XZ grid with the given size and line count
		glBegin (GL_LINES);
		float q = -half;
		for (int i = 0; i < (subsquares + 1); ++i)
		{
			const Vec3 x1 (q, +half, 0 ); // along X parallel to Z
			const Vec3 x2 (q, -half, 0);
			const Vec3 z1 (+half, q, 0 ); // along Z parallel to X
			const Vec3 z2 (-half, q, 0 );

			iglVertexVec3 (x1 + center);
			iglVertexVec3 (x2 + center);
			iglVertexVec3 (z1 + center);
			iglVertexVec3 (z2 + center);

			q += spacing;
		}
		glEnd ();
	}
	//-------------------------------------------------------------------------
	// draw the three axes of a LocalSpace: three lines parallel to the
	// basis vectors of the space, centered at its origin, of lengths
	// given by the coordinates of "size".
	void 
		OpenGLRenderer::drawAxes  (const AbstractLocalSpace& ls,
		const Vec3& size,
		const Color& color)
	{
		const Vec3 x (size.x / 2, 0, 0);
		const Vec3 y (0, size.y / 2, 0);
		const Vec3 z (0, 0, size.z / 2);

		iDrawLine (ls.globalizePosition (x), ls.globalizePosition (x * -1), color);
		iDrawLine (ls.globalizePosition (y), ls.globalizePosition (y * -1), color);
		iDrawLine (ls.globalizePosition (z), ls.globalizePosition (z * -1), color);
	}

	//-------------------------------------------------------------------------
	// draw the edges of a box with a given position, orientation, size
	// and color.  The box edges are aligned with the axes of the given
	// LocalSpace, and it is centered at the origin of that LocalSpace.
	// "size" is the main diagonal of the box.
	//
	// use gGlobalSpace to draw a box aligned with global space
	void 
		OpenGLRenderer::drawBoxOutline  (const AbstractLocalSpace& localSpace,
		const Vec3& size,
		const Color& color)
	{
		const Vec3 s = size / 2.0f;  // half of main diagonal

		const Vec3 a (+s.x, +s.y, +s.z);
		const Vec3 b (+s.x, -s.y, +s.z);
		const Vec3 c (-s.x, -s.y, +s.z);
		const Vec3 d (-s.x, +s.y, +s.z);

		const Vec3 e (+s.x, +s.y, -s.z);
		const Vec3 f (+s.x, -s.y, -s.z);
		const Vec3 g (-s.x, -s.y, -s.z);
		const Vec3 h (-s.x, +s.y, -s.z);

		const Vec3 A = localSpace.globalizePosition (a);
		const Vec3 B = localSpace.globalizePosition (b);
		const Vec3 C = localSpace.globalizePosition (c);
		const Vec3 D = localSpace.globalizePosition (d);

		const Vec3 E = localSpace.globalizePosition (e);
		const Vec3 F = localSpace.globalizePosition (f);
		const Vec3 G = localSpace.globalizePosition (g);
		const Vec3 H = localSpace.globalizePosition (h);

		iDrawLine (A, B, color);
		iDrawLine (B, C, color);
		iDrawLine (C, D, color);
		iDrawLine (D, A, color);

		iDrawLine (A, E, color);
		iDrawLine (B, F, color);
		iDrawLine (C, G, color);
		iDrawLine (D, H, color);

		iDrawLine (E, F, color);
		iDrawLine (F, G, color);
		iDrawLine (G, H, color);
		iDrawLine (H, E, color);
	}


	namespace {

		//-------------------------------------------------------------------------
		// this comes up often enough to warrant its own warning function

		inline void drawCameraLookAtCheck (const Vec3& cameraPosition,
			const Vec3& pointToLookAt,
			const Vec3& up)
		{
			const Vec3 view = pointToLookAt - cameraPosition;
			const Vec3 perp = view.perpendicularComponent (up);
			if (perp == Vec3::zero)
				std::cerr << "OpenSteer - LookAt: degenerate camera\n";
		}

	} // anonymous namespace

	//-------------------------------------------------------------------------
	// Define scene's camera (viewing transformation) in terms of the camera's
	// position, the point to look at (an "aim point" in the scene which will
	// end up at the center of the camera's view), and an "up" vector defining
	// the camera's "roll" around the "view axis" between cameraPosition and
	// pointToLookAt (the image of the up vector will be vertical in the
	// camera's view).
	void 
		OpenGLRenderer::drawCameraLookAt (const Vec3& cameraPosition,
		const Vec3& pointToLookAt,
		const Vec3& up)
	{
#ifdef HAVE_NO_GLUT
#else
		// check for valid "look at" parameters
		drawCameraLookAtCheck (cameraPosition, pointToLookAt, up);

		// use LookAt from OpenGL Utilities
		glLoadIdentity ();
		gluLookAt (cameraPosition.x, cameraPosition.y, cameraPosition.z,
			pointToLookAt.x,  pointToLookAt.y,  pointToLookAt.z,
			up.x,             up.y,             up.z);
#endif
	}

	//-----------------------------------------------------------------------------
	void 
		OpenGLRenderer::draw2dLine (const Vec3& startPoint,
		const Vec3& endPoint,
		const Color& color, 
		float w, float h)
	{
		const GLint originalMatrixMode = begin2dDrawing (w, h);

		iDrawLine (startPoint, endPoint, color);

		end2dDrawing (originalMatrixMode);
	}

	//-------------------------------------------------------------------------
	// draw a reticle at the center of the window.  Currently it is small
	// crosshair with a gap at the center, drawn in white with black borders
	void 
		OpenGLRenderer::drawReticle (float w, float h)
	{
		const int a = 10;
		const int b = 30;

		draw2dLine (Vec3 (w+a, h,   0), Vec3 (w+b, h,   0), gWhite, w, h);
		draw2dLine (Vec3 (w,   h+a, 0), Vec3 (w,   h+b, 0), gWhite, w, h);
		draw2dLine (Vec3 (w-a, h,   0), Vec3 (w-b, h,   0), gWhite, w, h);
		draw2dLine (Vec3 (w,   h-a, 0), Vec3 (w,   h-b, 0), gWhite, w, h);

		glLineWidth (3);
		draw2dLine (Vec3 (w+a, h,   0), Vec3 (w+b, h,   0), gBlack, w, h);
		draw2dLine (Vec3 (w,   h+a, 0), Vec3 (w,   h+b, 0), gBlack, w, h);
		draw2dLine (Vec3 (w-a, h,   0), Vec3 (w-b, h,   0), gBlack, w, h);
		draw2dLine (Vec3 (w,   h-a, 0), Vec3 (w,   h-b, 0), gBlack, w, h);
		glLineWidth (1);
	}


	//-------------------------------------------------------------------------


	// code (from main.cpp) used to draw "forward ruler" on vehicle

	//     // xxx --------------------------------------------------
	//     {
	//         const Vec3 p = gSelectedVehicle->position;
	//         const Vec3 f = gSelectedVehicle->forward;
	//         const Vec3 s = gSelectedVehicle->side * 0.25f;
	//         for (float i = 0; i <= 5; i++)
	//         {
	//             drawLine (p + (f * +i) + s, p + (f * +i) - s, gGray60);
	//             drawLine (p + (f * -i) + s, p + (f * -i) - s, gGray60);
	//         }
	//     }
	//     // xxx --------------------------------------------------


	//-------------------------------------------------------------------------
	// check for errors during redraw, report any and then exit
	void 
		OpenGLRenderer::checkForDrawError (const char * locationDescription)
	{
		checkForGLError (locationDescription);
	}


	//-----------------------------------------------------------------------------
	// return a normalized direction vector pointing from the camera towards a
	// given point on the screen: the ray that would be traced for that pixel
	//-----------------------------------------------------------------------------
	Vec3 
		OpenGLRenderer::directionFromCameraToScreenPosition (int x, int y, int h)
	{
#ifdef HAVE_NO_GLUT
		Vec3 direction(0,0,0);
#else

		// Get window height, viewport, modelview and projection matrices
		GLint vp[4];
		GLdouble mMat[16], pMat[16];
		glGetIntegerv (GL_VIEWPORT, vp);
		glGetDoublev (GL_MODELVIEW_MATRIX, mMat);
		glGetDoublev (GL_PROJECTION_MATRIX, pMat);
		GLdouble un0x, un0y, un0z, un1x, un1y, un1z;

		// Unproject mouse position at near and far clipping planes
		gluUnProject (x, h-y, 0, mMat, pMat, vp, &un0x, &un0y, &un0z);
		gluUnProject (x, h-y, 1, mMat, pMat, vp, &un1x, &un1y, &un1z);

		// "direction" is the normalized difference between these far and near
		// unprojected points.  Its parallel to the "eye-mouse" selection line.
		const Vec3 diffNearFar (
			static_cast<osScalar>(un1x-un0x), 
			static_cast<osScalar>(un1y-un0y), 
			static_cast<osScalar>(un1z-un0z));
		const Vec3 direction = diffNearFar.normalized ();
#endif
		return direction;
	}

}

//-----------------------------------------------------------------------------
namespace {

    //-----------------------------------------------------------------------------
    // deferred draw line
    //
    // For use during simulation phase.
    // Stores description of lines to be drawn later.


    class DeferredLine
    {
    public:

        static void addToBuffer (const OpenSteer::Vec3& s,
                                 const OpenSteer::Vec3& e,
                                 const OpenSteer::Color& c)
        {
            DeferredLine dl;
            dl.startPoint = s;
            dl.endPoint = e;
            dl.color = c;

            lines.push_back (dl);
        }

        static void drawAll (void)
        {
            // draw all deferred lines
            for (DeferredLines::iterator i = lines.begin();
                 i < lines.end();
                 i++)
            {
                DeferredLine& dl = *i;
                iDrawLine (dl.startPoint, dl.endPoint, dl.color);
            }

            // clear list of deferred lines
            lines.clear ();
        }

		static void clear (void)
		{
			lines.clear();
		}

        typedef std::vector<OpenSteer::wrapper<DeferredLine>> DeferredLines;

    private:

        OpenSteer::Vec3 startPoint;
        OpenSteer::Vec3 endPoint;
        OpenSteer::Color color;

        static DeferredLines lines;
    };


DeferredLine::DeferredLines DeferredLine::lines;


} // anonymous namespace


//-----------------------------------------------------------------------------
void 
OpenSteer::OpenGLRenderer::deferredDrawLine (const Vec3& startPoint,
                             const Vec3& endPoint,
                             const Color& color)
{
    DeferredLine::addToBuffer (startPoint, endPoint, color);
}


//-----------------------------------------------------------------------------
void 
OpenSteer::OpenGLRenderer::drawAllDeferredLines (void)
{
    DeferredLine::drawAll ();
}

//-----------------------------------------------------------------------------
namespace {

    //-----------------------------------------------------------------------------
    // deferred draw circle
    // XXX for now, just a modified copy of DeferredLine
    //
    // For use during simulation phase.
    // Stores description of circles to be drawn later.


    class DeferredCircle
    {
    public:

        static void addToBuffer (const float radius,
                                 const OpenSteer::Vec3& axis,
                                 const OpenSteer::Vec3& center,
                                 const OpenSteer::Color& color,
                                 const int segments,
                                 const bool filled,
                                 const bool in3d)
        {
            DeferredCircle dc;
            dc.radius   = radius;
            dc.axis     = axis;
            dc.center   = center;
            dc.color    = color;
            dc.segments = segments;
            dc.filled   = filled;
            dc.in3d     = in3d;
            circles.push_back (dc);
        }

        static void drawAll (void)
        {
			OpenSteer::OpenGLRenderer renderer;
            // draw all deferred circles
            for (DeferredCircles::iterator i = circles.begin();
                 i < circles.end();
                 i++)
            {
                DeferredCircle& dc = *i;
                renderer.drawCircleOrDisk (dc.radius, dc.axis, dc.center, dc.color,
                                  dc.segments, dc.filled, dc.in3d);
            }

            // clear list of deferred circles
            circles.clear ();
        }

		static void clear() 
		{
			circles.clear();
		}


        typedef std::vector<OpenSteer::wrapper<DeferredCircle>> DeferredCircles;

    private:

        float radius;
        OpenSteer::Vec3 axis;
        OpenSteer::Vec3 center;
        OpenSteer::Color color;
        int segments;
        bool filled;
        bool in3d;

        static DeferredCircles circles;
    };


DeferredCircle::DeferredCircles DeferredCircle::circles;

} // anonymous namesopace


//-----------------------------------------------------------------------------
namespace OpenSteer {

	//-----------------------------------------------------------------------------
	void 
		OpenGLRenderer::deferredDrawCircleOrDisk (const float radius,
		const Vec3& axis,
		const Vec3& center,
		const Color& color,
		const int segments,
		const bool filled,
		const bool in3d)
	{
		DeferredCircle::addToBuffer (radius, axis, center, color,
			segments, filled, in3d);
	}

	//-----------------------------------------------------------------------------
	void 
		OpenGLRenderer::drawAllDeferredCirclesOrDisks (void)
	{
		DeferredCircle::drawAll ();
	}

	void OpenGLRenderer::clearAllDeferredObjects (void)
	{
		DeferredLine::clear();
		DeferredCircle::clear();
	}

	bool _setTextColor = true;

	//-----------------------------------------------------------------------------
	void 
		OpenGLRenderer::draw2dTextAt3dLocation (const char* text,
		const Vec3& location,
		const Color& color, float w, float h)
	{
		if( (text == nullptr) || (0 == text[0]) ) {
			return;
		}
		// XXX NOTE: "it would be nice if" this had a 2d screenspace offset for
		// the origin of the text relative to the screen space projection of
		// the 3d point.

		// set text color and raster position
		if( _setTextColor == true )
		{
			glColor4f (color.r(), color.g(), color.b(), color.a());
		}
		glRasterPos3f (location.x, location.y, location.z);

		// switch into 2d screen space in case we need to handle a new-line
		GLint rasterPosition[4];
		glGetIntegerv (GL_CURRENT_RASTER_POSITION, rasterPosition);
		const GLint originalMatrixMode = begin2dDrawing (w, h);

		//xxx uncommenting this causes the "2d" version to print the wrong thing
		//xxx with it out the first line of a multi-line "3d" string jiggles
		//glRasterPos2i (rasterPosition[0], rasterPosition[1]);

		// loop over each character in string (until null terminator)
		int lines = 0;
		for (const char* p = text; *p; ++p)
		{
			if (*p == '\n')
			{
				// handle new-line character, reset raster position
				lines++;
				//           const int fontHeight = 15; // for GLUT_BITMAP_9_BY_15
				const int fontHeight = 13; // for GLUT_BITMAP_8_BY_13
				const int vOffset = lines * (fontHeight + 1);
				glRasterPos2i (rasterPosition[0], rasterPosition[1] - vOffset);
			}
			else
			{
				// otherwise draw character bitmap
#ifndef HAVE_NO_GLUT
				glutBitmapCharacter (GLUT_BITMAP_8_BY_13, *p);
				//               glutBitmapCharacter (GLUT_BITMAP_9_BY_15, *p);
#else
				// no character drawing with GLUT presently
#endif
			}
		}

		// switch back out of 2d screen space
		end2dDrawing (originalMatrixMode);
	}

	//-----------------------------------------------------------------------------
	void 
		OpenGLRenderer::draw2dTextAt3dLocation (const std::ostringstream& text,
		const Vec3& location,
		const Color& color, float w, float h)
	{
		draw2dTextAt3dLocation (text.str().c_str(), location, color, w, h);
	}

	//-----------------------------------------------------------------------------
	void 
		OpenGLRenderer::draw2dTextAt2dLocation (const char* text,
		const Vec3& location,
		const Color& color, float w, float h)
	{
		const GLint originalMatrixMode = begin2dDrawing (w, h);

		// draw text at specified location (which is now interpreted as
		// relative to screen space) and color
		draw2dTextAt3dLocation (text, location, color, w, h);

		end2dDrawing (originalMatrixMode);
	}

	//-----------------------------------------------------------------------------
	void 
		OpenGLRenderer::draw2dTextAt2dLocation (const std::ostringstream& text,
		const Vec3& location,
		const Color& color, float w, float h)
	{
		draw2dTextAt2dLocation (text.str().c_str(), location, color, w, h);
	}


    // This class provides methods to draw spheres.  The shape is represented
    // as a "geodesic" mesh of triangles generated by subviding an icosahedron
    // until an edge length criteria is met.  Supports wireframe and unshaded
    // triangle drawing styles.  Provides front/back/both culling of faces.
    //
    // see drawSphere below
    //
    class DrawSphereHelper
    {
    public:
        Vec3 center;
        float radius;
        float maxEdgeLength;
        bool filled;
        Color color;
        bool drawFrontFacing;
        bool drawBackFacing;
        Vec3 viewpoint;

		mutable OpenSteer::OpenGLRenderer renderer;


        // default constructor (at origin, radius=1, white, wireframe, nocull)
        DrawSphereHelper ()
            : center (Vec3::zero),
              radius (1.0f),
              maxEdgeLength (1.0f),
              filled (false),
              color (gWhite),
              drawFrontFacing (true),
              drawBackFacing (true),
              viewpoint (Vec3::zero)
        {}


        // "kitchen sink" constructor (allows specifying everything)
        DrawSphereHelper (const Vec3& _center,
                          const float _radius,
                          const float _maxEdgeLength,
                          const bool _filled,
                          const Color& _color,
                          const bool _drawFrontFacing,
                          const bool _drawBackFacing,
                          const Vec3& _viewpoint)
            : center (_center),
              radius (_radius),
              maxEdgeLength (_maxEdgeLength),
              filled (_filled),
              color (_color),
              drawFrontFacing (_drawFrontFacing),
              drawBackFacing (_drawBackFacing),
              viewpoint (_viewpoint)
        {}


        // draw as an icosahedral geodesic sphere
        void draw (void) const
        {
            // Geometry based on Paul Bourke's excellent article:
            //   Platonic Solids (Regular polytopes in 3D)
            //   http://astronomy.swin.edu.au/~pbourke/polyhedra/platonic/
            const float sqrt5 = sqrtXXX (5.0f);
            const float phi = (1.0f + sqrt5) * 0.5f; // "golden ratio"
            // ratio of edge length to radius
            const float ratio = sqrtXXX (10.0f + (2.0f * sqrt5)) / (4.0f * phi);
            const float a = (radius / ratio) * 0.5f;
            const float b = (radius / ratio) / (2.0f * phi);

            // define the icosahedron's 12 vertices:
            const Vec3 v1  = center + Vec3 ( 0,  b, -a);
            const Vec3 v2  = center + Vec3 ( b,  a,  0);
            const Vec3 v3  = center + Vec3 (-b,  a,  0);
            const Vec3 v4  = center + Vec3 ( 0,  b,  a);
            const Vec3 v5  = center + Vec3 ( 0, -b,  a);
            const Vec3 v6  = center + Vec3 (-a,  0,  b);
            const Vec3 v7  = center + Vec3 ( 0, -b, -a);
            const Vec3 v8  = center + Vec3 ( a,  0, -b);
            const Vec3 v9  = center + Vec3 ( a,  0,  b);
            const Vec3 v10 = center + Vec3 (-a,  0, -b);
            const Vec3 v11 = center + Vec3 ( b, -a,  0);
            const Vec3 v12 = center + Vec3 (-b, -a,  0);

            // draw the icosahedron's 20 triangular faces:
            drawMeshedTriangleOnSphere (v1, v2, v3);
            drawMeshedTriangleOnSphere (v4, v3, v2);
            drawMeshedTriangleOnSphere (v4, v5, v6);
            drawMeshedTriangleOnSphere (v4, v9, v5);
            drawMeshedTriangleOnSphere (v1, v7, v8);
            drawMeshedTriangleOnSphere (v1, v10, v7);
            drawMeshedTriangleOnSphere (v5, v11, v12);
            drawMeshedTriangleOnSphere (v7, v12, v11);
            drawMeshedTriangleOnSphere (v3, v6, v10);
            drawMeshedTriangleOnSphere (v12, v10, v6);
            drawMeshedTriangleOnSphere (v2, v8, v9);
            drawMeshedTriangleOnSphere (v11, v9, v8);
            drawMeshedTriangleOnSphere (v4, v6, v3);
            drawMeshedTriangleOnSphere (v4, v2, v9);
            drawMeshedTriangleOnSphere (v1, v3, v10);
            drawMeshedTriangleOnSphere (v1, v8, v2);
            drawMeshedTriangleOnSphere (v7, v10, v12);
            drawMeshedTriangleOnSphere (v7, v11, v8);
            drawMeshedTriangleOnSphere (v5, v12, v6);
            drawMeshedTriangleOnSphere (v5, v9, v11);
        }


        // given two points, take midpoint and project onto this sphere
        inline Vec3 midpointOnSphere (const Vec3& a, const Vec3& b) const
        {
            const Vec3 midpoint = (a + b) * 0.5f;
            const Vec3 unitRadial = (midpoint - center).normalized ();
            return center + (unitRadial * radius);
        }


        // given three points on the surface of this sphere, if the triangle
        // is "small enough" draw it, otherwise subdivide it into four smaller
        // triangles and recursively draw each of them.
        void drawMeshedTriangleOnSphere (const Vec3& a, 
                                         const Vec3& b,
                                         const Vec3& c) const
        {
            // if all edges are short enough
            if ((((a - b).length ()) < maxEdgeLength) &&
                (((b - c).length ()) < maxEdgeLength) &&
                (((c - a).length ()) < maxEdgeLength))
            {
                // draw triangle
                drawTriangleOnSphere (a, b, c);
            }
            else // otherwise subdivide and recurse
            {
                // find edge midpoints
                const Vec3 ab = midpointOnSphere (a, b);
                const Vec3 bc = midpointOnSphere (b, c);
                const Vec3 ca = midpointOnSphere (c, a);

                // recurse on four sub-triangles
                drawMeshedTriangleOnSphere ( a, ab, ca);
                drawMeshedTriangleOnSphere (ab,  b, bc);
                drawMeshedTriangleOnSphere (ca, bc,  c);
                drawMeshedTriangleOnSphere (ab, bc, ca);
            }
        }


        // draw one mesh element for drawMeshedTriangleOnSphere
        void drawTriangleOnSphere (const Vec3& a, 
                                   const Vec3& b,
                                   const Vec3& c) const
        {
            // draw triangle, subject to the camera orientation criteria
            // (according to drawBackFacing and drawFrontFacing)
            const Vec3 triCenter = (a + b + c) / 3.0f;
            const Vec3 triNormal = triCenter - center; // not unit length
            const Vec3 view = triCenter - viewpoint;
            const float dot = view.dot (triNormal); // project normal on view
            const bool seen = ((dot>0.0f) ? drawBackFacing : drawFrontFacing);
            if (seen)
            {
                if (filled)
                {
                    // draw filled triangle
                    if (drawFrontFacing)
                        renderer.drawTriangle (c, b, a, color);
                    else
                        renderer.drawTriangle (a, b, c, color);
                }
                else
                {
                    // draw triangle edges (use trick to avoid drawing each
                    // edge twice (for each adjacent triangle) unless we are
                    // culling and this tri is near the sphere's silhouette)
                    const float unitDot = view.dot (triNormal.normalized ());
                    const float t = 0.05f; // near threshold
                    const bool nearSilhouette = (unitDot<t) || (unitDot>-t);
                    if (nearSilhouette && !(drawBackFacing&&drawFrontFacing))
                    {
                        renderer.drawLine (a, b, color);
                        renderer.drawLine (b, c, color);
                        renderer.drawLine (c, a, color);
                    }
                    else
                    {
                        drawMeshedTriangleLine (a, b, color);
                        drawMeshedTriangleLine (b, c, color);
                        drawMeshedTriangleLine (c, a, color);
                    }
                }
            }
        }


        // Draws line from A to B but not from B to A: assumes each edge
        // will be drawn in both directions, picks just one direction for
        // drawing according to an arbitary but reproducable criterion.
        void drawMeshedTriangleLine (const Vec3& a, 
                                     const Vec3& b,
                                     const Color& color) const
        {
            if (a.x != b.x)
            {
                if (a.x > b.x) renderer.drawLine (a, b, color);
            }
            else
            {
                if (a.y != b.y)
                {
                    if (a.y > b.y) renderer.drawLine (a, b, color); 
                }
                else
                {
                    if (a.z > b.z) renderer.drawLine (a, b, color); 
                }
            }
        }

    };


    // draw a sphere (wireframe or opaque, with front/back/both culling)
    void OpenGLRenderer::drawSphere (const Vec3& center,
                     const float radius,
                     const float maxEdgeLength,
                     const bool filled,
                     const Color& color,
                     const bool drawFrontFacing,
                     const bool drawBackFacing,
                     const Vec3& viewpoint)
    {
        const DrawSphereHelper s (center, radius, maxEdgeLength, filled, color,
                                  drawFrontFacing, drawBackFacing, viewpoint);
        s.draw ();
    }


    // draw a SphereObstacle
    void OpenGLRenderer::drawSphereObstacle (const SphereObstacle& so,
                             const float maxEdgeLength,
                             const bool filled,
                             const Color& color,
                             const Vec3& viewpoint)
    {
        bool front, back;
        switch (so.seenFrom ())
        {
        default:
        case Obstacle::both:    front = true;  back = true;  break;
        case Obstacle::inside:  front = false; back = true;  break;
        case Obstacle::outside: front = true;  back = false; break;
        }
        drawSphere (so.center, so.radius, maxEdgeLength,
                    filled, color, front, back, viewpoint);
    }

	void profPrintText(float x, float y, char *str);
	float profPrintTextText_width(char *str);
	
	float _gw, _gh;
	void profPrintText(float x, float y, char *str)
	{
		OpenGLRenderer renderer;
		Color color( 1, 1, 1 );
		OpenSteer::Vec3 sp(x, y, 0);
		sp.y += 2;
		_setTextColor = false;
		renderer.draw2dTextAt2dLocation (str, sp, color, _gw, _gh);
		_setTextColor = true;
	}

	float profPrintTextText_width(char *str)
	{
		return 8.0f * strlen(str);
	}

	void 
		OpenGLRenderer::profileDraw(float sx, float sy,
		float width, float height,
		float line_spacing,
		int precision, float sw, float sh)
	{
#if OS_HAVE_PROFILE
		_gw = sw;
		_gh = sh;;
		const GLint originalMatrixMode = begin2dDrawing (sw, sh);
		Prof_draw_gl( sx, sy, width, height, line_spacing, precision, profPrintText, profPrintTextText_width );
		end2dDrawing (originalMatrixMode);
#endif
	}

	void 
		OpenGLRenderer::profileDrawGraph(float sx, float sy,
		float x_spacing, float y_spacing, float sw, float sh)
	{
#if OS_HAVE_PROFILE
		const GLint originalMatrixMode = begin2dDrawing (sw, sh);
		Prof_draw_graph_gl(sx, sy, x_spacing, y_spacing);
		end2dDrawing (originalMatrixMode);
#endif
	}

} // namespace OpenSteer


//-----------------------------------------------------------------------------
