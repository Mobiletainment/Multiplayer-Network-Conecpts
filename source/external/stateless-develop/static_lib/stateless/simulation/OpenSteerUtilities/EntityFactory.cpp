#include "stdafx.h"
#include "OpenSteer/Plugin.h"
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

#include "EntityFactory.h"
#include "EduNetCommon/EduNetCommon.h"


namespace OpenSteer	{

	//-----------------------------------------------------------------------------
	AbstractEntity* EntityFactory::createEntity( EntityClassId classId ) const
	{
		AbstractEntity* entity = Plugin::createSystemEntity( classId );
		if( entity == nullptr )
		{
			AbstractEntity* masterEntity = accessMasterEntity( classId );
			if( masterEntity != nullptr )
			{
				entity = masterEntity->cloneEntity();
			}
		}
		return entity;
	}

	//-----------------------------------------------------------------------------
	AbstractVehicle* EntityFactory::createVehicle( EntityClassId classId ) const
	{
		AbstractEntity* entity(createEntity( classId ));
		if( entity != nullptr )	{
			return dynamic_cast<AbstractVehicle*>(entity);
		}
		return nullptr;
	}

	//-----------------------------------------------------------------------------
	AbstractVehicle* EntityFactory::createVehicle(  ) const
	{
		if( _masterEntity != nullptr )	{
			// for debugging
			const EntityClassId classId(_masterEntity->getClassId());
			return dynamic_cast<AbstractVehicle*>(_masterEntity->cloneEntity( ));
		}
		return nullptr;
	};

	//-----------------------------------------------------------------------------
	bool EntityFactory::destroyEntity( AbstractEntity* entity ) const
	{
		ET_SAFE_DELETE( entity );
		return true;
	}

	//-----------------------------------------------------------------------------
	bool EntityFactory::destroyVehicle( AbstractVehicle* vehicle ) const
	{
		ET_SAFE_DELETE( vehicle );
		return true;
	}

	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------

	void EntityFactoryArray::addEntityFactory( 
		AbstractEntityFactory* factory )
	{
		EntityClassId classId(OS_CID_UNKNOWN);
		// force initialize
		AbstractEntity* masterVehicle(nullptr);
		AbstractEntity* factoryMasterVehicle = factory->accessMasterEntity( classId );
		if( factoryMasterVehicle == nullptr )	{
			masterVehicle = factory->getMasterEntity( );
		}
		else	{
			masterVehicle = factoryMasterVehicle;
		}
		if( masterVehicle != nullptr )	{
			if( findEntityFactory( masterVehicle->getClassId() ) == nullptr )	{
				push_back( factory );
			}
		}
		return;
	}

	//-----------------------------------------------------------------------------
	AbstractEntityFactory* EntityFactoryArray::findEntityFactory( EntityClassId classId ) const
	{
		TEntityFactoryArray::const_iterator iter(begin());
		TEntityFactoryArray::const_iterator endIter(end());
		while( iter != endIter )	{
			AbstractEntity* masterEntity((*iter)->accessMasterEntity( classId ));
			if( masterEntity != nullptr)	{
				return (*iter);
			}
			++iter;
		}
		return nullptr;
	}

	//-----------------------------------------------------------------------------
	AbstractEntity* EntityFactoryArray::createEntity( EntityClassId classId ) const
	{
		AbstractEntity* entity = Plugin::createSystemEntity( classId );
		if( entity == nullptr )
		{
			AbstractEntity* masterEntity(accessMasterEntity( classId ));
			if( masterEntity != nullptr)
			{
				entity = masterEntity->cloneEntity();
			}
		}
		return entity;
	}

	//-----------------------------------------------------------------------------
	AbstractVehicle* EntityFactoryArray::createVehicle( EntityClassId classId ) const
	{
		AbstractEntity* entity = createEntity( classId );
		if( entity != nullptr )
		{
			AbstractVehicle* vehicle(dynamic_cast<AbstractVehicle*>(entity));
			if(vehicle == nullptr)	{
				destroyEntity(entity);
			}
			return vehicle;
		}
		return nullptr;
	}

	//-----------------------------------------------------------------------------
	AbstractVehicle* EntityFactoryArray::createVehicle(  ) const
	{
		return nullptr;
	};

	//-----------------------------------------------------------------------------
	bool EntityFactoryArray::destroyEntity( AbstractEntity* entity ) const
	{
		ET_SAFE_DELETE( entity );
		return true;
	}

	//-----------------------------------------------------------------------------
	bool EntityFactoryArray::destroyVehicle( AbstractVehicle* vehicle ) const
	{
		ET_SAFE_DELETE( vehicle );
		return true;
	}


}

