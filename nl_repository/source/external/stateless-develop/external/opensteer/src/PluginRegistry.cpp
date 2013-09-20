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
#include "stdafx.h"

#include "OpenSteer/PluginRegistry.h"
#include "OpenSteer/Camera.h"
#include "OpenSteer/SteeringVehicle.h"
#include "OpenSteer/GlobalData.h"

//-----------------------------------------------------------------------------
namespace OpenSteer {

	PluginRegistry::PluginRegistry( void ):
		_selectedPlugin(nullptr),
		_on_plugin_selected_func(nullptr)
	{

	}

	PluginRegistry::~PluginRegistry( void )
	{
	}

	PluginRegistry* PluginRegistry::accessInstance( void )
	{
		return GlobalData::accessPluginRegistry();
	}

	//-------------------------------------------------------------------------
	AbstractPlugin*
	PluginRegistry::findNextPlugin( const AbstractPlugin* thisPtr ) const
	{
		const size_t itemsInRegistry( _registry.size() );
		for (size_t i = 0; i < itemsInRegistry; i++)
		{
			if (thisPtr == _registry[i])
			{
				const bool atEnd = (i == (itemsInRegistry - 1));
				return _registry [atEnd ? 0 : i + 1];
			}
		}
		return nullptr;
	}

	//-------------------------------------------------------------------------
	int PluginRegistry::getPlugiidx( const AbstractPlugin* plugin ) const
	{
		const size_t itemsInRegistry( _registry.size() );
		for (size_t i = 0; i < itemsInRegistry; ++i)
		{
			if (plugin == _registry[i])
			{
				return i;
			}
		}
		return -1;
	}

	//-------------------------------------------------------------------------
	// search the class _registry for a Plugin with the given name
	// returns nullptr if none is found
	AbstractPlugin*
	PluginRegistry::findByName (const char* string) const
	{
		if ( string && string[0] )
		{
			const size_t itemsInRegistry( _registry.size() );
			for (size_t i = 0; i < itemsInRegistry; i++)
			{
				AbstractPlugin* pi = _registry[i];
				AbstractEntity* pe = dynamic_cast<AbstractEntity*>(pi);
				if( pe != nullptr )
				{
					const char* s = pe->name();
					if (s && (strcmp (string, s) == 0)) return pi;
				}
			}
		}
		return nullptr;
	}

	//-------------------------------------------------------------------------
	// apply a given function to all Plugins in the _registry
	void
	PluginRegistry::applyToAll( plugInCallBackFunction f )
	{
		const size_t itemsInRegistry( _registry.size() );
		for (size_t i = 0; i < itemsInRegistry; i++)
		{
			f(*_registry[i]);
		}
	}

	//-------------------------------------------------------------------------
	// sort Plugin _registry by "selection order"
	//
	// XXX replace with STL utilities
	void
	PluginRegistry::sortBySelectionOrder (void)
	{
		// I know, I know, just what the world needs:
		// another inline shell sort implementation...

		// starting at each of the first n-1 elements of the array
		const size_t itemsInRegistry( _registry.size() );
		for (size_t i = 0; i < itemsInRegistry-1; i++)
		{
			// scan over subsequent pairs, swapping if larger value is first
			for (size_t j = i+1; j < itemsInRegistry; j++)
			{
				const float iKey = _registry[i]->selectionOrderSortKey ();
				const float jKey = _registry[j]->selectionOrderSortKey ();

				if (iKey > jKey)
				{
					AbstractPlugin* temporary = _registry[i];
					_registry[i] = _registry[j];
					_registry[j] = temporary;
				}
			}
		}
	}

	//-------------------------------------------------------------------------
	// returns pointer to default Plugin (currently, first in _registry)
	AbstractPlugin*
	PluginRegistry::findDefault (void) const
	{
		// return nullptr if no PlugIns exist
		const size_t itemsInRegistry( _registry.size() );
		if (itemsInRegistry == 0) return nullptr;

		// otherwise, return the first Plugin that requests initial selection
		for (size_t i = 0; i < itemsInRegistry; i++)
		{
			if (_registry[i]->requestInitialSelection ()) return _registry[i];
		}

		// otherwise, return the "first" Plugin (in "selection order")
		return _registry[0];
	}

	//-------------------------------------------------------------------------
	// save this instance in the class's _registry of instances
	// (for use by contractors)
	void
	PluginRegistry::addToRegistry (AbstractPlugin* plugin)
	{
		// better be safe here
		if( plugin == nullptr )
		{
			return;
		}
		// just for debugging
		const char* pluginName( plugin->pluginName() );

		// prevent adding plugins twice
		const size_t itemsInRegistry( _registry.size() );
		for (size_t i = 0; i < itemsInRegistry; ++i)
		{
			if (plugin == _registry[i])
			{
				return;
			}
		}

		// save this instance in the _registry
		_registry.push_back(plugin);
	}

	//-------------------------------------------------------------------------
	// save this instance in the class's _registry of instances
	// (for use by contractors)
	void
	PluginRegistry::removeFromRegistry( AbstractPlugin* plugin )
	{
		// better be safe here
		if( plugin == nullptr )
		{
			return;
		}
		// just for debugging
		const char* pluginName( plugin->pluginName() );

		AbstractPluginVector::iterator iter = _registry.begin();
		AbstractPluginVector::iterator end = _registry.end();
		while( iter != end  )
		{
			AbstractPlugin* registeredPlugin(*iter);
			if( registeredPlugin == plugin )
			{
				_registry.erase( iter );
				break;
			}
			++iter;
		}
	}
	//-------------------------------------------------------------------------
	// return a group (an STL vector of AbstractVehicle pointers) of all
	// vehicles(/agents/characters) defined by the currently selected Plugin
	const AVGroup&
	PluginRegistry::allVehiclesOfSelectedPlugin (void) const
	{
		static AVGroup kTrash;
		if(getSelectedPlugin() )
			return getSelectedPlugin()->allVehicles ();
		else
			return kTrash;
	}

	//-------------------------------------------------------------------------
	void
	PluginRegistry::selectPlugin( AbstractPlugin* plugin )
	{
		AbstractPlugin* selectedPlugin( getSelectedPlugin() );
		if( plugin == selectedPlugin )
		{
			return;
		}
		if( selectedPlugin != nullptr )
		{
			selectedPlugin->close();
		}

		// reset camera and selected vehicle
		Camera::accessInstance().reset ();
		SteeringVehicle::setSelectedVehicle( nullptr );

		setSelectedPlugin( plugin );
		if( getSelectedPlugin() != nullptr )
		{
			getSelectedPlugin()->prepareOpen();
			// note: call the application
			//       might initialize the gui
			if( _on_plugin_selected_func != nullptr )
			{
				_on_plugin_selected_func( getSelectedPlugin() );
			}
			getSelectedPlugin()->open();
		}
	}

	//-------------------------------------------------------------------------
	// select the "next" plug-in, cycling through "plug-in selection order"
	void
	PluginRegistry::selectNextPlugin (void)
	{
		if( getSelectedPlugin() == nullptr )
		{
			return;
		}
		selectPlugin( getSelectedPlugin()->next () );
	}

	//-------------------------------------------------------------------------
	// select the plug-in by index
	void PluginRegistry::selectPluginByIndex (size_t idx)
	{
		AbstractPlugin* p = getPluginAt( idx );
		if( ( p != nullptr ) && (p != getSelectedPlugin()) )
		{
			selectPlugin( p );
		}
	}

	//-------------------------------------------------------------------------
	// handle function keys an a per-plug-in basis
	void
	PluginRegistry::functionKeyForPlugin( int keyNumber ) const
	{
		if( getSelectedPlugin() == nullptr )
		{
			return;
		}
		getSelectedPlugin()->handleFunctionKeys (keyNumber);
	}

	//-------------------------------------------------------------------------
	// return name of currently selected plug-in
	const char*
	PluginRegistry::nameOfSelectedPlugin (void) const
	{
		return (getSelectedPlugin() ? getSelectedPlugin()->pluginName() : "no Plugin");
	}

	//-------------------------------------------------------------------------
	// reset the currently selected plug-in
	void
	PluginRegistry::resetSelectedPlugin (void)
	{
		if( getSelectedPlugin() == nullptr )
		{
			return;
		}
		// reset camera and selected vehicle
		Camera::accessInstance().reset ();
		SteeringVehicle::setSelectedVehicle( nullptr );

		getSelectedPlugin()->reset ();
	}


} //! namespace OpenSteer    
    

