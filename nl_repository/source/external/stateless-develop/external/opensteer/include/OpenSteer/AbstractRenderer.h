#ifndef OPENSTEER_ABSTRACTRENDERER_H
#define OPENSTEER_ABSTRACTRENDERER_H

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


#include "OpenSteer/Vec3.h"
#include "OpenSteer/Color.h"
#include "OpenSteer/AbstractVehicle.h"
#include "OpenSteer/Obstacle.h"

typedef int GLint;

namespace OpenSteer {

	extern float extern_drawGetWindowHeight(void);
	extern float extern_drawGetWindowWidth(void);

	//-------------------------------------------------------------------------
	//! this is a typedef for a triangle draw routine which can be passed in
	//! when using rendering API's of the user's choice.
	typedef void (*drawTriangleRoutine) (const Vec3& a,
		const Vec3& b,
		const Vec3& c,
		const Color& color);

	class AbstractRenderer	{
	public:
		virtual ~AbstractRenderer(){}
	
	virtual float drawGetWindowHeight(void) = 0;
	virtual float drawGetWindowWidth(void) = 0;


	virtual GLint begin2dDrawing (float w, float h) = 0;
	virtual void end2dDrawing (GLint originalMatrixMode) = 0;

    //-------------------------------------------------------------------------
    //! draw the three axes of a LocalSpace: three lines parallel to the
    //! basis vectors of the space, centered at its origin, of lengths
    //! given by the coordinates of "size".


    virtual void drawAxes  (const AbstractLocalSpace& localSpace,
                    const Vec3& size,
                    const Color& color) = 0;


    //-------------------------------------------------------------------------
    //! draw the edges of a box with a given position, orientation, size
    //! and color.  The box edges are aligned with the axes of the given
    //! LocalSpace, and it is centered at the origin of that LocalSpace.
    //! "size" is the main diagonal of the box.


    virtual void drawBoxOutline  (const AbstractLocalSpace& localSpace,
                          const Vec3& size,
                          const Color& color) = 0;


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
                                 const Color& color2) = 0;


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
                         const Color& color) = 0;

	virtual void drawXYLineGrid (const float size,
		const int subsquares,
		const Vec3& center,
		const Color& color) = 0;


    //-------------------------------------------------------------------------
    //! Circle/disk drawing utilities


    virtual void drawCircleOrDisk (const float radius,
                           const Vec3& axis,
                           const Vec3& center,
                           const Color& color,
                           const int segments,
                           const bool filled,
                           const bool in3d) = 0;

    virtual void drawXZCircleOrDisk (const float radius,
                             const Vec3& center,
                             const Color& color,
                             const int segments,
                             const bool filled) = 0;

    virtual void draw3dCircleOrDisk (const float radius,
                             const Vec3& center,
                             const Vec3& axis,
                             const Color& color,
                             const int segments,
                             const bool filled) = 0;

    virtual void drawXZCircle (const float radius,
                              const Vec3& center,
                              const Color& color,
                              const int segments) = 0;

    virtual void drawXZDisk (const float radius,
                            const Vec3& center,
                            const Color& color,
                            const int segments) = 0;

    virtual void draw3dCircle (const float radius,
                              const Vec3& center,
                              const Vec3& axis,
                              const Color& color,
                              const int segments) = 0;

    virtual void draw3dDisk (const float radius,
                            const Vec3& center,
                            const Vec3& axis,
                            const Color& color,
                            const int segments) = 0;


    //! draw a circular arc on the XZ plane, from a start point, around a center,
    //! for a given arc length, in a given number of segments and color.  The
    //! sign of arcLength determines the direction in which the arc is drawn.

    virtual void drawXZArc (const Vec3& start,
                    const Vec3& center,
                    const float arcLength,
                    const int segments,
                    const Color& color) = 0;


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
                     const Vec3& viewpoint = Vec3::zero) = 0;

    //! draw a SphereObstacle
    virtual void drawSphereObstacle (const SphereObstacle& so,
                             const float maxEdgeLength,
                             const bool filled,
                             const Color& color,
                             const Vec3& viewpoint = Vec3::zero) = 0;


    //-------------------------------------------------------------------------
    //! draw a reticle at the center of the window.  Currently it is small
    //! crosshair with a gap at the center, drawn in white with black borders
    //! width and height of screen are passed in


    virtual void drawReticle (float w, float h) = 0;


    //-------------------------------------------------------------------------
	virtual void drawBasic2dCircularLocalSpace (const LocalSpaceData& vehicle,
		const Color& color, float fRadius, bool bDrawCircle = true, float fUpOffset = 0.0f ) = 0;


    virtual void drawBasic2dCircularVehicle (const AbstractVehicle& bv,
                                     const Color& color) = 0;

    virtual void drawBasic3dSphericalVehicle (const AbstractVehicle& bv,
                                      const Color& color) = 0;

    virtual void drawBasic3dSphericalVehicle (drawTriangleRoutine, const AbstractVehicle& bv,
                                      const Color& color) = 0;

    //-------------------------------------------------------------------------
    //! 2d text drawing requires w, h since retrieving viewport w and h differs
    //! for every graphics API

    virtual void draw2dTextAt3dLocation (const char* text,
                                 const Vec3& location,
                                 const Color& color, float w, float h) = 0;

    virtual void draw2dTextAt3dLocation (const std::ostringstream& text,
                                 const Vec3& location,
                                 const Color& color, float w, float h) = 0;

    virtual void draw2dTextAt2dLocation (const char* text,
                                 const Vec3& location,
                                 const Color& color, float w, float h) = 0;

    virtual void draw2dTextAt2dLocation (const std::ostringstream& text,
                                 const Vec3& location,
                                 const Color& color, float w, float h) = 0;

    //-------------------------------------------------------------------------
    //! emit an OpenGL vertex based on a Vec3


    virtual void glVertexVec3 (const Vec3& v) = 0;


    //-----------------------------------------------------------------------------
    //! draw 3d "graphical annotation" lines, used for debugging


    virtual void drawLine (const Vec3& startPoint,
                   const Vec3& endPoint,
                   const Color& color) = 0;


    //-----------------------------------------------------------------------------
    //! draw 2d lines in screen space: x and y are the relevant coordinates
    //! w and h are the dimensions of the viewport in pixels
    virtual void draw2dLine (const Vec3& startPoint,
                    const Vec3& endPoint,
                    const Color& color,
                    float w, float h) = 0;


    //-----------------------------------------------------------------------------
    //! draw a line with alpha blending

    virtual void drawLineAlpha (const Vec3& startPoint,
                        const Vec3& endPoint,
                        const Color& color,
                        const float alpha) = 0;


    //-------------------------------------------------------------------------
    //! deferred drawing of lines, circles and (filled) disks


    virtual void deferredDrawLine (const Vec3& startPoint,
                           const Vec3& endPoint,
                           const Color& color) = 0;

    virtual void deferredDrawCircleOrDisk (const float radius,
                                   const Vec3& axis,
                                   const Vec3& center,
                                   const Color& color,
                                   const int segments,
                                   const bool filled,
                                   const bool in3d) = 0;

    virtual void drawAllDeferredLines (void) = 0;
    virtual void drawAllDeferredCirclesOrDisks (void) = 0;
	virtual void clearAllDeferredObjects(void) = 0;

    //-------------------------------------------------------------------------
    //! Draw a single OpenGL triangle given three Vec3 vertices.


    virtual void drawTriangle (const Vec3& a,
                       const Vec3& b,
                       const Vec3& c,
                       const Color& color) = 0;


    //-------------------------------------------------------------------------
    //! Draw a single OpenGL quadrangle given four Vec3 vertices, and color.


    virtual void drawQuadrangle (const Vec3& a,
                         const Vec3& b,
                         const Vec3& c,
                         const Vec3& d,
                         const Color& color) = 0;


    //-----------------------------------------------------------------------------
    //! draws a "wide line segment": a rectangle of the given width and color
    //! whose mid-line connects two given endpoints


    virtual void drawXZWideLine (const Vec3& startPoint,
                         const Vec3& endPoint,
                         const Color& color,
                         float width) = 0;


    //-----------------------------------------------------------------------------


    virtual void drawCameraLookAt (const Vec3& cameraPosition,
                           const Vec3& pointToLookAt,
                           const Vec3& up) = 0;


    //-----------------------------------------------------------------------------
    //! check for errors during redraw, report any and then exit


    virtual void checkForDrawError (const char * locationDescription) = 0;



    //-----------------------------------------------------------------------------
    //! return a normalized direction vector pointing from the camera towards a
    //! given point on the screen: the ray that would be traced for that pixel


    virtual Vec3 directionFromCameraToScreenPosition (int x, int y, int h) = 0;

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
		int precision, float sw, float sh) = 0;

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
		float x_spacing, float y_spacing, float sw, float sh) = 0;

	};
} //! namespace OpenSteer


//-----------------------------------------------------------------------------
#endif //! OPENSTEER_DRAW_H
