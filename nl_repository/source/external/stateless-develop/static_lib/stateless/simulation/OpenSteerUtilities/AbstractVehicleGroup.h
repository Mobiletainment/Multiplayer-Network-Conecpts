#ifndef __ABSTRACTVEHICLEGROUP_H__
#define __ABSTRACTVEHICLEGROUP_H__

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

#include "OpenSteerUtilities/AbstractEntityQuery.h"

namespace OpenSteer {


//-----------------------------------------------------------------------------
// a utility class to handle common vehicle related topics
class AbstractVehicleGroup : public OpenSteer::AbstractUpdated
{
public:
	AbstractVehicleGroup( AVGroup& group );
	AbstractVehicleGroup( const AVGroup& group );
	virtual ~AbstractVehicleGroup( void ){};

	//-------------------------------------------------------------------
	// interface AbstractUpdated
	virtual void updateCustom( AbstractUpdated* parent, const osScalar currentTime, const osScalar elapsedTime );
	virtual void update (const osScalar currentTime, const osScalar elapsedTime);

	virtual void setCustomUpdated( AbstractUpdated* updated )
	{
		_customUpdated = updated;
	}
	virtual AbstractUpdated* getCustomUpdated( void ) const
	{
		return _customUpdated;
	}

	virtual bool isEnabled( void ) const { return _enabled; };
	virtual void setEnabled( bool value ){ _enabled = value; };

	//-------------------------------------------------------------------------
	// vehicle group tools
	bool addVehicle( AbstractVehicle* vehicle, ProximityDatabase* proximityDatabase );
	void addVehicleToPlugin( AbstractVehicle* vehicle, AbstractPlugin* plugin );
	void removeVehicle( const AbstractVehicle* vehicle );

	void redraw (const float currentTime, const float elapsedTime);
	void reset( void );
	void allocateProximityToken( ProximityDatabase* pd );

 	AVGroup::const_iterator findVehicle( const AbstractVehicle* vehicle ) const;
	AVGroup::const_iterator findNetworkVehicle( NetworkId networkId ) const;

	AVGroup::iterator begin() { return _vehicles.begin(); }
	AVGroup::iterator end() { return _vehicles.end(); }
	AVGroup::const_iterator begin() const { return _vehicles.begin(); }
	AVGroup::const_iterator end() const { return _vehicles.end(); }
	size_t population() const { return _vehicles.size(); }
private:
	AbstractVehicleGroup( void );
	AVGroup& _vehicles;
	AbstractUpdated* _customUpdated;
	bool _enabled;

};

} // namespace OpenSteer

#endif //  __ABSTRACTVEHICLEGROUP_H__