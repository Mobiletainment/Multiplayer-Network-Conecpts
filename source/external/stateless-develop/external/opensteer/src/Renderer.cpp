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

#include "OpenSteer/Renderer.h"
#include "OpenSteer/GlobalData.h"

namespace	{
	//-----------------------------------------------------------------------------
	OpenSteer::AbstractRenderer* getRenderInterface( void )
	{
		return OpenSteer::GlobalData::getInstance()->accessRenderer();
	}
}

namespace OpenSteer	{

	//-----------------------------------------------------------------------------
	float drawGetWindowHeight(void)
	{
		return ::getRenderInterface()->drawGetWindowHeight();
	}

	//-----------------------------------------------------------------------------
	float drawGetWindowWidth(void)
	{
		return ::getRenderInterface()->drawGetWindowWidth();
	}

	//-----------------------------------------------------------------------------
	GLint begin2dDrawing (float w, float h)
	{
		return ::getRenderInterface()->begin2dDrawing( w, h );
	}

	//-----------------------------------------------------------------------------
	void end2dDrawing (GLint originalMatrixMode)
	{
		::getRenderInterface()->end2dDrawing(originalMatrixMode);
	}

	//-----------------------------------------------------------------------------
	void 
		drawLine (const Vec3& startPoint,
		const Vec3& endPoint,
		const Color& color)
	{
		::getRenderInterface()->drawLine(startPoint, endPoint, color);
	}

	//-----------------------------------------------------------------------------
	// draw a line with alpha blending

	// see also glAlphaFunc
	// glBlendFunc (GL_SRC_ALPHA)
	// glEnable (GL_BLEND)
	void 
		drawLineAlpha (const Vec3& startPoint,
		const Vec3& endPoint,
		const Color& color,
		const float alpha)
	{
		::getRenderInterface()->drawLineAlpha(startPoint, endPoint, color, alpha);
	}

	//-----------------------------------------------------------------------------
	void 
		drawTriangle (const Vec3& a,
		const Vec3& b,
		const Vec3& c,
		const Color& color)
	{
		::getRenderInterface()->drawTriangle(a,b,c,color);
	}

	//-----------------------------------------------------------------------------
	void 
		drawQuadrangle (const Vec3& a,
		const Vec3& b,
		const Vec3& c,
		const Vec3& d,
		const Color& color)
	{
		::getRenderInterface()->drawQuadrangle(a,b,c,d,color);
	}


	//-------------------------------------------------------------------------
	// draws a "wide line segment": a rectangle of the given width and color
	// whose mid-line connects two given endpoints
	void 
		drawXZWideLine (const Vec3& startPoint,
		const Vec3& endPoint,
		const Color& color,
		float width)
	{
		::getRenderInterface()->drawXZWideLine(startPoint,endPoint,color,width);
	}

	//-------------------------------------------------------------------------
	// General purpose circle/disk drawing routine.  Draws circles or disks (as
	// specified by "filled" argument) and handles both special case 2d circles
	// on the XZ plane or arbitrary circles in 3d space (as specified by "in3d"
	// argument)
	void 
		drawCircleOrDisk (const float radius,
		const Vec3& axis,
		const Vec3& center,
		const Color& color,
		const int segments,
		const bool filled,
		const bool in3d)
	{
		::getRenderInterface()->drawCircleOrDisk(radius,axis,center,color,segments,filled,in3d);
	}

	//-------------------------------------------------------------------------
	void 
		draw3dCircleOrDisk (const float radius,
		const Vec3& center,
		const Vec3& axis,
		const Color& color,
		const int segments,
		const bool filled)
	{
		::getRenderInterface()->draw3dCircleOrDisk(radius, center, axis, color, segments, filled);
	}

	//-------------------------------------------------------------------------
	// drawing utility used by both drawXZCircle and drawXZDisk
	void 
		drawXZCircleOrDisk (const float radius,
		const Vec3& center,
		const Color& color,
		const int segments,
		const bool filled)
	{
		::getRenderInterface()->drawXZCircleOrDisk(radius,center,color,segments,filled);
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
		drawXZArc (const Vec3& start,
		const Vec3& center,
		const float arcLength,
		const int segments,
		const Color& color)
	{
		::getRenderInterface()->drawXZArc(start,center,arcLength,segments,color);
	}

	//-------------------------------------------------------------------------
	// a simple 2d vehicle on the XZ plane
	//-----------------------------------------------------------------------------
	void 
		drawBasic2dCircularVehicle (const AbstractVehicle& vehicle,
		const Color& color)
	{
		::getRenderInterface()->drawBasic2dCircularVehicle(vehicle,color);
	}

	//-----------------------------------------------------------------------------
	void 
		drawBasic2dCircularLocalSpace (const LocalSpaceData& vehicle,
		const Color& color, float fRadius, bool bDrawCircle, float fUpOffset )
	{
		::getRenderInterface()->drawBasic2dCircularLocalSpace(vehicle,color,fRadius,bDrawCircle,fUpOffset);
	}

	//-------------------------------------------------------------------------
	// a simple 3d vehicle
	void 
		drawBasic3dSphericalVehicle (const AbstractVehicle& vehicle,
		const Color& color)
	{
		::getRenderInterface()->drawBasic3dSphericalVehicle(vehicle,color);
	}

	// drawBasic3dSphericalVehicle with a supplied draw routine
	// provided so non-OpenGL based apps can draw a boid
	void 
		drawBasic3dSphericalVehicle (drawTriangleRoutine draw, const AbstractVehicle& vehicle,
		const Color& color)
	{
		::getRenderInterface()->drawBasic3dSphericalVehicle(draw,vehicle,color);
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
		drawXZCheckerboardGrid (const float size,
		const int subsquares,
		const Vec3& center,
		const Color& color1,
		const Color& color2)
	{
		::getRenderInterface()->drawXZCheckerboardGrid(size,subsquares,center,color1,color2);
	}


	//-------------------------------------------------------------------------
	// draw a square grid of lines on the XZ (horizontal) plane.
	//
	// ("size" is the length of a side of the overall grid, "subsquares" is the
	// number of subsquares along each edge (for example a standard checkboard
	// has eight), "center" is the 3d position of the center of the grid, lines
	// are drawn in the specified "color".)
	void 
		drawXZLineGrid (const float size,
		const int subsquares,
		const Vec3& center,
		const Color& color)
	{
		::getRenderInterface()->drawXZLineGrid(size,subsquares,center,color);
	}

	//-------------------------------------------------------------------------
	// draw a square grid of lines on the XZ (horizontal) plane.
	//
	// ("size" is the length of a side of the overall grid, "subsquares" is the
	// number of subsquares along each edge (for example a standard checkboard
	// has eight), "center" is the 3d position of the center of the grid, lines
	// are drawn in the specified "color".)
	void 
		drawXYLineGrid (const float size,
		const int subsquares,
		const Vec3& center,
		const Color& color)
	{
		::getRenderInterface()->drawXYLineGrid(size,subsquares,center,color);
	}
	//-------------------------------------------------------------------------
	// draw the three axes of a LocalSpace: three lines parallel to the
	// basis vectors of the space, centered at its origin, of lengths
	// given by the coordinates of "size".
	void 
		drawAxes  (const AbstractLocalSpace& ls,
		const Vec3& size,
		const Color& color)
	{
		::getRenderInterface()->drawAxes(ls,size,color);
	}

	//-------------------------------------------------------------------------
	// draw the edges of a box with a given position, orientation, size
	// and color.  The box edges are aligned with the axes of the given
	// LocalSpace, and it is centered at the origin of that LocalSpace.
	// "size" is the main diagonal of the box.
	//
	// use gGlobalSpace to draw a box aligned with global space
	void 
		drawBoxOutline  (const AbstractLocalSpace& localSpace,
		const Vec3& size,
		const Color& color)
	{
		::getRenderInterface()->drawBoxOutline(localSpace,size,color);
	}

	//-------------------------------------------------------------------------
	// Define scene's camera (viewing transformation) in terms of the camera's
	// position, the point to look at (an "aim point" in the scene which will
	// end up at the center of the camera's view), and an "up" vector defining
	// the camera's "roll" around the "view axis" between cameraPosition and
	// pointToLookAt (the image of the up vector will be vertical in the
	// camera's view).
	void 
		drawCameraLookAt (const Vec3& cameraPosition,
		const Vec3& pointToLookAt,
		const Vec3& up)
	{
		::getRenderInterface()->drawCameraLookAt(cameraPosition,pointToLookAt,up);
	}

	//-----------------------------------------------------------------------------
	void 
		draw2dLine (const Vec3& startPoint,
		const Vec3& endPoint,
		const Color& color, 
		float w, float h)
	{
		::getRenderInterface()->draw2dLine(startPoint,endPoint,color,w,h);
	}

	//-------------------------------------------------------------------------
	// draw a reticle at the center of the window.  Currently it is small
	// crosshair with a gap at the center, drawn in white with black borders
	void 
		drawReticle (float w, float h)
	{
		::getRenderInterface()->drawReticle(w,h);
	}

	//-------------------------------------------------------------------------
	// check for errors during redraw, report any and then exit
	void 
		checkForDrawError (const char * locationDescription)
	{
		::getRenderInterface()->checkForDrawError(locationDescription);
	}

	//-----------------------------------------------------------------------------
	// return a normalized direction vector pointing from the camera towards a
	// given point on the screen: the ray that would be traced for that pixel
	//-----------------------------------------------------------------------------
	Vec3 
		directionFromCameraToScreenPosition (int x, int y, int h)
	{
		return ::getRenderInterface()->directionFromCameraToScreenPosition(x,y,h);
	}

	//-----------------------------------------------------------------------------
	void 
		deferredDrawLine (const Vec3& startPoint,
		const Vec3& endPoint,
		const Color& color)
	{
		::getRenderInterface()->deferredDrawLine(startPoint, endPoint, color);
	}

	//-----------------------------------------------------------------------------
	void 
		drawAllDeferredLines (void)
	{
		::getRenderInterface()->drawAllDeferredLines();
	}

	//-----------------------------------------------------------------------------
	void 
		deferredDrawCircleOrDisk (const float radius,
		const Vec3& axis,
		const Vec3& center,
		const Color& color,
		const int segments,
		const bool filled,
		const bool in3d)
	{
		::getRenderInterface()->deferredDrawCircleOrDisk(radius,axis,center,color,segments,filled,in3d);
	}

	//-----------------------------------------------------------------------------
	void 
		drawAllDeferredCirclesOrDisks (void)
	{
		::getRenderInterface()->drawAllDeferredCirclesOrDisks();
	}

	//-----------------------------------------------------------------------------
	void 
		draw2dTextAt3dLocation (const char* text,
		const Vec3& location,
		const Color& color, float w, float h)
	{
		::getRenderInterface()->draw2dTextAt3dLocation(text,location,color,w,h);
	}

	//-----------------------------------------------------------------------------
	void 
		draw2dTextAt3dLocation (const std::ostringstream& text,
		const Vec3& location,
		const Color& color, float w, float h)
	{
		::getRenderInterface()->draw2dTextAt3dLocation(text,location,color,w,h);
	}

	//-----------------------------------------------------------------------------
	void 
		draw2dTextAt2dLocation (const char* text,
		const Vec3& location,
		const Color& color, float w, float h)
	{
		::getRenderInterface()->draw2dTextAt2dLocation(text, location, color, w, h);
	}

	//-----------------------------------------------------------------------------
	void 
		draw2dTextAt2dLocation (const std::ostringstream& text,
		const Vec3& location,
		const Color& color, float w, float h)
	{
		::getRenderInterface()->draw2dTextAt2dLocation(text,location,color,w,h);
	}



	void drawSphere (const Vec3& center,
		const float radius,
		const float maxEdgeLength,
		const bool filled,
		const Color& color,
		const bool drawFrontFacing,
		const bool drawBackFacing,
		const Vec3& viewpoint)
	{
		::getRenderInterface()->drawSphere(center,radius,maxEdgeLength,filled,color,drawFrontFacing,drawBackFacing,viewpoint);
	}

	//! draw a SphereObstacle
	void drawSphereObstacle (const SphereObstacle& so,
		const float maxEdgeLength,
		const bool filled,
		const Color& color,
		const Vec3& viewpoint)
	{
		::getRenderInterface()->drawSphereObstacle(so,maxEdgeLength,filled,color,viewpoint);
	}

	void profileDraw(float sx, float sy,
		float width, float height,
		float line_spacing,
		int precision, float sw, float sh)
	{
		::getRenderInterface()->profileDraw( sx,sy,width, height,line_spacing,precision,sw,sh);
	}

	void  profileDrawGraph(float sx, float sy,
		float x_spacing, float y_spacing, float sw, float sh)
	{
		::getRenderInterface()->profileDrawGraph( sx,sy,x_spacing,y_spacing,sw,sh );
	}

}
