#ifndef __ABSTRACTENTITYQUERY_H__
#define __ABSTRACTENTITYQUERY_H__

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
#include "OpenSteer/AbstractEntity.h"
#include "OpenSteer/AbstractVehicle.h"
#include "OpenSteer/Obstacle.h"
#include "OpenSteerUtilities/OpenSteerUTTypes.h"

namespace OpenSteer {

	//-------------------------------------------------------------------------
	class AbstractEntityQuery
	{
	public:
		virtual ~AbstractEntityQuery() {}
		virtual bool isEntityMatch( const AbstractEntity* entity ) const ET_ABSTRACT;
	};

	//-------------------------------------------------------------------------
	class EntityPointerQuery : public AbstractEntityQuery
	{
	public:
		EntityPointerQuery( const AbstractEntity* entity ):_entity( entity ) {}
		virtual ~EntityPointerQuery() {}
		virtual bool isEntityMatch( const AbstractEntity* entity ) const
		{
			return ( entity == _entity );
		}
	private:
		const AbstractEntity* _entity;
	};

	//-------------------------------------------------------------------------
	class EntityClassIdQuery : public AbstractEntityQuery
	{
	public:
		EntityClassIdQuery( OS_CID classId ):_classId( classId ) {}
		virtual ~EntityClassIdQuery() {}
		virtual bool isEntityMatch( const AbstractEntity* entity ) const
		{
			return ( entity->getClassId() == _classId );
		}
	private:
		const OS_CID _classId;
	};

	//-------------------------------------------------------------------------
	class NetworkEntityQuery : public AbstractEntityQuery
	{
	public:
		NetworkEntityQuery( NetworkId networkId ):_networkId( networkId ) {}
		virtual ~NetworkEntityQuery() {}
		virtual bool isEntityMatch( const AbstractEntity* entity ) const
		{
			return ( entity->getNetworkId() == _networkId );
		}
	private:
		NetworkId _networkId;
	};

	//-------------------------------------------------------------------------
	class NoPlayerEntityQuery : public AbstractEntityQuery
	{
	public:
		NoPlayerEntityQuery( void ) {}
		virtual ~NoPlayerEntityQuery() {}
		virtual bool isEntityMatch( const AbstractEntity* entity ) const
		{
			return ( entity->getPlayer() == NULL );
		}
	private:
	};

	//-------------------------------------------------------------------------
	class NoControlledEntityQuery : public AbstractEntityQuery
	{
	public:
		NoControlledEntityQuery( void ) {}
		virtual ~NoControlledEntityQuery() {}
		virtual bool isEntityMatch( const AbstractEntity* entity ) const
		{
			const osAbstractPlayer* player = castToAbstractPlayer( entity );
			if( player != NULL )
			{
				return ( player->getControlledEntity() == NULL );
			}
			return false;
		}
	private:
	};

	//-------------------------------------------------------------------------
	template <class ContainerType, class EntityType >
	class TEntityQuery
	{
	public:
#pragma warning(push)
#pragma warning(disable: 4355) // warning C4355: 'this' : used in base member initializer list
		TEntityQuery( const ContainerType& container, AbstractEntityQuery* query ):
		  _container(container),
			  _query(query)
		{
		}
#pragma warning(pop)

		virtual ~TEntityQuery()
		{
		}

		EntityType* findType( void ) const
		{
			typename ContainerType::const_iterator iter = _container.begin();
			typename ContainerType::const_iterator last = _container.end();
			while( iter != last )
			{
				if( _query->isEntityMatch( *iter ) == true )
				{
					return (*iter);
				}
				++iter;
			}
			return NULL;
		}

		typename ContainerType::const_iterator find( void ) const
		{
			typename ContainerType::const_iterator iter = _container.begin();
			typename ContainerType::const_iterator last = _container.end();
			while( iter != last )
			{
				if( _query->isEntityMatch( *iter ) == true )
				{
					return iter;
				}
				++iter;
			}
			return last;
		}

		typename ContainerType::iterator find( void )
		{
			ContainerType& container = (ContainerType&)_container;
			typename ContainerType::iterator iter = container.begin();
			typename ContainerType::iterator last = container.end();
			while( iter != last )
			{
				if( _query->isEntityMatch( *iter ) == true )
				{
					return iter;
				}
				++iter;
			}
			return last;
		}

		size_t find( ContainerType& query ) const
		{
			typename ContainerType::const_iterator iter = _container.begin();
			typename ContainerType::const_iterator last = _container.end();
			while( iter != last )
			{
				if( _query->isEntityMatch( *iter ) == true )
				{
					query.push_back( *iter );
				}
				++iter;
			}
			return (query.size() > 0);
		}

	private:
		const ContainerType& _container;
		AbstractEntityQuery* _query;

	};

	typedef TEntityQuery<AVGroup, AbstractVehicle> AVQuery;
	typedef TEntityQuery<AbstractPlayerGroup, AbstractPlayer> AbstractPlayerQuery;
	typedef TEntityQuery<ObstacleGroup, AbstractObstacle> AbstractObstacleQuery;


}

#endif // __ABSTRACTENTITYQUERY_H__