#ifndef OPENSTEER_PLUGIN_H
#define OPENSTEER_PLUGIN_H

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
//
//
//! OpenSteerDemo Plugin class
//
//! Provides AbstractPlugin a pure abstract base class, and Plugin a partial
//! implementation providing default methods to be sub-classed by the
//! programmer defining a new "MyPlugin".
//
//! 10-04-04 bk:  put everything into the OpenSteer namespace
//! 11-13-02 cwr: created 
//
//
//-----------------------------------------------------------------------------

#include "OpenSteer/AbstractPlugin.h"

//-----------------------------------------------------------------------------
namespace OpenSteer {


	//-------------------------------------------------------------------------
	class Plugin : public EntityLocalSpace, public AbstractPlugin
    {
    public:
        //! prototypes for function pointers used with Plugins
        typedef void (* plugInCallBackFunction) ( AbstractPlugin& clientObject );
        typedef void (* voidCallBackFunction) (void);
        typedef void (* timestepCallBackFunction) (const float currentTime,
                                                   const float elapsedTime);

        //! constructor
        Plugin( bool addToRegistry = true );

        //! destructor
        virtual ~Plugin();

		//! return a pointer to this instance's character string name
		virtual const char* pluginName (void) const
		{
			return name();
		}

		virtual void prepareOpen (void) {};

		//! default reset method is to do a close then an open
		virtual void reset (void);

        //! default sort key (after the "built ins")
        float selectionOrderSortKey (void) const {return 1.0f;}

        //! default is to NOT request to be initially selected
        bool requestInitialSelection (void) const {return false;}

        //! default function key handler: ignore all
        //! (parameter names commented out to prevent compiler warning from "-W")
        void handleFunctionKeys (int /*keyNumber*/) {}

        //! default "mini help": print nothing
        void printMiniHelpForFunctionKeys (void) const {}

		//! return the current vehicle proximity database for this plugin
		virtual ProximityDatabase* accessProximityDataBase( void ) const { return nullptr; };

        //! returns pointer to the next Plugin in "selection order"
        AbstractPlugin* next (void) const;

		//! returns pointer to the parent Plugin
		virtual AbstractPlugin* getParentPlugin(void) const 
		{ 
			return getHostPlugin(); 
		};

		//! set a parent Plugin
		virtual void setParentPlugin( AbstractPlugin* plugin ) 
		{
			setParentEntity( castToAbstractEntity( plugin) ); 
		};

		//! implement to initialize additional gui functionality
		virtual void initGui( void* /*userdata*/ ) {};

		//! set an external vehicle factory
		virtual void setEntityFactory( AbstractEntityFactory* entityFactory ) { _entityFactory = entityFactory; }

		virtual AbstractEntityFactory* getEntityFactory( void ) const { return _entityFactory; };

		virtual AbstractEntity* createEntity( EntityClassId classId ) const;

		//! implement to create a vehicle of the specified class
		virtual AbstractVehicle* createVehicle( EntityClassId ) const { return nullptr; };
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

		//! format instance to characters for printing to stream
        friend std::ostream& operator<< (std::ostream& os, Plugin& pi)
        {
            os << "<Plugin " << '"' << pi.pluginName() << '"' << ">";
            return os;
        }

        //! CLASS FUNCTIONS

		static AbstractEntity* createSystemEntity( EntityClassId );


        //! search the class registry for a Plugin with the given name
        static AbstractPlugin* findByName (const char* string);

        //! apply a given function to all Plugins in the class registry
        static void applyToAll (plugInCallBackFunction f);

        //! sort Plugin registry by "selection order"
        static void sortBySelectionOrder (void);

        //! returns pointer to default Plugin (currently, first in registry)
        static AbstractPlugin* findDefault (void);

		//! save this instance in the class's registry of instances
		static void addToRegistry (AbstractPlugin*);
		static AbstractPlugin* findNextPlugin( const AbstractPlugin* thisPtr );

		//! 
		static AbstractPlugin* getSelectedPlugin( void );
		static int getNumPlugins( void );
		static AbstractPlugin* getPluginAt( size_t idx );
		static int getPlugiidx( const AbstractPlugin* plugin );

		//! utility function
		static const AVGroup& allVehiclesOfSelectedPlugin(void);

		// not public right now ... needs to forward gui update
		// select the "next" plug-in, cycling through "plug-in selection order"
		static void selectNextPlugin (void);

		static void selectPlugin( AbstractPlugin* plugin );

		// select the plug-in by index
		static void selectPluginByIndex( size_t idx );

		// handle function keys an a per-plug-in basis
		static void functionKeyForPlugin( int keyNumber );

		// return name of currently selected plug-in
		static const char* nameOfSelectedPlugin( void );

		// reset the currently selected plug-in
		static void resetSelectedPlugin( void );

	protected:
		AbstractEntityFactory* _entityFactory;
		ObstacleGroup _allObstacles;
		AbstractPlayerGroup _allPlayers;

    private:

	};

} //! namespace OpenSteer    
    

//-----------------------------------------------------------------------------
#endif //! OPENSTEER_PLUGIN_H
