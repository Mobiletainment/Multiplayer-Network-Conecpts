#ifndef OPENSTEER_ABSTRACTPLUGIN_H
#define OPENSTEER_ABSTRACTPLUGIN_H

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
//! A pattern for a specific user-defined OpenSteerDemo Plugin class called Foo.
//! Defines class FooPlugin, then makes a single instance (singleton) of it.
/*


class FooPlugin : public Plugin
{
    //! required methods:
    const char* name (void) const {return "Foo";}
    void open (void) {...}
    void update (const float currentTime, const float elapsedTime) {...}
    void redraw (const float currentTime, const float elapsedTime) {...}
    void close (void) {...}
    const AVGroup& allVehicles (void) const {...}

    //! optional methods (see comments in AbstractPlugin for explanation):
    void reset (void) {...} //! default is to reset by doing close-then-open
    float selectionOrderSortKey (void) const {return 1234;}
    bool requestInitialSelection (void) const {return false;}
    void handleFunctionKeys (int keyNumber) {...} //! fkeys reserved for Plugins
    void printMiniHelpForFunctionKeys (void) {...} //! if fkeys are used
};

FooPlugin gFooPlugin;


*/
//-----------------------------------------------------------------------------

#include <iostream>
#include "OpenSteer/Entity.h"
#include "OpenSteer/AbstractVehicle.h"
#include "OpenSteer/AbstractPlayer.h"
#include "OpenSteer/Obstacle.h"



//-----------------------------------------------------------------------------
namespace OpenSteer {

	class AbstractEntityFactory;

	//-------------------------------------------------------------------------
    class AbstractPlugin
    {
    public:
        
        virtual ~AbstractPlugin() { /* Nothing to do. */ }
        
        //! generic Plugin actions: prepareOpen open, update, redraw, close and reset
		virtual void prepareOpen (void) OS_ABSTRACT;
        virtual void open (void) OS_ABSTRACT;
// see AbstractUpdated
//       virtual void update (const float currentTime, const float elapsedTime) OS_ABSTRACT;
        virtual void redraw (const float currentTime, const float elapsedTime) OS_ABSTRACT;
        virtual void close (void) OS_ABSTRACT;
        virtual void reset (void) OS_ABSTRACT;

        //! return a pointer to this instance's character string name
        virtual const char* pluginName (void) const OS_ABSTRACT;

        //! numeric sort key used to establish user-visible Plugin ordering
        //! ("built ins" have keys greater than 0 and less than 1)
        virtual float selectionOrderSortKey (void) const OS_ABSTRACT;

        //! allows a Plugin to nominate itself as OpenSteerDemo's initially selected
        //! (default) Plugin, which is otherwise the first in "selection order"
        virtual bool requestInitialSelection (void) const OS_ABSTRACT;

        //! handle function keys (which are reserved by SterTest for Plugins)
        virtual void handleFunctionKeys (int keyNumber) OS_ABSTRACT;

        //! print "mini help" documenting function keys handled by this Plugin
        virtual void printMiniHelpForFunctionKeys (void) const OS_ABSTRACT;

		//! return the current vehicle proximity database for this plugin
		virtual ProximityDatabase* accessProximityDataBase( void ) const OS_ABSTRACT;

        //! return an AVGroup (an STL vector of AbstractVehicle pointers) of
        //! all vehicles(/agents/characters) defined by the Plugin
        virtual const AVGroup& allVehicles (void) const OS_ABSTRACT;
		virtual AVGroup& allVehicles (void) OS_ABSTRACT;
		virtual void addVehicle (AbstractVehicle* vehicle) OS_ABSTRACT;
		virtual void removeVehicle (AbstractVehicle* vehicle) OS_ABSTRACT;
		virtual bool queryVehicleColor( const AbstractVehicle& vehicle, Color& kColor ) const OS_ABSTRACT;

		//! return an STL vector of AbstractObstacle pointers of
		//! all obstacles defined by the Plugin
		virtual ObstacleGroup& allObstacles( void ) OS_ABSTRACT;
		virtual const ObstacleGroup& allObstacles( void ) const OS_ABSTRACT;
		virtual void addObstacle (AbstractObstacle* obstacle) OS_ABSTRACT;
		virtual void removeObstacle (AbstractObstacle* obstacle) OS_ABSTRACT;

		//! return  an STL vector of AbstractPlayer pointers of
		//! all players defined by the Plugin
		virtual AbstractPlayerGroup& allPlayers( void ) OS_ABSTRACT;
		virtual const AbstractPlayerGroup& allPlayers( void ) const OS_ABSTRACT;
		virtual void addPlayer (AbstractPlayer* player) OS_ABSTRACT;
		virtual void removePlayer (AbstractPlayer* player) OS_ABSTRACT;
 
		//! returns pointer to the next Plugin in "selection order"
		virtual AbstractPlugin* next(void) const OS_ABSTRACT;

		//! returns pointer to the parent Plugin
		virtual AbstractPlugin* getParentPlugin(void) const OS_ABSTRACT;

		//! set a parent Plugin
		virtual void setParentPlugin( AbstractPlugin* ) OS_ABSTRACT;

		//! implement to initialize additional gui functionality
		virtual void initGui( void* /*userdata*/ ) OS_ABSTRACT;

		//! set an external vehicle factory
		virtual void setEntityFactory( AbstractEntityFactory* ) OS_ABSTRACT;

		virtual AbstractEntityFactory* getEntityFactory( void ) const OS_ABSTRACT;

		//! implement to create a vehicle of the specified class
		virtual AbstractEntity* createEntity( EntityClassId ) const OS_ABSTRACT;

		//! implement to create a vehicle of the specified class
		virtual AbstractVehicle* createVehicle( EntityClassId ) const OS_ABSTRACT;

		//! format instance to characters for printing to stream
		friend std::ostream& operator<< (std::ostream& os, AbstractPlugin& pi)
		{
			os << "<Plugin " << '"' << pi.pluginName() << '"' << ">";
			return os;
		}
	
	};

} //! namespace OpenSteer    
    

typedef void (*on_plugin_selected_func)( OpenSteer::AbstractPlugin* );


//-----------------------------------------------------------------------------
#endif //! OPENSTEER_ABSTRACTPLUGIN_H
