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

#include "AbstractVehicleGroup.h"
#include "AbstractVehicleUpdate.h"

#include "OpenSteer/SteeringVehicle.h"
#include "OpenSteer/Plugin.h"

#include <algorithm>

namespace OpenSteer {

	//-----------------------------------------------------------------------------
	AbstractVehicleGroup::AbstractVehicleGroup( AVGroup& group )
		:_vehicles(group)
		,_customUpdated( NULL )
		,_enabled(true)
	{

	}

	//-----------------------------------------------------------------------------
	AbstractVehicleGroup::AbstractVehicleGroup( const AVGroup& group )
		:_vehicles((AVGroup&)group)
		,_customUpdated( NULL )
		,_enabled(true)
	{

	}

	//-----------------------------------------------------------------------------
	void AbstractVehicleGroup::allocateProximityToken( ProximityDatabase* pd )
	{
		AVIterator iter(_vehicles.begin());
		AVIterator last(_vehicles.end());
		while( iter != last )
		{
			(*iter)->allocateProximityToken( pd );
			++iter;
		}
	}

	//-----------------------------------------------------------------------------
	void AbstractVehicleGroup::updateCustom(
		AbstractUpdated* parent, const osScalar currentTime, const osScalar elapsedTime )
	{
		// note: do not call this function
		assert( false );
	}

	//-----------------------------------------------------------------------------
	void AbstractVehicleGroup::update( const float currentTime, const float elapsedTime )
	{
		AVIterator iter(_vehicles.begin());
		AVIterator last(_vehicles.end());
		while( iter != last )
		{
			AbstractVehicle* vehicle = (*iter);
			AbstractUpdated* vehicleCustomUpdate(vehicle->getCustomUpdated());
			// the abstract vehicle group supports a custom update handler
			// this allows to override the vehicle update implementation
			vehicle->setCustomUpdated( getCustomUpdated() );
			AbstractUpdated* customUpdate(vehicle->getCustomUpdated());
			if( customUpdate != NULL )
			{
				AbstractVehicleUpdate* vehicleUpdate = dynamic_cast<AbstractVehicleUpdate*>( customUpdate );
				if( vehicleUpdate != NULL )
				{
					vehicleUpdate->setVehicle( vehicle );
					vehicleUpdate->update( currentTime, elapsedTime );
					vehicleUpdate->setVehicle( NULL );
				}
				else
				{
					vehicle->update( currentTime, elapsedTime );
				}
			}
			else
			{
				vehicle->setCustomUpdated( vehicleCustomUpdate );
				vehicle->update( currentTime, elapsedTime );
			}
			vehicle->setCustomUpdated( vehicleCustomUpdate );
			++iter;
		}
	}

	//-----------------------------------------------------------------------------
	void AbstractVehicleGroup::redraw (const float currentTime, const float elapsedTime)
	{
		AVIterator iter = _vehicles.begin();
		AVIterator last = _vehicles.end();
		while( iter != last )	{
			(*iter)->draw( currentTime, elapsedTime );
			++iter;
		}
	}

	//-----------------------------------------------------------------------------
	void AbstractVehicleGroup::reset( void )
	{
		AVIterator iter = _vehicles.begin();
		AVIterator last = _vehicles.end();
		while( iter != last )	{
			(*iter)->reset( );
			++iter;
		}
	}

	//-----------------------------------------------------------------------------
	bool AbstractVehicleGroup::addVehicle( AbstractVehicle* vehicle, ProximityDatabase* proximityDatabase )	{
		// do not add NULL vehicles
		if( vehicle == nullptr )	{
			return false;
		}

		if(findVehicle(vehicle) != end())	{
			// vehicle already inside this group
			return false;
		}

		// allocate a proximity token in case there is a proximity database
		vehicle->allocateProximityToken( proximityDatabase );
		_vehicles.push_back( vehicle );
		if( _vehicles.size() == 1 )	{
			SteeringVehicle::setSelectedVehicle( vehicle );
		}
		else	{
			if( SteeringVehicle::getSelectedVehicle() == nullptr)	{
				SteeringVehicle::setSelectedVehicle( vehicle );
			}
		}
		return true;
	}

	//-----------------------------------------------------------------------------
	void AbstractVehicleGroup::addVehicleToPlugin( AbstractVehicle* vehicle, AbstractPlugin* plugin )
	{
		// do not add NULL vehicles
		if( vehicle == nullptr )	{
			return;
		}
		assert( plugin != nullptr );
		ProximityDatabase* proximityDatabase(plugin->accessProximityDataBase());

		if(addVehicle(vehicle, proximityDatabase))	{
			vehicle->setParentEntity( castToAbstractEntity( plugin ) );		
		}
	}

	//-----------------------------------------------------------------------------
	void AbstractVehicleGroup::removeVehicle( const AbstractVehicle* vehicle )	{
		AVGroup::const_iterator iter = findVehicle( vehicle );
		if(iter != _vehicles.end())	{
			_vehicles.erase( iter ); 		
		}
	}

	//-----------------------------------------------------------------------------
	AVGroup::const_iterator AbstractVehicleGroup::findVehicle( const AbstractVehicle* vehicle ) const	{
		EntityPointerQuery entityPointerQuery(vehicle);
		AVQuery query( _vehicles, &entityPointerQuery );
		return query.find( );
	}

	//-----------------------------------------------------------------------------
	AVGroup::const_iterator AbstractVehicleGroup::findNetworkVehicle( NetworkId networkId ) const	{
		NetworkEntityQuery networkEntityQuery(networkId);
		AVQuery query( _vehicles, &networkEntityQuery );
		return query.find( );
	}

} // namespace OpenSteer
