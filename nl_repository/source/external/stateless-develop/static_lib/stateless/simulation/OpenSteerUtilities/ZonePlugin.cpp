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

#include "ZonePlugin.h"
#include "CameraPlugin.h"
#include "OpenSteer/Camera.h"
#include "OpenSteer/Renderer.h"

//#include "EduNetCommon/EduNetDraw.h"

namespace OpenSteer
{
	//-----------------------------------------------------------------------------
	void initPluginCamera( AbstractPlugin* plugin )
	{
		// camera setup
		osAbstractLocalSpace* localSpacePtr = dynamic_cast<osAbstractLocalSpace*>(plugin);
		osAbstractLocalSpace& localSpace = *localSpacePtr;
		CameraPlugin::init2dCamera( localSpace );
		// Camera::accessInstance().mode = Camera::cmFixedDistanceOffset;
		Camera::accessInstance().mode = Camera::cmStraightDown;
		Camera::accessInstance().fixedTarget.set( 0, 0, 0 );
		Camera::accessInstance().fixedPosition.set( 0, 20, 0 );
		Camera::accessInstance().lookdownDistance = 15;
		// make camera jump immediately to new position
		Camera::accessInstance().doNotSmoothNextMove ();
		Camera::accessInstance().update( 0, 0, false );
	}

	//-----------------------------------------------------------------------------
	ZonePlugin::ZonePlugin( bool addToRegistry ):
	BaseClass(addToRegistry),
		_zoneExtent( OS_SCALAR( 0.0 ), OS_SCALAR( 0.0 ), OS_SCALAR( 0.0 ) ),
		_solid(0),
		_zoneId(0),
		_borderWidth( OS_SCALAR( 0.0 ) )
	{
		setPosition( osVector3::zero );
		setZoneExtent( osVector3( OS_SCALAR( 5.0 ), OS_SCALAR( 0.0 ), OS_SCALAR( 5.0 ) ) );
		setBorderWidth( 0.0f );
		setZoneColor( Color::_gBlue );
	}

	//-----------------------------------------------------------------------------
	void ZonePlugin::zoneUtility( void )
	{
		const osScalar drawExtent = _zoneExtent.x * OS_SCALAR( 2.0 ) - 0.005f;
		if( 1 == _solid )
		{
			// colors for checkerboard
			const Color gray(0.27f);
			Color zoneGray( getZoneColor() );
			zoneGray.setR( zoneGray.r() * gray.r() );
			zoneGray.setG( zoneGray.g() * gray.g() );
			zoneGray.setB( zoneGray.b() * gray.b() );
			// draw checkerboard grid
			drawXZCheckerboardGrid( drawExtent, 10, position(), zoneGray, gray);
#if 0
			AABBox kZoneAABBox;
			kZoneAABBox.initializeWithCenterAndExtent( position(), _zoneExtent );
			kZoneAABBox.draw( getZoneColor() );
#endif
		}
		else
		{
			// alternate style
			drawXZLineGrid( drawExtent, 1, position(), _ZoneColor );
		}

		const osScalar borderWidth = getBorderWidth();
		if( borderWidth > 0 )
		{
			drawXZLineGrid( drawExtent + borderWidth * OS_SCALAR( 2.0 ), 1, position(), _BorderColor );
		}
	}

	//-----------------------------------------------------------------------------
	void ZonePlugin::prepareOpen( void ) 
	{ 
		addSubZones();
		BaseClass::prepareOpen();
	}

	//-----------------------------------------------------------------------------
	void ZonePlugin::open( void ) 
	{ 
		AbstractPlugin* parent = getParentPlugin();
		ZonePlugin* parentZone = dynamic_cast<ZonePlugin*>(parent);
		// the root zone
		if( parentZone == NULL )
		{
			initPluginCamera( this );
		}

		BaseClass::open();
	}

	//-----------------------------------------------------------------------------
	bool ZonePlugin::isVehicleInside( const AbstractVehicle& vehicle ) const
	{
		return _zoneAABBox.insideXZWithRadius(vehicle);
	}

	bool ZonePlugin::isVehicleInsideBorder( const AbstractVehicle& vehicle ) const
	{
		osVector3 checkZoneExtent = _zoneExtent;
		checkZoneExtent.x += _borderWidth;	
		checkZoneExtent.z += _borderWidth;
		AABBox testBox;
		testBox.initializeWithCenterAndExtent( position(), checkZoneExtent );
		return testBox.insideXZWithRadius(vehicle);
	}


	//-----------------------------------------------------------------------------
	bool ZonePlugin::queryVehicleColor( const AbstractVehicle& vehicle, Color& kColor ) const 
	{ 
		kColor = getZoneColor();
		return true; 
	}

	//-----------------------------------------------------------------------------
	void ZonePlugin::addSubZones( void )
	{
		AbstractPlugin* parent = getParentPlugin();
		ZonePlugin* parentZone = dynamic_cast<ZonePlugin*>(parent);
		// the root zone
		if( parentZone == NULL )
		{
			// any subzones present ?
			if( size() == 0 )
			{
				osScalar colorBase = 0.8f;
				osScalar colorWhite = 0.3f;

				osColor kZoneColors[4] =
				{
					osColor( colorBase, colorWhite, colorWhite ),
					osColor( colorBase, colorBase, colorWhite ),
					osColor( colorWhite, colorBase, colorWhite ),
					osColor( colorWhite, colorWhite, colorBase ),
				};

				osVector3 halfExtent = getZoneExtent();
				halfExtent *= 0.5;
				osVector3 startOffset = position();
				startOffset -= halfExtent;
				osVector3 offset = startOffset; 
				for( size_t i = 0; i < 4; ++i )
				{
					if( i == 2 )
					{
						offset = startOffset;
						offset.x += halfExtent.z * 2;
					}
					ZonePlugin* subZone = ET_NEW ZonePlugin();
					subZone->_zoneId = i;
					subZone->setZoneCenter( getZoneCenter() + offset );
					subZone->setZoneExtent( getZoneExtent() * 0.5f );
					subZone->setZoneColor( kZoneColors[i] );
					subZone->setBorderWidth( 1.0f );

					addPlugin( subZone );
					onSubZoneAdded( subZone );

					offset.z += halfExtent.z * 2;
				}
				addPlugin( ET_NEW CameraPlugin() );
			}
		}
	}

	//-----------------------------------------------------------------------------
	void ZonePlugin::reset(void)
	{
		BaseClass::reset();
	}

	//-----------------------------------------------------------------------------
	void ZonePlugin::update( const float currentTime, const float elapsedTime ) 
	{
		BaseClass::update( currentTime, elapsedTime );
	}

	//-----------------------------------------------------------------------------
	void ZonePlugin::close( void ) 
	{ 
		BaseClass::close();
	}

	//-----------------------------------------------------------------------------
	void ZonePlugin::initGui( void* userdata )
	{
#if 0
		GLUI* glui = ::getRootGLUI();
		GLUI_Panel* pluginPanel = static_cast<GLUI_Panel*>( userdata );
		glui->add_checkbox_to_panel( pluginPanel, "Solid", &_solid);
		BaseClass::initGui( userdata );
#endif
	}

	//-----------------------------------------------------------------------------
	void ZonePlugin::redraw (const float currentTime, const float elapsedTime) 
	{ 
		AbstractPlugin* parent = getParentPlugin();
		ZonePlugin* parentZone = dynamic_cast<ZonePlugin*>(parent);
		// the root zone
		if( parentZone == NULL )
		{
			// right now do not call the base class
			// as we want to see the children ...
			BaseClass::redrawChildren( currentTime, elapsedTime );
			if( isVisible() == false )
			{
				return;
			}
			// draw "zone area"
			// zoneUtility(  );
		}
		else
		{
			// draw "zone area"
			if( isVisible() == true )
			{
				zoneUtility(  );
			}
			BaseClass::redraw( currentTime, elapsedTime );
			if( isVisible() == false )
			{
				return;
			}
			// textual annotation
			AbstractPlugin* contentPlugin = getPlugin(0);
			size_t population = 0;
			if( contentPlugin != NULL )
			{
				osAVGroup vehicles = contentPlugin->allVehicles();
				population = vehicles.size();
			}
			std::ostringstream annote;
			annote << std::setprecision (2) << std::setiosflags (std::ios::fixed);
			annote << "Zone: " << getZoneId() << std::endl << "pos(" << position() << ")" << std::endl;
			annote << "Population: " << population << std::endl << std::ends;
			draw2dTextAt3dLocation (annote, position(), gGreen, drawGetWindowWidth(), drawGetWindowHeight());
		}

	}

	void ZonePlugin::addVehicle( AbstractVehicle* vehicle )
	{
		AbstractPlugin* contentPlugin = getPlugin(0);
		if( contentPlugin != NULL )
		{
			contentPlugin->addVehicle(vehicle);
		}

	}

	void ZonePlugin::removeVehicle( AbstractVehicle* vehicle )
	{
		AbstractPlugin* contentPlugin = getPlugin(0);
		if( contentPlugin != NULL )
		{
			contentPlugin->removeVehicle(vehicle);
		}
	}

}

