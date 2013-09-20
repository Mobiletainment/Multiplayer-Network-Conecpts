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
//! Draw
//
//! This is a first stab at a graphics module for OpenSteerDemo.  It is intended
//! to encapsulate all functionality related to 3d graphics as well as windows
//! and graphics input devices such as the mouse.
//
//! However this is purely an OpenGL-based implementation.  No special effort
//! has been made to keep the "OpenGL way" from leaking through.  Attempting to
//! port this to another graphics substrate may run into modularity problems.
//
//! In any case, all calls to the underlying graphics substrate should be made
//! from this module only.
//
//! 10-04-04 bk:  put everything into the OpenSteer namespace
//! 06-25-02 cwr: created 
//
//
//-----------------------------------------------------------------------------


#ifndef OPENSTEER_DRAW_H
#define OPENSTEER_DRAW_H

#include "OpenSteer/AbstractRenderer.h"

namespace OpenSteer {

	//! hosting application must provide this bool. It's true when updating and not drawing,
	//! false otherwise.
	//! it has been externed as a first step in making the Draw library useful from
	//! other applications besides OpenSteerDemo
	
	extern bool updatePhaseActive;

	class OpenGLRenderer : public AbstractRenderer
	{
	public:
		virtual ~OpenGLRenderer(){}
		
		virtual float drawGetWindowHeight(void);
		virtual float drawGetWindowWidth(void);

		virtual GLint begin2dDrawing (float w, float h);
		virtual void end2dDrawing (GLint originalMatrixMode);

		//-------------------------------------------------------------------------
		//! warn when draw functions are called during OpenSteerDemo's update phase
		//
		//! XXX perhaps this should be made to "melt away" when not in debug mode?


		//-------------------------------------------------------------------------
		//! draw the three axes of a LocalSpace: three lines parallel to the
		//! basis vectors of the space, centered at its origin, of lengths
		//! given by the coordinates of "size".
		virtual void drawAxes  (const AbstractLocalSpace& localSpace,
						const Vec3& size,
						const Color& color);


		//-------------------------------------------------------------------------
		//! draw the edges of a box with a given position, orientation, size
		//! and color.  The box edges are aligned with the axes of the given
		//! LocalSpace, and it is centered at the origin of that LocalSpace.
		//! "size" is the main diagonal of the box.


		virtual void drawBoxOutline  (const AbstractLocalSpace& localSpace,
							  const Vec3& size,
							  const Color& color);


		//-------------------------------------------------------------------------
		//! draw a (filled-in, polygon-based) square checkerboard grid on the XZ
		//! (horizontal) plane.
		//
		//! ("size" is the length of a side of the overall checkerboard, "subsquares"
		//! is the number of subsquares along each edge (for example a standard
		//! checkboard has eight), "center" is the 3d position of the center of the
		//! grid, color1 and color2 are used for alternating subsquares.)


		virtual void drawXZCheckerboardGrid (const float size,
									 const int subsquares,
									 const Vec3& center,
									 const Color& color1,
									 const Color& color2);


		//-------------------------------------------------------------------------
		//! draw a square grid of lines on the XZ (horizontal) plane.
		//
		//! ("size" is the length of a side of the overall grid, "subsquares" is the
		//! number of subsquares along each edge (for example a standard checkboard
		//! has eight), "center" is the 3d position of the center of the grid, lines
		//! are drawn in the specified "color".)
		virtual void drawXZLineGrid (const float size,
							 const int subsquares,
							 const Vec3& center,
							 const Color& color);

		virtual void drawXYLineGrid (const float size,
			const int subsquares,
			const Vec3& center,
			const Color& color);


		//-------------------------------------------------------------------------
		//! Circle/disk drawing utilities
		virtual void drawCircleOrDisk (const float radius,
							   const Vec3& axis,
							   const Vec3& center,
							   const Color& color,
							   const int segments,
							   const bool filled,
							   const bool in3d);

		virtual void drawXZCircleOrDisk (const float radius,
								 const Vec3& center,
								 const Color& color,
								 const int segments,
								 const bool filled);

		virtual void draw3dCircleOrDisk (const float radius,
								 const Vec3& center,
								 const Vec3& axis,
								 const Color& color,
								 const int segments,
								 const bool filled);

		virtual void drawXZCircle (const float radius,
								  const Vec3& center,
								  const Color& color,
								  const int segments);

		virtual void drawXZDisk (const float radius,
								const Vec3& center,
								const Color& color,
								const int segments);

		virtual void draw3dCircle (const float radius,
								  const Vec3& center,
								  const Vec3& axis,
								  const Color& color,
								  const int segments);

		virtual void draw3dDisk (const float radius,
								const Vec3& center,
								const Vec3& axis,
								const Color& color,
								const int segments);


		//! draw a circular arc on the XZ plane, from a start point, around a center,
		//! for a given arc length, in a given number of segments and color.  The
		//! sign of arcLength determines the direction in which the arc is drawn.

		virtual void drawXZArc (const Vec3& start,
						const Vec3& center,
						const float arcLength,
						const int segments,
						const Color& color);


		//-------------------------------------------------------------------------
		//! Sphere drawing utilities


		//! draw a sphere (wireframe or opaque, with front/back/both culling)
		virtual void drawSphere (const Vec3& center,
						 const float radius,
						 const float maxEdgeLength,
						 const bool filled,
						 const Color& color,
						 const bool drawFrontFacing = true,
						 const bool drawBackFacing = true,
						 const Vec3& viewpoint = Vec3::zero);

		//! draw a SphereObstacle
		virtual void drawSphereObstacle (const SphereObstacle& so,
								 const float maxEdgeLength,
								 const bool filled,
								 const Color& color,
								 const Vec3& viewpoint);


		//-------------------------------------------------------------------------
		//! draw a reticle at the center of the window.  Currently it is small
		//! crosshair with a gap at the center, drawn in white with black borders
		//! width and height of screen are passed in


		virtual void drawReticle (float w, float h);


		//-------------------------------------------------------------------------
		virtual void drawBasic2dCircularLocalSpace (const LocalSpaceData& vehicle,
			const Color& color, float fRadius, bool bDrawCircle = true, float fUpOffset = 0.0f );


		virtual void drawBasic2dCircularVehicle (const AbstractVehicle& bv,
										 const Color& color);

		virtual void drawBasic3dSphericalVehicle (const AbstractVehicle& bv,
										  const Color& color);

		virtual void drawBasic3dSphericalVehicle (drawTriangleRoutine, const AbstractVehicle& bv,
										  const Color& color);

		//-------------------------------------------------------------------------
		//! 2d text drawing requires w, h since retrieving viewport w and h differs
		//! for every graphics API

		virtual void draw2dTextAt3dLocation (const char* text,
									 const Vec3& location,
									 const Color& color, float w, float h);

		virtual void draw2dTextAt3dLocation (const std::ostringstream& text,
									 const Vec3& location,
									 const Color& color, float w, float h);

		virtual void draw2dTextAt2dLocation (const char* text,
									 const Vec3& location,
									 const Color& color, float w, float h);

		virtual void draw2dTextAt2dLocation (const std::ostringstream& text,
									 const Vec3& location,
									 const Color& color, float w, float h);

		//-------------------------------------------------------------------------
		//! emit an OpenGL vertex based on a Vec3


		virtual void glVertexVec3 (const Vec3& v);


		//-------------------------------------------------------------------------
		//! draw 3d "graphical annotation" lines, used for debugging


		virtual void drawLine (const Vec3& startPoint,
					   const Vec3& endPoint,
					   const Color& color);


		//-------------------------------------------------------------------------
		//! draw 2d lines in screen space: x and y are the relevant coordinates
		//! w and h are the dimensions of the viewport in pixels
		virtual void draw2dLine (const Vec3& startPoint,
						const Vec3& endPoint,
						const Color& color,
						float w, float h);


		//-------------------------------------------------------------------------
		//! draw a line with alpha blending

		virtual void drawLineAlpha (const Vec3& startPoint,
							const Vec3& endPoint,
							const Color& color,
							const float alpha);


		//-------------------------------------------------------------------------
		//! deferred drawing of lines, circles and (filled) disks


		virtual void deferredDrawLine (const Vec3& startPoint,
							   const Vec3& endPoint,
							   const Color& color);

		virtual void deferredDrawCircleOrDisk (const float radius,
									   const Vec3& axis,
									   const Vec3& center,
									   const Color& color,
									   const int segments,
									   const bool filled,
									   const bool in3d);

		virtual void drawAllDeferredLines (void);
		virtual void drawAllDeferredCirclesOrDisks (void);

		virtual void clearAllDeferredObjects (void);


		//-------------------------------------------------------------------------
		//! Draw a single OpenGL triangle given three Vec3 vertices.


		virtual void drawTriangle (const Vec3& a,
						   const Vec3& b,
						   const Vec3& c,
						   const Color& color);


		//-------------------------------------------------------------------------
		//! Draw a single OpenGL quadrangle given four Vec3 vertices, and color.


		virtual void drawQuadrangle (const Vec3& a,
							 const Vec3& b,
							 const Vec3& c,
							 const Vec3& d,
							 const Color& color);


		//-------------------------------------------------------------------------
		//! draws a "wide line segment": a rectangle of the given width and color
		//! whose mid-line connects two given endpoints


		virtual void drawXZWideLine (const Vec3& startPoint,
							 const Vec3& endPoint,
							 const Color& color,
							 float width);


		//-------------------------------------------------------------------------


		virtual void drawCameraLookAt (const Vec3& cameraPosition,
							   const Vec3& pointToLookAt,
							   const Vec3& up);


		//-------------------------------------------------------------------------
		//! check for errors during redraw, report any and then exit


		virtual void checkForDrawError (const char * locationDescription);



		//-------------------------------------------------------------------------
		//! return a normalized direction vector pointing from the camera towards a
		//! given point on the screen: the ray that would be traced for that pixel


		virtual Vec3 directionFromCameraToScreenPosition (int x, int y, int h);

		/*
		*  Prof_draw_gl -- display the current report via OpenGL
		*
		*  You must provide a callable text-printing function.
		*  Put the opengl state into a 2d rendering mode.
		*
		*  Parameters:
		*    <sx,sy>         --  location where top line is drawn
		*    <width, height> --  total size of display (if too small, text will overprint)
		*    line_spacing    --  how much to move sy by after each line; use a
		*                        negative value if y decreases down the screen
		*    precision       --  decimal places of precision for time data, 1..4 (try 2)
		*    print_text      --  function to display a line of text starting at the
		*                        given coordinate; best if 0,1..9 are fixed-width
		*    text_width      --  a function that computes the pixel-width of
		*                        a given string before printing. you can fake with a
		*                        simple approximation of width('0')*strlen(str)
		*
		*  to avoid overprinting, you can make print_text truncate long strings
		*/
		virtual void profileDraw(float sx, float sy,
			float width, float height,
			float line_spacing,
			int precision, float sw, float sh);

		/*
		*  Parameters
		*    <sx, sy>      --  origin of the graph--location of (0,0)
		*    x_spacing     --  screenspace size of each history sample; e.g.
		*                         2.0 pixels
		*    y_spacing     --  screenspace size of one millisecond of time;
		*                         for an app with max of 20ms in any one zone,
		*                         8.0 would produce a 160-pixel tall display,
		*                         assuming screenspace is in pixels
		*/
		virtual void profileDrawGraph(float sx, float sy,
			float x_spacing, float y_spacing, float sw, float sh);

	};

} //! namespace OpenSteer


//-----------------------------------------------------------------------------
#endif //! OPENSTEER_DRAW_H
