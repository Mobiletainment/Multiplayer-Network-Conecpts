#include "stdafx.h"
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

#include "GridPlugin.h"
#include "OpenSteer/Color.h"
#include "OpenSteer/Draw.h"
#include "OpenSteer/Renderer.h"
//#include "EduNetCommon/EduNetDraw.h"


namespace OpenSteer	{
	//-----------------------------------------------------------------------------
	osVector3 GridPlugin::ms_kGridCenter(osVector3::zero);
	int GridPlugin::ms_iSolid(1);
	//-----------------------------------------------------------------------------
	void GridPlugin::gridUtility( const Vec3& gridTarget )
	{
		// round off target to the nearest multiple of 2 (because the
		// checkerboard grid with a pitch of 1 tiles with a period of 2)
		// then lower the grid a bit to put it under 2d annotation lines
		const Vec3 gridCenter ((round (gridTarget.x * 0.5f) * 2),
			(round (gridTarget.y * 0.5f) * 2) - .05f,
			(round (gridTarget.z * 0.5f) * 2));



		if( 1 == GridPlugin::ms_iSolid )
		{
			// colors for checkerboard
			const Color gray1(0.27f);
			const Color gray2(0.30f);
			// draw 50x50 checkerboard grid with 50 squares along each side
			drawXZCheckerboardGrid (50, 50, gridCenter, gray1, gray2);
		}
		else
		{
			// alternate style
			drawXYLineGrid (50, 50, gridCenter, gBlack);
			drawXZLineGrid (50, 50, gridCenter, gBlack);
		}
	}

	//-----------------------------------------------------------------------------
	void GridPlugin::initGui( void* userdata )
	{
#if 0
		GLUI* glui = ::getRootGLUI();
		GLUI_Panel* pluginPanel = static_cast<GLUI_Panel*>( userdata );
		glui->add_checkbox_to_panel( pluginPanel, "Solid", &GridPlugin::ms_iSolid);
#endif
	}

	//-----------------------------------------------------------------------------
	void GridPlugin::redraw (const float currentTime, const float elapsedTime) 
	{ 
		if( isVisible() == false )
		{
			return;
		}
		// draw "ground plane"
		GridPlugin::gridUtility( GridPlugin::ms_kGridCenter );
	}

}

