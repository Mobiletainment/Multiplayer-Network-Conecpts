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

#include "OpenSteer/GlobalSelection.h"
#include "OpenSteer/AbstractPlugin.h"

//-----------------------------------------------------------------------------
namespace OpenSteer {

	GlobalSelection* GlobalSelection::globalSelection = nullptr;

	//-------------------------------------------------------------------------
	void GlobalSelection::_SDMInitApp( void )
	{
		if( GlobalSelection::globalSelection == nullptr )
		{
			static GlobalSelection kGlobalSelection;
			GlobalSelection::setInstance( &kGlobalSelection );
		}
	}

	//-------------------------------------------------------------------------
	void GlobalSelection::_SDMInitDLL( GlobalSelection* selection )
	{
		GlobalSelection::setInstance( selection );
	}

	//-------------------------------------------------------------------------
	void GlobalSelection::setInstance( GlobalSelection* selection )
	{
		GlobalSelection::globalSelection = selection;
	}

	//-------------------------------------------------------------------------
	GlobalSelection* GlobalSelection::getInstance( void )
	{
		return GlobalSelection::globalSelection;
	}

	//-------------------------------------------------------------------------
	GlobalSelection::GlobalSelection( void ):
		selectedVehicle(nullptr),
		nearestMouseVehicle(nullptr),
		cameraLocalSpaceToTrack(nullptr)
	{
	}

	//-------------------------------------------------------------------------
	GlobalSelection::~GlobalSelection()
	{
		if( this == GlobalSelection::globalSelection )
		{
			GlobalSelection::globalSelection = nullptr;
		}
	}

	//-------------------------------------------------------------------------
	void GlobalSelection::setCameraLocalSpaceToTrack( const AbstractLocalSpace* localSpace )
	{
		if( GlobalSelection::globalSelection != nullptr )
		{
			if( localSpace != GlobalSelection::globalSelection->cameraLocalSpaceToTrack )
			{
				// in case we are watching a plugin do not change anything right now
				const AbstractLocalSpace* current = GlobalSelection::globalSelection->cameraLocalSpaceToTrack;
				const AbstractPlugin* plugin = dynamic_cast<const AbstractPlugin*>(current);
				if( plugin != nullptr )
				{
					const AbstractVehicle* vehicle = dynamic_cast<const AbstractVehicle*>(localSpace);
					if( vehicle != nullptr )
					{
						return;
					}
				}
				GlobalSelection::globalSelection->cameraLocalSpaceToTrack = localSpace;
			}
		}
		else
		{
			if( localSpace != nullptr )
			{
				assert( GlobalSelection::globalSelection != nullptr );
			}
		}
	}

	//-------------------------------------------------------------------------
	void GlobalSelection::setSelectedVehicle( AbstractVehicle* vehicle )
	{
		if( GlobalSelection::globalSelection != nullptr )
		{
			GlobalSelection::globalSelection->selectedVehicle = vehicle;
		}
		else
		{
			if( vehicle != nullptr )
			{
				assert( GlobalSelection::globalSelection != nullptr );
			}
		}
	}

	//-------------------------------------------------------------------------
	void GlobalSelection::setNearestMouseVehicle( AbstractVehicle* vehicle )
	{
		if( GlobalSelection::globalSelection != nullptr )
		{
			GlobalSelection::globalSelection->nearestMouseVehicle = vehicle;
		}
		else
		{
			if( vehicle != nullptr )
			{
				assert( GlobalSelection::globalSelection != nullptr );
			}
		}
	}

	//-------------------------------------------------------------------------
	const AbstractLocalSpace* GlobalSelection::getCameraLocalSpaceToTrack( void )
	{
		if( GlobalSelection::globalSelection != nullptr )
		{
			return GlobalSelection::globalSelection->cameraLocalSpaceToTrack;
		}
		return nullptr;
	}

	//-------------------------------------------------------------------------
	AbstractVehicle* GlobalSelection::getSelectedVehicle( void )
	{
		if( GlobalSelection::globalSelection != nullptr )
		{
			return GlobalSelection::globalSelection->selectedVehicle;
		}
		return nullptr;
	}

	//-------------------------------------------------------------------------
	AbstractVehicle* GlobalSelection::getNearestMouseVehicle( void )
	{
		if( GlobalSelection::globalSelection != nullptr )
		{
			return GlobalSelection::globalSelection->nearestMouseVehicle;
		}
		return nullptr;
	}


} //! namespace OpenSteer    


