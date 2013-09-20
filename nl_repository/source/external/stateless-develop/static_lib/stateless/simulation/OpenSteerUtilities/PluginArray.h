#ifndef __PLUGINARRAY_H__
#define __PLUGINARRAY_H__

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

#include "EduNetCore/EduNetMacros.h"
#include "OpenSteer/Plugin.h"
#include "OpenSteer/SharedPointer.h"

//-----------------------------------------------------------------------------
namespace OpenSteer
{
	typedef SharedPointer<AbstractPlugin> AbstractPluginPtr;
	typedef std::vector<AbstractPluginPtr> TPluginArray;

	//-------------------------------------------------------------------------
	class PluginArray : public EntityLocalSpace, 
		public AbstractPlugin, protected TPluginArray
	{
	public:
		PluginArray(bool addToRegistry = false);
		virtual ~PluginArray();

		//---------------------------------------------------------------------
		// functionality PluginArray
		size_t getPluginCount( void ) const { return size(); }
		void addPlugin( AbstractPlugin* plugin );
		void removePlugin( AbstractPlugin* plugin );
		void removeAllPlugins( void );
		AbstractPlugin* findPlugin( AbstractPlugin* plugin ) const;
		AbstractPlugin* getPlugin( size_t uiIdx ) const;

		// unit test function
		static void testPluginArray( void );

		OS_IMPLEMENT_CLASSNAME( PluginArray )
		//---------------------------------------------------------------------
		// AbstractPlugin interface
		virtual const char* pluginName (void) const
		{
			return name();
		}
		virtual void prepareOpen (void);
		virtual void open(void);
		virtual void update(const float currentTime, const float elapsedTime);
		virtual void redraw(const float currentTime, const float elapsedTime);
		virtual void close(void);
		virtual void reset(void);
		const char* name (void) const {return getClassName();}
		virtual float selectionOrderSortKey(void) const;
		virtual bool requestInitialSelection(void) const;
		virtual void handleFunctionKeys(int keyNumber);
		virtual void printMiniHelpForFunctionKeys(void) const;
		virtual ProximityDatabase* accessProximityDataBase( void ) const {return NULL;};


		virtual AbstractPlugin* next(void) const;

		//! returns pointer to the parent Plugin
		virtual AbstractPlugin* getParentPlugin(void) const;
		//! set a parent Plugin
		virtual void setParentPlugin( AbstractPlugin* plugin );

		// implement to initialize additional gui functionality
		virtual void initGui( void* userdata );

		// implement to create an entity of the specified class
		virtual AbstractEntity* createEntity( EntityClassId classId ) const
		{
			return Plugin::createSystemEntity( classId );
		}

		// implement to create a vehicle of the specified class
		virtual AbstractVehicle* createVehicle( EntityClassId ) const { return NULL; };

		// set an external vehicle factory
		virtual void setEntityFactory( AbstractEntityFactory* ){};

		virtual AbstractEntityFactory* getEntityFactory( void ) const {return NULL;};

		virtual const AVGroup& allVehicles(void) const;
		virtual AVGroup& allVehicles(void);
		virtual void addVehicle (AbstractVehicle* vehicle);
		virtual void removeVehicle (AbstractVehicle* vehicle);

		virtual bool queryVehicleColor( const AbstractVehicle& vehicle, Color& kColor ) const;

		virtual ObstacleGroup& allObstacles( void ) { return _allObstacles; };
		virtual const ObstacleGroup& allObstacles( void ) const { return _allObstacles; };
		virtual void addObstacle (AbstractObstacle* obstacle);
		virtual void removeObstacle (AbstractObstacle* obstacle);

		virtual AbstractPlayerGroup& allPlayers( void ) { return _allPlayers; };
		virtual const AbstractPlayerGroup& allPlayers( void ) const { return _allPlayers; };
		virtual void addPlayer (AbstractPlayer* player);
		virtual void removePlayer (AbstractPlayer* player);
	protected:
		void redrawChildren(const float currentTime, const float elapsedTime);

	private:
		AbstractPlayerGroup _allPlayers;
		ObstacleGroup _allObstacles;
		AVGroup _vehicles;

	};

	//-------------------------------------------------------------------------
	template <class Super>
	class PluginArrayMixin : public Super
	{
		ET_DECLARE_BASE( Super )
	public:
		PluginArrayMixin(bool addToRegistry = true):Super( addToRegistry ),_pluginArray( false ) {};
		virtual ~PluginArrayMixin() {};

		//---------------------------------------------------------------------
		// functionality PluginArray
		void addPlugin( AbstractPlugin* plugin )	{
			_pluginArray.addPlugin( plugin );
		}

		void removePlugin( AbstractPlugin* plugin )	{
			_pluginArray.removePlugin( plugin );
		}

		void removeAllPlugins( void )	{
			_pluginArray.removeAllPlugins( );
		}

		AbstractPlugin* findPlugin( AbstractPlugin* plugin ) const	{
			return _pluginArray.findPlugin( plugin );
		}

		AbstractPlugin* getPlugin( size_t uiIdx ) const	{
			return _pluginArray.getPlugin( uiIdx );
		}
		size_t getPluginCount( void ) const { return _pluginArray.getPluginCount(); }
	protected:
		PluginArray _pluginArray;
	};

	//-------------------------------------------------------------------------
	template <class Super>
	class PluginArrayPluginMixin : public PluginArrayMixin<Super>
	{
		ET_DECLARE_BASE( PluginArrayMixin<Super> )
	public:
		PluginArrayPluginMixin( bool addToRegistry = true ):BaseClass( addToRegistry ) {};
		virtual ~PluginArrayPluginMixin() {};

		virtual void open(void)	{
			_pluginArray.open(  );
			Super::open();
		}

		virtual void update(const float currentTime, const float elapsedTime)	{
			if( isEnabled() == false )
			{
				return;
			}

			const char* pszClassName = name();
			_pluginArray.update( currentTime, elapsedTime );
			Super::update( currentTime, elapsedTime );
		}

		virtual void redraw(const float currentTime, const float elapsedTime)	{
			if( isVisible() == false )
			{
				return;
			}
			_pluginArray.redraw( currentTime, elapsedTime );
			Super::redraw( currentTime, elapsedTime );
		}

		virtual void close(void)	{
			_pluginArray.close(  );
			Super::close();
		}

		virtual void reset(void)	{
			_pluginArray.reset(  );
			Super::reset();
		}

		virtual void initGui( void* userdata )	{
			_pluginArray.initGui( userdata );
			Super::initGui( userdata );
		}

	private:
	};

}


#endif // __PLUGINARRAY_H__
