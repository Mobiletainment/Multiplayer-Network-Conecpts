#ifndef __ENTITYFACTORY_H__
#define __ENTITYFACTORY_H__
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

#include "AbstractEntityFactory.h"

//-----------------------------------------------------------------------------
namespace OpenSteer
{

	//-------------------------------------------------------------------------
	class EntityFactory : public AbstractEntityFactory
	{
		ET_DECLARE_BASE( AbstractEntityFactory );
	public:
		EntityFactory():_masterEntity(NULL)
		  {
		  }

		  virtual ~EntityFactory() {}

		  virtual AbstractEntity* accessMasterEntity( EntityClassId classId ) const
		  {
			  if( ( _masterEntity != NULL ) && ( classId == _masterEntity->getClassId() ) )
			  {
				  return _masterEntity;
			  }
			  return NULL;
		  }
		  
		  virtual AbstractEntity* getMasterEntity( void ) const
		  {
			  return _masterEntity;
		  }

		  virtual void setMasterEntity( AbstractEntity* entity ) const
		  {
			  _masterEntity = entity;
		  }

		  virtual AbstractEntity* createEntity( EntityClassId ) const;
		  virtual AbstractVehicle* createVehicle( EntityClassId classId ) const;
		  virtual AbstractVehicle* createVehicle( void ) const;
		  virtual bool destroyEntity( AbstractEntity* entity ) const;
		  virtual bool destroyVehicle( AbstractVehicle* vehicle ) const;
	protected:
		mutable AbstractEntity* _masterEntity;
	};

	//-------------------------------------------------------------------------
	// TODO @CP : use map ?
	typedef std::vector<AbstractEntityFactory*> TEntityFactoryArray;

	//-------------------------------------------------------------------------
	class EntityFactoryArray : protected TEntityFactoryArray, public AbstractEntityFactory
	{
		ET_DECLARE_BASE( AbstractEntityFactory );
	public:
		EntityFactoryArray()	{
		}

		virtual ~EntityFactoryArray() {}

		void addEntityFactory( AbstractEntityFactory* factory );

		AbstractEntityFactory* findEntityFactory( EntityClassId classId ) const;

		virtual AbstractEntity* accessMasterEntity( EntityClassId classId ) const
		{
			AbstractEntityFactory* factory = findEntityFactory( classId );
			if( factory != nullptr )
			{
				return factory->accessMasterEntity( classId );
			}
			return nullptr;
		}

		virtual AbstractEntity* getMasterEntity( void ) const	{
			// important: a vehicle factory array does not have an own master entity
			return nullptr;
		}

		virtual void setMasterEntity( AbstractEntity* entity ) const	{

		}

		virtual AbstractEntity* createEntity( EntityClassId ) const;

		virtual AbstractVehicle* createVehicle( EntityClassId classId ) const;

		virtual AbstractVehicle* createVehicle( void ) const;

		virtual bool destroyEntity( AbstractEntity* entity ) const;
		virtual bool destroyVehicle( AbstractVehicle* vehicle ) const;
	protected:
	};

	//-------------------------------------------------------------------------
	typedef EntityFactory NullEntityFactory;

	//-------------------------------------------------------------------------
	template <class VehicleClass>
	class TEntityFactory : public EntityFactory
	{
		ET_DECLARE_BASE( EntityFactory )
	public:
		TEntityFactory()	{
		};

		virtual ~TEntityFactory() 
		{

		};

		virtual AbstractEntity* accessMasterEntity( EntityClassId classId ) const
		{
			_masterEntity = &_masterVehicle;
			return BaseClass::accessMasterEntity( classId );
		}
	private:
		mutable VehicleClass _masterVehicle;

	};

}


#endif // __ENTITYFACTORY_H__
