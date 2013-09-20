#ifndef __ZONEPLUGIN_H__
#define __ZONEPLUGIN_H__

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
#include "OpenSteer/AABBox.h"
#include "PluginArray.h"
#include "OpenSteerUTTypes.h"
namespace OpenSteer
{
	//-------------------------------------------------------------------------
	class ZonePlugin : public PluginArray
	{
		ET_DECLARE_BASE( PluginArray )
	public:
		ZonePlugin ( bool addToRegistry = false );

		OS_IMPLEMENT_CLASSNAME( ZonePlugin )
		//----------------------------------------------------------------------------
		// Plugin interface
		virtual void initGui( void* userdata );
		// required methods:
		const char* name( void ) const { return getClassName(); }
		virtual void prepareOpen (void);
		virtual void open( void );
		virtual void update( const float currentTime, const float elapsedTime );
		virtual void redraw( const float currentTime, const float elapsedTime );
		virtual void close( void );
		virtual const AVGroup& allVehicles( void ) const { return _vehicles; }
		virtual AVGroup& allVehicles( void ) { return _vehicles; }
		virtual bool queryVehicleColor( const AbstractVehicle& vehicle, osColor& kColor ) const;


		// optional methods (see comments in AbstractPlugin for explanation):
		virtual void reset (void); // default is to reset by doing close-then-open
		virtual float selectionOrderSortKey( void ) const {return 1000000;}
		virtual bool requestInitialSelection( void ) const {return false;}
		virtual void handleFunctionKeys( int keyNumber ) { } // fkeys reserved for Plugins
		virtual void printMiniHelpForFunctionKeys( void ) const { } // if fkeys are used

		virtual void addVehicle (AbstractVehicle* vehicle);
		virtual void removeVehicle (AbstractVehicle* vehicle);

		// extende interface
		virtual void onSubZoneAdded( ZonePlugin* subZone ){};

		void setBorderWidth( osScalar value )
		{
			_borderWidth = value;
			updateZoneAABBox();
		}

		void setZoneCenter( const osVector3& kGridCenter )
		{
			setPosition( kGridCenter );
			updateZoneAABBox();
		}

		void setZoneExtent( const osVector3& kExtent )
		{
			_zoneExtent = kExtent;
			updateZoneAABBox();
		}

		void setZoneColor( const osColor& kColor )
		{
			_ZoneColor = kColor;
			_BorderColor = kColor;
			_BorderColor *= 0.5f;
		}

		osScalar getBorderWidth( void ) const
		{
			return _borderWidth;
		}

		const osVector3& getZoneExtent( void ) const { return _zoneExtent; }
		const osVector3& getZoneCenter( void ) const { return position(); }
		const osColor& getZoneColor( void ) const { return _ZoneColor; }
		const osColor& getBorderColor( void ) const { return _BorderColor; }

		size_t getZoneId( void ) const { return _zoneId; }

		bool isVehicleInside( const AbstractVehicle& vehicle ) const;
		bool isVehicleInsideBorder( const AbstractVehicle& vehicle ) const;

		int _solid;
	private:

		void updateZoneAABBox( void )
		{
			osVector3 checkZoneExtent = _zoneExtent;
			//checkZoneExtent.x += _borderWidth;
//			checkZoneExtent.y += _borderWidth;
			//checkZoneExtent.z += _borderWidth;
			_zoneAABBox.initializeWithCenterAndExtent( position(), checkZoneExtent );
		}
		void zoneUtility( void );
		void addSubZones( void );

		osColor _BorderColor;
		osColor _ZoneColor;
		osScalar (_borderWidth);
		size_t _zoneId;

		AVGroup _vehicles;
		osVector3 _zoneExtent;

		AABBox _zoneAABBox;

		ET_IMPLEMENT_CLASS_NO_COPY(ZonePlugin)

	};

}


#endif  // __ZONEPLUGIN_H__
