#ifndef OPENSTEER_RENDERER_H
#define OPENSTEER_RENDERER_H

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


#include "OpenSteer/AbstractRenderer.h"


namespace OpenSteer {

	AbstractRenderer* getRenderInterface( void );


	float drawGetWindowHeight(void);
	float drawGetWindowWidth(void);


	GLint begin2dDrawing (float w, float h);
	void end2dDrawing (GLint originalMatrixMode);


    //-------------------------------------------------------------------------
    //! draw the three axes of a LocalSpace: three lines parallel to the
    //! basis vectors of the space, centered at its origin, of lengths
    //! given by the coordinates of "size".


    void drawAxes  (const AbstractLocalSpace& localSpace,
                    const Vec3& size,
                    const Color& color);


    //-------------------------------------------------------------------------
    //! draw the edges of a box with a given position, orientation, size
    //! and color.  The box edges are aligned with the axes of the given
    //! LocalSpace, and it is centered at the origin of that LocalSpace.
    //! "size" is the main diagonal of the box.


    void drawBoxOutline  (const AbstractLocalSpace& localSpace,
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


    void drawXZCheckerboardGrid (const float size,
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


    void drawXZLineGrid (const float size,
                         const int subsquares,
                         const Vec3& center,
                         const Color& color);

	void drawXYLineGrid (const float size,
		const int subsquares,
		const Vec3& center,
		const Color& color);


    //-------------------------------------------------------------------------
    //! Circle/disk drawing utilities


    void drawCircleOrDisk (const float radius,
                           const Vec3& axis,
                           const Vec3& center,
                           const Color& color,
                           const int segments,
                           const bool filled,
                           const bool in3d);

    void drawXZCircleOrDisk (const float radius,
                             const Vec3& center,
                             const Color& color,
                             const int segments,
                             const bool filled);

    void draw3dCircleOrDisk (const float radius,
                             const Vec3& center,
                             const Vec3& axis,
                             const Color& color,
                             const int segments,
                             const bool filled);

	inline
    void drawXZCircle (const float radius,
                              const Vec3& center,
                              const Color& color,
                              const int segments)
    {
        drawXZCircleOrDisk (radius, center, color, segments, false);
    }

	inline
    void drawXZDisk (const float radius,
                            const Vec3& center,
                            const Color& color,
                            const int segments)
    {
        drawXZCircleOrDisk (radius, center, color, segments, true);
    }

	inline
    void draw3dCircle (const float radius,
                              const Vec3& center,
                              const Vec3& axis,
                              const Color& color,
                              const int segments)
    {
        draw3dCircleOrDisk (radius, center, axis, color, segments, false);
    }

	inline
    void draw3dDisk (const float radius,
                            const Vec3& center,
                            const Vec3& axis,
                            const Color& color,
                            const int segments)
    {
        draw3dCircleOrDisk (radius, center, axis, color, segments, true);
    }


    //! draw a circular arc on the XZ plane, from a start point, around a center,
    //! for a given arc length, in a given number of segments and color.  The
    //! sign of arcLength determines the direction in which the arc is drawn.

    void drawXZArc (const Vec3& start,
                    const Vec3& center,
                    const float arcLength,
                    const int segments,
                    const Color& color);


    //-------------------------------------------------------------------------
    //! Sphere drawing utilities


    //! draw a sphere (wireframe or opaque, with front/back/both culling)
    void drawSphere (const Vec3& center,
                     const float radius,
                     const float maxEdgeLength,
                     const bool filled,
                     const Color& color,
                     const bool drawFrontFacing = true,
                     const bool drawBackFacing = true,
                     const Vec3& viewpoint = Vec3::zero);

    //! draw a SphereObstacle
	void drawSphereObstacle (const SphereObstacle& so,
		const float maxEdgeLength,
		const bool filled,
		const Color& color,
		const Vec3& viewpoint);


    //-------------------------------------------------------------------------
    //! draw a reticle at the center of the window.  Currently it is small
    //! crosshair with a gap at the center, drawn in white with black borders
    //! width and height of screen are passed in


    void drawReticle (float w, float h);


    //-------------------------------------------------------------------------
	void drawBasic2dCircularLocalSpace (const LocalSpaceData& vehicle,
		const Color& color, float fRadius, bool bDrawCircle = true, float fUpOffset = 0.0f );


    void drawBasic2dCircularVehicle (const AbstractVehicle& bv,
                                     const Color& color);

    void drawBasic3dSphericalVehicle (const AbstractVehicle& bv,
                                      const Color& color);

    void drawBasic3dSphericalVehicle (drawTriangleRoutine, const AbstractVehicle& bv,
                                      const Color& color);

    //-------------------------------------------------------------------------
    //! 2d text drawing requires w, h since retrieving viewport w and h differs
    //! for every graphics API

    void draw2dTextAt3dLocation (const char* text,
                                 const Vec3& location,
                                 const Color& color, float w, float h);

    void draw2dTextAt3dLocation (const std::ostringstream& text,
                                 const Vec3& location,
                                 const Color& color, float w, float h);

    void draw2dTextAt2dLocation (const char* text,
                                 const Vec3& location,
                                 const Color& color, float w, float h);

    void draw2dTextAt2dLocation (const std::ostringstream& text,
                                 const Vec3& location,
                                 const Color& color, float w, float h);

    //-----------------------------------------------------------------------------
    //! draw 3d "graphical annotation" lines, used for debugging


    void drawLine (const Vec3& startPoint,
                   const Vec3& endPoint,
                   const Color& color);


    //-----------------------------------------------------------------------------
    //! draw 2d lines in screen space: x and y are the relevant coordinates
    //! w and h are the dimensions of the viewport in pixels
    void draw2dLine (const Vec3& startPoint,
                    const Vec3& endPoint,
                    const Color& color,
                    float w, float h);


    //-----------------------------------------------------------------------------
    //! draw a line with alpha blending

    void drawLineAlpha (const Vec3& startPoint,
                        const Vec3& endPoint,
                        const Color& color,
                        const float alpha);


    //-------------------------------------------------------------------------
    //! deferred drawing of lines, circles and (filled) disks


    void deferredDrawLine (const Vec3& startPoint,
                           const Vec3& endPoint,
                           const Color& color);

    void deferredDrawCircleOrDisk (const float radius,
                                   const Vec3& axis,
                                   const Vec3& center,
                                   const Color& color,
                                   const int segments,
                                   const bool filled,
                                   const bool in3d);

    void drawAllDeferredLines (void);
    void drawAllDeferredCirclesOrDisks (void);


    //-------------------------------------------------------------------------
    //! Draw a single OpenGL triangle given three Vec3 vertices.


    void drawTriangle (const Vec3& a,
                       const Vec3& b,
                       const Vec3& c,
                       const Color& color);


    //-------------------------------------------------------------------------
    //! Draw a single OpenGL quadrangle given four Vec3 vertices, and color.


    void drawQuadrangle (const Vec3& a,
                         const Vec3& b,
                         const Vec3& c,
                         const Vec3& d,
                         const Color& color);


    //-----------------------------------------------------------------------------
    //! draws a "wide line segment": a rectangle of the given width and color
    //! whose mid-line connects two given endpoints


    void drawXZWideLine (const Vec3& startPoint,
                         const Vec3& endPoint,
                         const Color& color,
                         float width);


    //-----------------------------------------------------------------------------


    void drawCameraLookAt (const Vec3& cameraPosition,
                           const Vec3& pointToLookAt,
                           const Vec3& up);


    //-----------------------------------------------------------------------------
    //! check for errors during redraw, report any and then exit


    void checkForDrawError (const char * locationDescription);



    //-----------------------------------------------------------------------------
    //! return a normalized direction vector pointing from the camera towards a
    //! given point on the screen: the ray that would be traced for that pixel


    Vec3 directionFromCameraToScreenPosition (int x, int y, int h);

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
	void profileDraw(float sx, float sy,
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
	void profileDrawGraph(float sx, float sy,
		float x_spacing, float y_spacing, float sw, float sh);


} //! namespace OpenSteer


//-----------------------------------------------------------------------------
#endif //! OPENSTEER_RENDERER_H
