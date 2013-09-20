#ifndef __GRIDPLUGIN_H__
#define __GRIDPLUGIN_H__

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
#include "EduNetCommon/EduNetCommon.h"
#include "OpenSteerUtilities/OpenSteerUTTypes.h"
#include "OpenSteer/Plugin.h"


namespace OpenSteer
{
	//-------------------------------------------------------------------------
	class GridPlugin : public Plugin
	{
		ET_DECLARE_BASE(Plugin)
	public:
		GridPlugin (bool addToRegistry = false):BaseClass(addToRegistry)
		{};

		OS_IMPLEMENT_CLASSNAME( GridPlugin )
		//----------------------------------------------------------------------------
		// Plugin interface
		virtual void initGui( void* userdata );
		// required methods:
		const char* name( void ) const {return getClassName();}
		void open( void ) { }
		void update( const float currentTime, const float elapsedTime ) { }
		void redraw( const float currentTime, const float elapsedTime );
		void close( void ) { }
		const AVGroup& allVehicles( void ) const { return _Vehicles; }
		AVGroup& allVehicles( void ) { return _Vehicles; }

		// optional methods (see comments in AbstractPlugin for explanation):
		void reset (void) { } // default is to reset by doing close-then-open
		float selectionOrderSortKey( void ) const {return 1000000;}
		bool requestInitialSelection( void ) const {return false;}
		void handleFunctionKeys( int keyNumber ) { } // fkeys reserved for Plugins
		void printMiniHelpForFunctionKeys( void ) { } // if fkeys are used

		static void setGridCenter( const osVector3& kGridCenter )
		{
			GridPlugin::ms_kGridCenter = kGridCenter;
		}

		static void gridUtility( const Vec3& gridTarget );
		static int ms_iSolid;
	private:

		AVGroup _Vehicles;
		static osVector3 ms_kGridCenter;
		ET_IMPLEMENT_CLASS_NO_COPY(GridPlugin)

	};

}


#endif  // __GRIDPLUGIN_H__