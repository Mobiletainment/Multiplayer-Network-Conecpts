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
// OpenSteerDemo Plugin class
//
// 10-04-04 bk:  put everything into the OpenSteer namespace
// 11-13-02 cwr: created
//
//
//-----------------------------------------------------------------------------
#include "stdafx.h"


#include "OpenSteer/Plugin.h"
#include "OpenSteer/Player.h"
#include "OpenSteer/SteeringVehicle.h"
#include "OpenSteer/Camera.h"
#include "OpenSteer/PluginRegistry.h"

namespace OpenSteer	{

	//-----------------------------------------------------------------------------
	// constructor
	Plugin::Plugin( bool addToRegistry )
		:_entityFactory( nullptr )
	{
		// save this new instance in the registry
		if( addToRegistry == true )
		{
			Plugin::addToRegistry ( this );
		}
	}

	//-----------------------------------------------------------------------------
	// destructor
	Plugin::~Plugin()
	{
		if( this == Plugin::getSelectedPlugin() )
		{
			Plugin::selectPlugin( nullptr );
		}
		PluginRegistry::accessInstance()->removeFromRegistry( this );
	}

	//-----------------------------------------------------------------------------
	// default reset method is to do a close then an open
	void Plugin::reset (void) 
	{
		close (); 
		open ();
	}

	//-----------------------------------------------------------------------------
	AbstractEntity*
		Plugin::createEntity( EntityClassId classId ) const
	{
		AbstractEntity* entity(Plugin::createSystemEntity( classId ));
		if( entity == nullptr )
		{
			entity = createVehicle( classId );
		}
		return entity;
	}

	//-----------------------------------------------------------------------------
	AbstractEntity*
		Plugin::createSystemEntity( EntityClassId classId )
	{
		AbstractEntity* entity = nullptr;
		AbstractObstacle* obstacle = nullptr;
		switch( classId )
		{
		case( OS_CID_PLAYER ):
			{
				Player* player(OS_NEW Player());
				entity = player;
			}
			break;
		case( OS_CID_PLAYERCONTROLLER ):
			{
				Controller* controller(OS_NEW Controller());
				entity = controller;
			}
			break;
		case( OS_CID_OBSTACLE ):
			{
				// this is an abstract class no way to create it
				//			obstacle = OS_NEW Obstacle();
			}
			break;
		case( OS_CID_SPHEREOBSTACLE ):
			{
				obstacle = OS_NEW SphereObstacle();
				entity = obstacle;
			}
			break;
		case( OS_CID_BOXOBSTACLE ):
			{
				obstacle = OS_NEW BoxObstacle();
				entity = obstacle;
			}
			break;
		case( OS_CID_PLANEOBSTACLE ):
			{
				obstacle = OS_NEW PlaneObstacle();
				entity = obstacle;
			}
			break;
		case( OS_CID_RECTANGLEOBSTACLE ):
			{
				obstacle = OS_NEW RectangleObstacle();
				entity = obstacle;
			}
			break;
		}
		return entity;
	}

	//-----------------------------------------------------------------------------
	// returns pointer to the next Plugin in "selection order"
	//-----------------------------------------------------------------------------
	AbstractPlugin*
		Plugin::next (void) const
	{
		return PluginRegistry::accessInstance()->findNextPlugin( this );
	}

	//-----------------------------------------------------------------------------
	AbstractPlugin*
		Plugin::findNextPlugin( const AbstractPlugin* thisPtr )
	{
		return PluginRegistry::accessInstance()->findNextPlugin( thisPtr );
	}

	//-----------------------------------------------------------------------------
	int Plugin::getPlugiidx( const AbstractPlugin* plugin )
	{
		return PluginRegistry::accessInstance()->getPlugiidx(plugin);
	}

	//-----------------------------------------------------------------------------
	// search the class registry for a Plugin with the given name
	// returns nullptr if none is found
	AbstractPlugin*
		Plugin::findByName (const char* string)
	{
		return PluginRegistry::accessInstance()->findByName(string);
	}

	//-----------------------------------------------------------------------------
	// apply a given function to all Plugins in the registry
	void
		Plugin::applyToAll( plugInCallBackFunction f )
	{
		PluginRegistry::accessInstance()->applyToAll(f);
	}

	//-----------------------------------------------------------------------------
	// sort Plugin registry by "selection order"
	//
	// XXX replace with STL utilities
	void
		Plugin::sortBySelectionOrder (void)
	{
		PluginRegistry::accessInstance()->sortBySelectionOrder();
	}

	//-----------------------------------------------------------------------------
	// returns pointer to default Plugin (currently, first in registry)
	AbstractPlugin*
		Plugin::findDefault (void)
	{
		return PluginRegistry::accessInstance()->findDefault();
	}

	//-----------------------------------------------------------------------------
	// save this instance in the class's registry of instances
	// (for use by contractors)
	void
		Plugin::addToRegistry (AbstractPlugin* plugin)
	{
		PluginRegistry::accessInstance()->addToRegistry(plugin);
	}

	//-----------------------------------------------------------------------------
	// return a group (an STL vector of AbstractVehicle pointers) of all
	// vehicles(/agents/characters) defined by the currently selected Plugin
	const AVGroup&
		Plugin::allVehiclesOfSelectedPlugin (void)
	{
		return PluginRegistry::accessInstance()->allVehiclesOfSelectedPlugin();
	}

	//-----------------------------------------------------------------------------
	void
		Plugin::selectPlugin( AbstractPlugin* plugin )
	{
		PluginRegistry::accessInstance()->selectPlugin(plugin);
	}

	//-----------------------------------------------------------------------------
	// select the "next" plug-in, cycling through "plug-in selection order"
	void
		Plugin::selectNextPlugin (void)
	{
		PluginRegistry::accessInstance()->selectNextPlugin();
	}

	//-----------------------------------------------------------------------------
	// select the plug-in by index
	void Plugin::selectPluginByIndex (size_t idx)
	{
		PluginRegistry::accessInstance()->selectPluginByIndex(idx);
	}

	//-----------------------------------------------------------------------------
	// handle function keys an a per-plug-in basis
	void
		Plugin::functionKeyForPlugin (int keyNumber)
	{
		PluginRegistry::accessInstance()->functionKeyForPlugin(keyNumber);
	}

	//-----------------------------------------------------------------------------
	// return name of currently selected plug-in
	const char*
		Plugin::nameOfSelectedPlugin (void)
	{
		return PluginRegistry::accessInstance()->nameOfSelectedPlugin();
	}

	//-----------------------------------------------------------------------------
	// reset the currently selected plug-in
	void
		Plugin::resetSelectedPlugin (void)
	{
		PluginRegistry::accessInstance()->resetSelectedPlugin();
	}

	//-----------------------------------------------------------------------------
	int Plugin::getNumPlugins( void ) 
	{ 
		return PluginRegistry::accessInstance()->getNumPlugins(); 
	}

	//-----------------------------------------------------------------------------
	AbstractPlugin* Plugin::getPluginAt( size_t idx ) 
	{ 
		return PluginRegistry::accessInstance()->getPluginAt(idx); 
	}

	AbstractPlugin* Plugin::getSelectedPlugin( void )
	{
		return PluginRegistry::accessInstance()->getSelectedPlugin();
	}

	//-----------------------------------------------------------------------------
	void Plugin::addVehicle (
		AbstractVehicle* vehicle)
	{
		assert( false );
	}

	//-----------------------------------------------------------------------------
	void Plugin::removeVehicle (
		AbstractVehicle* vehicle)
	{
		assert( false );
	}

	//-----------------------------------------------------------------------------
	bool Plugin::queryVehicleColor( const AbstractVehicle& vehicle, Color& kColor ) const 
	{ 
		AbstractPlugin* parent = getParentPlugin();
		if( parent != nullptr )
		{
			return parent->queryVehicleColor( vehicle, kColor );
		}

		return false; 
	}

	//-----------------------------------------------------------------------------
	void Plugin::addObstacle (
		AbstractObstacle* obstacle)
	{
		if( obstacle == nullptr )
		{
			return;
		}
		obstacle->setParentEntity( this );
		allObstacles().push_back( obstacle );
	}

	//-----------------------------------------------------------------------------
	void Plugin::removeObstacle (
		AbstractObstacle* obstacle)
	{
		ObstacleGroup::iterator iter = std::find( _allObstacles.begin(), _allObstacles.end(), obstacle );
		if( iter != _allObstacles.end() )
		{
			obstacle->setParentEntity( nullptr );
			_allObstacles.erase( iter );
		}
	}

	//-----------------------------------------------------------------------------
	void Plugin::addPlayer (
		AbstractPlayer* player)
	{
		if( player == nullptr )
		{
			return;
		}
		player->setParentEntity( this );
		allPlayers().push_back( player );
	}

	//-----------------------------------------------------------------------------
	void Plugin::removePlayer (
		AbstractPlayer* player)
	{
		AbstractPlayerGroup::iterator iter = std::find( _allPlayers.begin(), _allPlayers.end(), player );
		if( iter != _allPlayers.end() )
		{
			player->setParentEntity( nullptr );
			_allPlayers.erase( iter );
		}
	}


}
