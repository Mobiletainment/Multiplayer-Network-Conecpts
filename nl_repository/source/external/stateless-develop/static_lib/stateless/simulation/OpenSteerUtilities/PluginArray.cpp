#include "stdafx.h"
#include "EduNetMemory/EduNetMemory.h"
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

#include "OpenSteerUtilities/PluginArray.h"

#include "OpenSteerUtilities/AbstractVehicleGroup.h"
#include "OpenSteerUtilities/AbstractPluginUtilities.h"

namespace OpenSteer
{
	//-----------------------------------------------------------------------------
	PluginArray::PluginArray( bool addToRegistry )
	{
		if( addToRegistry == true )
		{
			Plugin::addToRegistry( this );
		}
	}

	//-----------------------------------------------------------------------------
	PluginArray::~PluginArray()
	{
		removeAllPlugins();
	}

	//-----------------------------------------------------------------------------
	AbstractPlugin* PluginArray::getParentPlugin(void) const
	{
		return getHostPlugin();
	}

	//-----------------------------------------------------------------------------
	void PluginArray::setParentPlugin( AbstractPlugin* plugin )
	{
		setParentEntity( castToAbstractEntity( plugin) );
	}

	//-----------------------------------------------------------------------------
	void PluginArray::removeAllPlugins( void )
	{
		AbstractPlugin* plugin = getPlugin( 0 );
		while( plugin != NULL )
		{
			removePlugin( plugin );
			plugin = getPlugin( 0 );
		}
	}

	//-----------------------------------------------------------------------------
	void PluginArray::addPlugin( AbstractPlugin* plugin )
	{
		if( findPlugin( plugin ) == NULL )
		{
			AbstractPluginPtr spPlugin(plugin);
			plugin->setParentPlugin( this );
			push_back( spPlugin );
		}
	}

	//-----------------------------------------------------------------------------
	AbstractPlugin* PluginArray::findPlugin( AbstractPlugin* plugin ) const
	{
		TPluginArray::const_iterator iter = begin();
		TPluginArray::const_iterator endIter = end();
		while( iter != endIter  )
		{
			AbstractPlugin* arrayPlugin = (*iter).get();
			if( arrayPlugin == plugin )
			{
				return plugin;
			}
			++iter;
		}
		return NULL;
	}

	//-----------------------------------------------------------------------------
	void PluginArray::removePlugin( AbstractPlugin* plugin )
	{
		TPluginArray::iterator iter = begin();
		TPluginArray::iterator endIter = end();
		while( iter != endIter  )
		{
			AbstractPlugin* arrayPlugin = (*iter).get();
			if( arrayPlugin == plugin )
			{
				const char* pluginName( arrayPlugin->pluginName() );
				arrayPlugin->close();
				arrayPlugin->setParentPlugin( NULL );
				erase( iter );
				break;
			}
			++iter;
		}
	}

	//-----------------------------------------------------------------------------
	AbstractPlugin* PluginArray::getPlugin( size_t uiIdx ) const
	{
		if( uiIdx < size() )
		{
			return (*this)[uiIdx].get();
		}
		return NULL;
	}

	//-----------------------------------------------------------------------------
	void PluginArray::prepareOpen(void)
	{
		TPluginArray::iterator iter = begin();
		TPluginArray::iterator endIter = end();
		while( iter != endIter  )
		{
			AbstractPlugin* plugin = (*iter).get();
			plugin->prepareOpen();
			++iter;
		}
	}

	//-----------------------------------------------------------------------------
	void PluginArray::open(void)
	{
		TPluginArray::iterator iter = begin();
		TPluginArray::iterator endIter = end();
		while( iter != endIter  )
		{
			AbstractPlugin* plugin = (*iter).get();
			plugin->open();
			++iter;
		}
	}

	//-----------------------------------------------------------------------------
	void PluginArray::update(const float currentTime, const float elapsedTime)
	{
		if( isEnabled() == false )
		{
			return;
		}
		TPluginArray::iterator iter = begin();
		TPluginArray::iterator endIter = end();
		while( iter != endIter  )
		{
			AbstractPlugin* plugin = (*iter).get();
			AbstractUpdated* updatedPlugin = dynamic_cast<osAbstractUpdated*>(plugin);
			assert( updatedPlugin != NULL );
			updatedPlugin->update( currentTime, elapsedTime );
			++iter;
		}
	}

	//-----------------------------------------------------------------------------
	void PluginArray::redraw(const float currentTime, const float elapsedTime)
	{
		if( isVisible() == false )
		{
			return;
		}
		redrawChildren( currentTime, elapsedTime );
	}

	//-----------------------------------------------------------------------------
	void PluginArray::redrawChildren(const float currentTime, const float elapsedTime)
	{
		TPluginArray::iterator iter = begin();
		TPluginArray::iterator endIter = end();
		while( iter != endIter  )
		{
			AbstractPlugin* plugin = (*iter).get();
			plugin->redraw( currentTime, elapsedTime );
			++iter;
		}
	}

	//-----------------------------------------------------------------------------
	void PluginArray::close(void)
	{
		TPluginArray::iterator iter = begin();
		TPluginArray::iterator endIter = end();
		while( iter != endIter  )
		{
			AbstractPlugin* plugin = (*iter).get();
			plugin->close(  );
			++iter;
		}
	}

	//-----------------------------------------------------------------------------
	void PluginArray::reset(void)
	{
		close();
		open();
	}

	//-----------------------------------------------------------------------------
	float PluginArray::selectionOrderSortKey(void) const
	{
		return 1.0f;
	}

	//-----------------------------------------------------------------------------
	bool PluginArray::requestInitialSelection(void) const
	{
		return false;
	}

	//-----------------------------------------------------------------------------
	void PluginArray::handleFunctionKeys(int keyNumber)
	{
		TPluginArray::iterator iter = begin();
		TPluginArray::iterator endIter = end();
		while( iter != endIter  )
		{
			AbstractPlugin* plugin = (*iter).get();
			plugin->handleFunctionKeys( keyNumber );
			++iter;
		}
	}

	//-----------------------------------------------------------------------------
	void PluginArray::printMiniHelpForFunctionKeys(void) const
	{
		TPluginArray::const_iterator iter = begin();
		TPluginArray::const_iterator endIter = end();
		while( iter != endIter  )
		{
			AbstractPlugin* plugin = (*iter).get();
			plugin->printMiniHelpForFunctionKeys( );
			++iter;
		}
	}

	//-----------------------------------------------------------------------------
	const AVGroup& PluginArray::allVehicles(void) const
	{
		{
			const AVGroup& vehicles = _vehicles;
			if( vehicles.size() > 0 )
			{
				return vehicles;
			}
		}
		TPluginArray::const_iterator iter = begin();
		TPluginArray::const_iterator endIter = end();

		// non remote plugins first
		while( iter != endIter  )
		{
			AbstractPlugin* plugin = (*iter).get();
			AbstractEntity* pluginEntity = dynamic_cast<AbstractEntity*>( plugin );
			assert( pluginEntity != NULL );
			if( pluginEntity->isRemoteObject() == false )
			{
				const AVGroup& vehicles = plugin->allVehicles();
				if( vehicles.size() > 0 )
				{
					return vehicles;
				}
			}
			++iter;
		}
		iter = begin();
		while( iter != endIter  )
		{
			AbstractPlugin* plugin = (*iter).get();
			AbstractEntity* pluginEntity = dynamic_cast<AbstractEntity*>( plugin );
			assert( pluginEntity != NULL );
			if( pluginEntity->isRemoteObject() == true )
			{
				const AVGroup& vehicles = plugin->allVehicles();
				if( vehicles.size() > 0 )
				{
					return vehicles;
				}
			}
			++iter;
		}


		return _vehicles;
	}

	//-----------------------------------------------------------------------------
	AVGroup& PluginArray::allVehicles(void)
	{
		{
			AVGroup& vehicles = _vehicles;
			if( vehicles.size() > 0 )
			{
				return vehicles;
			}
		}
		TPluginArray::const_iterator iter = begin();
		TPluginArray::const_iterator endIter = end();
		while( iter != endIter  )
		{
			AbstractPlugin* plugin = (*iter).get();
			AbstractEntity* pluginEntity = dynamic_cast<AbstractEntity*>( plugin );
			assert( pluginEntity != NULL );
			if( pluginEntity->isRemoteObject() == false )
			{
				AVGroup& vehicles = plugin->allVehicles();
				if( vehicles.size() > 0 )
				{
					return vehicles;
				}
			}
			++iter;
		}
		iter = begin();
		while( iter != endIter  )
		{
			AbstractPlugin* plugin = (*iter).get();
			AbstractEntity* pluginEntity = dynamic_cast<AbstractEntity*>( plugin );
			assert( pluginEntity != NULL );
			if( pluginEntity->isRemoteObject() == true )
			{
				AVGroup& vehicles = plugin->allVehicles();
				if( vehicles.size() > 0 )
				{
					return vehicles;
				}
			}
			++iter;
		}
		return _vehicles;
	}

	//-----------------------------------------------------------------------------
	void PluginArray::addVehicle ( AbstractVehicle* vehicle)
	{
		assert( false );
	}

	//-----------------------------------------------------------------------------
	void PluginArray::removeVehicle ( AbstractVehicle* vehicle)
	{
		assert( false );
	}

	//-----------------------------------------------------------------------------
	bool PluginArray::queryVehicleColor( const AbstractVehicle& vehicle, Color& kColor ) const
	{

		return false;
	}

	//-----------------------------------------------------------------------------
	void PluginArray::addObstacle ( AbstractObstacle* obstacle)
	{
		if( obstacle == NULL )
		{
			return;
		}
		obstacle->setParentEntity( this );
		allObstacles().push_back( obstacle );
	}

	//-----------------------------------------------------------------------------
	void PluginArray::removeObstacle ( AbstractObstacle* obstacle)
	{
		ObstacleGroup::iterator iter = std::find( _allObstacles.begin(), _allObstacles.end(), obstacle );
		if( iter != _allObstacles.end() )
		{
			obstacle->setParentEntity( NULL );
			_allObstacles.erase( iter );
		}
	}

	//-----------------------------------------------------------------------------
	void PluginArray::addPlayer ( AbstractPlayer* player)
	{
		if( player == NULL )
		{
			return;
		}
		player->setParentEntity( this );
		allPlayers().push_back( player );
	}

	//-----------------------------------------------------------------------------
	void PluginArray::removePlayer ( AbstractPlayer* player)
	{
		AbstractPlayerGroup::iterator iter = std::find( _allPlayers.begin(), _allPlayers.end(), player );
		if( iter != _allPlayers.end() )
		{
			player->setParentEntity( NULL );
			_allPlayers.erase( iter );
		}
	}
	//-----------------------------------------------------------------------------
	AbstractPlugin* PluginArray::next(void) const
	{
		return Plugin::findNextPlugin( this );
	}

	// implement to initialize additional gui functionality
	//-----------------------------------------------------------------------------
	void PluginArray::initGui( void* userdata )
	{
#if 0
		GLUI* glui = ::getRootGLUI();
		GLUI_Panel* pluginPanel = static_cast<GLUI_Panel*>(userdata);

		TPluginArray::iterator iter = begin();
		TPluginArray::iterator endIter = end();
		while( iter != endIter  )
		{
			AbstractPlugin* plugin = (*iter).get();
			GLUI_Panel* subPluginPanel = AbstractPluginGui::initSubPluginGui( plugin, userdata );
			if( subPluginPanel != NULL )
			{

			}
			++iter;
		}
#endif
	}

	//-----------------------------------------------------------------------------
	class FooPlugin : public Plugin
	{
		ET_DECLARE_BASE(Plugin)
	public:
		FooPlugin (bool addToRegistry = false):BaseClass(addToRegistry){};

		// required methods:
		const char* name (void) const {return "Foo";}
		void open (void) { }
		void update (const float currentTime, const float elapsedTime) { }
		void redraw (const float currentTime, const float elapsedTime) { }
		void close (void) { }
		const AVGroup& allVehicles (void) const { return _vehicles; }

		// optional methods (see comments in AbstractPlugin for explanation):
		void reset (void) { } // default is to reset by doing close-then-open
		float selectionOrderSortKey (void) const {return 1234;}
		bool requestInitialSelection (void) const {return false;}
		void handleFunctionKeys (int keyNumber) { } // fkeys reserved for Plugins
		void printMiniHelpForFunctionKeys (void) { } // if fkeys are used
	private:
		AVGroup _vehicles;
		ET_IMPLEMENT_CLASS_NO_COPY(FooPlugin)

	};

	//-----------------------------------------------------------------------------
	void PluginArray::testPluginArray( void )
	{
		PluginArray gTestPluginArray;

		PluginArray* add[] =
		{
			ET_NEW PluginArray,
			ET_NEW PluginArray,
			ET_NEW PluginArray
		};
		gTestPluginArray.addPlugin( add[0] );
		gTestPluginArray.addPlugin( add[1] );
		gTestPluginArray.addPlugin( add[2] );


		gTestPluginArray.removePlugin( add[0] );
	}

}

