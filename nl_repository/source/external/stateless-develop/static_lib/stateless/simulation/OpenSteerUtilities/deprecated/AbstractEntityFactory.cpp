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

#include "OpenSteerUtilities/AbstractEntityFactory.h"
#include "EduNetCommon/EduNetCommon.h"

using namespace OpenSteer;

//-----------------------------------------------------------------------------
OpenSteer::AbstractEntity* EntityFactory::createEntity( OpenSteer::EntityClassId classId ) const
{
	OpenSteer::AbstractEntity* pkEntity = OpenSteer::Plugin::createSystemEntity( classId );
	if( NULL == pkEntity )
	{
		OpenSteer::AbstractEntity* pkMasterEntity = this->accessMasterEntity( classId );
		if( NULL != pkMasterEntity )
		{
			pkEntity = pkMasterEntity->cloneEntity();
		}
	}
	return pkEntity;
}

//-----------------------------------------------------------------------------
OpenSteer::AbstractVehicle* EntityFactory::createVehicle( OpenSteer::EntityClassId classId ) const
{
	OpenSteer::AbstractEntity* pkEntity = this->createEntity( classId );
	if( NULL != pkEntity )
	{
		return dynamic_cast<OpenSteer::AbstractVehicle*>(pkEntity);
	}
	return NULL;
}

//-----------------------------------------------------------------------------
OpenSteer::AbstractVehicle* EntityFactory::createVehicle(  ) const
{
	if( NULL != this->m_pkMasterEntity )
	{
		// for debugging
		const OpenSteer::EntityClassId classId = this->m_pkMasterEntity->getClassId();
		return dynamic_cast<OpenSteer::AbstractVehicle*>(this->m_pkMasterEntity->cloneEntity( ));
	}
	return NULL;
};

//-----------------------------------------------------------------------------
bool EntityFactory::destroyEntity( OpenSteer::AbstractEntity* pkEntity ) const
{
	ET_SAFE_DELETE( pkEntity );
	return true;
}

//-----------------------------------------------------------------------------
bool EntityFactory::destroyVehicle( OpenSteer::AbstractVehicle* pkVehicle ) const
{
	ET_SAFE_DELETE( pkVehicle );
	return true;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void EntityFactoryArray::addEntityFactory( AbstractEntityFactory* pkFactory )
{
	OpenSteer::EntityClassId classId(0);
	// force initialize
	OpenSteer::AbstractEntity* pkMasterVehicle = NULL;
	OpenSteer::AbstractEntity* pkFactoryMasterVehicle = pkFactory->accessMasterEntity( classId );
	if( NULL == pkFactoryMasterVehicle )
	{
		pkMasterVehicle = pkFactory->getMasterEntity( );
	}
	else
	{
		pkMasterVehicle = pkFactoryMasterVehicle;
	}
	if( NULL != pkMasterVehicle )
	{
		if( NULL == this->findEntityFactory( pkMasterVehicle->getClassId() ) )
		{
			this->push_back( pkFactory );
		}
	}
	return;
}

//-----------------------------------------------------------------------------
AbstractEntityFactory* EntityFactoryArray::findEntityFactory( OpenSteer::EntityClassId classId ) const
{
	//			TEntityFactoryArray::const_iterator kFound = std::find( this->begin(), this->end(), classId );
	TEntityFactoryArray::const_iterator kIter = this->begin();
	TEntityFactoryArray::const_iterator kEnd = this->end();
	while( kIter != kEnd )
	{
		OpenSteer::AbstractEntity* pkMasterEntity = (*kIter)->accessMasterEntity( classId );
		if( NULL != pkMasterEntity )
		{
			return (*kIter);
		}
		++kIter;
	}
	return NULL;
}

//-----------------------------------------------------------------------------
OpenSteer::AbstractEntity* EntityFactoryArray::createEntity( OpenSteer::EntityClassId classId ) const
{
	OpenSteer::AbstractEntity* pkEntity = OpenSteer::Plugin::createSystemEntity( classId );
	if( NULL == pkEntity )
	{
		OpenSteer::AbstractEntity* pkMasterEntity = this->accessMasterEntity( classId );
		if( NULL != pkMasterEntity )
		{
			pkEntity = pkMasterEntity->cloneEntity();
		}
	}
	return pkEntity;
}

//-----------------------------------------------------------------------------
OpenSteer::AbstractVehicle* EntityFactoryArray::createVehicle( OpenSteer::EntityClassId classId ) const
{
	OpenSteer::AbstractEntity* pkEntity = this->createEntity( classId );
	if( NULL != pkEntity )
	{
		return dynamic_cast<OpenSteer::AbstractVehicle*>(pkEntity);
	}
	return NULL;
}

//-----------------------------------------------------------------------------
OpenSteer::AbstractVehicle* EntityFactoryArray::createVehicle(  ) const
{
	return NULL;
};

//-----------------------------------------------------------------------------
bool EntityFactoryArray::destroyEntity( OpenSteer::AbstractEntity* pkEntity ) const
{
	ET_SAFE_DELETE( pkEntity );
	return true;
}

//-----------------------------------------------------------------------------
bool EntityFactoryArray::destroyVehicle( OpenSteer::AbstractVehicle* pkVehicle ) const
{
	ET_SAFE_DELETE( pkVehicle );
	return true;
}
