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


#include "OpenSteerUT.h"

#include "OpenSteer/GlobalSelection.h"
#include "OpenSteer/GlobalData.h"
#include "OpenSteer/AbstractRenderer.h"

#include "LocalController.h"

OpenSteerUTData* OpenSteerUTData::g_openSteerUTDataPtr = NULL;

//-----------------------------------------------------------------------------
void OpenSteerUTData::_SDMInitApp( EduNet::IProfile* profile )
{
	OpenSteer::GlobalData::_SDMInitApp( profile );
	// note: set up data to pass to loaded modules
	static OpenSteerUTData g_openSteerUTData;
	OpenSteerUTData::g_openSteerUTDataPtr = &g_openSteerUTData;
	OpenSteerUTData::g_openSteerUTDataPtr->globalSelection = OpenSteer::GlobalSelection::getInstance();
	OpenSteerUTData::g_openSteerUTDataPtr->globalData = OpenSteer::GlobalData::getInstance();
}

//-----------------------------------------------------------------------------
void OpenSteerUTData::_SDMInitDLL( OpenSteerUTData* data )
{
	OpenSteerUTData::g_openSteerUTDataPtr = data;
	OpenSteer::GlobalSelection::_SDMInitDLL( OpenSteerUTData::g_openSteerUTDataPtr->globalSelection );
	OpenSteer::GlobalData::_SDMInitDLL( OpenSteerUTData::g_openSteerUTDataPtr->globalData );
}

//-----------------------------------------------------------------------------
GLUI* getRootGLUI()
{
	assert( OpenSteerUTData::g_openSteerUTDataPtr != NULL );
	return OpenSteerUTData::g_openSteerUTDataPtr->appGlui;
}

#if 0
//-----------------------------------------------------------------------------
// accessors for GLUT's window dimensions
float
OpenSteer::extern_drawGetWindowHeight ( void )
{
	assert( OpenSteerUTData::g_openSteerUTDataPtr != NULL );
	if ( OpenSteerUTData::g_openSteerUTDataPtr->viewPort.th > 0 )
	{
		return OpenSteerUTData::g_openSteerUTDataPtr->viewPort.th;
	}
	return glutGet ( GLUT_WINDOW_HEIGHT );
}

//-----------------------------------------------------------------------------
float
OpenSteer::extern_drawGetWindowWidth ( void )
{
	assert( OpenSteerUTData::g_openSteerUTDataPtr != NULL );
	if ( OpenSteerUTData::g_openSteerUTDataPtr->viewPort.tw > 0 )
	{
		return OpenSteerUTData::g_openSteerUTDataPtr->viewPort.tw;
	}
	return glutGet ( GLUT_WINDOW_WIDTH );
}
#endif

